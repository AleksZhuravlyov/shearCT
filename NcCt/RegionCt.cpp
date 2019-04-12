#include <RegionCt.h>
#include <vector>

RegionCt::RegionCt() : start({0, 0, 0}),
                       width({1, 1, 1}),
                       dimArrays(std::vector<std::vector<float>>(3)),
                       val(std::vector<short>(1)) {

    for (int i = 0; i < width.size(); i++)
        dimArrays[i].resize(width[i]);

    val.resize(width[0] * width[1] * width[2]);
}

void RegionCt::initiateRegionCt(const std::vector<size_t> &_start,
                                const std::vector<size_t> &_width) {
    start = _start;

    if (width != _width) {
        width = _width;

        for (int i = 0; i < width.size(); i++)
            dimArrays[i].resize(width[i]);

        val.resize(width[0] * width[1] * width[2]);
    }
}

/// TODO continue here
double RegionCt::computePointVal(Point point) {

    auto x = point.x();
    auto y = point.y();
    auto z = point.z();


    auto xInit = double(dimArrays[0][0]);
    auto yInit = double(dimArrays[1][0]);
    auto zInit = double(dimArrays[2][0]);

    int nz = dimArrays[0].size();
    int ny = dimArrays[1].size();
    int nx = dimArrays[2].size();


    auto zStep = double(dimArrays[0][1] - dimArrays[0][0]);
    auto yStep = double(dimArrays[1][1] - dimArrays[1][0]);
    auto xStep = double(dimArrays[2][1] - dimArrays[2][0]);

    int ix = (x - xInit) / xStep;
    int iy = (y - yInit) / yStep;
    int iz = (z - zInit) / zStep;

    double xWeight = (x - ix * xStep - xInit) / xStep;
    double yWeight = (y - iy * yStep - yInit) / yStep;
    double zWeight = (z - iz * zStep - zInit) / zStep;

    /*std::cout << "x " << x << std::endl;
    std::cout << "xInit " << xInit << std::endl;
    std::cout << "ix " << ix << std::endl;
    std::cout << "xWeight " << xWeight << std::endl;
    std::cout << std::endl;*/

    auto iCorners = std::vector<int>(8, 0);
    auto CornerWeights = std::vector<double>(8, 0);

    iCorners[0] = (iz + 0) * ny * nx + (iy + 0) * nx + (ix + 0);
    CornerWeights[0] = (1 - zWeight) * (1 - yWeight) * (1 - xWeight);

    iCorners[1] = (iz + 0) * ny * nx + (iy + 0) * nx + (ix + 1);
    CornerWeights[1] = (1 - zWeight) * (1 - yWeight) * xWeight;

    iCorners[2] = (iz + 0) * ny * nx + (iy + 1) * nx + (ix + 0);
    CornerWeights[2] = (1 - zWeight) * yWeight * (1 - xWeight);

    iCorners[3] = (iz + 0) * ny * nx + (iy + 1) * nx + (ix + 1);
    CornerWeights[3] = (1 - zWeight) * yWeight * xWeight;

    iCorners[4] = (iz + 1) * ny * nx + (iy + 0) * nx + (ix + 0);
    CornerWeights[4] = zWeight * (1 - yWeight) * (1 - xWeight);

    iCorners[5] = (iz + 1) * ny * nx + (iy + 0) * nx + (ix + 1);
    CornerWeights[5] = zWeight * (1 - yWeight) * xWeight;

    iCorners[6] = (iz + 1) * ny * nx + (iy + 1) * nx + (ix + 0);
    CornerWeights[6] = zWeight * yWeight * (1 - xWeight);

    iCorners[7] = (iz + 1) * ny * nx + (iy + 1) * nx + (ix + 1);
    CornerWeights[7] = zWeight * yWeight * xWeight;


    double pointVal = 0;
    for (int i = 0; i < 8; i++)
        if (CornerWeights[i] != 0)
            pointVal += CornerWeights[i] * val[iCorners[i]];


    return pointVal;
}


void RegionCt::computePointsVal() {

    for (int i = 0; i < points->size(); i++)
        (*pointsVal)[i] = computePointVal((*points)[i]);

}

