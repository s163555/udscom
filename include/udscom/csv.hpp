#pragma once
#include <string>
#include <vector>
#include <filesystem>

#include "udscom/parser.hpp"

namespace uds {

struct DataRow {
    std::string    label;
    std::uint16_t  id;
    ScalarType     type;
    ScalarValue    value{};          // filled during polling
};

std::vector<DataRow> load_list(const std::filesystem::path& file);

std::vector<std::uint8_t> build_rdbi(std::uint16_t did);   // uds 0x22 hi lo

} // namespace uds