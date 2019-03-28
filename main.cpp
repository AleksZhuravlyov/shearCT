#include <iostream>
#include <string>

#include <NcCt.h>
#include <VtpCt.h>

int main() {

    /*auto ncFileName = std::string("/Volumes/ElkData/CT/samples/10.nc");

    NcCt ncCt(ncFileName);

    std::cout << ncCt << std::endl;

    ncCt.setRegion({500, 600, 600}, {30, 400, 500});

    ncCt.saveRegion("/Volumes/ElkData/CT/tmp/10Cut.nc");*/


    std::vector<double> pointsArray;
    std::vector<double> xyz = {0, 0, 0};

    for (int i = 0; i < 7; i++) {
        for (auto &&value : xyz)
            value = i;
        pointsArray.insert(pointsArray.end(), xyz.begin(), xyz.end());
    }

    VtpCt vtpCt(pointsArray);

    std::vector<double> result(vtpCt.size());

    for (int i = 0; i < result.size(); i++)
        result[i] = i;

    vtpCt.setResultArray(result);

    vtpCt.setIsBinary(true);

    vtpCt.savePoints("tmpA.vtp");


    for (int i = 0; i < pointsArray.size(); i++)
        if (i % 3 == 0)
            pointsArray[i] *= 2;

    vtpCt.setXyzArray(pointsArray);

    for (int i = 0; i < result.size(); i++)
        result[i] = i * 2 + 7;

    vtpCt.setResultArray(result);

    vtpCt.setTomoAArray(result);

    vtpCt.savePoints("tmpB.vtp");

    double *valuePtr;
    for (int i = 0; i < vtpCt.size(); i++) {
        valuePtr = vtpCt.getResultValue(i);
        std::cout << *valuePtr;
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}