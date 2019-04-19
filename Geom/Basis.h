#ifndef BASIS_H
#define BASIS_H

#include <vector>

#include <CgalAliases.h>

class Basis {

public:

    Basis();

    Basis(const Point &_origin);

    Basis(const std::vector<Direction> &_axes);

    Basis(const std::vector<Direction> &_axes, const Point &_origin);


    Basis(const Basis &basis);

    Basis(Basis &&basis);


    virtual ~Basis() {}


    Basis &operator=(Basis &&basis);


    void transform(const Aff_transformation &transformation);


    Aff_transformation generateTransformation();


    void setOrigin(const Point &_origin);

    void setAxes(const std::vector<Direction> &_axes);


    std::shared_ptr<Point> getOrigin();

    std::shared_ptr<std::vector<Direction>> getAxes();


    friend std::ostream &operator<<(std::ostream &stream, const Basis &basis);


private:

    Point origin;

    std::vector<Direction> axes;

};


#endif //BASIS_H
