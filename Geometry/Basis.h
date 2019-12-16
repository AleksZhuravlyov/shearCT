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
   Constructor sets the point of basis_ origin_.
   @param[in] origin is the point of base origin_.
  */
  Basis(const Point &origin);

  /**
   Constructor sets the directions for basis_ axes_.
   @param[in] axes is the directions for basis_ axes_.
  */
  Basis(const std::vector<Direction> &axes);

  /**
   Constructor sets the point of basis_ origin_ and
   the directions for basis_ axes_. It is general constructor.
   @param[in] axes is the directions for basis_ axes_.
   @param[in] origin is the point of base origin_.
  */
  Basis(const std::vector<Direction> &axes, const Point &origin);

  /**
   Move constructor.
   @param[in] basis is the instance of class Basis.
  */
  Basis(const Basis &basis);

  /**
   Copy constructor.
   @param[in, out] basis is the instance of class Basis.
  */
  Basis(Basis &&basis);


  /**
   Destructor sets by default.
  */
  virtual ~Basis() {}


  /**
   Overload assignment operator.
   @param[in, out] basis is the instance of class Basis.
  */
  Basis &operator=(Basis &&basis);

  /**
   Make transformation for this basis_.
   @param[in] transformation acts on this Basis class.
  */
  void transform(const Transformation &transformation);

  /**
   Generate transformation constructed from this Basis class.
   @return transformation is constructed from this Basis class.
  */
  Transformation generateTransformation();

  /**
   Mutator for origin_ attribute.
   @param[in] origin is assigned to origin_ attribute.
  */
  void setOrigin(const Point &origin);

  /**
   Mutator for axes_ attribute.
   @param[in] axes is assigned to axes_ attribute.
  */
  void setAxes(const std::vector<Direction> &axes);

  /**
   Accessor for origin_ attribute.
   @return shared pointer of origin_ attribute.
  */
  std::shared_ptr<Point> getOrigin();

  /**
   Accessor for axes_ attribute.
   @return shared pointer of axes_ attribute.
  */
  std::shared_ptr<std::vector<Direction>> getAxes();

  /**
   Overload insertion operator.
   @param[in, out] stream is the instance of class std::ostream.
   @param[in] basis is the instance of class Basis.
  */
  friend std::ostream &operator<<(std::ostream &stream, const Basis &basis);


private:

  Point origin_; ///< The origin_ point of this basis_.

  std::vector<Direction> axes_; ///< The directions of axes_ of this basis_.

};


#endif // GEOMETRY_BASIS_H
