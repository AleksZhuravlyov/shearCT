cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

project(registration)

add_dependencies(geometry geometry)
add_dependencies(ncOps ncOps)
add_dependencies(scanGrid scanGrid)
add_dependencies(geometry geometry)
add_dependencies(vtkOps vtkOps)

find_package(dlib REQUIRED)

set(CMAKE_BUILD_TYPE Release)

set(SOURCE_CODE Registration.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(${PROJECT_NAME} ${SOURCE_CODE})

target_link_libraries(${PROJECT_NAME} PRIVATE
        geometry
        ncOps
        scanGrid
        geometry
        vtkOps
        dlib::dlib)