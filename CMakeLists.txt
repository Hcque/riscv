cmake_minimum_required(VERSION 3.10)
project(RISCV_Simulator)

set(CMAKE_CXX_STANDARD 14)


set(CMAKE_BUILD_TYPE Debug)


add_library(RISCV_Simulator INTERFACE)
target_include_directories(RISCV_Simulator INTERFACE src/)

add_executable(RISCV_Simulator_Test src/test1.cpp)
add_executable(code src/main.cpp)
# add_executable(RISCV_Simulator_Pre src/main_presentation.cpp)

find_package(GTest MODULE REQUIRED)

target_link_libraries(RISCV_Simulator_Test PRIVATE RISCV_Simulator GTest::GTest GTest::Main)
target_link_libraries(code RISCV_Simulator)
# target_link_libraries(RISCV_Simulator_Pre RISCV_Simulator)

enable_testing()
add_test(AllTestsInMain RISCV_Simulator_Test)
