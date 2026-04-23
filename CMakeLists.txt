cmake_minimum_required(VERSION 3.16)
project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json so IDEs (clangd, VS Code C/C++) pick up include
# paths for sources in src/, benchmarks/, etc. without extra configuration.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.3
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)

set(SUDOKU_SOURCES
    src/square.cpp
    src/board.cpp
)

add_executable(hello main.cpp ${SUDOKU_SOURCES})
target_include_directories(hello PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(bench benchmarks/bench_permutations.cpp ${SUDOKU_SOURCES})
target_include_directories(bench PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(bench PRIVATE benchmark::benchmark_main)
