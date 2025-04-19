cmake_minimum_required(VERSION 3.20)
project(udscom LANGUAGES CXX)

# ── Optional flag: -DUDSCOM_ENABLE_COVERAGE=ON ────────────────────────
option(UDSCOM_ENABLE_COVERAGE "Build with coverage instrumentation" OFF)

if(UDSCOM_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  message(STATUS "⚑  Building with GCC coverage flags")
  add_compile_options(--coverage -O0)
  add_link_options(--coverage)
elseif(UDSCOM_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  message(STATUS "⚑  Building with Clang coverage flags")
  add_compile_options(-fprofile-instr-generate -fcoverage-mapping -O0)
  add_link_options(-fprofile-instr-generate)
endif()

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(third_party/FTXUI EXCLUDE_FROM_ALL)
add_subdirectory(third_party/cxxopts EXCLUDE_FROM_ALL)

add_library(udscom STATIC
  src/parser.cpp
  src/csv.cpp
  $<$<PLATFORM_ID:Linux>:src/socketcan_backend.cpp>
#  $<$<PLATFORM_ID:Windows>:src/pcan_backend.cpp>
)
target_include_directories(udscom PUBLIC include)
target_link_libraries(udscom PUBLIC ftxui::screen ftxui::dom ftxui::component
  $<$<PLATFORM_ID:Linux>:pthread>
  $<$<PLATFORM_ID:Windows>:PCANBasic PCAN-ISO-TP>
)

# ---------- final executable ----------------------------------------
add_executable(udscom_tui src/main.cpp)
target_link_libraries(udscom_tui PRIVATE cxxopts::cxxopts)
target_link_libraries(udscom_tui PRIVATE udscom)

# ---------- tests ---------------------------------------------------
enable_testing()
add_subdirectory(tests)

if(UDSCOM_ENABLE_COVERAGE)
  find_program(LCOV_EXECUTABLE lcov REQUIRED)
  find_program(GENHTML_EXECUTABLE genhtml REQUIRED)

  set(LCOV_RAW      "${CMAKE_BINARY_DIR}/coverage.info")
  set(LCOV_FILTERED "${CMAKE_BINARY_DIR}/coverage.filtered.info")
  set(HTML_DIR      "${CMAKE_BINARY_DIR}/coverage_html")

  add_custom_target(coverage
    # 1) capture only your udscom library’s .gcda
    COMMAND ${LCOV_EXECUTABLE}
            --capture
            --directory "${CMAKE_BINARY_DIR}/CMakeFiles/udscom.dir"
            --output-file "${LCOV_RAW}"

    # 2) strip out /usr, tests/, third_party/
    COMMAND ${LCOV_EXECUTABLE}
            --remove "${LCOV_RAW}"
                     #"*/tests/*"
                     #"*/third_party/*"
            --output-file "${LCOV_FILTERED}"

    # 3) generate HTML into coverage_html/
    COMMAND ${GENHTML_EXECUTABLE}
            --output-directory "${HTML_DIR}"
            "${LCOV_FILTERED}"

    BYPRODUCTS
      "${LCOV_RAW}"
      "${LCOV_FILTERED}"
      "${HTML_DIR}"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Capturing & filtering coverage"
  )
endif()