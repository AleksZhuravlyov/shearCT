#include <demoSchema.h>

#include <iostream>
#include <string>

#include <NcOps/Image.h>
#include <Vtp/PointsIO.h>
#include <Geometry/Basis.h>

#include <points.h>

void demoSchema() {

  /// General stuff
  /// =======================================================================

  Image imageA("/Users/bigelk/data/samples/tomography/shearCT/Processed/5.nc");
  Image imageB("/Users/bigelk/data/samples/tomography/shearCT/Processed/10.nc");
  PointsIO vtkPointsCt;

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

  // Generate base square and save into file
  auto squareCt = extractSquarePointsCt(imageA,
                                        0.5,
                                        0.5,
                                        initZ,
                                        20,
                                        20,
                                        100, 100);
  vtkPointsCt.setPointsCt(squareCt);
  vtkPointsCt.savePointsCtToFile(baseSquareFromCtAName);

  // Read base square from file
  // auto squareCt = getPointCtFromFile(baseSquareFromCtAName);

  /// Write CT A value from top square into tomoBuffer of squareCt

  getBaseSquareFromCtAWithTop(imageA, shiftZ, squareCt);

  /// Find base square from CT B

  getBaseSquareFromCtB(imageB, squareCt, 1.e-7);
  // Save base square from CT B into file
  vtkPointsCt.setPointsCt(squareCt);
  vtkPointsCt.savePointsCtToFile(baseSquareFromCtBName);

  // Read base square from CT B from file
  // auto squareCt = getPointCtFromFile(baseSquareFromCtBName);

  // Take origin from base square from CT B
  auto bottomOrigin = *(squareCt->getBasis()->getOrigin());

  /// Find top square from CT B

  getTopSquareFromCtB(imageB, shiftZ, squareCt, 1.e-8);
  // Take origin from top square from CT B
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

  // Take CT A base square from file

  auto baseCtA = std::make_shared<Points>();
  vtkPointsCt.setPointsCt(baseCtA);
  vtkPointsCt.loadPointsCtFromFile(baseSquareFromCtAName);

  // auto baseCtA = getPointCtFromFile(baseSquareFromCtAName);

  auto cylinderCt = getCylinderSectorFromCtAAndBaseSquare(imageA, baseCtA);

  // Save cylinder sector into file
  vtkPointsCt.setPointsCt(cylinderCt);
  vtkPointsCt.savePointsCtToFile(cylinderSectorFromCtAAndBaseSquareName);
  // Save nc region of cylinder sector into file
  // imageA.saveRegion("cylinder.nc");

  /// Translation of cylinder sector to CT B and basis of base square CT B

  // Take CT B base square from file
  auto baseCtB = std::make_shared<Points>();
  vtkPointsCt.setPointsCt(baseCtB);
  vtkPointsCt.loadPointsCtFromFile(baseSquareFromCtBName);


  auto transformationA = baseCtA->getBasis()->generateTransformation();
  auto transformationB = baseCtB->getBasis()->generateTransformation();

  cylinderCt->transform(transformationA.inverse());
  cylinderCt->transform(transformationB);

  // Save cylinder sector into file
  vtkPointsCt.setPointsCt(cylinderCt);
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

