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

typedef CGAL::Simple_cartesian<double> Kernel_3;
typedef CGAL::Aff_transformation_3<Kernel_3> Aff_transformation_3;
typedef Kernel_3::Point_3 Point_3;
typedef Kernel_3::Direction_3 Direction_3;


/// This class is realisation of orthogonal coordinate system on CGAL library.
class Basis {

public:

  /**
   Constructor sets by default.
  */
  Basis();

  /**
   Constructor sets the point of basis origin.
   @param[in] _origin is the point of base origin.
  */
  Basis(const Point_3 &_origin);

  /**
   Constructor sets the directions for basis axes.
   @param[in] _axes is the directions for basis axes.
  */
  Basis(const std::vector<Direction_3> &_axes);

  /**
   Constructor sets the point of basis origin and
   the directions for basis axes. It is general constructor.
   @param[in] _axes is the directions for basis axes.
   @param[in] _origin is the point of base origin.
  */
  Basis(const std::vector<Direction_3> &_axes, const Point_3 &_origin);

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
   Make aff_transformation_3 for this basis.
   @param[in] aff_transformation_3 acts on this Basis class.
  */
  void transform(const Aff_transformation_3 &aff_transformation_3);

  /**
   Generate transformation constructed from this Basis class.
   @return transformation is constructed from this Basis class.
  */
  Aff_transformation_3 generateTransformation();

  /**
   Mutator for origin attribute.
   @param[in] _origin is assigned to origin attribute.
  */
  void setOrigin(const Point_3 &_origin);

  /**
   Mutator for axes attribute.
   @param[in] _axes is assigned to axes attribute.
  */
  void setAxes(const std::vector<Direction_3> &_axes);

  /**
   Accessor for origin attribute.
   @return shared pointer of origin attribute.
  */
  std::shared_ptr<Point_3> getOrigin();

  /**
   Accessor for axes attribute.
   @return shared pointer of axes attribute.
  */
  std::shared_ptr<std::vector<Direction_3>> getAxes();

  /**
   Overload insertion operator.
   @param[in, out] stream is the instance of class std::ostream.
   @param[in] basis is the instance of class Basis.
  */
  friend std::ostream &operator<<(std::ostream &stream, const Basis &basis);


private:

  Point_3 origin; ///< The origin point of this basis.

  std::vector<Direction_3> axes; ///< The directions of axes of this basis.

};


#endif // GEOMETRY_BASIS_H
