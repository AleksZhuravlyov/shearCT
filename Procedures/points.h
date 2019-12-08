#ifndef POINTS_H
#define POINTS_H

#include <iostream>
#include <string>
#include <vector>

#include <StringAndNumber.h>
#include <gnuplot.h>

#include <Image.h>
#include <Basis.h>
#include <PointsCt.h>
#include <Transformation.h>


// creating points taking snapshot form CT image
std::shared_ptr<PointsCt> extractSquarePointsCt(Image &image,
                                                const double &xCenterFactor,
                                                const double &yCenterFactor,
                                                const double &zCenterMeter,
                                                const double &xWidthVoxel,
                                                const double &yWidthVoxel,
                                                const int &nX, const int &nY);

// writing values form shifted points into buffer
void getBaseSquareFromCtAWithTop(Image &ncCt, const double &shiftZ,
                                 std::shared_ptr<PointsCt> &pointsCt);

// writing values form shifted points into buffer
std::shared_ptr<PointsCt> getBaseSquareFromCtAWithTop(
        Image &ncCt, const double &shiftZ, const std::string &fileName);

// search of base points in B CT image employing registration
std::shared_ptr<PointsCt> getBaseSquareFromCtB(Image &ncCt,
                                               const std::string &fileName);

// search of base points in B CT image employing registration
void getBaseSquareFromCtB(Image &ncCt, std::shared_ptr<PointsCt> &pointsCt,
                          const double &accuracy);

// search of top points in B CT image employing registration
void getTopSquareFromCtB(Image &ncCt, const double &shiftZ,
                         std::shared_ptr<PointsCt> &pointsCt,
                         const double &accuracy);

// search of top points in B CT image employing registration
std::shared_ptr<PointsCt> getTopSquareFromCtB(Image &ncCt, const double &shiftZ,
                                              const std::string &fileName);

// getting cylinder from A CT image and base points
std::shared_ptr<PointsCt> getCylinderSectorFromCtAAndBaseSquare(
        Image &ncCt, std::shared_ptr<PointsCt> &baseSquare);

// search of cylinder in B CT image
double getCylinderSectorFromCtB(Image &ncCt,
                                std::shared_ptr<PointsCt> &pointsCt);


#endif //POINTS_H
