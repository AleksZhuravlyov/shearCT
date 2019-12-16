/**
 @file
 @brief File for translation realisations of TransformationFunctor class.
 It is header which contains translation realisations of
 TransformationFunctor class.
*/


#ifndef GEOMETRY_TRANSLATION_H
#define GEOMETRY_TRANSLATION_H

#include <Geometry/TransformationFunctor.h>


/// This class is realization of TransformationFunctor class for X translation.
class TranslationX : public TransformationFunctor {
public:
  TranslationX() {}

  virtual Transformation operator()(const double &value) override final;
};

/// This class is realization of TransformationFunctor class for Y translation.
class TranslationY : public TransformationFunctor {
public:
  TranslationY() {}

  virtual Transformation operator()(const double &value) override final;
};

/// This class is realization of TransformationFunctor class for Z translation.
class TranslationZ : public TransformationFunctor {
public:
  TranslationZ() {}

  virtual Transformation operator()(const double &value) override final;
};


#endif // GEOMETRY_TRANSLATION_H
