#ifndef VARIATION_H
#define VARIATION_H


#include <string>
#include <vector>

#include <NcOps/Image.h>
#include <ScanGrid/ScanGrid.h>
#include <Geometry/TransformationFunctor.h>

// using automatic registration
double variatePoints(std::shared_ptr<ScanGrid> pointsCt, Region &regionCt,
                     std::shared_ptr<TransformationFunctor> transformationFunctor,
                     const std::vector<double> &valuesRelative,
                     const std::vector<double> &valuesAbsolute,
                     const std::vector<double> &valuesAbsoluteReverse,
                     const std::string &fileNamesPrefix,
                     const bool &isFilesSaved);

// using manual registration
double processVariation(std::shared_ptr<ScanGrid> pointsCt, Image &ncCt,
                        std::shared_ptr<TransformationFunctor> transformationFunctor,
                        const double &valueWidth, const int &nValues,
                        const std::string &fileNamesPrefix,
                        const bool &isFilesSaved);


#endif //VARIATION_H
