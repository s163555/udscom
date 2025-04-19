#include <catch2/catch_all.hpp>
#include "udscom/parser.hpp"

using Catch::Approx;

TEST_CASE("parse_payload decodes UInt16 (big‑endian)", "[parser]") {
    std::array<std::uint8_t, 2> buf{0x12, 0x34};   // 0x1234 = 4660
    auto v = uds::parse_payload(buf, uds::ScalarType::UInt16);
    REQUIRE(v);
    REQUIRE(uds::to_double(*v) == Approx(4660.0));
}

TEST_CASE("parse_payload returns empty on size mismatch", "[parser]") {
    std::array<std::uint8_t, 1> bad{0xFF};
    auto v = uds::parse_payload(bad, uds::ScalarType::UInt32);
    REQUIRE_FALSE(v);
}