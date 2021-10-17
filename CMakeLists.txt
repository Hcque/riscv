project(RISC_V)

set(CMAKE_FLAGS "-g -std=c++11 -Wall")
set(CMAKE_BUILD_TYPE Debug)

add_executable(run main.cpp)