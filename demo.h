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

PointsCt createInitBaseSquare(NcCt &ncCt);

double variatePoints(std::shared_ptr<PointsCt> pointsCt, RegionCt &regionCt,
                     std::shared_ptr<Transformation> transformation,
                     const std::vector<double> &relativeValues,
                     const std::vector<double> &absoluteValues,
                     const std::string &fileNamesPrefix,
                     const bool &isFilesSaved);

void takeBaseDataFromFirstCt(std::shared_ptr<PointsCt> pointsCt, NcCt &ncCt);

void variateBaseOffsetZ(std::shared_ptr<PointsCt> pointsCt, NcCt &ncCt);



#endif //DEMO_H
