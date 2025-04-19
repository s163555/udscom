# udscom/tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.20)

# 1. Pull in Catch2 (local sub‑directory)
add_subdirectory(Catch2)   # <= must point to an EXISTING directory

# 2. Test executable(s)
add_executable(udscom_tests
  parser_tests.cpp
  csv_tests.cpp
 # history_tests.cpp
)
target_link_libraries(udscom_tests
  PRIVATE
    udscom                 # library under test
    Catch2::Catch2WithMain # exported by Catch2's CMakeLists.txt
)

# 3. Register with CTest
include(Catch)             # comes from Catch2
catch_discover_tests(udscom_tests)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../resources/data_list.txt   # from source tree
  ${CMAKE_CURRENT_BINARY_DIR}/data_list.txt             # to build dir
  COPYONLY)
