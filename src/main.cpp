#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "udscom/can_backend.hpp"
#include "udscom/parser.hpp"
#include "udscom/csv.hpp"

#include <cxxopts.hpp>

#include <thread>
#include <chrono>
#include <iostream>
#include <filesystem>

using namespace std::chrono_literals;
using namespace ftxui;

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
        can->open(iface, rx, tx);          // <‑‑ MUST succeed first
    }
    catch (const std::exception& e) {
        std::cerr << "CAN open failed: " << e.what() << '\n';
        return 1;
    }
    // ----------------------------------------------------------------  UI
    bool polling = false;
    std::string mode = "dec";               // dec/hex/bin

    auto table_renderer = Renderer([&] {
        Elements es;
        for (auto& r : rows) {
            es.push_back(hbox({
                text(r.label) | size(WIDTH, EQUAL, 18),
                text(uds::format(r.value, r.type, mode)) | bold
            }));
        }
        return vbox(es) | border;
    });

    auto root = CatchEvent(table_renderer, [&](Event e){
        if (e == Event::Character(' '))  polling = !polling;
        if (e == Event::Character('h'))  mode = (mode=="hex")? "dec":"hex";
        if (e == Event::Character('b'))  mode = (mode=="bin")? "dec":"bin";
        return false;
    });

    auto scr = ScreenInteractive::TerminalOutput();

    std::jthread poll_thread([&]{
        while (true) {
            if (polling) {
                for (auto& r : rows) {
                    auto resp = can->request(uds::build_rdbi(r.id), 100ms);
                    if (resp.size() > 3 && resp[0] == 0x62) {
                        std::span<const std::uint8_t> payload(resp.data() + 3, resp.size() - 3);
                        if (auto val = uds::parse_payload(payload, r.type))
                            r.value = *val;
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
}
