/**
 @file
 @brief File for stretch realisations of TransformationFunctor class.
 It is header which contains stretch realisations of
 TransformationFunctor class.
*/


#ifndef STRETCH_H
#define STRETCH_H

#include "TransformationFunctor.h"


/// This class is realization of TransformationFunctor class for XY stretch.
class StretchXY : public TransformationFunctor {
public:
    StretchXY() {}

    virtual Transformation operator()(const double &value) override final;
};

/// This class is realization of TransformationFunctor class for XYZ stretch.
class StretchXYZ : public TransformationFunctor {
public:
    StretchXYZ() {}

    virtual Transformation operator()(const double &value) override final;
};


#endif // STRETCH_H
