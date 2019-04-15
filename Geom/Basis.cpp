#include <Basis.h>
#include <Transformation.h>


Basis::Basis(const std::vector<Direction> &_axis, const Point &_origin) :
        axes(_axis), origin(_origin) {}

Basis::Basis(const Point &_origin) :
        Basis({Direction(1, 0, 0), Direction(0, 1, 0), Direction(0, 0, 1)},
              _origin) {}

Basis::Basis(const std::vector<Direction> &_axis) :
        Basis(_axis, Point(0, 0, 0)) {}

Basis::Basis() :
        Basis({Direction(1, 0, 0), Direction(0, 1, 0), Direction(0, 0, 1)},
              Point(0, 0, 0)) {}

Basis::Basis(const Basis &basis) : axes(basis.axes), origin(basis.origin) {}

Basis::Basis(Basis &&basis) : axes(basis.axes), origin(basis.origin) {}


Basis &Basis::operator=(Basis &&basis) {
    std::swap(axes, basis.axes);
    std::swap(origin, basis.origin);
    return *this;
}


Basis Basis::transform(const Aff_transformation &aff_transformation) {

    Basis basis;

    for (auto &&axis : basis.axes)
        axis = aff_transformation(axis);

    basis.origin = aff_transformation(origin);

    return basis;

}


Aff_transformation Basis::createAff_transformation() {

    double m00 = axes[0].dx();
    double m01 = axes[0].dy();
    double m02 = axes[0].dz();

    double m10 = axes[1].dx();
    double m11 = axes[1].dy();
    double m12 = axes[1].dz();

    double m20 = axes[2].dx();
    double m21 = axes[2].dy();
    double m22 = axes[2].dz();

    double m03 = origin.x();
    double m13 = origin.y();
    double m23 = origin.z();

    return Aff_transformation(m00, m01, m02, m03,
                              m10, m11, m12, m13,
                              m20, m21, m22, m23);
}


void Basis::setAxes(const std::vector<Direction> &_axes) {
    axes = _axes;
}

void Basis::setOrigin(const Point &_origin) {
    origin = _origin;
}


std::shared_ptr<std::vector<Direction>> Basis::getDirections() {
    return std::make_shared<std::vector<Direction>>(axes);
}

std::shared_ptr<Point> Basis::getOrigin() {
    return std::make_shared<Point>(origin);
}


std::ostream &operator<<(std::ostream &stream, const Basis &basis) {
    for (auto axes : basis.axes)
        stream << axes << std::endl;
    stream << basis.origin << std::endl;
    return stream;
}



