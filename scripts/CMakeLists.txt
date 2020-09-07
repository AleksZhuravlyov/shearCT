cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)

project(scripts)

find_package(pybind11 REQUIRED)

add_dependencies(procedures procedures)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

pybind11_add_module(${PROJECT_NAME} wrapper.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE procedures)

set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../../scripts)
