/**
 @file
 @brief File for stretch realisations of TransformationFunctor class.
 It is header which contains stretch realisations of
 TransformationFunctor class.
*/


#ifndef GEOMETRY_STRETCH_H
#define GEOMETRY_STRETCH_H

#include <Geometry/TransformationFunctor.h>


/// This class is realization of TransformationFunctor class for XY stretch.
class StretchXY : public TransformationFunctor {
public:
  StretchXY() {}

  virtual Transformation operator()(const double &value) override final;
};


#endif // GEOMETRY_STRETCH_H
