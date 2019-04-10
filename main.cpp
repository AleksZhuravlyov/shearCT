#include <iostream>
#include <string>
#include <cmath>

#include <NcCt.h>
#include <VtpCt.h>
#include <Geom.h>
#include <RegionCt.h>

#include <StringAndNumber.h>

int main() {

    auto ncFileName = std::string("/Volumes/ElkData/CT/samples/10.nc");

    NcCt ncCt(ncFileName);

    auto dims = ncCt.getDims();

    std::cout << ncCt << std::endl;

    ncCt.setRegionCt({500, 600, 600}, {30, 400, 500});

    ncCt.saveRegionCt("/Volumes/ElkData/CT/tmp/_10Cut.nc");


    return EXIT_SUCCESS;
}