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

#include <demo.h>


int main() {

    NcCt ncCt5("/Volumes/ElkData/CT/samples/5.nc");
    NcCt ncCt10("/Volumes/ElkData/CT/samples/10.nc");

    auto pointsBottom = std::make_shared<PointsCt>(
            createInitBaseSquare(ncCt5));

    takeBaseDataFromFirstCt(pointsBottom, ncCt5);

    auto pointsTopFirstCt = createTopFirstCt(pointsBottom, ncCt5);

    takeBaseDataFromFirstCt(pointsTopFirstCt, ncCt5);

    variateBaseOffsetZ(pointsBottom, ncCt10);

    auto vtpCt = VtpCt(pointsBottom);
    vtpCt.savePointsFile("finalPoints.vtp", "0");

    auto vtpCtTmp = VtpCt(pointsTopFirstCt);
    vtpCtTmp.savePointsFile("pointsTopFirstCt.vtp", "0");

    return EXIT_SUCCESS;

}


