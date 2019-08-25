#include <RegionCt.h>


RegionCt::RegionCt() : start({0, 0, 0}),
                       width({1, 1, 1}),
                       dimArrays(std::vector<std::vector<float>>(3)),
                       value(std::vector<short>(1)) {

    for (int i = 0; i < width.size(); i++)
        dimArrays[i].resize(width[i]);

    value.resize(width[0] * width[1] * width[2]);
}

void RegionCt::initiateRegionCt(const std::vector<size_t> &_start,
                                const std::vector<size_t> &_width) {
    start = _start;

    if (width != _width) {
        width = _width;

        for (int i = 0; i < width.size(); i++)
            dimArrays[i].resize(width[i]);

        value.resize(width[0] * width[1] * width[2]);
    }
}


double RegionCt::computePointValue(Point point) {

    auto x = point.x();
    auto y = point.y();
    auto z = point.z();


    auto nz = int(dimArrays[0].size());
    auto ny = int(dimArrays[1].size());
    auto nx = int(dimArrays[2].size());


    auto ix = int((x - xInit) / xStep);
    auto iy = int((y - yInit) / yStep);
    auto iz = int((z - zInit) / zStep);


    auto xWeight = double((x - ix * xStep - xInit) / xStep);
    auto yWeight = double((y - iy * yStep - yInit) / yStep);
    auto zWeight = double((z - iz * zStep - zInit) / zStep);


    int nCorners = 8;
    auto iCorners = std::vector<int>(nCorners, 0);
    auto CornerWeights = std::vector<double>(nCorners, 0);

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


    double pointValue = 0;
    for (int i = 0; i < nCorners; i++)
        pointValue += CornerWeights[i] * value[iCorners[i]];


    return pointValue;

}


void RegionCt::computePointsValue() {

    for (int i = 0; i < points->size(); i++)
        (*pointsValue)[i] = computePointValue((*points)[i]);

}


void RegionCt::computeInitsAndSteps() {

    xInit = dimArrays[2][0];
    yInit = dimArrays[1][0];
    zInit = dimArrays[0][0];

    xStep = dimArrays[2][1] - xInit;
    yStep = dimArrays[1][1] - yInit;
    zStep = dimArrays[0][1] - zInit;

}
