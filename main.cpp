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

    NcCt ncCtA("/Volumes/ElkData/CT/samples/5.nc");
    NcCt ncCtB("/Volumes/ElkData/CT/samples/10.nc");
    double shiftZ = 0.0554 - 3.60179e-05 * 90;

    std::string getBaseSquareFromCtAName = "getBaseSquareFromCtA.vtp";
    // auto pointsCt = getBaseSquareFromCtA(ncCtA);
    // auto vtpCt = VtpCt(pointsCt);
    // vtpCt.savePointsFile(getBaseSquareFromCtAName);
    auto pointsCt = getPointCtFromFile(getBaseSquareFromCtAName);

    getBaseSquareFromCtAWithTop(ncCtA, shiftZ, pointsCt);

    getBaseSquareFromCtB(ncCtB, pointsCt);
    auto bottomOrigin = *(pointsCt->getBasis()->getOrigin());

    getTopSquareFromCtB(ncCtB, shiftZ, pointsCt);
    auto topOrigin = *(pointsCt->getBasis()->getOrigin());


    std::cout << std::endl << "shiftZ " << shiftZ << std::endl;
    auto distance = sqrt(CGAL::squared_distance(topOrigin, bottomOrigin));
    std::cout << std::endl << "distance " << distance << std::endl;
    auto deltaL = shiftZ - distance;
    std::cout << std::endl << "deltaL " << deltaL << std::endl;
    auto stretch = deltaL / shiftZ;
    std::cout << std::endl << "stretch " << stretch << std::endl;
    double youngsModulus = 500. / stretch;
    std::cout << std::endl << "youngsModulus " << youngsModulus << std::endl;


    return EXIT_SUCCESS;

}


