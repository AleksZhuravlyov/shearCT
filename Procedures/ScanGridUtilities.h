#ifndef PROCEDURES_SCANGRIDUTILITIES_H
#define PROCEDURES_SCANGRIDUTILITIES_H

#include <iostream>
#include <string>
#include <vector>

#include <StringAndNumber.h>
#include <gnuplot.h>

#include <NcOps/Image.h>
#include <Geometry/Basis.h>
#include <ScanGrid/ScanGrid.h>
#include <Geometry/TransformationFunctor.h>


// creating cgalPoints_ taking snapshot form CT image
std::shared_ptr<ScanGrid> extractSquarePointsCt(Image &image,
                                                const double &xCenterFactor,
                                                const double &yCenterFactor,
                                                const double &zCenterMeter,
                                                const double &xWidthVoxel,
                                                const double &yWidthVoxel,
                                                const int &nX, const int &nY);

// writing values form shifted cgalPoints_ into buffer
void getBaseSquareFromCtAWithTop(Image &image, const double &shiftZ,
                                 std::shared_ptr<ScanGrid> &scanGrid);

// writing values form shifted cgalPoints_ into buffer
std::shared_ptr<ScanGrid> getBaseSquareFromCtAWithTop(
    Image &image, const double &shiftZ, const std::string &fileName);

// search of base cgalPoints_ in B CT image employing registration
std::shared_ptr<ScanGrid> getBaseSquareFromCtB(Image &image,
                                               const std::string &fileName);

// search of base cgalPoints_ in B CT image employing registration
void getBaseSquareFromCtB(Image &image, std::shared_ptr<ScanGrid> &scanGrid,
                          const double &accuracy);

// search of top cgalPoints_ in B CT image employing registration
void getTopSquareFromCtB(Image &image, const double &shiftZ,
                         std::shared_ptr<ScanGrid> &scanGrid,
                         const double &accuracy);

// search of top cgalPoints_ in B CT image employing registration
std::shared_ptr<ScanGrid> getTopSquareFromCtB(Image &image, const double &shiftZ,
                                              const std::string &fileName);

// getting cylinder from A CT image and base cgalPoints_
std::shared_ptr<ScanGrid> getCylinderSectorFromCtAAndBaseSquare(
    Image &image, std::shared_ptr<ScanGrid> &baseSquare);

// search of cylinder in B CT image
double getCylinderSectorFromCtB(Image &image,
                                std::shared_ptr<ScanGrid> &scanGrid);


#endif // PROCEDURES_SCANGRIDUTILITIES_H
