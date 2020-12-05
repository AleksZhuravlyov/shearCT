cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

set(POJECT_NAME shearCT)

project(${POJECT_NAME})

if (APPLE AND EXISTS /usr/local/opt/qt5)
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif ()

add_subdirectory(geometry)
add_subdirectory(ncOps)
add_subdirectory(scanGrid)
add_subdirectory(vtkOps)
add_subdirectory(registration)
add_subdirectory(procedures)
add_subdirectory(scripts)

set(SOURCE_CODE main.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_BUILD_TYPE Release)

add_executable(${POJECT_NAME} ${SOURCE_CODE})

target_link_libraries(${POJECT_NAME} PRIVATE
        geometry
        ncOps
        scanGrid
        vtkOps
        registration
        procedures)