#include "udscom/csv.hpp"
#include "udscom/parser.hpp"

#include <fstream>
#include <sstream>
#include <charconv>

namespace uds {

std::vector<DataRow> load_list(const std::filesystem::path& f) {
    std::vector<DataRow> out;
    std::ifstream in(f);
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string lbl, idstr, tstr;
        if (!std::getline(ss, lbl,   ',')) continue;
        if (!std::getline(ss, idstr, ',')) continue;
        if (!std::getline(ss, tstr,  ',')) continue;

        std::uint16_t id{};
        std::from_chars(idstr.data(), idstr.data()+idstr.size(), id);

        auto opt = type_from_string(tstr);
        if (!opt) continue;

        out.push_back({lbl, id, *opt});
    }
    return out;
}

std::vector<std::uint8_t> build_rdbi(std::uint16_t did) {
    return {0x22, static_cast<std::uint8_t>(did>>8), static_cast<std::uint8_t>(did)};
}

} // namespace uds