#ifndef GEOMETRY_STRETCH_H
#define GEOMETRY_STRETCH_H

#include <Geometry/TransformationFunctor.h>


/// This class is realization of interface TransformationFunctor for XY stretching.
class StretchXY : public TransformationFunctor {
public:
  StretchXY() {}
  virtual Transformation operator()(const double &value) override final;
};


#endif // GEOMETRY_STRETCH_H
