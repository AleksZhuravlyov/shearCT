#include <demoVideo.h>

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

  std::string baseSquareFromCtAName = "baseSquareFromCtA.vtp";
  std::string baseSquareFromCtBName = "baseSquareFromCtB.vtp";

  std::string cylinderSectorFromCtAAndBaseSquareName =
          "cylinderSectorFromCtAAndBaseSquare.vtp";
  std::string cylinderSectorFromCtBAndBaseSquareName =
          "cylinderSectorFromCtBAndBaseSquare.vtp";

  double stretchZ = 0.000979212;

  /// =======================================================================


  /// Young's modulus calculation
  /// =======================================================================

  /// Length of measurement

  double shiftZ = 0.0554 - 3.60179e-05 * 90;
  double initZ = 3.60179e-05 * 90;

  /// Base square from CT A

  // Generate base square and save into ncFile_
  auto squareCt = extractSquarePointsCt(imageA,
                                        0.5,
                                        0.5,
                                        initZ,
                                        50,
                                        50,
                                        1000, 1000);
  vtkPointsCt.setScanGrid(squareCt);
  vtkPointsCt.savePointsCtToFile(baseSquareFromCtAName);

  // Read base square from ncFile_
  // auto squareCt = getPointCtFromFile(baseSquareFromCtAName);

  /// Write CT A value from top square into buffer_ of squareCt

  getBaseSquareFromCtAWithTop(imageA, shiftZ, squareCt);

  /// Find base square from CT B

  getBaseSquareFromCtB(imageB, squareCt, 1.e-7);
  // Save base square from CT B into ncFile_
  vtkPointsCt.setScanGrid(squareCt);
  vtkPointsCt.savePointsCtToFile(baseSquareFromCtBName);

  // Read base square from CT B from ncFile_
  // auto squareCt = getPointCtFromFile(baseSquareFromCtBName);

  // Take origin_ from base square from CT B
  auto bottomOrigin = *(squareCt->getBasis()->getOrigin());

  /// Find top square from CT B

  getTopSquareFromCtB(imageB, shiftZ, squareCt, 1.e-8);
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
  vtkPointsCt.loadPointsCtFromFile(baseSquareFromCtAName);

  auto cylinderCt = getCylinderSectorFromCtAAndBaseSquare(imageA, baseCtA);

  // Save cylinder sector into ncFile_
  vtkPointsCt.setScanGrid(cylinderCt);
  vtkPointsCt.savePointsCtToFile(cylinderSectorFromCtAAndBaseSquareName);
  // Save nc region of cylinder sector into ncFile_
  // imageA.saveRegion("cylinder.nc");

  /// Translation of cylinder sector to CT B and basis_ of base square CT B

  // Take CT B base square from ncFile_
  auto baseCtB = std::make_shared<ScanGrid>();
  vtkPointsCt.setScanGrid(baseCtB);
  vtkPointsCt.loadPointsCtFromFile(baseSquareFromCtBName);

  auto transformationA = baseCtA->getBasis()->generateTransformation();
  auto transformationB = baseCtB->getBasis()->generateTransformation();

  cylinderCt->transform(transformationA.inverse());
  cylinderCt->transform(transformationB);

  // Save cylinder sector into ncFile_
  vtkPointsCt.setScanGrid(cylinderCt);
  vtkPointsCt.savePointsCtToFile(cylinderSectorFromCtBAndBaseSquareName);

  // Demonstration of transformation validity
  // std::cout <<"Basis of cylinder sector from CT B:"<<std::endl;
  // std::cout << *(cylinderCt->getBasis())<<std::endl;
  // std::cout <<"Basis of base square from CT B:"<<std::endl;
  // std::cout << *(baseCtB->getBasis())<<std::endl;


  /// Find cylinder sector from CT B

  auto stretchWidth = getCylinderSectorFromCtB(imageB, cylinderCt);

  /// Final Poisson's ratio calculation

  auto poissonsRatio = stretchWidth / stretchZ;

  std::cout << "stretchWidth " << stretchWidth << std::endl;
  std::cout << "stretchZ " << stretchZ << std::endl;
  std::cout << "Poisson's ratio " << poissonsRatio << std::endl;

  /// =======================================================================

}

