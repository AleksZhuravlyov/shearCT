#ifndef VARIATION_H
#define VARIATION_H


#include <string>
#include <vector>

#include <NcOps/Image.h>
#include <Points/Points.h>
#include <Geometry/Transformation.h>

// using automatic registration
double variatePoints(std::shared_ptr<Points> pointsCt, Region &regionCt,
                     std::shared_ptr<Transformation> transformation,
                     const std::vector<double> &valuesRelative,
                     const std::vector<double> &valuesAbsolute,
                     const std::vector<double> &valuesAbsoluteReverse,
                     const std::string &fileNamesPrefix,
                     const bool &isFilesSaved);

// using manual registration
double processVariation(std::shared_ptr<Points> pointsCt, Image &ncCt,
                        std::shared_ptr<Transformation> transformation,
                        const double &valueWidth, const int &nValues,
                        const std::string &fileNamesPrefix,
                        const bool &isFilesSaved);


#endif //VARIATION_H
