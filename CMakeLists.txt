cmake_minimum_required(VERSION 3.16)
project(sudoku-structs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json so IDEs (clangd, VS Code C/C++) pick up include
# paths (src/ for public headers) for sources in src/, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
# GoogleTest for unit tests (driven by CTest via gtest_discover_tests).
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

set(SUDOKU_SOURCES
    src/square.cpp
    src/board.cpp
)

add_executable(sudoku-structs main.cpp ${SUDOKU_SOURCES})
target_include_directories(sudoku-structs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

enable_testing()

add_executable(sudoku_tests tests/test_board_square_gtest.cpp ${SUDOKU_SOURCES})
target_include_directories(sudoku_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)
target_link_libraries(sudoku_tests PRIVATE GTest::gtest_main)
gtest_discover_tests(sudoku_tests)
