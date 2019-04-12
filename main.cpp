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


    auto points = std::make_shared<Points>();


    double step = 3.60179e-05;
    double z = 505 * step;
    for (double x = 550 * step; x < 660 * step; x += step / 10)
        for (double y = 550 * step; y < 660 * step; y += step / 10)
            points->push_back(Point(x, y, z));

    auto transformation1 = constructXRotation(7. * M_PI / 180);

    for (auto &&point : *points)
        point = point.transform(transformation1);

    auto transformation2 = constructZRotation(7. * M_PI / 180);

    for (auto &&point : *points)
        point = point.transform(transformation2);


    auto bbox = CGAL::bbox_3(points->begin(), points->end());

    std::cout << bbox << std::endl;


    ncCt.setRegionCt(bbox);

    // ncCt.setRegionCt({500, 520, 520}, {10, 160, 160});

    ncCt.saveRegionCt("/Volumes/ElkData/CT/tmp/_10Cut.nc");


    VtpCt vtpCt(points);
    auto result = vtpCt.getResult();


    ncCt.regionCt.setPoints(points, result);

    ncCt.regionCt.computePointsVal();


    vtpCt.savePointsFile("result.vtp", "0");


    return EXIT_SUCCESS;
}