cmake_minimum_required(VERSION 3.13)
project(shearCT)

set(CMAKE_CXX_STANDARD 17)

include_directories(NcCt)
include_directories(PointsCt)

add_executable(shearCT main.cpp
        NcCt/NcCt.cpp
        PointsCt/PointsCt.cpp)