#ifndef POINTS_H
#define POINTS_H

#include <iostream>
#include <string>
#include <vector>

#include <StringAndNumber.h>
#include <gnuplot.h>

#include <NcOps/Image.h>
#include <Geometry/Basis.h>
#include <Points/Points.h>
#include <Geometry/Transformation.h>


// creating points_3 taking snapshot form CT image
std::shared_ptr<Points> extractSquarePointsCt(Image &image,
                                              const double &xCenterFactor,
                                              const double &yCenterFactor,
                                              const double &zCenterMeter,
                                              const double &xWidthVoxel,
                                              const double &yWidthVoxel,
                                              const int &nX, const int &nY);

// writing values form shifted points_3 into buffer
void getBaseSquareFromCtAWithTop(Image &ncCt, const double &shiftZ,
                                 std::shared_ptr<Points> &pointsCt);

// writing values form shifted points_3 into buffer
std::shared_ptr<Points> getBaseSquareFromCtAWithTop(
        Image &ncCt, const double &shiftZ, const std::string &fileName);

// search of base points_3 in B CT image employing registration
std::shared_ptr<Points> getBaseSquareFromCtB(Image &ncCt,
                                             const std::string &fileName);

// search of base points_3 in B CT image employing registration
void getBaseSquareFromCtB(Image &ncCt, std::shared_ptr<Points> &pointsCt,
                          const double &accuracy);

// search of top points_3 in B CT image employing registration
void getTopSquareFromCtB(Image &ncCt, const double &shiftZ,
                         std::shared_ptr<Points> &pointsCt,
                         const double &accuracy);

// search of top points_3 in B CT image employing registration
std::shared_ptr<Points> getTopSquareFromCtB(Image &ncCt, const double &shiftZ,
                                            const std::string &fileName);

// getting cylinder from A CT image and base points_3
std::shared_ptr<Points> getCylinderSectorFromCtAAndBaseSquare(
        Image &ncCt, std::shared_ptr<Points> &baseSquare);

// search of cylinder in B CT image
double getCylinderSectorFromCtB(Image &ncCt,
                                std::shared_ptr<Points> &pointsCt);


#endif //POINTS_H
