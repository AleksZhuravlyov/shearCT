/**
 @file
 @brief File for PointsCt class.
 It is header which contains PointsCt class.
*/

#ifndef POINTSCT_H
#define POINTSCT_H

#include <vector>
#include <string>

#include <Basis.h>


typedef std::vector<double> Value;


/// This class is set of CGAL points with convenient methods and attributes.
class PointsCt {

public:

    /**
     Constructor sets by default.
    */
    PointsCt();

    /**
     Constructor sets CGAL points.
     @param[in] _points is CGAL points for points attribute.
    */
    PointsCt(std::shared_ptr<Points> _points);

    /**
     Constructor sets CGAL points and basis.
     @param[in] _points is CGAL points for points attribute.
     @param[in] _basis is CGAL based basis for basis attribute.
    */
    PointsCt(std::shared_ptr<Points> _points,
             std::shared_ptr<Basis> _basis);

    /**
     Move constructor.
     @param[in] pointsCt is the instance of class PointsCt.
    */
    PointsCt(const PointsCt &pointsCt);

    /**
     Copy constructor.
     @param[in, out] pointsCt is the instance of class PointsCt.
    */
    PointsCt(PointsCt &&pointsCt);


    /**
     Destructor sets by default.
    */
    virtual ~PointsCt() {}

    /**
     Overload assignment operator.
     @param[in, out] pointsCt is the instance of class PointsCt.
    */
    PointsCt &operator=(PointsCt &&pointsCt);


    /**
     Make transformation for this PointsCt class.
     @param[in] transformation acts on this PointsCt class.
    */
    void transform(const Aff_transformation &transformation);


    /**
     Move origin attribute of basis attribute to particular point.
     @param[in] point setts origin attribute of basis attribute.
    */
    void translateBasis(const Point &point);

    /**
     Move basis attribute to the center of points attribute.
    */
    void translateBasisToCenter();


    /**
     Accessor for points number in points attribute.
     @return number of points in points attribute.
    */
    int size();

    /**
     Mutator for points attribute.
     @param[in] _points is assigned to points attribute.
    */
    void setPoints(std::shared_ptr<Points> _points);

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
     Accessor for points attribute.
     @return points attribute.
    */
    std::shared_ptr<Points> getPoints();

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
     Generate CGAL boundary box from points attribute.
    */
    Bbox generateBbox();

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
     Generate square in XY plane inside this class PointsCt.
     @param[in] xCenter is the X center of square.
     @param[in] yCenter is the Y center of square.
     @param[in] zCenter is the Z center of square.
     @param[in] xWidth is the X width of square.
     @param[in] yWidth is the Y width of square.
     @param[in] nX is the number of points on X axis.
     @param[in] nY is the number of points on Y axis.
    */
    void createXYSquare(const double &xCenter,
                        const double &yCenter,
                        const double &zCenter,
                        const double &xWidth, const double &yWidth,
                        const int &nX, const int &nY);

    /**
     Generate cylinder surface segment in XY plane inside this class PointsCt.
     @param[in] xCylinderBaseCenter is the X center of cylinder base.
     @param[in] yCylinderBaseCenter is the Y center of cylinder base.
     @param[in] zCylinderBaseCenter is the Z center of cylinder base.
     @param[in] R is the radius of cylinder segment.
     @param[in] angleCenter is the orientation of cylinder segment.
     @param[in] zWidth is height of cylinder segment.
     @param[in] angleWidth is the angle width of cylinder segment.
     @param[in] nZ is the number of points on Z axis.
     @param[in] nAngle is the number of points on nAngle axis.
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

    std::shared_ptr<Points> points; ///< CGAL points.

    std::shared_ptr<Basis> basis; ///< Basis based on CGAL.


    std::shared_ptr<Value> tomoA; ///< Value connected to points.

    std::shared_ptr<Value> tomoB; ///< Value connected to points.

    std::shared_ptr<Value> tomoBuffer; ///< Value connected to points.

    std::shared_ptr<Value> result; ///< Value connected to points.

};


#endif //POINTSCT_H
