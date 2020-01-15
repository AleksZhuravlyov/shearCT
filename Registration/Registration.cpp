#include <Registration.h>

#include <iomanip>

#include <Geometry/Translation.h>
#include <Geometry/Rotation.h>
#include <Geometry/Stretch.h>

#include <dlib/global_optimization.h>

#include <StringAndNumber.h>


typedef std::vector<Transformation> Transformations;


TransformationFunctors generateLinearTransformation() {
  TransformationFunctors transformationFunctors;
  transformationFunctors.push_back(std::make_shared<TranslationX>());
  transformationFunctors.push_back(std::make_shared<TranslationY>());
  transformationFunctors.push_back(std::make_shared<TranslationZ>());
  transformationFunctors.push_back(std::make_shared<RotationX>());
  transformationFunctors.push_back(std::make_shared<RotationY>());
  transformationFunctors.push_back(std::make_shared<RotationZ>());
  return transformationFunctors;
}

TransformationFunctors generateStretchingXY() {
  TransformationFunctors transformationFunctors;
  transformationFunctors.push_back(std::make_shared<StretchXY>());
  return transformationFunctors;
}


std::vector<double> makeRegistration(
    Image &image, std::shared_ptr<ScanGrid> &scanGrid,
    TransformationFunctors &transformationFunctors,
    const double &accuracy,
    const std::vector<double> &constraintsMin,
    const std::vector<double> &constraintsMax,
    const std::string &fileNamesPrefix,
    const bool &isFilesSaved) {

  auto transformationBbox = calculateTransformationBbox(scanGrid,
                                                        transformationFunctors,
                                                        constraintsMin,
                                                        constraintsMax);

  image.setRegion(transformationBbox);
  int iterationForValue = -1;
  ScanGridIO scanGridIo;


  NegativePearsonCorrelation functionForValue(
      transformationFunctors, *scanGrid, image.region,
      fileNamesPrefix, isFilesSaved,
      iterationForValue, scanGridIo);

  int iterationForDerivative = -1;
  NegativePearsonCorrelation functionForDerivative(
      transformationFunctors, *scanGrid, image.region,
      fileNamesPrefix, false,
      iterationForDerivative, scanGridIo);


  unsigned int vectorSize = transformationFunctors.size();
  ColumnVector searchVector(vectorSize);
  ColumnVector lowerConstraint(vectorSize);
  ColumnVector upperConstraint(vectorSize);
  for (int i = 0; i < vectorSize; i++) {

    if (typeid(*transformationFunctors[i]).name() == typeid(StretchXY).name())
      searchVector(i) = 1;
    else
      searchVector(i) = 0;

    lowerConstraint(i) = constraintsMin[i];
    upperConstraint(i) = constraintsMax[i];

  }


  std::cout << std::endl;
  auto pearsonCorrelation = -find_min_box_constrained(
      dlib::lbfgs_search_strategy(10),
      dlib::objective_delta_stop_strategy(accuracy).be_verbose(),
      functionForValue, dlib::derivative(functionForDerivative),
      searchVector, lowerConstraint, upperConstraint);


  functionForValue.implementSearchVector(searchVector);

  std::cout << std::endl;
  std::cout << "pearsonCorrelation " << pearsonCorrelation << std::endl;
  std::cout << "searchVector:\n" << searchVector << std::endl;

  std::vector<double> answerVector;
  for (int i = 0; i < transformationFunctors.size(); i++)
    answerVector.push_back(searchVector(i));

  return answerVector;

}


