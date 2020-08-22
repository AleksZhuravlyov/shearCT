cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

project(vtkOps)

add_dependencies(geometry geometry)
add_dependencies(scanGrid scanGrid)

find_package(VTK REQUIRED)
find_package(pugixml)

set(CMAKE_BUILD_TYPE Release)

set(SOURCE_CODE ScanGridIO.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(${PROJECT_NAME} ${SOURCE_CODE})

target_link_libraries(${PROJECT_NAME} PRIVATE
        geometry
        scanGrid
        ${VTK_LIBRARIES}
        pugixml)