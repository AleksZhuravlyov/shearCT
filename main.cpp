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
    double z = (0.5 + 505) * step;
    for (double x = 560 * step; x < 640 * step; x += 0.1 * step)
        for (double y = 560 * step; y < 640 * step; y += 0.1 * step)
            points->push_back(Point(x, y, z));

    VtpCt vtpCt(points);
    auto result = vtpCt.getResult();

    auto transformation = constructZRotation(1. * M_PI / 180);

    for (auto &&point : *points)
        point = point.transform(transformation);

    ncCt.regionCt.setPoints(points, result);

    ncCt.regionCt.computePointsVal();


    vtpCt.savePointsFile("result.vtp", "0");


    return EXIT_SUCCESS;
}