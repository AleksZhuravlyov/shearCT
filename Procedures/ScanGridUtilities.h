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
                                                const double &xCenterFactor,
                                                const double &yCenterFactor,
                                                const double &zCenterMeter,
                                                const double &xWidthVoxel,
                                                const double &yWidthVoxel,
                                                const int &nX, const int &nY);

// writing values form shifted scanGridBase into buffer
void writeShiftedZScanGridToBuffer(Image &image, const double &shiftZ,
                                   std::shared_ptr<ScanGrid> &scanGridBase);

// search of base scanGridBase in image employing registration
void searchScanGridBase(Image &image, std::shared_ptr<ScanGrid> &scanGridBase,
                        const double &accuracy);

// search of top scanGridBase in image employing registration
void searchScanGridTop(Image &image, const double &shiftZ,
                       std::shared_ptr<ScanGrid> &scanGridBase,
                       const double &accuracy);

// creating cylinder scanGrid taking snapshot form image using base scanGrid
std::shared_ptr<ScanGrid> extractScanGridCylinder(
    Image &image, std::shared_ptr<ScanGrid> &scanGridBase);

// search of cylinder scanGrid in image
double searchScanGridCylinder(Image &image,
                              std::shared_ptr<ScanGrid> &scanGridCylinder);


#endif // PROCEDURES_SCANGRIDUTILITIES_H
