#include <iostream>
#include <string>
#include <cmath>

#include <NcCt.h>
#include <VtpCt.h>
#include <StringAndNumber.h>

int main() {


    double RMin = 10;
    double RStep = 0.1;
    double RMax = 20;

    int nAnglePoints = 1000;
    int nZPoints = 200;
    double height = 10;

    VtpCt vtpCt(nAnglePoints * nZPoints);

    std::cout << vtpCt.size() << std::endl;

    auto xyz = vtpCt.getXyzArray();

    auto result = vtpCt.getResult();


    for (double R = RMin; R < RMax; R += RStep) {
        std::cout << R << std::endl;
        for (int i = 0; i < nZPoints; i++)
            for (int j = 0, k = 0; j < nAnglePoints; ++j) {

                k = (i * nAnglePoints + j);

                (*xyz)[k * 3] = R * cos(2. * M_PI / nAnglePoints * j);
                (*xyz)[k * 3 + 1] = R * sin(2. * M_PI / nAnglePoints * j);
                (*xyz)[k * 3 + 2] = height / nZPoints * i;


                (*result)[k] = (*xyz)[k * 3] * (*xyz)[k * 3];
                (*result)[k] += (*xyz)[k * 3 + 1] * (*xyz)[k * 3 + 1];
                (*result)[k] += (*xyz)[k * 3 + 2] * (*xyz)[k * 3 + 2];
                (*result)[k] = sqrt((*result)[k]);

            }

        vtpCt.savePointsFile(toString(R) + ".vtp", toString(R));

    }

    vtpCt.saveCollectionFile("points.pvd");


    return EXIT_SUCCESS;
}