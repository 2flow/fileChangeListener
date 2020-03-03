cmake_minimum_required(VERSION 3.17)

set(CMAKE_Sznc_COMPILER g++)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)
project(Display Sync)
set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(SOURCES src/main.cpp)
#file(GLOB SOURCES "src/*.cpp")

add_executable(testMain $(SOURCES))
