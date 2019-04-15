#include <iostream>
#include <string>
#include <cmath>

#include <NcCt.h>
#include <VtpCt.h>
#include <Basis.h>
#include <PointsCt.h>
#include <Transformation.h>

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


    auto pointsCt = PointsCt(points);
    pointsCt.transform(constructXTranslation(20e-5));
    pointsCt.transform(constructXRotation(20. * M_PI / 180));
    pointsCt.transform(constructZRotation(20. * M_PI / 180));
    pointsCt.transform(constructXYStretching(2));


    ncCt.setRegionCt(pointsCt.generateBbox());
    ncCt.saveRegionCt("/Volumes/ElkData/CT/tmp/_10Cut.nc");

    ncCt.regionCt.setPoints(pointsCt.getPoints(), pointsCt.getResult());
    ncCt.regionCt.computePointsVal();


    auto vtpCt = VtpCt(std::make_shared<PointsCt>(pointsCt));
    vtpCt.savePointsFile("result.vtp", "0");


    PointsCt pointsCtRead;
    VtpCt vtpCtRead(std::make_shared<PointsCt>(pointsCtRead));
    vtpCtRead.readPointsFile("result.vtp");
    vtpCtRead.savePointsFile("rewriteResult.vtp", "0");


    return EXIT_SUCCESS;
}