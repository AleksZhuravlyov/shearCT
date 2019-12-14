#ifndef GEOMETRY_ROTATION_H
#define GEOMETRY_ROTATION_H

#include <Geometry/Transformation.h>


/// This class is realization of interface Transformation for X rotation.
class RotationX : public Transformation {
public:
  RotationX() {}

  virtual CgalTransformation operator()(const double &value) override final;
};

/// This class is realization of interface Transformation for Y rotation.
class RotationY : public Transformation {
public:
  RotationY() {}

  virtual CgalTransformation operator()(const double &value) override final;
};

/// This class is realization of interface Transformation for Z rotation.
class RotationZ : public Transformation {
public:
  RotationZ() {}

  virtual CgalTransformation operator()(const double &value) override final;
};


#endif // GEOMETRY_ROTATION_H
