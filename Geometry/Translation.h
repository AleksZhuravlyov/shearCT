#ifndef GEOMETRY_TRANSLATION_H
#define GEOMETRY_TRANSLATION_H

#include <Geometry/Transformation.h>


/// This class is realization of interface Transformation for X translation.
class TranslationX : public Transformation {
public:
  TranslationX() {}
  virtual CgalTransformation operator()(const double &value) override final;
};

/// This class is realization of interface Transformation for Y translation.
class TranslationY : public Transformation {
public:
  TranslationY() {}
  virtual CgalTransformation operator()(const double &value) override final;
};

/// This class is realization of interface Transformation for Z translation.
class TranslationZ : public Transformation {
public:
  TranslationZ() {}
  virtual CgalTransformation operator()(const double &value) override final;
};


#endif // GEOMETRY_TRANSLATION_H
