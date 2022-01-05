cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

project(ncOps)

find_package(netCDF)
find_package(CGAL)

set(CMAKE_BUILD_TYPE Release)

set(SOURCE_CODE Image.cpp Region.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(${PROJECT_NAME} ${SOURCE_CODE})

target_link_libraries(${PROJECT_NAME} PRIVATE
        /opt/homebrew/lib/libnetcdf-cxx4.dylib
        CGAL::CGAL)