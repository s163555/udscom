# tests/CMakeLists.txt

# 1) Pull in the Catch2 submodule (out‑of‑tree build)
#    SOURCE_DIR: point at the submodule in your repo
#    BINARY_DIR: somewhere under build/, e.g. build/third_party/Catch2
add_subdirectory(
  ${CMAKE_SOURCE_DIR}/third_party/Catch2
  ${CMAKE_BINARY_DIR}/third_party/Catch2-build
  EXCLUDE_FROM_ALL
)

# 2) Declare your test executable
file(GLOB TEST_SOURCES
  parser_tests.cpp
  csv_tests.cpp
  #history_tests.cpp
)
add_executable(udscom_tests ${TEST_SOURCES})

# 3) Link against your library and Catch2’s CMake targets
target_link_libraries(udscom_tests
  PRIVATE udscom
  PRIVATE Catch2::Catch2WithMain
)

# 4) Register with CTest
include(Catch)             # comes from Catch2
catch_discover_tests(udscom_tests)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../resources/data_list.txt   # from source tree
  ${CMAKE_CURRENT_BINARY_DIR}/data_list.txt             # to build dir
  COPYONLY)