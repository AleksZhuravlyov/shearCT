#include <Procedures/ScanGridUtilities.h>

#include <cmath>

#include <VtkOps/ScanGridIO.h>
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
      -M_PI / 7., -M_PI / 11.5, -M_PI / 90.};
  auto constraintsMax = std::vector<double>{
      20e-5, 20e-5, 20e-5,
      M_PI / 90., M_PI / 90., M_PI / 45.};

  std::string transformationType = "linear";
  std::string registrationType = "bottom";
  std::cout << transformationType << " " << registrationType << std::endl;
  makeRegistration(image, scanGridBase, transformationType, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, false);

}

void searchScanGridBaseWithStretch(Image &image, std::shared_ptr<ScanGrid> &scanGridBase,
                                   const double &accuracy) {

  auto constraintsMin = std::vector<double>{
      -20e-5, -20e-5, -20e-5,
      -M_PI / 7., -M_PI / 11.5, -M_PI / 90., 0.99};
  auto constraintsMax = std::vector<double>{
      20e-5, 20e-5, 20e-5,
      M_PI / 90., M_PI / 90., M_PI / 45., 1.01};

  std::string transformationType = "linearWithStretchingXY";
  std::string registrationType = "bottom";
  std::cout << transformationType << " " << registrationType << std::endl;
  makeRegistration(image, scanGridBase, transformationType, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, false);

}


void searchScanGridTop(Image &image, const double &shiftZ,
                       std::shared_ptr<ScanGrid> &scanGridBase,
                       const double &accuracy) {

  scanGridBase->swapTomoAAndBuffer();
  scanGridBase->transform(TranslationZ()(shiftZ));

  auto constraintsMin = std::vector<double>{
      -100e-5, -100e-5, -100e-5,
      -M_PI / 15., -M_PI / 15., -M_PI / 15.};
  auto constraintsMax = std::vector<double>{
      100e-5, 100e-5, 100e-5,
      M_PI / 15., M_PI / 15., M_PI / 15.};

  std::string transformationType = "linear";
  std::string registrationType = "top";
  std::cout << transformationType << " " << registrationType << std::endl;
  makeRegistration(image, scanGridBase, transformationType, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, false);

}

void searchScanGridTopWithStretch(Image &image, const double &shiftZ,
                                  std::shared_ptr<ScanGrid> &scanGridBase,
                                  const double &accuracy) {

  scanGridBase->swapTomoAAndBuffer();
  scanGridBase->transform(TranslationZ()(shiftZ));

  auto constraintsMin = std::vector<double>{
      -100e-5, -100e-5, -100e-5,
      -M_PI / 15., -M_PI / 15., -M_PI / 15., 0.99};
  auto constraintsMax = std::vector<double>{
      100e-5, 100e-5, 100e-5,
      M_PI / 15., M_PI / 15., M_PI / 15., 1.01};

  std::string transformationType = "linearWithStretchingXY";
  std::string registrationType = "top";
  std::cout << transformationType << " " << registrationType << std::endl;
  makeRegistration(image, scanGridBase, transformationType, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, false);

}


std::shared_ptr<ScanGrid> extractScanGridCylinder(
    Image &image, std::shared_ptr<ScanGrid> &scanGridBase) {

  auto origin = scanGridBase->getBasis()->getOrigin();

  double R = image.getXStep() * 700;
  double angleCenter = 0.;
  double zWidth = image.getZStep() * 30;
  double angleWidth = M_PI * 2.;
  int nZ = 200;
  int nAngle = 7000;

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

  auto constraintsMin = std::vector<double>{0.99};
  auto constraintsMax = std::vector<double>{1.01};


  std::string transformationType = "XYStretching";
  std::string registrationType = "side";
  std::cout << transformationType << " " << registrationType << std::endl;
  auto answerVector = makeRegistration(image, scanGridCylinder,
                                       transformationType, 1.e-13,
                                       constraintsMin, constraintsMax,
                                       registrationType,
                                       false);

  return answerVector[0] - 1;

}
