#include <Procedures/ScanGridUtilities.h>

#include <cmath>

#include <Vtp/ScanGridIO.h>
#include <Registration.h>
#include <Geometry/Translation.h>


std::shared_ptr<ScanGrid> extractSquarePointsCt(Image &image,
                                                const double &xCenterFactor,
                                                const double &yCenterFactor,
                                                const double &zCenterMeter,
                                                const double &xWidthVoxel,
                                                const double &yWidthVoxel,
                                                const int &nX, const int &nY) {

  double xCenter = image.getXInit() +
                   (image.getXStep() * image.getNX()) * xCenterFactor;
  double yCenter = image.getYInit() +
                   (image.getYStep() * image.getNY()) * yCenterFactor;
  double zCenter = image.getZInit() + zCenterMeter;

  double xWidth = xWidthVoxel * image.getXStep();
  double yWidth = yWidthVoxel * image.getYStep();

  auto scanGrid = std::make_shared<ScanGrid>();
  scanGrid->createXYSquare(xCenter, yCenter, zCenter,
                           xWidth, yWidth,
                           nX, nY);

  image.setRegion(scanGrid->generateBbox());
  image.region.setPoints(scanGrid->getPoints(), scanGrid->getTomoA());
  image.region.computePointsValue();

  return scanGrid;

}


void getBaseSquareFromCtAWithTop(Image &image, const double &shiftZ,
                                 std::shared_ptr<ScanGrid> &scanGrid) {

  scanGrid->transform(TranslationZ()(shiftZ));
  image.setRegion(scanGrid->generateBbox());
  image.region.setPoints(scanGrid->getPoints(), scanGrid->getBuffer());
  image.region.computePointsValue();
  scanGrid->transform(TranslationZ()(-shiftZ));

}

std::shared_ptr<ScanGrid> getBaseSquareFromCtAWithTop(
    Image &image, const double &shiftZ, const std::string &fileName) {

  auto scanGrid = std::make_shared<ScanGrid>();

  auto vtkPointsCt = ScanGridIO();
  vtkPointsCt.setScanGrid(scanGrid);
  vtkPointsCt.loadPointsCtFromFile(fileName);

  getBaseSquareFromCtAWithTop(image, shiftZ, scanGrid);

  return scanGrid;

}


void getBaseSquareFromCtB(Image &image, std::shared_ptr<ScanGrid> &scanGrid,
                          const double &accuracy) {

  // processVariation(scanGrid, image, std::make_shared<TranslationZ>(),
  //                  20e-5, 11, "bottom",
  //                  false);
  // processVariation(scanGrid, image, std::make_shared<TranslationX>(),
  //                  10e-5, 21, "bottom",
  //                  false);
  // processVariation(scanGrid, image, std::make_shared<TranslationY>(),
  //                  10e-5, 21, "bottom",
  //                  false);
  // processVariation(scanGrid, image, std::make_shared<RotationX>(),
  //                  M_PI / 140, 31, "bottom",
  //                  false);
  // processVariation(scanGrid, image, std::make_shared<RotationY>(),
  //                  M_PI / 200, 21, "bottom",
  //                  false);
  // processVariation(scanGrid, image, std::make_shared<TranslationZ>(),
  //                  1e-5, 21, "bottom",
  //                  false);


  auto transformations = generateTranslationAnaRotationXYZ();
  auto constraintsMin = std::vector<double>{
          -20e-5, -20e-5, -20e-5,
          -M_PI / 180., -M_PI / 180., -M_PI / 180.};
  auto constraintsMax = std::vector<double>{
          20e-5, 20e-5, 20e-5,
          M_PI / 180., M_PI / 180., M_PI / 180.};

  std::string registrationType = "XYZTAndRTop";
  std::cout << registrationType << std::endl;
  makeRegistration(image, scanGrid, transformations, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, true);

}

std::shared_ptr<ScanGrid> getBaseSquareFromCtB(Image &ncCt,
                                               const std::string &fileName,
                                               const double &accuracy) {

  auto scanGrid = std::make_shared<ScanGrid>();

  auto vtkPointsCt = ScanGridIO();
  vtkPointsCt.setScanGrid(scanGrid);
  vtkPointsCt.loadPointsCtFromFile(fileName);


  getBaseSquareFromCtB(ncCt, scanGrid, accuracy);

  return scanGrid;

}


