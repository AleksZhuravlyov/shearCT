cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

project(scanGrid)

add_dependencies(geometry geometry)
find_package(CGAL)

set(CMAKE_BUILD_TYPE Release)

set(SOURCE_CODE ScanGrid.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(${PROJECT_NAME} ${SOURCE_CODE})

target_link_libraries(${PROJECT_NAME} PRIVATE
        geometry
        CGAL::CGAL)