#include <stretchZvsShift.h>

#include <iostream>
#include <string>

#include <NcCt.h>
#include <VtpCt.h>
#include <Basis.h>

#include <points.h>

double stretchZVsShift(const double &accuracy,
                       const double &shiftZ,
                       const double &xCenterFactor,
                       const double &yCenterFactor,
                       const double &zCenterMeter,
                       const double &xWidthVoxel,
                       const double &yWidthVoxel,
                       const int &nX, const int &nY) {


    NcCt ncCtA("/Volumes/ElkData/CT/samples/5.nc");
    NcCt ncCtB("/Volumes/ElkData/CT/samples/10.nc");
    VtpCt vtpCt;


    // Generate base square from CT A
    auto squareCt = getBaseSquareFromCtA(ncCtA,
                                         xCenterFactor,
                                         yCenterFactor,
                                         zCenterMeter,
                                         xWidthVoxel,
                                         yWidthVoxel,
                                         nX, nY);
    vtpCt.setPointsCt(squareCt);

    // Write CT A value from top square into tomoBuffer of squareCt
    getBaseSquareFromCtAWithTop(ncCtA, shiftZ, squareCt);

    // Find base square from CT B
    getBaseSquareFromCtB(ncCtB, squareCt, accuracy);

    // Take origin from base square from CT B
    auto bottomOrigin = *(squareCt->getBasis()->getOrigin());

    // Find top square from CT B
    getTopSquareFromCtB(ncCtB, shiftZ, squareCt, accuracy);

    // Take origin from top square from CT B
    auto topOrigin = *(squareCt->getBasis()->getOrigin());

    // Final stretch Z calculation

    auto distance = sqrt(CGAL::squared_distance(topOrigin, bottomOrigin));
    auto deltaL = shiftZ - distance;
    auto stretchZ = deltaL / shiftZ;

    std::cout << std::setw(20) << "shiftZ";
    std::cout << std::setw(20) << "stretchZ" << std::endl;

    std::cout << std::setw(20) << shiftZ;
    std::cout << std::setw(20) << stretchZ << std::endl;

    return stretchZ;

}


