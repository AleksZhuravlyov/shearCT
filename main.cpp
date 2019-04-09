#include <iostream>
#include <string>
#include <cmath>

#include <NcCt.h>
#include <VtpCt.h>
#include <Geom.h>

#include <StringAndNumber.h>

int main() {

    double RMin = 10;
    double RStep = 0.1;
    double RMax = 20;

    int nAnglePoints = 1000;
    int nZPoints = 200;
    double height = 10;

    auto points = std::make_shared<Points>();


    for (int i = 0; i < nZPoints; i++)
        for (int j = 0; j < nAnglePoints; ++j) {

            points->push_back(Point(
                    RMin * cos(2. * M_PI / nAnglePoints * j),
                    RMin * sin(2. * M_PI / nAnglePoints * j),
                    height / nZPoints * i
            ));
        }


    VtpCt vtpCt(points);

    auto result = vtpCt.getResult();

    for (int i = 0; i < (RMax - RMin) / RStep; i++) {
        for (int j = 0; j < points->size(); j++) {

            makeRZPointTransformation((*points)[j], RStep);

            (*result)[j] = (*points)[j].x() * (*points)[j].x();
            (*result)[j] += (*points)[j].y() * (*points)[j].y();
            (*result)[j] += (*points)[j].z() * (*points)[j].z();
            (*result)[j] = sqrt((*result)[j]);

        }

        vtpCt.savePointsFile(toString(RMin + RStep * i) + ".vtp",
                             toString(RMin + RStep * i));

        std::cout << "R " << RMin + RStep * i << std::endl;

    }

    vtpCt.saveCollectionFile("points.pvd");


    return EXIT_SUCCESS;
}