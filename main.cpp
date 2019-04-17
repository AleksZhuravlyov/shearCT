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


    /*double step = 3.60179e-05;
    double xCenter = 700 * step;
    double yCenter = 700 * step;
    double zCenter = 700 * step;
    double xWidth = 400 * step;
    double yWidth = 400 * step;

    PointsCt pointsCt;
    pointsCt.createXYSquare(xCenter, yCenter, zCenter,
                            xWidth, yWidth, 1000, 1000);


    pointsCt.transform(constructXTranslation(20e-5));
    pointsCt.transform(constructXRotation(20. * M_PI / 180));
    pointsCt.transform(constructZRotation(20. * M_PI / 180));
    pointsCt.transform(constructXYStretching(2));*/

    double step = 3.60179e-05;
    double xCylinderBaseCenter = 700 * step;
    double yCylinderBaseCenter = 700 * step;
    double zCylinderBaseCenter = 700 * step;
    double R = 200 * step;
    double angleCenter = 0;
    double zWidth = 200 * step;
    const double &angleWidth = M_PI * 2;

    PointsCt pointsCt;
    pointsCt.createZCylinderSegment(xCylinderBaseCenter,
                                    yCylinderBaseCenter,
                                    zCylinderBaseCenter,
                                    R, angleCenter,
                                    zWidth, angleWidth,
                                    1000, 2000);

    pointsCt.transform(constructXYStretching(1.1));


    ncCt.setRegionCt(pointsCt.generateBbox());
    ncCt.saveRegionCt("/Volumes/ElkData/CT/tmp/_10Cut.nc");

    ncCt.regionCt.setPoints(pointsCt.getPoints(), pointsCt.getResult());
    ncCt.regionCt.computePointsValue();


    auto vtpCt = VtpCt(std::make_shared<PointsCt>(pointsCt));
    vtpCt.savePointsFile("result.vtp", "0");


    /*PointsCt pointsCtRead;
    VtpCt vtpCtRead(std::make_shared<PointsCt>(pointsCtRead));
    vtpCtRead.readPointsFile("result.vtp");
    vtpCtRead.savePointsFile("rewriteResult.vtp", "0");*/


    return EXIT_SUCCESS;
}