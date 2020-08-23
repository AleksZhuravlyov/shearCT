#include "Stretch.h"


Transformation StretchXY::operator()(const double &value) {
    return Transformation(value, 0, 0,
                          0, value, 0,
                          0, 0, 1);
}

Transformation StretchXYZ::operator()(const double &value) {
    return Transformation(value, 0, 0,
                          0, value, 0,
                          0, 0, value);
}
