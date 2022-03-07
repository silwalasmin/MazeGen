cmake_minimum_required(VERSION 3.21)
project(MazeGen)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)
add_executable(MazeGen src/main.cpp)
target_link_libraries(MazeGen  X11 GL pthread png stdc++fs)