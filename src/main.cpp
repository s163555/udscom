#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include <cxxopts.hpp>

#include "udscom/can_backend.hpp"
#include "udscom/parser.hpp"
#include "udscom/csv.hpp"

#include <thread>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <atomic>
#include <stop_token>
#include <algorithm>
#include <vector>
#include <limits>
#include <cmath>

using namespace std::chrono_literals;
using namespace ftxui;

std::atomic<bool> running = true;      // global flag (or capture in lambda)

constexpr size_t HISTORY_MAX = 200;      // width of a wide terminal
std::vector<double> history;             // data to plot
bool show_plot = false;
const size_t PLOT_INDEX = 0;             // which row to graph

int main(int argc, char** argv) {
    cxxopts::Options opts("udscom_tui");
    opts.add_options()
        ("i,iface", "CAN interface",
                     cxxopts::value<std::string>()->default_value("can0"))
        ("r,rx",    "RX CAN‑ID (hex)",
                     cxxopts::value<std::string>()->default_value("18DAF101"))
        ("t,tx",    "TX CAN‑ID (hex)",
                     cxxopts::value<std::string>()->default_value("18DA01F1"))
        ("L,list",  "Data‑ID list file",
                     cxxopts::value<std::string>()
                              ->default_value("data_list.txt"))
        ("h,help",  "Show help");

    auto cli = opts.parse(argc, argv);
    if (cli.count("help")) {
        std::cout << opts.help() << '\n';
        return 0;
    }
    std::string iface = cli["iface"].as<std::string>();
    uint32_t    rx    = std::stoul(cli["rx"].as<std::string>(), nullptr, 16);
    uint32_t    tx    = std::stoul(cli["tx"].as<std::string>(), nullptr, 16);
    std::string list_file = cli["list"].as<std::string>();
    // ------------------------------------------------------------------ data
    if (!std::filesystem::exists(list_file)) {
        std::cerr << "List file \"" << list_file << "\" not found!\n";
        return 1;
    }
    auto rows = uds::load_list(list_file);
    if (rows.empty()) {
        std::cerr << "No entries loaded from " << list_file << '\n';
        return 1;
    }

    // ---------------------------------------------------------------- back‑end
    auto can = make_backend();
    try {
        can->open(iface, rx, tx);
    }
    catch (const std::exception& e) {
        std::cerr << "CAN open failed: " << e.what() << '\n';
        return 1;
    }
    // ----------------------------------------------------------------  UI
    auto scr = ScreenInteractive::Fullscreen();
    bool polling = false;
    std::string mode = "dec";               // dec/hex/bin

    auto history_graph = [&](int width, int height) {
        std::vector<int> out(width, 0);
        if (history.empty())
            return out;
    
        /* resample history to match 'width' */
        for (int x = 0; x < width; ++x) {
            size_t idx = x * history.size() / width;
            out[x] = history[idx];
        }
    
        /* auto‑scale y to [0, height-1] */
        auto [min_it, max_it] = std::minmax_element(history.begin(), history.end());
        double min = *min_it;
        double max = *max_it;
        double span = (max - min > 1e-9) ? (max - min) : 1.0;
    
        for (int& v : out) {
            double norm = (v - min) / span;        // 0..1
            v = static_cast<int>(norm * (height - 1));
        }
        return out;
    };

    auto table_renderer = Renderer([&] {
        /* numeric table */
        Elements rows_el;
        for (auto& r : rows) {
            std::string txt = std::isnan(uds::to_double(r.value))
                              ? "--"
                              : uds::format(r.value, r.type, mode);
            rows_el.push_back(hbox({
                text(r.label) | size(WIDTH,EQUAL,18),
                text(txt)     | bold
            }));
        }
        Element table = vbox(rows_el) | border;
    
        /* optional graph panel */
        if (!show_plot || history.empty())
            return table;
    
        Element plot = graph(std::ref(history_graph)) | flex | border;
        return vbox({table, plot});
    });
    

    auto root = CatchEvent(table_renderer, [&](Event e){
        if (e == Event::Character('q') || e == Event::Escape)
        {
            running = false;
            scr.Exit();
            return true;
        }
        if (e == Event::Character('p')) {
            show_plot = !show_plot;
            return true;
        }
        if (e == Event::Character(' '))  polling = !polling;
        if (e == Event::Character('h'))  mode = (mode=="hex")? "dec":"hex";
        if (e == Event::Character('b'))  mode = (mode=="bin")? "dec":"bin";
        return false;
    });

    std::jthread poll([&](std::stop_token st){
        while (running && !st.stop_requested()) {
            if (polling) {
                for (auto& r : rows) {
                    auto resp = can->request(uds::build_rdbi(r.id), 100ms);
                    if (resp.size() > 3 && resp[0] == 0x62) {
                        std::span<const std::uint8_t> payload(resp.data() + 3, resp.size() - 3);
                        if (auto v = uds::parse_payload(payload, r.type)) {
                            r.value = *v;
                        
                            if (&r == &rows[PLOT_INDEX]) {           // add to history
                                double d = uds::to_double(*v);
                                history.push_back(d);
                                if (history.size() > HISTORY_MAX)
                                    history.erase(history.begin(),
                                                  history.begin() + (history.size() - HISTORY_MAX));
                            }
                        }
                        else if (resp.empty()) {
                            r.value = std::numeric_limits<double>::quiet_NaN();
                        }
                    }
                    else if (resp.empty()) {
                        r.value = std::numeric_limits<double>::quiet_NaN();
                    }
                }
                scr.Post(Event::Custom);
            }
            std::this_thread::sleep_for(100ms);
        }
    });

    scr.Loop(root);
    poll.request_stop();
}
