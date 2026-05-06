cmake_minimum_required(VERSION 3.10)
project(RISCVEmulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

# Main emulator executable
add_executable(emulator src/main.cpp src/emulator.cpp)
target_include_directories(emulator PRIVATE include)

# Interactive emulator executable
add_executable(emulator_interactive src/interactive.cpp src/emulator.cpp)
target_include_directories(emulator_interactive PRIVATE include)

# Enable testing
enable_testing()

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Test executable
add_executable(emulator_tests tests/emulator_test.cpp src/emulator.cpp)
target_link_libraries(emulator_tests gtest gtest_main)
target_include_directories(emulator_tests PRIVATE include)

add_test(NAME EmulatorTests COMMAND emulator_tests)