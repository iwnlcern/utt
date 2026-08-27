cmake_minimum_required(VERSION 3.28)
project(uttt_engine CXX)
# Vendored (Task 1, pinned): doctest v2.4.12 sha256 94029a7d32da24a56249658147dbd2b33ff0b9ed665295cbbaf19aafff5b0ced
#                            nlohmann/json v3.12.0 sha256 aaf127c04cb31c406e5b04a63f1ae89369fccde6d8fa7cdda1ed4f32dfc5de63
# nlohmann is linked into adapter/test targets ONLY, never uttt_core.
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(UTTT_COMPILE_OPTIONS -Wall -Wextra -Werror -O3)
add_compile_options(${UTTT_COMPILE_OPTIONS})

string(TOUPPER "${CMAKE_BUILD_TYPE}" UTTT_BUILD_TYPE_UPPER)
set(UTTT_CONFIG_CXX_FLAGS "${CMAKE_CXX_FLAGS_${UTTT_BUILD_TYPE_UPPER}}")
list(JOIN UTTT_COMPILE_OPTIONS " " UTTT_COMPILE_OPTIONS_TEXT)
set(UTTT_COMPILE_FLAGS_VALUE
  "${CMAKE_CXX_FLAGS} ${UTTT_CONFIG_CXX_FLAGS} -std=c++${CMAKE_CXX_STANDARD} ${UTTT_COMPILE_OPTIONS_TEXT}")
string(STRIP "${UTTT_COMPILE_FLAGS_VALUE}" UTTT_COMPILE_FLAGS_VALUE)

add_library(uttt_core
  src/core/local_table.cpp
  src/core/position.cpp
  src/core/zobrist.cpp
  src/search/backup.hpp
  src/search/game_model.hpp
  src/search/search.cpp
  src/search/search.hpp
  src/search/tt.cpp
  src/search/tt.hpp
  src/search/tvalue.hpp)
target_include_directories(uttt_core PUBLIC src)

add_executable(uttt_tests
  tests/test_smoke.cpp
  tests/test_backup.cpp
  tests/test_game_model.cpp
  tests/test_cutoffs.cpp
  tests/test_search_core.cpp
  tests/test_tt.cpp
  tests/test_ttt3_oracles.cpp
  tests/test_rational.cpp
  tests/test_tvalue.cpp
  tests/test_types.cpp
  tests/test_naive_local.cpp
  tests/test_local_table.cpp
  tests/test_position.cpp
  tests/test_lifecycle.cpp
  tests/test_zobrist.cpp
  tests/test_perft.cpp
  tests/test_fixtures.cpp
  tests/test_wire.cpp
  src/adapter/wire.cpp)
target_link_libraries(uttt_tests PRIVATE uttt_core)
target_include_directories(uttt_tests PRIVATE third_party)

add_executable(uttt_engine
  src/adapter/main.cpp
  src/adapter/wire.cpp)
target_link_libraries(uttt_engine PRIVATE uttt_core)
target_include_directories(uttt_engine PRIVATE third_party)

add_executable(uttt_bench bench/bench_playout.cpp)
target_link_libraries(uttt_bench PRIVATE uttt_core)
target_include_directories(uttt_bench PRIVATE third_party)
target_compile_definitions(uttt_bench PRIVATE
  UTTT_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
  UTTT_COMPILE_FLAGS="${UTTT_COMPILE_FLAGS_VALUE}")

enable_testing()
add_test(NAME unit COMMAND uttt_tests)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
add_test(NAME e2e COMMAND ${Python3_EXECUTABLE}
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_engine_e2e.py
  $<TARGET_FILE:uttt_engine>
  ${CMAKE_CURRENT_SOURCE_DIR}/..)
