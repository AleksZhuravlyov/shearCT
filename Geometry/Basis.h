/**
 @file
 @brief File for Basis class.
 It is header which contains Basis class.
*/


#ifndef GEOMETRY_BASIS_H
#define GEOMETRY_BASIS_H

#include <vector>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Direction_3.h>


typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Aff_transformation_3<Kernel> Transformation;
typedef Kernel::Point_3 Point;
typedef Kernel::Direction_3 Direction;


/// This class is realisation of orthogonal coordinate system on CGAL library.
class Basis {

public:

  /**
   Constructor sets by default.
  */
  Basis();

  /**
   Constructor sets point which is used for origin_ initialization.
   @param[in] origin is point for origin_ initialization.
  */
  Basis(const Point &origin);

  /**
   Constructor sets directions for axes_ initialization.
   @param[in] axes is directions for axes_ initialization.
  */
  Basis(const std::vector<Direction> &axes);

  /**
   Constructor sets directions and point which are used for
   axes_ and origin_ initialization. This constructor is general.
   @param[in] axes is directions for axes_ initialization.
   @param[in] origin is point for origin_ initialization.
  */
  Basis(const std::vector<Direction> &axes, const Point &origin);


  /**
   Move constructor.
   @param[in] basis is instance of class Basis.
  */
  Basis(const Basis &basis);

  /**
   Copy constructor.
   @param[in, out] basis is instance of class Basis.
  */
  Basis(Basis &&basis);


  /**
   Destructor is set by default.
  */
  virtual ~Basis() {}


  /**
   Overload assignment operator.
   @param[in, out] basis is instance of this class.
  */
  Basis &operator=(Basis &&basis);


  /**
   Make transformation for this class.
   @param[in] transformation governs the transformation.
  */
  void transform(const Transformation &transformation);

  /**
   Generate transformation constructed from this class.
   @return transformation is constructed from this class.
  */
  Transformation generateTransformation();


  /**
   Mutator for origin_.
   @param[in] origin is assigned to origin_.
  */
  void setOrigin(const Point &origin);

  /**
   Mutator for axes_.
   @param[in] axes is assigned to axes_.
  */
  void setAxes(const std::vector<Direction> &axes);


  /**
   Accessor for origin_.
   @return shared pointer of origin_.
  */
  std::shared_ptr<Point> getOrigin();

  /**
   Accessor for axes_.
   @return shared pointer of axes_.
  */
  std::shared_ptr<std::vector<Direction>> getAxes();


  /**
   Overload insertion operator.
   @param[in, out] stream is the instance of class std::ostream.
   @param[in] basis is the instance of this class.
  */
  friend std::ostream &operator<<(std::ostream &stream, const Basis &basis);


private:

  Point origin_; ///< Origin of this class.

  std::vector<Direction> axes_; ///< Axes of this class.

};


#endif // GEOMETRY_BASIS_H
