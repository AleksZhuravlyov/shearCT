cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

set(POJECT_NAME shearCT)

project(${POJECT_NAME})

find_package(netCDF)
find_package(VTK REQUIRED)
find_package(pugixml)
find_package(CGAL)
find_package(dlib REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)

set(SOURCE_CODE main.cpp

        NcOps/Image.cpp
        NcOps/Region.cpp

        VtkOps/ScanGridIO.cpp

        Geometry/Basis.cpp
        Geometry/Translation.cpp
        Geometry/Rotation.cpp
        Geometry/Stretch.cpp

        ScanGrid/ScanGrid.cpp

        Registration/Registration.cpp

        Procedures/UniformSearch.cpp
        Procedures/ScanGridUtilities.cpp

        Procedures/DemoSchema.cpp
        Procedures/SwellingSchema.cpp)

include_directories(.)

set(CMAKE_BUILD_TYPE Release)

add_executable(${POJECT_NAME} ${SOURCE_CODE})

target_link_libraries(${POJECT_NAME} PRIVATE
        ${VTK_LIBRARIES}
        pugixml
        /usr/local/lib/libnetcdf-cxx4.dylib
        CGAL::CGAL
        dlib::dlib)