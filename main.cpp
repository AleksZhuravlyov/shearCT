#include <iostream>
#include <string>
#include <cmath>

#include <NcCt.h>
#include <VtpCt.h>
#include <Geom.h>

#include <StringAndNumber.h>

int main() {

    auto ncFileName = std::string("/Volumes/ElkData/CT/samples/10.nc");

    NcCt ncCt(ncFileName);

    auto dims = ncCt.getDims();

    std::cout << ncCt << std::endl;


    ncCt.setRegionCt({500, 500, 500}, {10, 200, 200});

    ncCt.saveRegionCt("/Volumes/ElkData/CT/tmp/_10Cut.nc");

    auto points = std::make_shared<Points>();


    double step = 3.60179e-05;
    double z = 505 * step;
    for (double x = 550 * step; x < 650 * step; x += step / 50)
        for (double y = 550 * step; y < 650 * step; y += step / 50)
            points->push_back(Point(x, y, z));

    VtpCt vtpCt(points);
    auto result = vtpCt.getResult();

    /*auto transformation = constructZRotation(1. * M_PI / 180);

    for (auto &&point : *points)
        point = point.transform(transformation);*/

    ncCt.regionCt.setPoints(points, result);

    ncCt.regionCt.computePointsVal();


    vtpCt.savePointsFile("result.vtp", "0");


    return EXIT_SUCCESS;
}