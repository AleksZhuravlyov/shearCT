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

    auto pointsCt = std::make_shared<PointsCt>(createInitBaseSquare(ncCt5));

    takeBaseDataFromFirstCt(pointsCt, ncCt5);

    variateBaseOffsetZ(pointsCt, ncCt10);

    auto vtpCt = VtpCt(pointsCt);

    vtpCt.savePointsFile("finalPoints.vtp", "0");

    return EXIT_SUCCESS;

}


