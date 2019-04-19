/**
 @file
 @brief File for Basis class.
 It is header which contains Basis class.
*/

#ifndef BASIS_H
#define BASIS_H

#include <vector>

#include <CgalAliases.h>


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
    Basis(const Point &_origin);

    /**
     Constructor sets the directions for basis axes.
     @param[in] _axes is the directions for basis axes.
    */
    Basis(const std::vector<Direction> &_axes);

    /**
     Constructor sets the point of basis origin and
     the directions for basis axes. It is general constructor.
     @param[in] _axes is the directions for basis axes.
     @param[in] _origin is the point of base origin.
    */
    Basis(const std::vector<Direction> &_axes, const Point &_origin);

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
     Make transformation for this basis.
     @param[in] transformation acts on this Basis class.
    */
    void transform(const Aff_transformation &transformation);

    /**
     Generate transformation constructed from this Basis class.
     @return transformation is constructed from this Basis class.
    */
    Aff_transformation generateTransformation();

    /**
     Mutator for origin attribute.
     @param[in] _origin is assigned to origin attribute.
    */
    void setOrigin(const Point &_origin);

    /**
     Mutator for axes attribute.
     @param[in] _axes is assigned to axes attribute.
    */
    void setAxes(const std::vector<Direction> &_axes);

    /**
     Accessor for origin attribute.
     @return shared pointer of origin attribute.
    */
    std::shared_ptr<Point> getOrigin();

    /**
     Accessor for axes attribute.
     @return shared pointer of axes attribute.
    */
    std::shared_ptr<std::vector<Direction>> getAxes();

    /**
     Overload insertion operator.
     @param[in, out] stream is the instance of class std::ostream.
     @param[in] basis is the instance of class Basis.
    */
    friend std::ostream &operator<<(std::ostream &stream, const Basis &basis);


private:

    Point origin; ///< The origin point of this basis.

    std::vector<Direction> axes; ///< The directions of axes of this basis.

};


#endif //BASIS_H
