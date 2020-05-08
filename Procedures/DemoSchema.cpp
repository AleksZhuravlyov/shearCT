#include <Procedures/DemoSchema.h>

#include <iostream>
#include <string>

#include <NcOps/Image.h>
#include <VtkOps/ScanGridIO.h>
#include <Geometry/Basis.h>

#include <Procedures/ScanGridUtilities.h>

void demoSchema() {

  /// General stuff
  /// =======================================================================


  Image imageA("/Users/bigelk/data/samples/tomography/shearCT/Processed/5.nc");
  Image imageB("/Users/bigelk/data/samples/tomography/shearCT/Processed/10.nc");
  ScanGridIO scanGridIo;

  std::string baseSquareFromAName = "baseSquareFromA.vtp";
  std::string baseSquareFromBName = "baseSquareFromB.vtp";

  std::string cylinderSectorFromAAndBaseSquareName =
      "cylinderSectorFromAAndBaseSquare.vtp";
  std::string cylinderSectorFromBAndBaseSquareName =
      "cylinderSectorFromBAndBaseSquare.vtp";


  /// =======================================================================


  /// Young's modulus calculation
  /// =======================================================================

  /// Length of measurement

  double shiftZ = 0.0554 - 3.60179e-05 * 90;
  double initZ = 3.60179e-05 * 90;

  /// Base square from CT A

  // Generate base square and save into vtk file
  auto scanGridSquare = extractScanGridSquare(imageA,
                                              0.0432,
                                              0.0432,
                                              initZ,
                                              0.0252,
                                              0.0252,
                                              100, 100);
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
      -20e-5, -20e-5, -20e-5,
      -M_PI / 180., -M_PI / 180., -M_PI / 180.};
  auto constraintsMax = std::vector<double>{
      20e-5, 20e-5, 20e-5,
      M_PI / 180., M_PI / 180., M_PI / 180.};
  searchScanGridBase(imageB, scanGridSquare, 1.e-7,
                     constraintsMin, constraintsMax);
  scanGridIo.setScanGrid(scanGridSquare);
  scanGridIo.saveScanGridToFile(baseSquareFromBName);

  // Take origin from base square from B
  auto bottomOrigin = *(scanGridSquare->getBasis()->getOrigin());

  /// Find top square from CT B

  constraintsMin = std::vector<double>{
      -25e-5, -25e-5, -25e-5,
      -M_PI / 100., -M_PI / 100., -M_PI / 100.};
  constraintsMax = std::vector<double>{
      25e-5, 25e-5, 25e-5,
      M_PI / 100., M_PI / 100., M_PI / 100.};
  searchScanGridTop(imageB, shiftZ, scanGridSquare, 1.e-8,
                    constraintsMin, constraintsMax);
  // Take origin from top square from CT B
  auto topOrigin = *(scanGridSquare->getBasis()->getOrigin());

  /// Final Young's modulus calculation

  std::cout << std::endl << "shiftZ " << shiftZ << std::endl;
  auto distance = sqrt(CGAL::squared_distance(topOrigin, bottomOrigin));
  std::cout << std::endl << "distance " << distance << std::endl;
  auto deltaL = shiftZ - distance;
  std::cout << std::endl << "deltaL " << deltaL << std::endl;
  double stretchZ = deltaL / shiftZ;
  std::cout << std::endl << "stretchZ " << stretchZ << std::endl;
  double youngsModulus = 500. / stretchZ;
  std::cout << std::endl << "youngsModulus " << youngsModulus << std::endl;

  /// =======================================================================



  /// Poisson's ratio calculation
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

  /// Final Poisson's ratio calculation

  auto poissonsRatio = stretchWidth / stretchZ;

  std::cout << "stretchWidth " << stretchWidth << std::endl;
  std::cout << "stretchZ " << stretchZ << std::endl;
  std::cout << "Poisson's ratio " << poissonsRatio << std::endl;

  /// =======================================================================

}

