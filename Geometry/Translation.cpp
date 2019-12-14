#include <Geometry/Translation.h>

typedef CgalKernel::Vector_3 Vector_3;


CgalTransformation TranslationX::operator()(const double &value) {
  return CgalTransformation(CGAL::TRANSLATION, Vector_3(value, 0, 0));
}

CgalTransformation TranslationY::operator()(const double &value) {
  return CgalTransformation(CGAL::TRANSLATION, Vector_3(0, value, 0));
}

CgalTransformation TranslationZ::operator()(const double &value) {
  return CgalTransformation(CGAL::TRANSLATION, Vector_3(0, 0, value));
}