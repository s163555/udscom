cmake_minimum_required(VERSION 3.20)
project(udscom LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(third_party/FTXUI EXCLUDE_FROM_ALL)

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
target_link_libraries(udscom_tui PRIVATE udscom)
