#include <points.h>

#include <cmath>

#include <Vtp/PointsIO.h>
#include <Registration.h>


std::shared_ptr<Points> extractSquarePointsCt(Image &image,
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

  auto pointsCt = std::make_shared<Points>();
  pointsCt->createXYSquare(xCenter, yCenter, zCenter,
                           xWidth, yWidth,
                           nX, nY);

  image.setRegion(pointsCt->generateBbox());
  image.region.setPoints(pointsCt->getPoints(), pointsCt->getTomoA());
  image.region.computePointsValue();

  return pointsCt;

}


void getBaseSquareFromCtAWithTop(Image &ncCt, const double &shiftZ,
                                 std::shared_ptr<Points> &pointsCt) {

  pointsCt->transform(TranslationZ()(shiftZ));
  ncCt.setRegion(pointsCt->generateBbox());
  ncCt.region.setPoints(pointsCt->getPoints(), pointsCt->getTomoBuffer());
  ncCt.region.computePointsValue();
  pointsCt->transform(TranslationZ()(-shiftZ));

}

std::shared_ptr<Points> getBaseSquareFromCtAWithTop(
        Image &ncCt, const double &shiftZ, const std::string &fileName) {

  auto pointsCt = std::make_shared<Points>();

  auto vtkPointsCt = PointsIO();
  vtkPointsCt.setPointsCt(pointsCt);
  vtkPointsCt.loadPointsCtFromFile(fileName);

  getBaseSquareFromCtAWithTop(ncCt, shiftZ, pointsCt);

  return pointsCt;

}


void getBaseSquareFromCtB(Image &ncCt, std::shared_ptr<Points> &pointsCt,
                          const double &accuracy) {

  // processVariation(pointsCt_, ncCt, std::make_shared<TranslationZ>(),
  //                  20e-5, 11, "bottom",
  //                  false);
  // processVariation(pointsCt_, ncCt, std::make_shared<TranslationX>(),
  //                  10e-5, 21, "bottom",
  //                  false);
  // processVariation(pointsCt_, ncCt, std::make_shared<TranslationY>(),
  //                  10e-5, 21, "bottom",
  //                  false);
  // processVariation(pointsCt_, ncCt, std::make_shared<RotationX>(),
  //                  M_PI / 140, 31, "bottom",
  //                  false);
  // processVariation(pointsCt_, ncCt, std::make_shared<RotationY>(),
  //                  M_PI / 200, 21, "bottom",
  //                  false);
  // processVariation(pointsCt_, ncCt, std::make_shared<TranslationZ>(),
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
  makeRegistration(ncCt, pointsCt, transformations, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, true);

}

std::shared_ptr<Points> getBaseSquareFromCtB(Image &ncCt,
                                             const std::string &fileName,
                                             const double &accuracy) {

  auto pointsCt = std::make_shared<Points>();

  auto vtkPointsCt = PointsIO();
  vtkPointsCt.setPointsCt(pointsCt);
  vtkPointsCt.loadPointsCtFromFile(fileName);


  getBaseSquareFromCtB(ncCt, pointsCt, accuracy);

  return pointsCt;

}


void getTopSquareFromCtB(Image &ncCt, const double &shiftZ,
                         std::shared_ptr<Points> &pointsCt,
                         const double &accuracy) {

  pointsCt->swapTomoAAndTomoBuffer();
  pointsCt->transform(TranslationZ()(shiftZ));


  // processVariation(pointsCt_, ncCt, std::make_shared<TranslationY>(),
  //                  40e-5, 41, "top1", false);
  // processVariation(pointsCt_, ncCt, std::make_shared<TranslationX>(),
  //                  35e-5, 31, "top1", false);
  // processVariation(pointsCt_, ncCt, std::make_shared<RotationX>(),
  //                  M_PI / 30, 41, "top1", false);
  // processVariation(pointsCt_, ncCt, std::make_shared<RotationY>(),
  //                  M_PI / 50, 41, "top1", false);
  // processVariation(pointsCt_, ncCt, std::make_shared<TranslationZ>(),
  //                  19e-5, 51, "top1", false);
  // processVariation(pointsCt_, ncCt, std::make_shared<TranslationZ>(),
  //                  1.5e-5, 41, "top1", false);
  //
  //
  // processVariation(pointsCt_, ncCt, std::make_shared<TranslationY>(),
  //                  15e-5, 41, "top2", false);
  // processVariation(pointsCt_, ncCt, std::make_shared<TranslationX>(),
  //                  5e-5, 31, "top2", false);
  // processVariation(pointsCt_, ncCt, std::make_shared<RotationX>(),
  //                  M_PI / 70, 100, "top2", false);
  // processVariation(pointsCt_, ncCt, std::make_shared<RotationY>(),
  //                  M_PI / 150, 41, "top2", false);
  // processVariation(pointsCt_, ncCt, std::make_shared<TranslationZ>(),
  //                  9e-5, 61, "top2", false);
  // processVariation(pointsCt_, ncCt, std::make_shared<TranslationZ>(),
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
  makeRegistration(ncCt, pointsCt, transformations, accuracy,
                   constraintsMin, constraintsMax,
                   registrationType, false);


}


std::shared_ptr<Points> getCylinderSectorFromCtAAndBaseSquare(
        Image &ncCt, std::shared_ptr<Points> &baseSquare) {

  auto origin = baseSquare->getBasis()->getOrigin();

  double R = ncCt.getXStep() * 500;
  double angleCenter = M_PI * 1.5;
  double zWidth = ncCt.getZStep() * 50;
  double angleWidth = M_PI / 10;
  int nZ = 500;
  int nAngle = 5000;

  auto pointsCt = std::make_shared<Points>();
  pointsCt->createZCylinderSegment(origin->x(), origin->y(), origin->z(),
                                   R, angleCenter, zWidth, angleWidth,
                                   nZ, nAngle);

  ncCt.setRegion(pointsCt->generateBbox());
  ncCt.region.setPoints(pointsCt->getPoints(), pointsCt->getTomoA());
  ncCt.region.computePointsValue();

  return pointsCt;

}


double getCylinderSectorFromCtB(Image &ncCt,
                                std::shared_ptr<Points> &pointsCt) {

  // processVariation(pointsCt_, ncCt,
  //                  std::make_shared<StretchingXY>(),
  //                  0.007, 21, "cylinder1", false);
  // processVariation(pointsCt_, ncCt,
  //                  std::make_shared<StretchingXY>(),
  //                  0.0007, 21, "cylinder2", false);
  // processVariation(pointsCt_, ncCt,
  //                  std::make_shared<StretchingXY>(),
  //                  0.0002, 21, "cylinder3", false);
  // processVariation(pointsCt_, ncCt,
  //                  std::make_shared<StretchingXY>(),
  //                               0.00015, 21, "cylinder4", false);

  // return mult1 * mult2 * mult3 * mult4 - 1;

  auto transformations = generateStretchingXY();
  auto constraintsMin = std::vector<double>{0.98};
  auto constraintsMax = std::vector<double>{1.02};

  std::string registrationType = "XYStretching";
  std::cout << registrationType << std::endl;
  auto answerVector = makeRegistration(ncCt, pointsCt,
                                       transformations, 1.e-12,
                                       constraintsMin, constraintsMax,
                                       registrationType,
                                       false);

  return answerVector[0] - 1;

}
