/**
 @file
 @brief File for Transformation interface and its realization.
 It is header which contains Transformation class and its inheritances.
*/

#ifndef GEOMETRY_TRANSFORMATIONFUCTOR_H
#define GEOMETRY_TRANSFORMATIONFUCTOR_H

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Aff_transformation_3.h>


typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Aff_transformation_3<Kernel> Transformation;


/// This class is interface for translations of CGAL library.
class TransformationFunctor {

public:

  /**
   Constructor sets by default.
  */
  TransformationFunctor() = default;

  /**
   Constructor sets by default.
  */
  virtual ~TransformationFunctor() {}

  /**
   Pure virtual overload function call operator.
   This method constructs CGAL transformation.
   @param[in] value is parameter for transformation.
   @return transformation is constructed by this method.
  */
  virtual Transformation operator()(const double &value) = 0;

};


#endif // GEOMETRY_TRANSFORMATIONFUCTOR_H
