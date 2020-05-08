#include <Procedures/ScanGridUtilities.h>

#include <cmath>

#include <VtkOps/ScanGridIO.h>
#include <Registration/Registration.h>
#include <Geometry/Translation.h>


std::shared_ptr<ScanGrid> extractScanGridSquare(Image &image,
                                                const double &xCenter,
                                                const double &yCenter,
                                                const double &zCenter,
                                                const double &xWidth,
                                                const double &yWidth,
                                                const int &nX, const int &nY) {


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
                        const double &accuracy,
                        const std::vector<double> &constraintsMin,
                        const std::vector<double> &constraintsMax) {

  std::string transformationType = "linear";
  std::string registrationType = "bottom";
  std::cout << transformationType << " " << registrationType << std::endl;
  makeRegistration(image, scanGridBase, transformationType, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, false);

}

void searchScanGridBaseWithStretch(Image &image,
                                   std::shared_ptr<ScanGrid> &scanGridBase,
                                   const double &accuracy,
                                   const std::vector<double> &constraintsMin,
                                   const std::vector<double> &constraintsMax) {

  std::string transformationType = "linearWithStretchingXY";
  std::string registrationType = "bottom";
  std::cout << transformationType << " " << registrationType << std::endl;
  makeRegistration(image, scanGridBase, transformationType, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, false);

}


void searchScanGridTop(Image &image, const double &shiftZ,
                       std::shared_ptr<ScanGrid> &scanGridBase,
                       const double &accuracy,
                       const std::vector<double> &constraintsMin,
                       const std::vector<double> &constraintsMax) {

  scanGridBase->swapTomoAAndBuffer();
  scanGridBase->transform(TranslationZ()(shiftZ));

  std::string transformationType = "linear";
  std::string registrationType = "top";
  std::cout << transformationType << " " << registrationType << std::endl;
  makeRegistration(image, scanGridBase, transformationType, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, false);

}

void searchScanGridTopWithStretch(Image &image, const double &shiftZ,
                                  std::shared_ptr<ScanGrid> &scanGridBase,
                                  const double &accuracy,
                                  const std::vector<double> &constraintsMin,
                                  const std::vector<double> &constraintsMax) {

  scanGridBase->swapTomoAAndBuffer();
  scanGridBase->transform(TranslationZ()(shiftZ));

  std::string transformationType = "linearWithStretchingXY";
  std::string registrationType = "top";
  std::cout << transformationType << " " << registrationType << std::endl;
  makeRegistration(image, scanGridBase, transformationType, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, false);

}


std::shared_ptr<ScanGrid> extractScanGridCylinder(
    Image &image, std::shared_ptr<ScanGrid> &scanGridBase,
    const double &R, const double &angleCenter,
    const double &zWidth, const double &angleWidth,
    const int &nZ, const int &nAngle) {

  auto origin = scanGridBase->getBasis()->getOrigin();

  /*double R = image.getXStep() * 700;
  double angleCenter = 0.;
  double zWidth = image.getZStep() * 30;
  double angleWidth = M_PI * 2.;
  int nZ = 200;
  int nAngle = 7000;*/

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
                              std::shared_ptr<ScanGrid> &scanGridCylinder,
                              const std::vector<double> &constraintsMin,
                              const std::vector<double> &constraintsMax) {


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
