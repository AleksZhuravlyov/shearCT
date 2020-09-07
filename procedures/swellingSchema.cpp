#include "swellingSchema.h"

#include <iostream>
#include <string>

#include <ncOps/Image.h>
#include <vtkOps/ScanGridIO.h>
#include <geometry/Basis.h>

#include "scanGridUtilities.h"

void swellingSchema() {

    /// General stuff
    /// =======================================================================


    Image imageA("/Users/bigelk/tmp/microct/zak/CO2Z0al_01.nc", "array");
    std::cout << "test" << std::endl;

    Image imageB("/Users/bigelk/tmp/microct/zak/CO2Z0al_10.nc", "array");
    ScanGridIO scanGridIo;

    std::string baseSquareFromAName = "baseSquareFromA.vtp";
    std::string baseSquareFromBName = "baseSquareFromB.vtp";

    std::string cylinderSectorFromAAndBaseSquareName =
            "cylinderSectorFromAAndBaseSquare.vtp";
    std::string cylinderSectorFromBAndBaseSquareName =
            "cylinderSectorFromBAndBaseSquare.vtp";

    /// =======================================================================


    double shiftZ = 0.002965;
    shiftZ = 0.001186;
    double initZ = 0.0008895;

    /// Base square from CT A

    // Generate base square and save into vtk file
    auto scanGridSquare = extractScanGridSquare(imageA,
                                                0.00360544,
                                                0.00364102,
                                                initZ,
                                                0.0047,
                                                0.0047,
                                                300, 300);
    // Set basis to pivot
    // scanGridSquare->translateBasis(0.037494, 0.037872, initZ);

    scanGridIo.setScanGrid(scanGridSquare);
    scanGridIo.saveScanGridToFile(baseSquareFromAName);

    // Read base square from vtk file
    /*auto scanGridSquare = std::make_shared<scanGrid>();
    scanGridIo.setScanGrid(scanGridSquare);
    scanGridIo.loadScanGridFromFile(baseSquareFromAName);*/

    /// Write CT A value from top square into buffer of scanGridSquare

    writeShiftedZScanGridToBuffer(imageA, shiftZ, scanGridSquare);

    /// Find base square from CT B

    auto constraintsMin = std::vector<double>{
            -30e-6, -30e-6, -30e-6,
            -M_PI / 40., -M_PI / 40., -M_PI / 40., 0.99};
    auto constraintsMax = std::vector<double>{
            30e-6, 30e-6, 30e-6,
            M_PI / 40., M_PI / 40., M_PI / 40., 1.01};
    searchScanGridBaseWithStretch(imageB, scanGridSquare, 1.e-10,
                                  constraintsMin,
                                  constraintsMax);
    scanGridIo.setScanGrid(scanGridSquare);
    scanGridIo.saveScanGridToFile(baseSquareFromBName);

    // Take origin from base square from B
    auto bottomOrigin = *(scanGridSquare->getBasis()->getXYZOrigin());

    /// Find top square from CT B

    constraintsMin = std::vector<double>{
            -30e-6, -30e-6, -30e-6,
            -M_PI / 15., -M_PI / 15., -M_PI / 15., 0.99};
    constraintsMax = std::vector<double>{
            30e-6, 30e-6, 30e-6,
            M_PI / 15., M_PI / 15., M_PI / 15., 1.01};
    searchScanGridTopWithStretch(imageB, shiftZ, scanGridSquare, 1.e-9,
                                 constraintsMin,
                                 constraintsMax);
    // Take origin from top square from CT B
    auto topOrigin = *(scanGridSquare->getBasis()->getXYZOrigin());


    std::cout << std::endl << "shiftZ " << shiftZ << std::endl;

    double distance = 0;
    for (int i = 0; i < 3; i++)
        distance += pow(topOrigin[i] - bottomOrigin[i], 2);
    distance = sqrt(distance);
    std::cout << std::endl << "distance " << distance << std::endl;

    auto deltaL = distance - shiftZ;
    std::cout << std::endl << "deltaL " << deltaL << std::endl;
    double stretchZ = 1 + deltaL / shiftZ;
    std::cout << std::endl << "stretchZ " << stretchZ << std::endl;

}

