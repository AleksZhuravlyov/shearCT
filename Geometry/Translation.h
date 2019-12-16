#ifndef GEOMETRY_TRANSLATION_H
#define GEOMETRY_TRANSLATION_H

#include <Geometry/TransformationFunctor.h>


/// This class is realization of interface TransformationFunctor for X translation.
class TranslationX : public TransformationFunctor {
public:
  TranslationX() {}
  virtual Transformation operator()(const double &value) override final;
};

/// This class is realization of interface TransformationFunctor for Y translation.
class TranslationY : public TransformationFunctor {
public:
  TranslationY() {}
  virtual Transformation operator()(const double &value) override final;
};

/// This class is realization of interface TransformationFunctor for Z translation.
class TranslationZ : public TransformationFunctor {
public:
  TranslationZ() {}
  virtual Transformation operator()(const double &value) override final;
};


#endif // GEOMETRY_TRANSLATION_H
