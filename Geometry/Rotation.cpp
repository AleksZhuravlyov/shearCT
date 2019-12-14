#include <Geometry/Rotation.h>

#include <cmath>


CgalTransformation RotationX::operator()(const double &value) {
  return CgalTransformation(1, 0, 0,
                            0, cos(value), -sin(value),
                            0, sin(value), cos(value));
}

CgalTransformation RotationY::operator()(const double &value) {
  return CgalTransformation(cos(value), 0, sin(value),
                            0, 1, 0,
                            -sin(value), 0, cos(value));
}

CgalTransformation RotationZ::operator()(const double &value) {
  return CgalTransformation(cos(value), -sin(value), 0,
                            sin(value), cos(value), 0,
                            0, 0, 1);
}