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

typedef CGAL::Simple_cartesian<double> CgalKernel;
typedef CGAL::Aff_transformation_3<CgalKernel> CgalTransformation;
typedef CgalKernel::Point_3 CgalPoint;
typedef CgalKernel::Direction_3 CgalDirection;


/// This class is realisation of orthogonal coordinate system on CGAL library.
class Basis {

public:

  /**
   Constructor sets by default.
  */
  Basis();

  /**
   Constructor sets the point of basis origin_.
   @param[in] origin is the point of base origin_.
  */
  Basis(const CgalPoint &origin);

  /**
   Constructor sets the directions for basis axes_.
   @param[in] axes is the directions for basis axes_.
  */
  Basis(const std::vector<CgalDirection> &axes);

  /**
   Constructor sets the point of basis origin_ and
   the directions for basis axes_. It is general constructor.
   @param[in] axes is the directions for basis axes_.
   @param[in] origin is the point of base origin_.
  */
  Basis(const std::vector<CgalDirection> &axes, const CgalPoint &origin);

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
   Make cgalTransformation for this basis.
   @param[in] cgalTransformation acts on this Basis class.
  */
  void transform(const CgalTransformation &cgalTransformation);

  /**
   Generate transformation constructed from this Basis class.
   @return transformation is constructed from this Basis class.
  */
  CgalTransformation generateTransformation();

  /**
   Mutator for origin_ attribute.
   @param[in] origin is assigned to origin_ attribute.
  */
  void setOrigin(const CgalPoint &origin);

  /**
   Mutator for axes_ attribute.
   @param[in] axes is assigned to axes_ attribute.
  */
  void setAxes(const std::vector<CgalDirection> &axes);

  /**
   Accessor for origin_ attribute.
   @return shared pointer of origin_ attribute.
  */
  std::shared_ptr<CgalPoint> getOrigin();

  /**
   Accessor for axes_ attribute.
   @return shared pointer of axes_ attribute.
  */
  std::shared_ptr<std::vector<CgalDirection>> getAxes();

  /**
   Overload insertion operator.
   @param[in, out] stream is the instance of class std::ostream.
   @param[in] basis is the instance of class Basis.
  */
  friend std::ostream &operator<<(std::ostream &stream, const Basis &basis);


private:

  CgalPoint origin_; ///< The origin_ point of this basis.

  std::vector<CgalDirection> axes_; ///< The directions of axes_ of this basis.

};


#endif // GEOMETRY_BASIS_H
