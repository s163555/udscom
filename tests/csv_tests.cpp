#include <catch2/catch_all.hpp>
#include "udscom/csv.hpp"

using Catch::Approx;

TEST_CASE("load_list parses three columns", "[csv]") {
    auto rows = uds::load_list("data_list.txt");
    REQUIRE(rows.size() == 3);
    REQUIRE(rows[0].label == "uC idle average");
    REQUIRE(rows[0].id    == 500);
    REQUIRE(rows[1].label == "uC idle min");
    REQUIRE(rows[1].id    == 501);
    REQUIRE(rows[2].label == "uC idle max");
    REQUIRE(rows[2].id    == 502);
}