void getTopSquareFromCtB(Image &image, const double &shiftZ,
                         std::shared_ptr<ScanGrid> &scanGrid,
                         const double &accuracy) {

  scanGrid->swapTomoAAndTomoBuffer();
  scanGrid->transform(TranslationZ()(shiftZ));


  // processVariation(scanGrid, image, std::make_shared<TranslationY>(),
  //                  40e-5, 41, "top1", false);
  // processVariation(scanGrid, image, std::make_shared<TranslationX>(),
  //                  35e-5, 31, "top1", false);
  // processVariation(scanGrid, image, std::make_shared<RotationX>(),
  //                  M_PI / 30, 41, "top1", false);
  // processVariation(scanGrid, image, std::make_shared<RotationY>(),
  //                  M_PI / 50, 41, "top1", false);
  // processVariation(scanGrid, image, std::make_shared<TranslationZ>(),
  //                  19e-5, 51, "top1", false);
  // processVariation(scanGrid, image, std::make_shared<TranslationZ>(),
  //                  1.5e-5, 41, "top1", false);
  //
  //
  // processVariation(scanGrid, image, std::make_shared<TranslationY>(),
  //                  15e-5, 41, "top2", false);
  // processVariation(scanGrid, image, std::make_shared<TranslationX>(),
  //                  5e-5, 31, "top2", false);
  // processVariation(scanGrid, image, std::make_shared<RotationX>(),
  //                  M_PI / 70, 100, "top2", false);
  // processVariation(scanGrid, image, std::make_shared<RotationY>(),
  //                  M_PI / 150, 41, "top2", false);
  // processVariation(scanGrid, image, std::make_shared<TranslationZ>(),
  //                  9e-5, 61, "top2", false);
  // processVariation(scanGrid, image, std::make_shared<TranslationZ>(),
  //                  .7e-5, 61, "top2", false);


  auto transformations = generateTranslationAnaRotationXYZ();
  auto constraintsMin = std::vector<double>{
          -25e-5, -25e-5, -25e-5,
          -M_PI / 100., -M_PI / 100., -M_PI / 100.};
  auto constraintsMax = std::vector<double>{
          25e-5, 25e-5, 25e-5,
          M_PI / 100., M_PI / 100., M_PI / 100.};

  std::string registrationType = "XYZTAndRBot";
  std::cout << registrationType << std::endl;
  makeRegistration(image, scanGrid, transformations, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, false);


}


std::shared_ptr<ScanGrid> getCylinderSectorFromCtAAndBaseSquare(
    Image &image, std::shared_ptr<ScanGrid> &baseSquare) {

  auto origin = baseSquare->getBasis()->getOrigin();

  double R = image.getXStep() * 500;
  double angleCenter = M_PI * 1.5;
  double zWidth = image.getZStep() * 50;
  double angleWidth = M_PI / 10;
  int nZ = 500;
  int nAngle = 5000;

  auto scanGrid = std::make_shared<ScanGrid>();
  scanGrid->createZCylinderSegment(origin->x(), origin->y(), origin->z(),
                                   R, angleCenter, zWidth, angleWidth,
                                   nZ, nAngle);

  image.setRegion(scanGrid->generateBbox());
  image.region.setPoints(scanGrid->getPoints(), scanGrid->getTomoA());
  image.region.computePointsValue();

  return scanGrid;

}


double getCylinderSectorFromCtB(Image &image,
                                std::shared_ptr<ScanGrid> &scanGrid) {

  // processVariation(scanGrid, image,
  //                  std::make_shared<StretchXY>(),
  //                  0.007, 21, "cylinder1", false);
  // processVariation(scanGrid, image,
  //                  std::make_shared<StretchXY>(),
  //                  0.0007, 21, "cylinder2", false);
  // processVariation(scanGrid, image,
  //                  std::make_shared<StretchXY>(),
  //                  0.0002, 21, "cylinder3", false);
  // processVariation(scanGrid, image,
  //                  std::make_shared<StretchXY>(),
  //                               0.00015, 21, "cylinder4", false);

  // return mult1 * mult2 * mult3 * mult4 - 1;

  auto transformations = generateStretchingXY();
  auto constraintsMin = std::vector<double>{0.98};
  auto constraintsMax = std::vector<double>{1.02};

  std::string registrationType = "XYStretching";
  std::cout << registrationType << std::endl;
  auto answerVector = makeRegistration(image, scanGrid,
                                       transformations, 1.e-12,
                                       constraintsMin, constraintsMax,
                                       registrationType,
                                       false);

  return answerVector[0] - 1;

}
