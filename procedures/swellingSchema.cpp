#include "swellingSchema.h"

#include <iostream>
#include <variant>
#include <vector>

#include <ncOps/Image.h>
#include <vtkOps/ScanGridIO.h>
#include <geometry/Basis.h>


#include "scanGridUtilities.h"

std::map<std::string,
        std::variant<double, std::vector<double>>> swellingSchema(
        std::map<std::string,
                std::variant<bool, int, double, std::string, std::vector<double>>> params) {

    /// General stuff
    /// =======================================================================


    Image imageA(std::get<std::string>(params["imageAFileName"]),
                 std::get<std::string>(params["valueName"]));

    Image imageB(std::get<std::string>(params["imageBFileName"]),
                 std::get<std::string>(params["valueName"]));
    ScanGridIO scanGridIo;

    // std::string baseSquareFromAName = "baseSquareFromA.vtp";
    // std::string baseSquareFromBName = "baseSquareFromB.vtp";

    // std::string cylinderSectorFromAAndBaseSquareName =
    //         "cylinderSectorFromAAndBaseSquare.vtp";
    // std::string cylinderSectorFromBAndBaseSquareName =
    //         "cylinderSectorFromBAndBaseSquare.vtp";

    /// =======================================================================


    auto &shiftZ = std::get<double>(params["shiftZ"]);

    /// Base square from CT A

    // Generate base square and save into vtk file
    auto scanGridSquare = extractScanGridSquare(imageA,
                                                std::get<double>(params["xCenter"]),
                                                std::get<double>(params["yCenter"]),
                                                std::get<double>(params["initZ"]),
                                                std::get<double>(params["xWidth"]),
                                                std::get<double>(params["yWidth"]),
                                                std::get<int>(params["nX"]),
                                                std::get<int>(params["nY"]));
    // Set basis to pivot
    // scanGridSquare->translateBasis(0.037494, 0.037872, initZ);

    // scanGridIo.setScanGrid(scanGridSquare);
    // scanGridIo.saveScanGridToFile(baseSquareFromAName);

    // Read base square from vtk file
    // auto scanGridSquare = std::make_shared<scanGrid>();
    // scanGridIo.setScanGrid(scanGridSquare);
    // scanGridIo.loadScanGridFromFile(baseSquareFromAName);

    /// Write CT A value from top square into buffer of scanGridSquare

    writeShiftedZScanGridToBuffer(imageA, shiftZ, scanGridSquare);

    /// Find base square from CT B


    auto answerBase = searchScanGridBaseWithStretch(
            imageB, scanGridSquare, std::get<double>(params["baseAccuracy"]),
            std::get<std::vector<double>>(params["baseConstraintsMin"]),
            std::get<std::vector<double>>(params["baseConstraintsMax"]),
            std::get<bool>(params["isVerbose"]));
    // scanGridIo.setScanGrid(scanGridSquare);
    // scanGridIo.saveScanGridToFile(baseSquareFromBName);

    // Take origin from base square from B
    auto bottomOrigin = *(scanGridSquare->getBasis()->getXYZOrigin());

    /// Find top square from CT B

    auto answerTop = searchScanGridTopWithStretch(
            imageB, shiftZ, scanGridSquare, std::get<double>(params["topAccuracy"]),
            std::get<std::vector<double>>(params["topConstraintsMin"]),
            std::get<std::vector<double>>(params["topConstraintsMax"]),
            std::get<bool>(params["isVerbose"]));
    // Take origin from top square from CT B
    auto topOrigin = *(scanGridSquare->getBasis()->getXYZOrigin());


    // std::cout << std::endl << "shiftZ " << shiftZ << std::endl;

    double distance = 0;
    for (int i = 0; i < 3; i++)
        distance += pow(topOrigin[i] - bottomOrigin[i], 2);
    distance = sqrt(distance);
    // std::cout << std::endl << "distance " << distance << std::endl;

    auto deltaL = distance - shiftZ;
    // std::cout << std::endl << "deltaL " << deltaL << std::endl;
    double stretchZ = 1 + deltaL / shiftZ;
    // std::cout << std::endl << "stretchZ " << stretchZ << std::endl;

    std::map<std::string, std::variant<double, std::vector<double>>> answer;
    answer["stretch"] = stretchZ;
    answer["answerBase"] = answerBase;
    answer["answerTop"] = answerTop;

    return answer;

}

