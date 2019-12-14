/**
 @file
 @brief File for Transformation interface and its realization.
 It is header which contains Transformation class and its inheritances.
*/

#ifndef GEOMETRY_TRANSFORMATION_H
#define GEOMETRY_TRANSFORMATION_H

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Aff_transformation_3.h>


typedef CGAL::Simple_cartesian<double> CgalKernel;
typedef CGAL::Aff_transformation_3<CgalKernel> CgalTransformation;


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
  virtual CgalTransformation operator()(const double &value) = 0;

};


#endif // GEOMETRY_TRANSFORMATION_H
