#ifndef BASIS_H
#define BASIS_H

#include <cmath>
#include <vector>

#include <CALIncludes.h>

class Basis {
public:

    Basis(const std::vector<Direction> &_axis, const Point &_origin);

    Basis(const Point &_origin);

    Basis(const std::vector<Direction> &_axis);

    Basis();

    Basis(const Basis &basis);

    Basis(Basis &&basis);

    virtual ~Basis() {}


    Basis &operator=(Basis &&basis);

    Basis transform(const Aff_transformation &aff_transformation);

    Aff_transformation createAff_transformation();


    void setAxes(const std::vector<Direction> &_axes);

    void setOrigin(const Point &_origin);


    std::shared_ptr<std::vector<Direction>> getDirections();

    std::shared_ptr<Point> getOrigin();

    friend std::ostream &operator<<(std::ostream &stream, const Basis &basis);


private:

    std::vector<Direction> axes;

    Point origin;

};


#endif //BASIS_H
