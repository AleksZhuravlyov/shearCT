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


    double xCenter = ncCt.getXInit() + 1200 * ncCt.getXStep();
    double yCenter = ncCt.getYInit() + 1200 * ncCt.getYStep();
    double zCenter = ncCt.getZInit() + 800 * ncCt.getZStep();
    double xWidth = 600 * ncCt.getXStep();
    double yWidth = 600 * ncCt.getYStep();

    PointsCt pointsCt;
    pointsCt.createXYSquare(xCenter, yCenter, zCenter,
                            xWidth, yWidth, 1000, 1000);


    pointsCt.transform(constructXTranslation(20e-5));
    pointsCt.transform(constructXRotation(20. * M_PI / 180));
    pointsCt.transform(constructZRotation(20. * M_PI / 180));
    pointsCt.transform(constructXYStretching(2));


    /*double xCylinderBaseCenter = ncCt.getXInit() + 1200 * ncCt.getXStep();
    double yCylinderBaseCenter = ncCt.getYInit() + 1200 * ncCt.getYStep();
    double zCylinderBaseCenter = ncCt.getZInit() + 800 * ncCt.getZStep();
    double R = 300 * (ncCt.getXStep() + ncCt.getYStep()) / 2;
    double angleCenter = 0;
    double zWidth = 200 * ncCt.getZStep();
    const double &angleWidth = M_PI * 2;

    PointsCt pointsCt;
    pointsCt.createZCylinderSegment(xCylinderBaseCenter,
                                    yCylinderBaseCenter,
                                    zCylinderBaseCenter,
                                    R, angleCenter,
                                    zWidth, angleWidth,
                                    1000, 4000);

    pointsCt.transform(constructXYStretching(1.1));*/


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