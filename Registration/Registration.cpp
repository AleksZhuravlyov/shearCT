#include <Registration/Registration.h>

#include <iomanip>
#include <bitset>

#include <Geometry/Translation.h>
#include <Geometry/Rotation.h>
#include <Geometry/Stretch.h>

#include <dlib/global_optimization.h>

#include <ThirdParty/StringAndNumber.h>


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
    const std::string &transformationType,
    const double &accuracy,
    const std::vector<double> &constraintsMin,
    const std::vector<double> &constraintsMax,
    const std::string &fileNamesPrefix,
    const bool &isFilesSaved) {

  TransformationFunctors transformationFunctors;
  if (transformationType == "linear")
    transformationFunctors = generateLinearTransformation();
  else if (transformationType == "XYStretching")
    transformationFunctors = generateStretchingXY();


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

    if (typeid(*transformationFunctors[i]) == typeid(StretchXY))
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
  for (int i = 0; i < searchVector.size(); i++)
    answerVector.push_back(searchVector(i));

  return answerVector;

}


Bbox calculateTransformationBbox(
    std::shared_ptr<ScanGrid> &scanGrid,
    const TransformationFunctors &transformationFunctors,
    const std::vector<double> &constraintsMin,
    const std::vector<double> &constraintsMax) {

  std::vector<int> translationIndices;
  for (int i = 0; i < transformationFunctors.size(); i++)
    if (typeid(*transformationFunctors[i]) == typeid(TranslationX) ||
        typeid(*transformationFunctors[i]) == typeid(TranslationY) ||
        typeid(*transformationFunctors[i]) == typeid(TranslationZ))
      translationIndices.push_back(i);

  std::vector<int> rotationIndices;
  for (int i = 0; i < transformationFunctors.size(); i++)
    if (typeid(*transformationFunctors[i]) == typeid(RotationX) ||
        typeid(*transformationFunctors[i]) == typeid(RotationY) ||
        typeid(*transformationFunctors[i]) == typeid(RotationZ))
      rotationIndices.push_back(i);

  std::vector<int> stretchIndices;
  for (int i = 0; i < transformationFunctors.size(); i++)
    if (typeid(*transformationFunctors[i]) == typeid(StretchXY))
      stretchIndices.push_back(i);

  auto bboxIni = scanGrid->generateBbox();
  auto bboxIniScanGrid = ScanGrid(bboxIni);
  for (int i = 0; i < stretchIndices.size(); i++)
    bboxIniScanGrid.transform((*transformationFunctors[i])(constraintsMax[i]));
  auto generalBbox = bboxIniScanGrid.generateBbox();


  if (!rotationIndices.empty()) {

    if (rotationIndices.size() != 3) {
      std::cerr << "The number of rotation transformations is not 3."
                << std::endl;
      std::exit(EXIT_FAILURE);
    }

    std::vector<Bbox> rotationBboxes;
    std::vector<std::bitset<3>> combinations;
    int combinationsN = int(std::bitset<3>(0b111).to_ulong());
    for (int i = 0; i <= combinationsN; i++)
      combinations.emplace_back(i);

    for (int i = 0; i <= combinationsN; i++) {
      auto scanGrid = bboxIniScanGrid;
      for (int j = 0; j < 3; j++) {
        int k = rotationIndices[j];
        if (combinations[i][j] == 0)
          scanGrid.transform((*transformationFunctors[k])(constraintsMin[k]));
        else
          scanGrid.transform((*transformationFunctors[k])(constraintsMax[k]));
      }
      rotationBboxes.emplace_back(scanGrid.generateBbox());
    }

    for (auto &bbox : rotationBboxes)
      generalBbox += bbox;
  }


  std::vector<Bbox> translationBboxes;
  for (int i = 0; i < translationIndices.size(); i++) {
    auto scanGridMin = ScanGrid(generalBbox);
    scanGridMin.transform((*transformationFunctors[i])(constraintsMin[i]));
    translationBboxes.push_back(scanGridMin.generateBbox());
    auto scanGridMax = ScanGrid(generalBbox);
    scanGridMax.transform((*transformationFunctors[i])(constraintsMax[i]));
    translationBboxes.push_back(scanGridMax.generateBbox());
  }
  for (auto &bbox : translationBboxes)
    generalBbox += bbox;

  return generalBbox;

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
    scanGridIo.saveScanGridToFile(
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
