#ifndef REGIONCT_H
#define REGIONCT_H

#include <vector>

#include <netcdf>

#include <Geom.h>


class RegionCt {

public:

    RegionCt();

    virtual ~RegionCt() {}

    void initiateRegionCt(const std::vector<size_t> &_start,
                        const std::vector<size_t> &_width);

    std::vector<size_t> start;
    std::vector<size_t> width;
    std::vector<std::vector<float>> dimArrays;
    std::vector<short> val;

    void setPoints(std::shared_ptr<Points> _points,
                   std::shared_ptr<std::vector<double>> _pointsVal) {
        points = _points;
        pointsVal = _pointsVal;
    }

private:

    std::shared_ptr<Points> points;
    std::shared_ptr<std::vector<double>> pointsVal;
};


#endif // REGIONCT_H
