#include <Geometry/Translation.h>

typedef Kernel::Vector_3 Vector_3;


Transformation TranslationX::operator()(const double &value) {
  return Transformation(CGAL::TRANSLATION, Vector_3(value, 0, 0));
}

Transformation TranslationY::operator()(const double &value) {
  return Transformation(CGAL::TRANSLATION, Vector_3(0, value, 0));
}

Transformation TranslationZ::operator()(const double &value) {
  return Transformation(CGAL::TRANSLATION, Vector_3(0, 0, value));
}