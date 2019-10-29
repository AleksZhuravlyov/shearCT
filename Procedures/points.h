#ifndef POINTS_H
#define POINTS_H

#include <iostream>
#include <string>
#include <vector>

#include <StringAndNumber.h>
#include <gnuplot.h>

#include <NcCt.h>
#include <Basis.h>
#include <PointsCt.h>
#include <Transformation.h>

// just reading points from file
std::shared_ptr<PointsCt> getPointCtFromFile(const std::string &fileName);

// creating points taking snapshot form CT image
std::shared_ptr<PointsCt> getBaseSquareFromCtA(NcCt &ncCt);

// writing values form shifted points into buffer
void getBaseSquareFromCtAWithTop(NcCt &ncCt, const double &shiftZ,
                                 std::shared_ptr<PointsCt> &pointsCt);

// writing values form shifted points into buffer
std::shared_ptr<PointsCt> getBaseSquareFromCtAWithTop(
        NcCt &ncCt, const double &shiftZ, const std::string &fileName);

// search of base points in B CT image employing registration
std::shared_ptr<PointsCt> getBaseSquareFromCtB(NcCt &ncCt,
                                               const std::string &fileName);

// search of base points in B CT image employing registration
void getBaseSquareFromCtB(NcCt &ncCt, std::shared_ptr<PointsCt> &pointsCt);

// search of top points in B CT image employing registration
void getTopSquareFromCtB(NcCt &ncCt, const double &shiftZ,
                         std::shared_ptr<PointsCt> &pointsCt);

// search of top points in B CT image employing registration
std::shared_ptr<PointsCt> getTopSquareFromCtB(NcCt &ncCt, const double &shiftZ,
                                              const std::string &fileName);

// getting cylinder from A CT image and base points
std::shared_ptr<PointsCt> getCylinderSectorFromCtAAndBaseSquare(
        NcCt &ncCt, std::shared_ptr<PointsCt> &baseSquare);

// search of cylinder in B CT image
double getCylinderSectorFromCtB(NcCt &ncCt,
                                std::shared_ptr<PointsCt> &pointsCt);



#endif //POINTS_H
