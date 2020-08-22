/**
 @file
 @brief File for translation realisations of TransformationFunctor class.
 It is header which contains translation realisations of
 TransformationFunctor class.
*/


#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "TransformationFunctor.h"


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


#endif // TRANSLATION_H
