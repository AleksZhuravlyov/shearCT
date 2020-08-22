#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <vector>
#include <string>
#include <scanGrid/ScanGrid.h>
#include <ncOps/Image.h>
#include <geometry/TransformationFunctor.h>
#include <vtkOps/ScanGridIO.h>

#include <dlib/optimization.h>


typedef dlib::matrix<double, 0, 1> ColumnVector;

typedef std::vector<std::shared_ptr<TransformationFunctor>>
    TransformationFunctors;

TransformationFunctors generateLinearTransformation();

TransformationFunctors generateStretchingXY();

TransformationFunctors generateLinearTransformationAndStretchingXY();

std::vector<double> makeRegistration(
    Image &image, std::shared_ptr<ScanGrid> &scanGrid,
    const std::string &transformationType,
    const double &accuracy,
    const std::vector<double> &constraintsMin,
    const std::vector<double> &constraintsMax,
    const std::string &fileNamesPrefix,
    const bool &isFilesSaved);

Bbox calculateTransformationBbox(
    std::shared_ptr<ScanGrid> &scanGrid,
    const TransformationFunctors &transformationFunctors,
    const std::vector<double> &constraintsMin,
    const std::vector<double> &constraintsMax);

class NegativePearsonCorrelation {

public:

  NegativePearsonCorrelation(TransformationFunctors &transformationFunctors_,
                             ScanGrid &scanGrid_,
                             Region &region_,
                             const std::string &fileNamesPrefix_,
                             const bool &isFilesSaved_,
                             int &iteration_,
                             ScanGridIO &scanGridIo_);

  virtual ~NegativePearsonCorrelation() = default;

  double operator()(const ColumnVector &searchVector) const;

  TransformationFunctors &transformationFunctors;

  ScanGrid &scanGrid;

  Region &region;

  std::string fileNamesPrefix;

  bool isFilesSaved;

  int &iteration;

  ScanGridIO &scanGridIo;


  void implementSearchVector(const ColumnVector &searchVector);


};


#endif // REGISTRATION_H
