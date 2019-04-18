#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include <NcCt.h>
#include <VtpCt.h>
#include <Basis.h>
#include <PointsCt.h>
#include <Transformation.h>

#include <StringAndNumber.h>

#include <demo.h>


int main() {

    double shiftZ = 0.0554 - 3.60179e-05 * 90;


    NcCt ncCtA("/Volumes/ElkData/CT/samples/5.nc");
    NcCt ncCtB("/Volumes/ElkData/CT/samples/10.nc");

    // std::cout << ncCtA << std::endl;

    auto pointsCt = std::make_shared<PointsCt>(
            createInitBaseSquare(ncCtA));
    // auto vtpCt = VtpCt(pointsCt);
    takeBaseDataFromFirstCt(pointsCt, ncCtA);
    // vtpCt.savePointsFile("InitialBottom.vtp", "0");

    auto tomoATopFirstCt = computeTomoATopFirstCt(pointsCt, ncCtA, shiftZ);







    processVariation(pointsCt, ncCtB, std::make_shared<TranslationZ>(),
                     15e-5, 31, "bottom", false);
    processVariation(pointsCt, ncCtB, std::make_shared<TranslationX>(),
                     10e-5, 21, "bottom", false);
    processVariation(pointsCt, ncCtB, std::make_shared<TranslationY>(),
                     10e-5, 21, "bottom", false);
    processVariation(pointsCt, ncCtB, std::make_shared<RotationX>(),
                     M_PI / 140, 31, "bottom", false);
    processVariation(pointsCt, ncCtB, std::make_shared<RotationY>(),
                     M_PI / 200, 21, "bottom", false);
    processVariation(pointsCt, ncCtB, std::make_shared<TranslationZ>(),
                     1e-5, 21, "bottom", false);

    auto bottomOrigin = *(pointsCt->getBasis()->getOrigin());
    // std::cout << "bottomOrigin " << bottomOrigin << std::endl;

    // vtpCt.savePointsFile("finalBottom.vtp", "1");

    pointsCt->setTomoA(tomoATopFirstCt);
    pointsCt->transform(TranslationZ()(shiftZ));
    // vtpCt.savePointsFile("initialTop.vtp", "2");


    processVariation(pointsCt, ncCtB, std::make_shared<TranslationY>(),
                     40e-5, 41, "top1", false);
    processVariation(pointsCt, ncCtB, std::make_shared<TranslationX>(),
                     35e-5, 31, "top1", false);
    processVariation(pointsCt, ncCtB, std::make_shared<RotationX>(),
                     M_PI / 30, 41, "top1", false);
    processVariation(pointsCt, ncCtB, std::make_shared<RotationY>(),
                     M_PI / 50, 41, "top1", false);
    processVariation(pointsCt, ncCtB, std::make_shared<TranslationZ>(),
                     19e-5, 51, "top1", false);
    processVariation(pointsCt, ncCtB, std::make_shared<TranslationZ>(),
                     1.5e-5, 41, "top1", false);


    processVariation(pointsCt, ncCtB, std::make_shared<StretchingXY>(),
                     0.0005, 40, "top1", false);


    processVariation(pointsCt, ncCtB, std::make_shared<TranslationY>(),
                     15e-5, 41, "top2", false);
    processVariation(pointsCt, ncCtB, std::make_shared<TranslationX>(),
                     5e-5, 31, "top2", false);
    processVariation(pointsCt, ncCtB, std::make_shared<RotationX>(),
                     M_PI / 70, 100, "top2", false);
    processVariation(pointsCt, ncCtB, std::make_shared<RotationY>(),
                     M_PI / 150, 41, "top2", false);
    processVariation(pointsCt, ncCtB, std::make_shared<TranslationZ>(),
                     9e-5, 61, "top2", false);
    processVariation(pointsCt, ncCtB, std::make_shared<TranslationZ>(),
                     .7e-5, 61, "top2", false);


    auto topOrigin = *(pointsCt->getBasis()->getOrigin());
    // std::cout << "topOrigin " << topOrigin << std::endl;


    std::cout << std::endl;
    std::cout << "shiftZ " << shiftZ << std::endl;


    auto baseDistance = sqrt(CGAL::squared_distance(
            topOrigin, bottomOrigin));


    std::cout << std::endl;
    std::cout << "baseDistance " << baseDistance << std::endl;

    auto baseDeltaL = shiftZ - baseDistance;

    std::cout << std::endl;
    std::cout << "baseDeltaL " << baseDeltaL << std::endl;


    auto baseStretch = baseDeltaL / shiftZ;
    std::cout << std::endl << "baseStretch " << baseStretch << std::endl;


    double baseYoungsModulus = 500. / baseStretch;
    std::cout << std::endl << "baseYoungsModulus " <<
              baseYoungsModulus << std::endl;


    return EXIT_SUCCESS;

}


