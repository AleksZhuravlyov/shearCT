#include <Geometry/Basis.h>

#include <cmath>


Basis::Basis() : Basis({Direction(1, 0, 0),
                        Direction(0, 1, 0),
                        Direction(0, 0, 1)},
                       Point(0, 0, 0)) {}

Basis::Basis(const Point &origin) : Basis({Direction(1, 0, 0),
                                           Direction(0, 1, 0),
                                           Direction(0, 0, 1)},
                                          origin) {}

Basis::Basis(const std::vector<Direction> &axes) :
    Basis(axes, Point(0, 0, 0)) {}

Basis::Basis(const std::vector<Direction> &axes,
             const Point &origin) : axes_(axes), origin_(origin) {}


Basis::Basis(const Basis &basis) : axes_(basis.axes_),
                                   origin_(basis.origin_) {}

Basis::Basis(Basis &&basis) : axes_(basis.axes_),
                              origin_(basis.origin_) {}


Basis &Basis::operator=(Basis &&basis) {

  if (&basis == this)
    return *this;

  axes_ = basis.axes_;
  origin_ = basis.origin_;
  return *this;

}


void Basis::transform(const Transformation &transformation) {

  origin_ = transformation(origin_);

  for (auto &&axis : axes_)
    axis = transformation(axis);

  double absValue;
  for (auto &&axis : axes_) {

    absValue = sqrt(axis.dx() * axis.dx() +
                    axis.dy() * axis.dy() +
                    axis.dz() * axis.dz());

    axis = Direction(axis.dx() / absValue,
                         axis.dy() / absValue,
                         axis.dz() / absValue);

  }

}


Transformation Basis::generateTransformation() {

  double m00 = axes_[0].dx();
  double m01 = axes_[0].dy();
  double m02 = axes_[0].dz();

  double m10 = axes_[1].dx();
  double m11 = axes_[1].dy();
  double m12 = axes_[1].dz();

  double m20 = axes_[2].dx();
  double m21 = axes_[2].dy();
  double m22 = axes_[2].dz();

  double m03 = origin_.x();
  double m13 = origin_.y();
  double m23 = origin_.z();

  return Transformation(m00, m01, m02, m03,
                        m10, m11, m12, m13,
                        m20, m21, m22, m23);
}


void Basis::setOrigin(const Point &origin) {
  origin_ = origin;
}

void Basis::setAxes(const std::vector<Direction> &axes) {
  axes_ = axes;
}


std::shared_ptr<Point> Basis::getOrigin() {
  return std::make_shared<Point>(origin_);
}

std::shared_ptr<std::vector<Direction>> Basis::getAxes() {
  return std::make_shared<std::vector<Direction>>(axes_);
}


std::ostream &operator<<(std::ostream &stream, const Basis &basis) {
  for (auto axes : basis.axes_)
    stream << axes << std::endl;
  stream << basis.origin_ << std::endl;
  return stream;
}
