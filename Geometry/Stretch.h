#ifndef GEOMETRY_STRETCH_H
#define GEOMETRY_STRETCH_H

#include <Geometry/Transformation.h>


/// This class is realization of interface Transformation for XY stretching.
class StretchXY : public Transformation {
public:
  StretchXY() {}
  virtual CgalTransformation operator()(const double &value) override final;
};


#endif // GEOMETRY_STRETCH_H
