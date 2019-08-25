#include <iostream>
#include <string>

#include <NcCt.h>
#include <VtpCt.h>
#include <Basis.h>
#include <PointsCt.h>
#include <Transformation.h>

#include <demo.h>

int main() {

    /// General stuff
    /// =======================================================================

    NcCt ncCtA("/Volumes/ElkData/CT/samples/5.nc");
    NcCt ncCtB("/Volumes/ElkData/CT/samples/10.nc");
    VtpCt vtpCt;

    std::string baseSquareFromCtAName = "baseSquareFromCtA.vtp";
    std::string baseSquareFromCtBName = "baseSquareFromCtB.vtp";

    std::string cylinderSectorFromCtAAndBaseSquareName =
            "cylinderSectorFromCtAAndBaseSquare.vtp";
    std::string cylinderSectorFromCtBAndBaseSquareName =
            "cylinderSectorFromCtBAndBaseSquare.vtp";

    double stretchLength = 0.000979212;

    /// =======================================================================


    /// Young's modulus calculation
    /// =======================================================================

    /// Length of measurement

    double shiftZ = 0.0554 - 3.60179e-05 * 90;

    /// Base square from CT A

    // Generate base square and save into file
    auto squareCt = getBaseSquareFromCtA(ncCtA);
    vtpCt.setPointsCt(squareCt);
    vtpCt.savePointsFile(baseSquareFromCtAName);

    // Read base square from file
    // auto squareCt = getPointCtFromFile(baseSquareFromCtAName);

    /// Write CT A value from top square into tomoBuffer of squareCt

    getBaseSquareFromCtAWithTop(ncCtA, shiftZ, squareCt);

    /// Find base square from CT B

    getBaseSquareFromCtB(ncCtB, squareCt);
    // Save base square from CT B into file
    vtpCt.setPointsCt(squareCt);
    vtpCt.savePointsFile(baseSquareFromCtBName);

    // Read base square from CT B from file
    // auto squareCt = getPointCtFromFile(baseSquareFromCtBName);

    // Take origin from base square from CT B
    auto bottomOrigin = *(squareCt->getBasis()->getOrigin());

    /// Find top square from CT B

    getTopSquareFromCtB(ncCtB, shiftZ, squareCt);
    // Take origin from top square from CT B
    auto topOrigin = *(squareCt->getBasis()->getOrigin());

    /// Final Young's modulus calculation

    std::cout << std::endl << "shiftZ " << shiftZ << std::endl;
    auto distance = sqrt(CGAL::squared_distance(topOrigin, bottomOrigin));
    std::cout << std::endl << "distance " << distance << std::endl;
    auto deltaL = shiftZ - distance;
    std::cout << std::endl << "deltaL " << deltaL << std::endl;
    stretchLength = deltaL / shiftZ;
    std::cout << std::endl << "stretchLength " << stretchLength << std::endl;
    double youngsModulus = 500. / stretchLength;
    std::cout << std::endl << "youngsModulus " << youngsModulus << std::endl;

    /// =======================================================================



    /// Poisson's ratio calculation
    /// =======================================================================

    /// Take cylinder sector from CT A and base square

    // Take CT A base square from file
    auto baseCtA = getPointCtFromFile(baseSquareFromCtAName);

    auto cylinderCt = getCylinderSectorFromCtAAndBaseSquare(ncCtA, baseCtA);

    // Save cylinder sector into file
    vtpCt.setPointsCt(cylinderCt);
    vtpCt.savePointsFile(cylinderSectorFromCtAAndBaseSquareName);
    // Save nc region of cylinder sector into file
    // ncCtA.saveRegionCt("cylinder.nc");

    /// Translation of cylinder sector to CT B and basis of base square CT B

    // Take CT B base square from file
    auto baseCtB = getPointCtFromFile(baseSquareFromCtBName);

    auto transformationA = baseCtA->getBasis()->generateTransformation();
    auto transformationB = baseCtB->getBasis()->generateTransformation();

    cylinderCt->transform(transformationA.inverse());
    cylinderCt->transform(transformationB);

    // Save cylinder sector into file
    vtpCt.setPointsCt(cylinderCt);
    vtpCt.savePointsFile(cylinderSectorFromCtBAndBaseSquareName);

    // Demonstration of transformation validity
    // std::cout <<"Basis of cylinder sector from CT B:"<<std::endl;
    // std::cout << *(cylinderCt->getBasis())<<std::endl;
    // std::cout <<"Basis of base square from CT B:"<<std::endl;
    // std::cout << *(baseCtB->getBasis())<<std::endl;


    /// Find cylinder sector from CT B

    auto stretchWidth = getCylinderSectorFromCtB(ncCtB, cylinderCt);

    /// Final Poisson's ratio calculation

    auto poissonsRatio = stretchWidth / stretchLength;

    std::cout << "stretchWidth " << stretchWidth << std::endl;
    std::cout << "stretchLength " << stretchLength << std::endl;
    std::cout << "Poisson's ratio " << poissonsRatio << std::endl;

    /// =======================================================================


    return EXIT_SUCCESS;

}
