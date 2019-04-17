#ifndef REGIONCT_H
#define REGIONCT_H

#include <vector>

#include <netcdf>

#include <Basis.h>


class RegionCt {

public:

    RegionCt();

    virtual ~RegionCt() {}

    void initiateRegionCt(const std::vector<size_t> &_start,
                          const std::vector<size_t> &_width);

    std::vector<size_t> start;
    std::vector<size_t> width;
    std::vector<std::vector<float>> dimArrays;
    std::vector<short> value;

    void setPoints(std::shared_ptr<Points> _points,
                   std::shared_ptr<std::vector<double>> _pointsValue) {
        points = _points;
        pointsValue = _pointsValue;
    }


    double computePointValue(Point point);

    void computePointsValue();


    void computeInitsAndSteps();


private:

    std::shared_ptr<Points> points;

    std::shared_ptr<std::vector<double>> pointsValue;


    double xInit;

    double yInit;

    double zInit;


    double xStep;

    double yStep;

    double zStep;

};


#endif // REGIONCT_H
