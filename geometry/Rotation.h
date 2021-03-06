/**
 @file
 @brief File for rotation realisations of TransformationFunctor class.
 It is header which contains rotation realisations of
 TransformationFunctor class.
*/


#ifndef ROTATION_H
#define ROTATION_H

#include "TransformationFunctor.h"


/// This class is realization of TransformationFunctor class for X rotation.
class RotationX : public TransformationFunctor {
public:
    RotationX() {}

    virtual Transformation operator()(const double &value) override final;
};

/// This class is realization of TransformationFunctor class for Y rotation.
class RotationY : public TransformationFunctor {
public:
    RotationY() {}

    virtual Transformation operator()(const double &value) override final;
};

/// This class is realization of TransformationFunctor class for Z rotation.
class RotationZ : public TransformationFunctor {
public:
    RotationZ() {}

    virtual Transformation operator()(const double &value) override final;
};


#endif // ROTATION_H
