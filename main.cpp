#include <iostream>
#include <string>

#include <NcCt.h>
#include <PointsCt.h>

int main() {

    auto ncFileName = std::string("/Volumes/ElkData/CT/samples/10.nc");

    NcCt ncCt(ncFileName);

    auto dims = ncCt.getDims();

    std::cout << ncCt << std::endl;

    auto region = Region({{500, 600,  600},
                          {20,  1800, 1800}});

    ncCt.setRegion(region);

    ncCt.saveRegion("/Volumes/ElkData/CT/tmp/10Cut.nc");

    return EXIT_SUCCESS;
}