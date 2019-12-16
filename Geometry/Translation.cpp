#include <Geometry/Translation.h>

typedef Kernel::Vector_3 Vector;


Transformation TranslationX::operator()(const double &value) {
  return Transformation(CGAL::TRANSLATION, Vector(value, 0, 0));
}

Transformation TranslationY::operator()(const double &value) {
  return Transformation(CGAL::TRANSLATION, Vector(0, value, 0));
}

Transformation TranslationZ::operator()(const double &value) {
  return Transformation(CGAL::TRANSLATION, Vector(0, 0, value));
}
