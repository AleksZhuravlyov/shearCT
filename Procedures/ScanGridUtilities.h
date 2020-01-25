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


// creating points_ taking snapshot form CT image
std::shared_ptr<ScanGrid> extractScanGridSquare(Image &image,
                                                const double &xCenterFactor,
                                                const double &yCenterFactor,
                                                const double &zCenterMeter,
                                                const double &xWidthVoxel,
                                                const double &yWidthVoxel,
                                                const int &nX, const int &nY);

// writing values form shifted points_ into buffer
void getBaseSquareFromAWithTop(Image &image, const double &shiftZ,
                               std::shared_ptr<ScanGrid> &scanGrid);

// writing values form shifted points_ into buffer
std::shared_ptr<ScanGrid> getBaseSquareFromAWithTop(
    Image &image, const double &shiftZ, const std::string &fileName);

// search of base points_ in B CT image employing registration
std::shared_ptr<ScanGrid> getBaseSquareFromCtB(Image &image,
                                               const std::string &fileName);

// search of base points_ in B CT image employing registration
void getBaseSquareFromB(Image &image, std::shared_ptr<ScanGrid> &scanGrid,
                        const double &accuracy);

// search of top points_ in B CT image employing registration
void getTopSquareFromB(Image &image, const double &shiftZ,
                       std::shared_ptr<ScanGrid> &scanGrid,
                       const double &accuracy);

// getting cylinder from A CT image and base points_
std::shared_ptr<ScanGrid> getCylinderSectorFromCtAAndBaseSquare(
    Image &image, std::shared_ptr<ScanGrid> &baseSquare);

// search of cylinder in B CT image
double getCylinderSectorFromB(Image &image,
                              std::shared_ptr<ScanGrid> &scanGrid);


#endif // PROCEDURES_SCANGRIDUTILITIES_H
