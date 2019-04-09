cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

set(POJECT_NAME shearCT)

project(${POJECT_NAME})


find_package(VTK COMPONENTS
        vtkCommonCore
        vtkCommonDataModel
        vtkIOXML QUIET
        )

find_package(pugixml)

find_package(CGAL)


set(SOURCE_CODE main.cpp
        NcCt/NcCt.cpp
        VtpCt/VtpCt.cpp
        Geom/Geom.cpp)

include_directories(.)
include_directories(NcCt)
include_directories(VtpCt)
include_directories(Geom)
include_directories(/usr/local/Cellar/netcdf/4.6.2/include)

set(CMAKE_BUILD_TYPE Release)

add_executable(${POJECT_NAME} ${SOURCE_CODE})

target_link_libraries(${POJECT_NAME} PRIVATE
        ${VTK_LIBRARIES}
        pugixml
        /usr/local/lib/libnetcdf-cxx4.dylib
        CGAL::CGAL)