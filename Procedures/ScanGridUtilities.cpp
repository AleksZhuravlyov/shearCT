#include <Procedures/ScanGridUtilities.h>

#include <cmath>

#include <Vtp/ScanGridIO.h>
#include <Registration.h>
#include <Geometry/Translation.h>


std::shared_ptr<ScanGrid> extractScanGridSquare(Image &image,
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


void getBaseSquareFromAWithTop(Image &image, const double &shiftZ,
                               std::shared_ptr<ScanGrid> &scanGrid) {

  scanGrid->transform(TranslationZ()(shiftZ));
  image.setRegion(scanGrid->generateBbox());
  image.region.setPoints(scanGrid->getPoints(), scanGrid->getBuffer());
  image.region.computePointsValue();
  scanGrid->transform(TranslationZ()(-shiftZ));

}

std::shared_ptr<ScanGrid> getBaseSquareFromAWithTop(
    Image &image, const double &shiftZ, const std::string &fileName) {

  auto scanGrid = std::make_shared<ScanGrid>();

  auto vtkPointsCt = ScanGridIO();
  vtkPointsCt.setScanGrid(scanGrid);
  vtkPointsCt.loadScanGridFromFile(fileName);

  getBaseSquareFromAWithTop(image, shiftZ, scanGrid);

  return scanGrid;

}


void getBaseSquareFromB(Image &image, std::shared_ptr<ScanGrid> &scanGrid,
                        const double &accuracy) {

  auto constraintsMin = std::vector<double>{
      -20e-5, -20e-5, -20e-5,
      -M_PI / 180., -M_PI / 180., -M_PI / 180.};
  auto constraintsMax = std::vector<double>{
      20e-5, 20e-5, 20e-5,
      M_PI / 180., M_PI / 180., M_PI / 180.};

  std::string transformationType = "linear";
  std::string registrationType = "bottom";
  std::cout << transformationType << " " << registrationType << std::endl;
  makeRegistration(image, scanGrid, transformationType, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, true);

}

std::shared_ptr<ScanGrid> getBaseSquareFromCtB(Image &ncCt,
                                               const std::string &fileName,
                                               const double &accuracy) {

  auto scanGrid = std::make_shared<ScanGrid>();

  auto vtkPointsCt = ScanGridIO();
  vtkPointsCt.setScanGrid(scanGrid);
  vtkPointsCt.loadScanGridFromFile(fileName);


  getBaseSquareFromB(ncCt, scanGrid, accuracy);

  return scanGrid;

}


void getTopSquareFromB(Image &image, const double &shiftZ,
                       std::shared_ptr<ScanGrid> &scanGrid,
                       const double &accuracy) {

  scanGrid->swapTomoAAndBuffer();
  scanGrid->transform(TranslationZ()(shiftZ));

  auto constraintsMin = std::vector<double>{
      -25e-5, -25e-5, -25e-5,
      -M_PI / 100., -M_PI / 100., -M_PI / 100.};
  auto constraintsMax = std::vector<double>{
      25e-5, 25e-5, 25e-5,
      M_PI / 100., M_PI / 100., M_PI / 100.};

  std::string transformationType = "linear";
  std::string registrationType = "top";
  std::cout << transformationType << " " << registrationType << std::endl;
  makeRegistration(image, scanGrid, "linear", accuracy,
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


double getCylinderSectorFromB(Image &image,
                              std::shared_ptr<ScanGrid> &scanGrid) {

  auto constraintsMin = std::vector<double>{0.98};
  auto constraintsMax = std::vector<double>{1.02};


  std::string transformationType = "XYStretching";
  std::string registrationType = "side";
  std::cout << transformationType << " " << registrationType << std::endl;
  auto answerVector = makeRegistration(image, scanGrid,
                                       transformationType, 1.e-12,
                                       constraintsMin, constraintsMax,
                                       registrationType,
                                       false);

  return answerVector[0] - 1;

}
