/**
 @file
 @brief File for Transformation interface and its realization.
 It is header which contains Transformation class and its inheritances.
*/

#ifndef GEOMETRY_TRANSFORMATION_H
#define GEOMETRY_TRANSFORMATION_H

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Aff_transformation_3.h>


typedef CGAL::Simple_cartesian<double> Kernel_3;
typedef CGAL::Aff_transformation_3<Kernel_3> Aff_transformation_3;


/// This class is interface for translations of CGAL library.
class Transformation {

public:

    /**
     Constructor sets by default.
    */
    Transformation() = default;

    /**
     Constructor sets by default.
    */
    virtual ~Transformation() {}

    /**
     Pure virtual overload function call operator.
     This method constructs CGAL transformation.
     @param[in] value is parameter for transformation.
     @return transformation is constructed by this method.
    */
    virtual Aff_transformation_3 operator()(const double &value) = 0;

};


/// This class is realization of interface Transformation for X rotation.
class RotationX : public Transformation {
public:
    RotationX() {}
    virtual Aff_transformation_3 operator()(const double &value) override final;
};

/// This class is realization of interface Transformation for Y rotation.
class RotationY : public Transformation {
public:
    RotationY() {}
    virtual Aff_transformation_3 operator()(const double &value) override final;
};

/// This class is realization of interface Transformation for Z rotation.
class RotationZ : public Transformation {
public:
    RotationZ() {}
    virtual Aff_transformation_3 operator()(const double &value) override final;
};

/// This class is realization of interface Transformation for X translation.
class TranslationX : public Transformation {
public:
    TranslationX() {}
    virtual Aff_transformation_3 operator()(const double &value) override final;
};

/// This class is realization of interface Transformation for Y translation.
class TranslationY : public Transformation {
public:
    TranslationY() {}
    virtual Aff_transformation_3 operator()(const double &value) override final;
};

/// This class is realization of interface Transformation for Z translation.
class TranslationZ : public Transformation {
public:
    TranslationZ() {}
    virtual Aff_transformation_3 operator()(const double &value) override final;
};

/// This class is realization of interface Transformation for XY stretching.
class StretchingXY : public Transformation {
public:
    StretchingXY() {}
    virtual Aff_transformation_3 operator()(const double &value) override final;
};


#endif // GEOMETRY_TRANSFORMATION_H
