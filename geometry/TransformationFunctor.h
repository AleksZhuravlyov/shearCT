/**
 @file
 @brief File for TransformationFunctor class which is interface.
 It is header which contains TransformationFunctor class.
*/

#ifndef TRANSFORMATIONFUCTOR_H
#define TRANSFORMATIONFUCTOR_H

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Aff_transformation_3.h>


typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Aff_transformation_3<Kernel> Transformation;


/// This class is interface for transformation functor.
class TransformationFunctor {

public:

  /**
   Constructor is set by default.
  */
  TransformationFunctor() = default;

  /**
   Destructor is set by default.
  */
  virtual ~TransformationFunctor() {}

  /**
   Pure virtual overload function call operator.
   This method constructs simple transformation.
   @param[in] value is parameter for transformation.
   @return transformation is constructed by this method.
  */
  virtual Transformation operator()(const double &value) = 0;

};


#endif // TRANSFORMATIONFUCTOR_H
