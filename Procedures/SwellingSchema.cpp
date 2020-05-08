#include <Procedures/SwellingSchema.h>

#include <iostream>
#include <string>

#include <NcOps/Image.h>
#include <VtkOps/ScanGridIO.h>
#include <Geometry/Basis.h>

#include <Procedures/ScanGridUtilities.h>

void swellingSchema() {

  /// General stuff
  /// =======================================================================


  Image imageA("/Users/bigelk/data/workspace/swelling/registered/C2K12B.nc");
  Image imageB("/Users/bigelk/data/workspace/swelling/registered/C2K13B.nc");
  ScanGridIO scanGridIo;

  std::string baseSquareFromAName = "baseSquareFromA.vtp";
  std::string baseSquareFromBName = "baseSquareFromB.vtp";

  std::string cylinderSectorFromAAndBaseSquareName =
      "cylinderSectorFromAAndBaseSquare.vtp";
  std::string cylinderSectorFromBAndBaseSquareName =
      "cylinderSectorFromBAndBaseSquare.vtp";

  /// =======================================================================


  double shiftZ = 0.02376;
  double initZ = 0.01728;

  /// Base square from CT A

  // Generate base square and save into vtk file
  auto scanGridSquare = extractScanGridSquare(imageA,
                                              0.035838,
                                              0.04005,
                                              initZ,
                                              0.032364,
                                              0.032364,
                                              700, 700);
  // Set basis to pivot
  scanGridSquare->translateBasis(0.037494, 0.037872, initZ);

  scanGridIo.setScanGrid(scanGridSquare);
  scanGridIo.saveScanGridToFile(baseSquareFromAName);

  // Read base square from vtk file
  /*auto scanGridSquare = std::make_shared<ScanGrid>();
  scanGridIo.setScanGrid(scanGridSquare);
  scanGridIo.loadScanGridFromFile(baseSquareFromAName);*/

  /// Write CT A value from top square into buffer of scanGridSquare

  writeShiftedZScanGridToBuffer(imageA, shiftZ, scanGridSquare);

  /// Find base square from CT B

  auto constraintsMin = std::vector<double>{
      -40e-5, -40e-5, -20e-5,
      -M_PI / 90., -M_PI / 90., -M_PI / 90., 0.99};
  auto constraintsMax = std::vector<double>{
      20e-5, 20e-5, 20e-5,
      M_PI / 90., M_PI / 90., M_PI / 90., 1.01};
  searchScanGridBaseWithStretch(imageB, scanGridSquare, 1.e-8,
                                constraintsMin,
                                constraintsMax);
  scanGridIo.setScanGrid(scanGridSquare);
  scanGridIo.saveScanGridToFile(baseSquareFromBName);

  // Take origin from base square from B
  auto bottomOrigin = *(scanGridSquare->getBasis()->getOrigin());

  /// Find top square from CT B

  constraintsMin = std::vector<double>{
      -100e-5, -100e-5, -100e-5,
      -M_PI / 15., -M_PI / 15., -M_PI / 15., 0.99};
  constraintsMax = std::vector<double>{
      100e-5, 100e-5, 100e-5,
      M_PI / 15., M_PI / 15., M_PI / 15., 1.01};
  searchScanGridTopWithStretch(imageB, shiftZ, scanGridSquare, 1.e-8,
                               constraintsMin,
                               constraintsMax);
  // Take origin from top square from CT B
  auto topOrigin = *(scanGridSquare->getBasis()->getOrigin());


  std::cout << std::endl << "shiftZ " << shiftZ << std::endl;
  auto distance = sqrt(CGAL::squared_distance(topOrigin, bottomOrigin));
  std::cout << std::endl << "distance " << distance << std::endl;
  auto deltaL = shiftZ - distance;
  std::cout << std::endl << "deltaL " << deltaL << std::endl;
  double stretchZ = deltaL / shiftZ;
  std::cout << std::endl << "stretchZ " << stretchZ << std::endl;


  /// =======================================================================

  /// Take cylinder sector from CT A and base square

  // Take CT A base square from vtk file

  auto scanGridBaseA = std::make_shared<ScanGrid>();
  scanGridIo.setScanGrid(scanGridBaseA);
  scanGridIo.loadScanGridFromFile(baseSquareFromAName);

  auto scanGridCylinder = extractScanGridCylinder(imageA, scanGridBaseA,
                                                  0.0252, 0.,
                                                  0.00108, M_PI * 2.,
                                                  200, 7000);

  // Save cylinder sector into vtk file
  scanGridIo.setScanGrid(scanGridCylinder);
  scanGridIo.saveScanGridToFile(cylinderSectorFromAAndBaseSquareName);
  // Save nc region of cylinder sector into netCDF file
  // imageA.saveRegion("cylinder.nc");

  /// Translation of cylinder sector to B and basis of base square B

  // Take CT B base square from vtk file
  auto scanGridBaseB = std::make_shared<ScanGrid>();
  scanGridIo.setScanGrid(scanGridBaseB);
  scanGridIo.loadScanGridFromFile(baseSquareFromBName);


  auto transformationA = scanGridBaseA->getBasis()->generateTransformation();
  auto transformationB = scanGridBaseB->getBasis()->generateTransformation();

  scanGridCylinder->transform(transformationA.inverse());
  scanGridCylinder->transform(transformationB);

  // Save cylinder sector into vtk file
  scanGridIo.setScanGrid(scanGridCylinder);
  scanGridIo.saveScanGridToFile(cylinderSectorFromBAndBaseSquareName);

  /// Find cylinder sector from CT B

  constraintsMin = std::vector<double>{0.99};
  constraintsMax = std::vector<double>{1.01};
  auto stretchWidth = searchScanGridCylinder(imageB, scanGridCylinder,
                                             constraintsMin,
                                             constraintsMax);

  std::cout << "stretchWidth " << stretchWidth << std::endl;
  std::cout << "stretchZ " << stretchZ << std::endl;

  /// =======================================================================

}

