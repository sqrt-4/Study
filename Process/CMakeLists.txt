cmake_minimum_required(VERSION 3.17)
project(HW1)

set(CMAKE_CXX_STANDARD 17)

add_executable(HW1 src/main.cpp src/process.cpp)
target_include_directories(HW1 PUBLIC include)