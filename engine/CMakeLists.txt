cmake_minimum_required(VERSION 3.28)
project(uttt_engine CXX)
# Vendored (Task 1, pinned): doctest v2.4.12 sha256 94029a7d32da24a56249658147dbd2b33ff0b9ed665295cbbaf19aafff5b0ced
#                            nlohmann/json v3.12.0 sha256 aaf127c04cb31c406e5b04a63f1ae89369fccde6d8fa7cdda1ed4f32dfc5de63
# nlohmann is linked into adapter/test targets ONLY, never uttt_core.
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-std=c++2c -Wall -Wextra -Werror)

add_library(uttt_core
  src/core/local_table.cpp
  src/core/position.cpp
  src/core/zobrist.cpp)
target_include_directories(uttt_core PUBLIC src)

add_executable(uttt_tests
  tests/test_smoke.cpp
  tests/test_types.cpp
  tests/test_naive_local.cpp
  tests/test_local_table.cpp
  tests/test_position.cpp)
target_link_libraries(uttt_tests PRIVATE uttt_core)
target_include_directories(uttt_tests PRIVATE third_party)

enable_testing()
add_test(NAME unit COMMAND uttt_tests)
