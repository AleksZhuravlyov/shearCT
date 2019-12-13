#include <Geometry/Transformation.h>

#include <cmath>

typedef Kernel_3::Vector_3 Vector_3;


Aff_transformation_3 RotationX::operator()(const double &value) {
  return Aff_transformation_3(1, 0, 0,
                              0, cos(value), -sin(value),
                              0, sin(value), cos(value));
}

Aff_transformation_3 RotationY::operator()(const double &value) {
  return Aff_transformation_3(cos(value), 0, sin(value),
                              0, 1, 0,
                              -sin(value), 0, cos(value));
}

Aff_transformation_3 RotationZ::operator()(const double &value) {
  return Aff_transformation_3(cos(value), -sin(value), 0,
                              sin(value), cos(value), 0,
                              0, 0, 1);
}

Aff_transformation_3 TranslationX::operator()(const double &value) {
  return Aff_transformation_3(CGAL::TRANSLATION, Vector_3(value, 0, 0));
}

Aff_transformation_3 TranslationY::operator()(const double &value) {
  return Aff_transformation_3(CGAL::TRANSLATION, Vector_3(0, value, 0));
}

Aff_transformation_3 TranslationZ::operator()(const double &value) {
  return Aff_transformation_3(CGAL::TRANSLATION, Vector_3(0, 0, value));
}

Aff_transformation_3 StretchingXY::operator()(const double &value) {
  return Aff_transformation_3(value, 0, 0,
                              0, value, 0,
                              0, 0, 1);
}
