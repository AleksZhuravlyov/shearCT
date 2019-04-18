#ifndef DEMO_H
#define DEMO_H

#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include <NcCt.h>
#include <VtpCt.h>
#include <Basis.h>
#include <PointsCt.h>
#include <Transformation.h>

#include <StringAndNumber.h>
#include <gnuplot.h>

PointsCt createInitBaseSquare(NcCt &ncCt);

double variatePoints(std::shared_ptr<PointsCt> pointsCt, RegionCt &regionCt,
                     std::shared_ptr<Transformation> transformation,
                     const std::vector<double> &valuesRelative,
                     const std::vector<double> &valuesAbsolute,
                     const std::vector<double> &valuesAbsoluteReverse,
                     const std::string &fileNamesPrefix,
                     const bool &isFilesSaved);

void takeBaseDataFromFirstCt(std::shared_ptr<PointsCt> pointsCt, NcCt &ncCt);


double processVariation(std::shared_ptr<PointsCt> pointsCt, NcCt &ncCt,
                        std::shared_ptr<Transformation> transformation,
                        const double &valueWidth, const int &nValues,
                        const std::string &fileNamesPrefix,
                        const bool &isFilesSaved);


std::shared_ptr<std::vector<double>> computeTomoATopFirstCt(
        std::shared_ptr<PointsCt> pointsCt,
        NcCt &ncCt, const double &shiftZ);


#endif //DEMO_H
