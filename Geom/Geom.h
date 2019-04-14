#ifndef GEOM_H
#define GEOM_H

#include <cmath>
#include <vector>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/Kernel_d/Aff_transformation_d.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Direction_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Vector_3.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Aff_transformation_3<Kernel> Aff_transformation;
typedef CGAL::Bbox_3 Bbox;
typedef Kernel::Point_3 Point;
typedef std::vector<Point> Points;
typedef Kernel::Vector_3 Vector;
typedef Kernel::Direction_3 Direction;


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


    void setOrigin(const Point &_origin);


    std::shared_ptr<std::vector<Direction>> getDirections();

    std::shared_ptr<Point> getOrigin();

    friend std::ostream &operator<<(std::ostream &stream, const Basis &basis);


private:

    std::vector<Direction> axis;

    Point origin;

};


Aff_transformation constructXRotation(const double &angle);

Aff_transformation constructYRotation(const double &angle);

Aff_transformation constructZRotation(const double &angle);

void makeRZPointTransformation(Point &point, const double &dr);


#endif //GEOM_H
