#ifndef GEOMETRY_ROTATION_H
#define GEOMETRY_ROTATION_H

#include <Geometry/TransformationFunctor.h>


/// This class is realization of interface TransformationFunctor for X rotation.
class RotationX : public TransformationFunctor {
public:
  RotationX() {}

  virtual Transformation operator()(const double &value) override final;
};

/// This class is realization of interface TransformationFunctor for Y rotation.
class RotationY : public TransformationFunctor {
public:
  RotationY() {}

  virtual Transformation operator()(const double &value) override final;
};

/// This class is realization of interface TransformationFunctor for Z rotation.
class RotationZ : public TransformationFunctor {
public:
  RotationZ() {}

  virtual Transformation operator()(const double &value) override final;
};


#endif // GEOMETRY_ROTATION_H
