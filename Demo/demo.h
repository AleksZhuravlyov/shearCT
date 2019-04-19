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

double variatePoints(std::shared_ptr<PointsCt> pointsCt, RegionCt &regionCt,
                     std::shared_ptr<Transformation> transformation,
                     const std::vector<double> &valuesRelative,
                     const std::vector<double> &valuesAbsolute,
                     const std::vector<double> &valuesAbsoluteReverse,
                     const std::string &fileNamesPrefix,
                     const bool &isFilesSaved);


double processVariation(std::shared_ptr<PointsCt> pointsCt, NcCt &ncCt,
                        std::shared_ptr<Transformation> transformation,
                        const double &valueWidth, const int &nValues,
                        const std::string &fileNamesPrefix,
                        const bool &isFilesSaved);


std::shared_ptr<PointsCt> getPointCtFromFile(const std::string &fileName);


std::shared_ptr<PointsCt> getBaseSquareFromCtA(NcCt &ncCt);


void getBaseSquareFromCtAWithTop(
        NcCt &ncCt, const double &shiftZ, std::shared_ptr<PointsCt> &pointsCt);

std::shared_ptr<PointsCt> getBaseSquareFromCtAWithTop(
        NcCt &ncCt, const double &shiftZ, const std::string &fileName);


std::shared_ptr<PointsCt> getBaseSquareFromCtB(NcCt &ncCt,
                                               const std::string &fileName);

void getBaseSquareFromCtB(NcCt &ncCt, std::shared_ptr<PointsCt> &pointsCt);


void getTopSquareFromCtB(NcCt &ncCt, const double &shiftZ,
                         std::shared_ptr<PointsCt> &pointsCt);

std::shared_ptr<PointsCt>
getTopSquareFromCtB(NcCt &ncCt, const double &shiftZ,
                    const std::string &fileName);


#endif //DEMO_H
