#include "Rotation.h"

#include <cmath>


Transformation RotationX::operator()(const double &value) {
    return Transformation(1, 0, 0,
                          0, cos(value), -sin(value),
                          0, sin(value), cos(value));
}

Transformation RotationY::operator()(const double &value) {
    return Transformation(cos(value), 0, sin(value),
                          0, 1, 0,
                          -sin(value), 0, cos(value));
}

Transformation RotationZ::operator()(const double &value) {
    return Transformation(cos(value), -sin(value), 0,
                          sin(value), cos(value), 0,
                          0, 0, 1);
}
