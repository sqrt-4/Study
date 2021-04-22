cmake_minimum_required(VERSION 3.17)
project(HW2)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(HW2 src/main.cpp src/BaseLogger.cpp 
	src/FileLogger.cpp src/StderrLogger.cpp 
    src/StdoutLogger.cpp src/Logger.cpp)