cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

set(POJECT_NAME shearCT)

project(${POJECT_NAME})


find_package(VTK COMPONENTS
        vtkCommonCore
        vtkCommonDataModel
        vtkIOXML QUIET
        )


set(SOURCE_CODE main.cpp
        NcCt/NcCt.cpp
        VtpCt/VtpCt.cpp)

include_directories(NcCt)
include_directories(VtpCt)
include_directories(/usr/local/Cellar/netcdf/4.6.2/include)

add_executable(${POJECT_NAME} ${SOURCE_CODE})

target_link_libraries(${POJECT_NAME}
        PRIVATE ${VTK_LIBRARIES}
        /usr/local/lib/libnetcdf-cxx4.dylib
        )