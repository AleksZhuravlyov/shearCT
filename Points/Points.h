/**
 @file
 @brief File for Points class.
 It is header which contains Points class.
*/

#ifndef POINTSCT_H
#define POINTSCT_H

#include <vector>
#include <string>

#include <CGAL/Point_3.h>
#include <CGAL/Bbox_3.h>

#include <Geometry/Basis.h>

typedef std::vector<double> Value;
typedef std::vector<CgalPoint> Points_3;
typedef CGAL::Bbox_3 Bbox_3;


/// This class is set of CGAL points_3 with convenient methods and attributes.
class Points {

public:

    /**
     Constructor sets by default.
    */
    Points();

    /**
     Constructor sets CGAL points_3.
     @param[in] _points_3 is CGAL points_3 for points_3 attribute.
    */
    Points(std::shared_ptr<Points_3> _points_3);

    /**
     Constructor sets CGAL Bbox_3.
     @param[in] bbox_3 is CGAL Bbox_3 for points_3 settings.
    */
    Points(const Bbox_3 &bbox_3);

    /**
     Constructor sets CGAL points_3 and basis.
     @param[in] _points_3 is CGAL points_3 for points_3 attribute.
     @param[in] _basis is CGAL based basis for basis attribute.
    */
    Points(std::shared_ptr<Points_3> _points_3,
           std::shared_ptr<Basis> _basis);

    /**
     Move constructor.
     @param[in] points is the instance of class Points.
    */
    Points(const Points &points);

    /**
     Copy constructor.
     @param[in, out] points is the instance of class Points.
    */
    Points(Points &&points);


    /**
     Destructor sets by default.
    */
    virtual ~Points() {}

    /**
     Overload assignment operator.
     @param[in, out] points is the instance of class Points.
    */
    Points &operator=(Points &&points);


    /**
     Make aff_transformation_3 for this Points class.
     @param[in] aff_transformation_3 acts on this Points class.
    */
    void transform(const CgalTransformation &aff_transformation_3);


    /**
     Make aff_transformations_3 for this Points class.
     @param[in] aff_transformations_3 acts on this Points class.
    */
    void transform(const std::vector<CgalTransformation> &aff_transformations_3);


    /**
     Move origin_ attribute of basis attribute to particular point_3.
     @param[in] point_3 setts origin_ attribute of basis attribute.
    */
    void translateBasis(const CgalPoint &point_3);

    /**
     Move basis attribute to the center of points_3 attribute.
    */
    void translateBasisToCenter();


    /**
     Accessor for points_3 number in points_3 attribute.
     @return number of points_3 in points_3 attribute.
    */
    int size();

    /**
     Mutator for points_3 attribute.
     @param[in] _points_3 is assigned to points_3 attribute.
    */
    void setPoints(std::shared_ptr<Points_3> _points_3);

    /**
     Mutator for basis attribute.
     @param[in] _basis is assigned to basis attribute.
    */
    void setBasis(std::shared_ptr<Basis> _basis);


    /**
     Mutator for tomoA attribute.
     @param[in] _tomoA is assigned to tomoA attribute.
    */
    void setTomoA(std::shared_ptr<Value> _tomoA);

    /**
     Mutator for tomoB attribute.
     @param[in] _tomoB is assigned to tomoB attribute.
    */
    void setTomoB(std::shared_ptr<Value> _tomoB);

    /**
     Mutator for tomoBuffer attribute.
     @param[in] _tomoBuffer is assigned to tomoBuffer attribute.
    */
    void setTomoBuffer(std::shared_ptr<Value> _tomoBuffer);

    /**
     Mutator for result attribute.
     @param[in] _result is assigned to result attribute.
    */
    void setResult(std::shared_ptr<Value> _result);

    /**
     Swap tomoA and tomoBuffer attributes.
    */
    void swapTomoAAndTomoBuffer();


    /**
     Accessor for points_3 attribute.
     @return points_3 attribute.
    */
    std::shared_ptr<Points_3> getPoints();

    /**
     Accessor for basis attribute.
     @return basis attribute.
    */
    std::shared_ptr<Basis> getBasis();


    /**
     Accessor for tomoA attribute.
     @return tomoA attribute.
    */
    std::shared_ptr<Value> getTomoA();

    /**
     Accessor for tomoB attribute.
     @return tomoB attribute.
    */
    std::shared_ptr<Value> getTomoB();

    /**
     Accessor for tomoBuffer attribute.
     @return tomoBuffer attribute.
    */
    std::shared_ptr<Value> getTomoBuffer();

    /**
     Accessor for result attribute.
     @return result attribute.
    */
    std::shared_ptr<Value> getResult();


    /**
     Generate CGAL boundary box from points_3 attribute.
    */
    Bbox_3 generateBbox();

    /**
     Assign subtraction tomoA and TomoB attributes to result attribute.
    */
    void computeResult();

    /**
     Compute Pearson correlation between tomoA and TomoB attributes.
     @return Pearson correlation between tomoA and TomoB attributes.
    */
    double computePearsonCorrelation();

    /**
     Generate square in XY plane inside this class Points.
     @param[in] xCenter is the X center of square.
     @param[in] yCenter is the Y center of square.
     @param[in] zCenter is the Z center of square.
     @param[in] xWidth is the X width of square.
     @param[in] yWidth is the Y width of square.
     @param[in] nX is the number of points_3 on X axis.
     @param[in] nY is the number of points_3 on Y axis.
    */
    void createXYSquare(const double &xCenter,
                        const double &yCenter,
                        const double &zCenter,
                        const double &xWidth, const double &yWidth,
                        const int &nX, const int &nY);

    /**
     Generate cylinder surface segment in XY plane inside this class Points.
     @param[in] xCylinderBaseCenter is the X center of cylinder base.
     @param[in] yCylinderBaseCenter is the Y center of cylinder base.
     @param[in] zCylinderBaseCenter is the Z center of cylinder base.
     @param[in] R is the radius of cylinder segment.
     @param[in] angleCenter is the orientation of cylinder segment.
     @param[in] zWidth is height of cylinder segment.
     @param[in] angleWidth is the angle width of cylinder segment.
     @param[in] nZ is the number of points_3 on Z axis.
     @param[in] nAngle is the number of points_3 on nAngle axis.
    */
    void createZCylinderSegment(const double &xCylinderBaseCenter,
                                const double &yCylinderBaseCenter,
                                const double &zCylinderBaseCenter,
                                const double &R,
                                const double &angleCenter,
                                const double &zWidth,
                                const double &angleWidth,
                                const int &nZ, const int &nAngle);


private:

    std::shared_ptr<Points_3> points_3; ///< CGAL points_3.

    std::shared_ptr<Basis> basis; ///< Basis based on CGAL.


    std::shared_ptr<Value> tomoA; ///< Value connected to points_3.

    std::shared_ptr<Value> tomoB; ///< Value connected to points_3.

    std::shared_ptr<Value> tomoBuffer; ///< Value connected to points_3.

    std::shared_ptr<Value> result; ///< Value connected to points_3.

};


#endif //POINTSCT_H
