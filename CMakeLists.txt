cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

set(POJECT_NAME shearCT)

project(${POJECT_NAME})

find_package(VTK REQUIRED)

find_package(pugixml)

find_package(CGAL)

find_package(dlib REQUIRED)

find_package(Boost REQUIRED COMPONENTS thread system)

set(SOURCE_CODE main.cpp
        NcCt/NcCt.cpp
        VtpCt/VtpCt.cpp
        Geom/Basis.cpp
        NcCt/RegionCt.cpp
        PointsCt/PointsCt.cpp
        Geom/Transformation.cpp
        Procedures/variation.cpp
        Procedures/points.cpp
        Registration/Registration.cpp Procedures/points.cpp Procedures/points.h Procedures/demoSchema.cpp Procedures/demoSchema.h Procedures/stretchZvsShift.cpp Procedures/stretchZvsShift.h)

include_directories(Registration)
include_directories(Procedures)
include_directories(ThirdParty)
include_directories(NcCt)
include_directories(VtpCt)
include_directories(Geom)
include_directories(PointsCt)
include_directories(/usr/local/Cellar/netcdf/4.6.3_1/include)

set(CMAKE_BUILD_TYPE Release)

add_executable(${POJECT_NAME} ${SOURCE_CODE})

target_link_libraries(${POJECT_NAME} PRIVATE
        ${VTK_LIBRARIES}
        pugixml
        /usr/local/lib/libnetcdf-cxx4.dylib
        CGAL::CGAL
        dlib::dlib)