#ifndef POCEDURES_UNIFORMSEARCH_H
#define POCEDURES_UNIFORMSEARCH_H


#include <string>
#include <vector>

#include <NcOps/Image.h>
#include <ScanGrid/ScanGrid.h>
#include <Geometry/TransformationFunctor.h>

// using automatic registration
double computeBlindSearch(
    std::shared_ptr<ScanGrid> scanGrid, Region &region,
    std::shared_ptr<TransformationFunctor> transformationFunctor,
    const std::vector<double> &valuesRelative,
    const std::vector<double> &valuesAbsolute,
    const std::vector<double> &valuesAbsoluteReverse,
    const std::string &fileNamesPrefix,
    const bool &isFilesSaved);

// using manual registration
double computeUniformSearch(
    std::shared_ptr<ScanGrid> scanGrid, Image &image,
    std::shared_ptr<TransformationFunctor> transformationFunctor,
    const double &valueWidth, const int &nValues,
    const std::string &fileNamesPrefix,
    const bool &isFilesSaved);


#endif // POCEDURES_UNIFORMSEARCH_H
