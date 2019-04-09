#include "Geom.h"


Basis::Basis(const std::vector<Direction> &_axis, const Point &_origin) :
        axis(_axis), origin(_origin) {}

Basis::Basis(const Point &_origin) :
        Basis({Direction(1, 0, 0), Direction(0, 1, 0), Direction(0, 0, 1)},
              _origin) {}

Basis::Basis(const std::vector<Direction> &_axis) :
        Basis(_axis, Point(0, 0, 0)) {}

Basis::Basis() :
        Basis({Direction(1, 0, 0), Direction(0, 1, 0), Direction(0, 0, 1)},
              Point(0, 0, 0)) {}

Basis::Basis(const Basis &basis) : axis(basis.axis), origin(basis.origin) {}

Basis::Basis(Basis &&basis) : axis(basis.axis), origin(basis.origin) {}


Basis &Basis::operator=(Basis &&basis) {
    std::swap(axis, basis.axis);
    std::swap(origin, basis.origin);
    return *this;
}


Basis Basis::transform(const Aff_transformation &aff_transformation) {

    Basis basis;

    for (int i = 0; i < axis.size(); i++)
        basis.axis[i] = aff_transformation(axis[i]);

    basis.origin = aff_transformation(origin);

    return basis;

}


Aff_transformation Basis::createAff_transformation() {

    double m00 = axis[0].dx();
    double m01 = axis[0].dy();
    double m02 = axis[0].dz();

    double m10 = axis[1].dx();
    double m11 = axis[1].dy();
    double m12 = axis[1].dz();

    double m20 = axis[2].dx();
    double m21 = axis[2].dy();
    double m22 = axis[2].dz();

    double m03 = origin.x();
    double m13 = origin.y();
    double m23 = origin.z();

    return Aff_transformation(m00, m01, m02, m03,
                              m10, m11, m12, m13,
                              m20, m21, m22, m23);
}


std::ostream &operator<<(std::ostream &stream, const Basis &basis) {
    for (auto axes : basis.axis)
        stream << axes << std::endl;
    stream << basis.origin << std::endl;
    return stream;
}


Aff_transformation constructXRotation(const double &angle) {
    return Aff_transformation(1, 0, 0,
                              0, cos(angle), -sin(angle),
                              0, sin(angle), cos(angle));
}

Aff_transformation constructYRotation(const double &angle) {
    return Aff_transformation(cos(angle), 0, sin(angle),
                              0, 1, 0,
                              -sin(angle), 0, cos(angle));
}

Aff_transformation constructZRotation(const double &angle) {
    return Aff_transformation(cos(angle), -sin(angle), 0,
                              sin(angle), cos(angle), 0,
                              0, 0, 1);
}

void makeRZPointTransformation(Point &point, const double &dr) {

    double dx = 0;
    double dy = 0;

    double x = point.x();
    double y = point.y();

    if (x == 0 && y != 0)

        dy = dr * (y > 0) - (y < 0);

    else if (x != 0 && y == 0)

        dx = dr * (x > 0) - (x < 0);

    else if (x != 0 && y != 0) {

        double Cx = y / x * y / x;
        double Cy = x / y * x / y;

        dx = dr / sqrt(1 + Cx) * ((x > 0) - (x < 0));
        dy = dr / sqrt(1 + Cy) * ((y > 0) - (y < 0));

    }

    auto translation = Aff_transformation(CGAL::TRANSLATION, Vector(dx, dy, 0));

    point = translation(point);

}
