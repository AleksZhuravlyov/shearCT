#include <iostream>
#include <string>
#include <cmath>

#include <NcCt.h>
#include <VtpCt.h>
#include <Geom.h>
#include <PointsCt.h>

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
    ncCt.setRegionCt(bbox);
    ncCt.saveRegionCt("/Volumes/ElkData/CT/tmp/_10Cut.nc");


    auto pointsCt = PointsCt(points);
    auto result = pointsCt.getResult();


    auto vtpCt = VtpCt(std::make_shared<PointsCt>(pointsCt));
    vtpCt.setFileIsBinary(false);


    ncCt.regionCt.setPoints(points, result);
    ncCt.regionCt.computePointsVal();


    vtpCt.savePointsFile("result.vtp", "0");


    /*PointsCt pointsCt;
    VtpCt vtpCt(std::make_shared<PointsCt>(pointsCt));
    vtpCt.readPointsFile("result.vtp");
    vtpCt.savePointsFile("rewriteResult.vtp", "0");*/


    return EXIT_SUCCESS;
}