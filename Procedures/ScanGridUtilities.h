#ifndef PROCEDURES_SCANGRIDUTILITIES_H
#define PROCEDURES_SCANGRIDUTILITIES_H

#include <iostream>
#include <string>
#include <vector>

#include <ThirdParty/StringAndNumber.h>
#include <ThirdParty/gnuplot.h>

#include <NcOps/Image.h>
#include <Geometry/Basis.h>
#include <ScanGrid/ScanGrid.h>
#include <Geometry/TransformationFunctor.h>


// creating scanGrid taking snapshot form image
std::shared_ptr<ScanGrid> extractScanGridSquare(Image &image,
                                                const double &xCenter,
                                                const double &yCenter,
                                                const double &zCenter,
                                                const double &xWidth,
                                                const double &yWidth,
                                                const int &nX, const int &nY);

// writing values form shifted scanGridBase into buffer
void writeShiftedZScanGridToBuffer(Image &image, const double &shiftZ,
                                   std::shared_ptr<ScanGrid> &scanGridBase);

// search of base scanGridBase in image employing registration
void searchScanGridBase(Image &image, std::shared_ptr<ScanGrid> &scanGridBase,
                        const double &accuracy,
                        const std::vector<double> &constraintsMin,
                        const std::vector<double> &constraintsMax);

void searchScanGridBaseWithStretch(Image &image,
                                   std::shared_ptr<ScanGrid> &scanGridBase,
                                   const double &accuracy,
                                   const std::vector<double> &constraintsMin,
                                   const std::vector<double> &constraintsMax);


// search of top scanGridBase in image employing registration
void searchScanGridTop(Image &image, const double &shiftZ,
                       std::shared_ptr<ScanGrid> &scanGridBase,
                       const double &accuracy,
                       const std::vector<double> &constraintsMin,
                       const std::vector<double> &constraintsMax);

void searchScanGridTopWithStretch(Image &image, const double &shiftZ,
                                  std::shared_ptr<ScanGrid> &scanGridBase,
                                  const double &accuracy,
                                  const std::vector<double> &constraintsMin,
                                  const std::vector<double> &constraintsMax);


// creating cylinder scanGrid taking snapshot form image using base scanGrid
std::shared_ptr<ScanGrid> extractScanGridCylinder(
    Image &image, std::shared_ptr<ScanGrid> &scanGridBase,
    const double &R, const double &angleCenter,
    const double &zWidth, const double &angleWidth,
    const int &nZ, const int &nAngle);

// search of cylinder scanGrid in image
double searchScanGridCylinder(Image &image,
                              std::shared_ptr<ScanGrid> &scanGridCylinder,
                              const std::vector<double> &constraintsMin,
                              const std::vector<double> &constraintsMax);


#endif // PROCEDURES_SCANGRIDUTILITIES_H
