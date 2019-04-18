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

    double shiftZ = 5e-2;


    NcCt ncCtA("/Volumes/ElkData/CT/samples/5.nc");
    NcCt ncCtB("/Volumes/ElkData/CT/samples/10.nc");

    auto pointsCt = std::make_shared<PointsCt>(
            createInitBaseSquare(ncCtA));
    // auto vtpCt = VtpCt(pointsCt);
    takeBaseDataFromFirstCt(pointsCt, ncCtA);
    // vtpCt.savePointsFile("InitialBottom.vtp", "0");

    auto tomoATopFirstCt = computeTomoATopFirstCt(pointsCt, ncCtA, shiftZ);

    processVariation(pointsCt, ncCtB, std::make_shared<TranslationZ>(),
                     10e-5, 21, "bottom", false);
    processVariation(pointsCt, ncCtB, std::make_shared<TranslationX>(),
                     10e-5, 21, "bottom", false);
    processVariation(pointsCt, ncCtB, std::make_shared<TranslationY>(),
                     10e-5, 21, "bottom", false);
    processVariation(pointsCt, ncCtB, std::make_shared<RotationX>(),
                     M_PI / 200, 21, "bottom", false);
    processVariation(pointsCt, ncCtB, std::make_shared<RotationY>(),
                     M_PI / 200, 21, "bottom", false);
    processVariation(pointsCt, ncCtB, std::make_shared<RotationZ>(),
                     M_PI / 200, 21, "bottom", false);
    processVariation(pointsCt, ncCtB, std::make_shared<TranslationZ>(),
                     1e-5, 21, "bottom", false);

    // vtpCt.savePointsFile("finalBottom.vtp", "1");

    pointsCt->setTomoA(tomoATopFirstCt);
    pointsCt->transform(TranslationZ()(shiftZ));
    // vtpCt.savePointsFile("initialTop.vtp", "2");



    processVariation(pointsCt, ncCtB, std::make_shared<TranslationY>(),
                     30e-5, 31, "top", false);
    processVariation(pointsCt, ncCtB, std::make_shared<TranslationX>(),
                     30e-5, 31, "top", false);
    processVariation(pointsCt, ncCtB, std::make_shared<RotationX>(),
                     M_PI / 80, 31, "top", false);
    processVariation(pointsCt, ncCtB, std::make_shared<RotationY>(),
                     M_PI / 100, 31, "top", false);

    auto offsetZTop1 = processVariation(pointsCt, ncCtB,
                                        std::make_shared<TranslationZ>(),
                                        15e-5, 41, "top", false);

    auto offsetZTop2 = processVariation(pointsCt, ncCtB,
                                        std::make_shared<TranslationZ>(),
                                        1.5e-5, 41, "top", false);


    auto stretch = -(offsetZTop1 + offsetZTop2) / shiftZ;
    std::cout << std::endl << "stretch " << stretch << std::endl;

    double youngsModulus = 500. / stretch;
    std::cout << std::endl << "youngsModulus " << youngsModulus << std::endl;

    return EXIT_SUCCESS;

}