Bbox calculateTransformationBbox(
    std::shared_ptr<ScanGrid> &scanGrid,
    const TransformationFunctors &transformationFunctors,
    const std::vector<double> &constraintsMin,
    const std::vector<double> &constraintsMax) {

  auto bboxIni = scanGrid->generateBbox();
  std::vector<Bbox> rotationBboxes;
  auto bboxIniScanGrid = ScanGrid(bboxIni);

  for (int i = 0; i < transformationFunctors.size(); i++)

    if (typeid(*transformationFunctors[i]).name() == typeid(RotationX).name() ||
        typeid(*transformationFunctors[i]).name() == typeid(RotationY).name() ||
        typeid(*transformationFunctors[i]).name() == typeid(RotationZ).name()) {

      auto scanGridMin = bboxIniScanGrid;
      scanGridMin.transform((*transformationFunctors[i])(constraintsMin[i]));
      rotationBboxes.push_back(scanGridMin.generateBbox());

      auto scanGridMax = bboxIniScanGrid;
      scanGridMax.transform((*transformationFunctors[i])(constraintsMax[i]));
      rotationBboxes.push_back(scanGridMax.generateBbox());

    }

  auto generalRotationBbox = bboxIni;
  for (auto &bbox : rotationBboxes)
    generalRotationBbox += bbox;


  std::vector<Bbox> translationBboxes;
  auto bboxRotationScanGrid = ScanGrid(generalRotationBbox);

  for (int i = 0; i < transformationFunctors.size(); i++)

    if (typeid(*transformationFunctors[i]).name() != typeid(RotationX).name() &&
        typeid(*transformationFunctors[i]).name() != typeid(RotationY).name() &&
        typeid(*transformationFunctors[i]).name() != typeid(RotationZ).name()) {

      auto scanGridMin = bboxRotationScanGrid;
      scanGridMin.transform((*transformationFunctors[i])(constraintsMin[i]));
      translationBboxes.push_back(scanGridMin.generateBbox());

      auto scanGridMax = bboxRotationScanGrid;
      scanGridMax.transform((*transformationFunctors[i])(constraintsMax[i]));
      translationBboxes.push_back(scanGridMax.generateBbox());

    }

  auto transformationBbox = generalRotationBbox;
  for (auto &bbox : translationBboxes)
    transformationBbox += bbox;

  return transformationBbox;

}


double NegativePearsonCorrelation::operator()(
    const ColumnVector &searchVector) const {

  ++iteration;

  auto scanGridCurr = scanGrid;
  region.setPoints(scanGridCurr.getPoints(), scanGridCurr.getTomoB());

  Transformations transformations;
  for (int i = 0; i < transformationFunctors.size(); i++)
    transformations.push_back(
        (*transformationFunctors[i])(searchVector(i)));

  scanGridCurr.transform(transformations);
  region.computePointsValue();
  scanGridCurr.computeDifferenceAB();

  if (isFilesSaved) {
    scanGridIo.setScanGrid(std::make_shared<ScanGrid>(scanGridCurr));
    scanGridIo.savePointsCtToFile(
        fileNamesPrefix + "_" + toString(iteration) + ".vtp",
        toString(iteration));
  }

  return -scanGridCurr.computePearsonCorrelationAB();
}


void NegativePearsonCorrelation::implementSearchVector(
    const ColumnVector &searchVector) {

  if (isFilesSaved)
    scanGridIo.saveFilesCollectionToFile(fileNamesPrefix + ".pvd");

  region.setPoints(scanGrid.getPoints(), scanGrid.getTomoB());

  Transformations transformations;
  for (int i = 0; i < transformationFunctors.size(); i++)
    transformations.push_back((*transformationFunctors[i])(searchVector(i)));

  scanGrid.transform(transformations);

  region.computePointsValue();
  scanGrid.computeDifferenceAB();

}


NegativePearsonCorrelation::NegativePearsonCorrelation(
    TransformationFunctors &transformationFunctors_,
    ScanGrid &scanGrid_,
    Region &region_,
    const std::string &fileNamesPrefix_,
    const bool &isFilesSaved_,
    int &iteration_,
    ScanGridIO &scanGridIo_) : transformationFunctors(transformationFunctors_),
                               scanGrid(scanGrid_),
                               region(region_),
                               fileNamesPrefix(fileNamesPrefix_),
                               isFilesSaved(isFilesSaved_),
                               iteration(iteration_),
                               scanGridIo(scanGridIo_) {}
