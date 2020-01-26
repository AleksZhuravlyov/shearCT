#include <Procedures/ScanGridUtilities.h>

#include <cmath>

#include <Vtp/ScanGridIO.h>
#include <Registration/Registration.h>
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

  auto scanGridBase = std::make_shared<ScanGrid>();
  scanGridBase->createXYSquare(xCenter, yCenter, zCenter,
                               xWidth, yWidth,
                               nX, nY);

  image.setRegion(scanGridBase->generateBbox());
  image.region.setPoints(scanGridBase->getPoints(), scanGridBase->getTomoA());
  image.region.computePointsValue();

  return scanGridBase;

}


void writeShiftedZScanGridToBuffer(Image &image, const double &shiftZ,
                                   std::shared_ptr<ScanGrid> &scanGridBase) {

  scanGridBase->transform(TranslationZ()(shiftZ));
  image.setRegion(scanGridBase->generateBbox());
  image.region.setPoints(scanGridBase->getPoints(), scanGridBase->getBuffer());
  image.region.computePointsValue();
  scanGridBase->transform(TranslationZ()(-shiftZ));

}

void searchScanGridBase(Image &image, std::shared_ptr<ScanGrid> &scanGridBase,
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
  makeRegistration(image, scanGridBase, transformationType, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, true);

}

void searchScanGridTop(Image &image, const double &shiftZ,
                       std::shared_ptr<ScanGrid> &scanGridBase,
                       const double &accuracy) {

  scanGridBase->swapTomoAAndBuffer();
  scanGridBase->transform(TranslationZ()(shiftZ));

  auto constraintsMin = std::vector<double>{
      -25e-5, -25e-5, -25e-5,
      -M_PI / 100., -M_PI / 100., -M_PI / 100.};
  auto constraintsMax = std::vector<double>{
      25e-5, 25e-5, 25e-5,
      M_PI / 100., M_PI / 100., M_PI / 100.};

  std::string transformationType = "linear";
  std::string registrationType = "top";
  std::cout << transformationType << " " << registrationType << std::endl;
  makeRegistration(image, scanGridBase, "linear", accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, false);

}


std::shared_ptr<ScanGrid> extractScanGridCylinder(
    Image &image, std::shared_ptr<ScanGrid> &scanGridBase) {

  auto origin = scanGridBase->getBasis()->getOrigin();

  double R = image.getXStep() * 500;
  double angleCenter = M_PI * 1.5;
  double zWidth = image.getZStep() * 50;
  double angleWidth = M_PI / 10;
  int nZ = 500;
  int nAngle = 5000;

  auto scanGridCylinder = std::make_shared<ScanGrid>();
  scanGridCylinder->createZCylinderSegment(origin->x(), origin->y(),
                                           origin->z(),
                                           R, angleCenter, zWidth, angleWidth,
                                           nZ, nAngle);

  image.setRegion(scanGridCylinder->generateBbox());
  image.region.setPoints(scanGridCylinder->getPoints(),
                         scanGridCylinder->getTomoA());
  image.region.computePointsValue();

  return scanGridCylinder;

}


double searchScanGridCylinder(Image &image,
                              std::shared_ptr<ScanGrid> &scanGridCylinder) {

  auto constraintsMin = std::vector<double>{0.98};
  auto constraintsMax = std::vector<double>{1.02};


  std::string transformationType = "XYStretching";
  std::string registrationType = "side";
  std::cout << transformationType << " " << registrationType << std::endl;
  auto answerVector = makeRegistration(image, scanGridCylinder,
                                       transformationType, 1.e-12,
                                       constraintsMin, constraintsMax,
                                       registrationType,
                                       false);

  return answerVector[0] - 1;

}
