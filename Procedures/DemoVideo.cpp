#include <DemoVideo.h>

#include <iostream>
#include <string>

#include <NcOps/Image.h>
#include <Vtp/ScanGridIO.h>
#include <Geometry/Basis.h>

#include <ScanGridUtilities.h>

void demoVideo() {

  /// General stuff
  /// =======================================================================

  Image imageA("/Users/bigelk/data/samples/tomography/shearCT/Processed/5.nc");
  Image imageB("/Users/bigelk/data/samples/tomography/shearCT/Processed/10.nc");
  ScanGridIO vtkPointsCt;

  std::string baseSquareFromAName = "baseSquareFromA.vtp";
  std::string baseSquareFromBName = "baseSquareFromB.vtp";

  std::string cylinderSectorFromAAndBaseSquareName =
      "cylinderSectorFromAAndBaseSquare.vtp";
  std::string cylinderSectorFromBAndBaseSquareName =
      "cylinderSectorFromBAndBaseSquare.vtp";

  double stretchZ = 0.000979212;

  /// =======================================================================


  /// Young's modulus calculation
  /// =======================================================================

  /// Length of measurement

  double shiftZ = 0.0554 - 3.60179e-05 * 90;
  double initZ = 3.60179e-05 * 90;

  /// Base square from CT A

  // Generate base square and save into vtk file
  auto squareCt = extractScanGridSquare(imageA,
                                        0.5,
                                        0.5,
                                        initZ,
                                        50,
                                        50,
                                        1000, 1000);
  vtkPointsCt.setScanGrid(squareCt);
  vtkPointsCt.saveScanGridToFile(baseSquareFromAName);

  /// Write CT A value from top square into buffer_ of squareCt

  getBaseSquareFromAWithTop(imageA, shiftZ, squareCt);

  /// Find base square from CT B

  getBaseSquareFromB(imageB, squareCt, 1.e-7);
  // Save base square from CT B into ncFile_
  vtkPointsCt.setScanGrid(squareCt);
  vtkPointsCt.saveScanGridToFile(baseSquareFromBName);

  // Take origin from base square from B
  auto bottomOrigin = *(squareCt->getBasis()->getOrigin());

  /// Find top square from CT B

  getTopSquareFromB(imageB, shiftZ, squareCt, 1.e-8);
  // Take origin_ from top square from CT B
  auto topOrigin = *(squareCt->getBasis()->getOrigin());

  /// Final Young's modulus calculation

  std::cout << std::endl << "shiftZ " << shiftZ << std::endl;
  auto distance = sqrt(CGAL::squared_distance(topOrigin, bottomOrigin));
  std::cout << std::endl << "distance " << distance << std::endl;
  auto deltaL = shiftZ - distance;
  std::cout << std::endl << "deltaL " << deltaL << std::endl;
  stretchZ = deltaL / shiftZ;
  std::cout << std::endl << "stretchZ " << stretchZ << std::endl;
  double youngsModulus = 500. / stretchZ;
  std::cout << std::endl << "youngsModulus " << youngsModulus << std::endl;

  /// =======================================================================



  /// Poisson's ratio calculation
  /// =======================================================================

  /// Take cylinder sector from CT A and base square

  // Take CT A base square from ncFile_
  auto baseCtA = std::make_shared<ScanGrid>();
  vtkPointsCt.setScanGrid(baseCtA);
  vtkPointsCt.loadScanGridFromFile(baseSquareFromAName);

  auto cylinderCt = getCylinderSectorFromCtAAndBaseSquare(imageA, baseCtA);

  // Save cylinder sector into vtk file
  vtkPointsCt.setScanGrid(cylinderCt);
  vtkPointsCt.saveScanGridToFile(cylinderSectorFromAAndBaseSquareName);

  /// Translation of cylinder sector to CT B and basis_ of base square CT B

  // Take CT B base square from vtk file
  auto baseCtB = std::make_shared<ScanGrid>();
  vtkPointsCt.setScanGrid(baseCtB);
  vtkPointsCt.loadScanGridFromFile(baseSquareFromBName);

  auto transformationA = baseCtA->getBasis()->generateTransformation();
  auto transformationB = baseCtB->getBasis()->generateTransformation();

  cylinderCt->transform(transformationA.inverse());
  cylinderCt->transform(transformationB);

  // Save cylinder sector into ncFile_
  vtkPointsCt.setScanGrid(cylinderCt);
  vtkPointsCt.saveScanGridToFile(cylinderSectorFromBAndBaseSquareName);

  // Demonstration of transformation validity
  // std::cout <<"Basis of cylinder sector from CT B:"<<std::endl;
  // std::cout << *(cylinderCt->getBasis())<<std::endl;
  // std::cout <<"Basis of base square from CT B:"<<std::endl;
  // std::cout << *(baseCtB->getBasis())<<std::endl;


  /// Find cylinder sector from CT B

  auto stretchWidth = getCylinderSectorFromB(imageB, cylinderCt);

  /// Final Poisson's ratio calculation

  auto poissonsRatio = stretchWidth / stretchZ;

  std::cout << "stretchWidth " << stretchWidth << std::endl;
  std::cout << "stretchZ " << stretchZ << std::endl;
  std::cout << "Poisson's ratio " << poissonsRatio << std::endl;

  /// =======================================================================

}

