#include <Geometry/Basis.h>

#include <cmath>


Basis::Basis() :
    Basis({Direction_3(1, 0, 0),
           Direction_3(0, 1, 0),
           Direction_3(0, 0, 1)},
          Point_3(0, 0, 0)) {}

Basis::Basis(const Point_3 &_origin) :
    Basis({Direction_3(1, 0, 0),
           Direction_3(0, 1, 0),
           Direction_3(0, 0, 1)},
          _origin) {}

Basis::Basis(const std::vector<Direction_3> &_axes) :
    Basis(_axes, Point_3(0, 0, 0)) {}

Basis::Basis(const std::vector<Direction_3> &_axes, const Point_3 &_origin) :
    axes(_axes), origin(_origin) {}


Basis::Basis(const Basis &basis) : axes(basis.axes), origin(basis.origin) {}

Basis::Basis(Basis &&basis) : axes(basis.axes), origin(basis.origin) {}


Basis &Basis::operator=(Basis &&basis) {

  if (&basis == this)
    return *this;

  axes = basis.axes;
  origin = basis.origin;
  return *this;

}


void Basis::transform(const Aff_transformation_3 &aff_transformation_3) {

  origin = aff_transformation_3(origin);

  for (auto &&axis : axes)
    axis = aff_transformation_3(axis);

  double absValue;
  for (auto &&axis : axes) {

    absValue = sqrt(axis.dx() * axis.dx() +
                    axis.dy() * axis.dy() +
                    axis.dz() * axis.dz());

    axis = Direction_3(axis.dx() / absValue,
                       axis.dy() / absValue,
                       axis.dz() / absValue);

  }

}


Aff_transformation_3 Basis::generateTransformation() {

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

  return Aff_transformation_3(m00, m01, m02, m03,
                              m10, m11, m12, m13,
                              m20, m21, m22, m23);
}


void Basis::setOrigin(const Point_3 &_origin) {
  origin = _origin;
}

void Basis::setAxes(const std::vector<Direction_3> &_axes) {
  axes = _axes;
}


std::shared_ptr<Point_3> Basis::getOrigin() {
  return std::make_shared<Point_3>(origin);
}

std::shared_ptr<std::vector<Direction_3>> Basis::getAxes() {
  return std::make_shared<std::vector<Direction_3>>(axes);
}


std::ostream &operator<<(std::ostream &stream, const Basis &basis) {
  for (auto axes : basis.axes)
    stream << axes << std::endl;
  stream << basis.origin << std::endl;
  return stream;
}
