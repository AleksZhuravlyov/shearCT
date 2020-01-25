#ifndef POCEDURES_STRETCHZVSSHIFT_H
#define POCEDURES_STRETCHZVSSHIFT_H

#include <string>


double stretchZVsShift(const double &accuracy,
                       const double &shiftZ,
                       const double &xCenterFactor,
                       const double &yCenterFactor,
                       const double &zCenterMeter,
                       const double &xWidthVoxel,
                       const double &yWidthVoxel,
                       const int &nX, const int &nY,
                       const std::string &ncAFileName,
                       const std::string &ncBFileName);


#endif // POCEDURES_STRETCHZVSSHIFT_H
