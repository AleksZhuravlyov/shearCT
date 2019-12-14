#include <Geometry/Stretch.h>


CgalTransformation StretchXY::operator()(const double &value) {
  return CgalTransformation(value, 0, 0,
                            0, value, 0,
                            0, 0, 1);
}
