#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "udscom/can_backend.hpp"
#include "udscom/parser.hpp"
#include "udscom/csv.hpp"

#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;
using namespace ftxui;

int main(int argc, char** argv) {
    const char* list_file = (argc>1) ? argv[1] : "data_list.txt";

    // ------------------------------------------------------------------ data
    auto rows = uds::load_list(list_file);
    if (rows.empty()) {
        std::cerr << "No entries loaded from " << list_file << '\n';
        return 1;
    }

    // ---------------------------------------------------------------- back‑end
    auto can = make_backend();
    can->open("can0", 0x18DAF101, 0x18DA01F1);

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
                    auto resp = can->request(uds::build_rdbi(r.id), 500ms);
                    auto val  = uds::parse_payload(resp, r.type);
                    if (val) r.value = *val;
                }
                scr.Post(Event::Custom);
            }
            std::this_thread::sleep_for(100ms);
        }
    });

    scr.Loop(root);
}
