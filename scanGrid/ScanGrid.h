/**
 @file
 @brief File for scanGrid class.
 It is header which contains scanGrid class.
*/

#ifndef SCANGRID_H
#define SCANGRID_H

#include <vector>
#include <string>

#include <CGAL/Point_3.h>
#include <CGAL/Bbox_3.h>

#include <geometry/Basis.h>

using Points = std::vector<Point>;
using Bbox = CGAL::Bbox_3;


/// This class is set of CGAL points_ with convenient methods and attributes.
class ScanGrid {

public:

    /**
     Constructor sets by default.
    */
    ScanGrid();

    /**
     Constructor sets CGAL points_.
     @param[in] points is CGAL points_ for points_ attribute.
    */
    ScanGrid(std::shared_ptr<Points> points);

    /**
     Constructor sets CGAL Bbox.
     @param[in] bbox is CGAL Bbox for points_ settings.
    */
    ScanGrid(const Bbox &bbox);

    /**
     Constructor sets CGAL Bbox.
     @param[in] bbox is CGAL Bbox for points_ settings.
     @param[in] basis is CGAL based basis_ for basis_ attribute.
    */
    ScanGrid(const Bbox &bbox, std::shared_ptr<Basis> basis);

    /**
     Constructor sets CGAL points_ and basis_.
     @param[in] points is CGAL points_ for points_ attribute.
     @param[in] basis is CGAL based basis_ for basis_ attribute.
    */
    ScanGrid(std::shared_ptr<Points> points,
             std::shared_ptr<Basis> basis);

    /**
     Move constructor.
     @param[in] scanGrid is the instance of class scanGrid.
    */
    ScanGrid(const ScanGrid &scanGrid);

    /**
     Copy constructor.
     @param[in, out] scanGrid is the instance of class scanGrid.
    */
    ScanGrid(ScanGrid &&scanGrid);


    /**
     Destructor sets by default.
    */
    virtual ~ScanGrid() {}

    /**
     Overload assignment operator.
     @param[in, out] scanGrid is the instance of class scanGrid.
    */
    ScanGrid &operator=(ScanGrid &&scanGrid);


    /**
     Make transformation for this scanGrid class.
     @param[in] transformation acts on this scanGrid class.
    */
    void transform(const Transformation &transformation);


    /**
     Make transformations for this scanGrid class.
     @param[in] transformations acts on this scanGrid class.
    */
    void transform(const std::vector<Transformation> &transformations);


    /**
     Move origin_ attribute of basis_ attribute to particular point.
     @param[in] point setts origin_ attribute of basis_ attribute.
    */
    void translateBasis(const Point &point);

    /**
     Move origin_ attribute of basis_ attribute to particular point.
     @param[in] x setts x origin_ coordinate.
     @param[in] y setts y origin_ coordinate.
     @param[in] z setts z origin_ coordinate.
    */
    void translateBasis(const double &x,
                        const double &y,
                        const double &z);

    /**
     Move basis_ attribute to the center of points_ attribute.
    */
    void translateBasisToCenter();


    /**
     Accessor for points_ number in points_ attribute.
     @return number of points_ in points_ attribute.
    */
    int size();

    /**
     Mutator for points_ attribute.
     @param[in] points is assigned to points_ attribute.
    */
    void setPoints(std::shared_ptr<Points> points);

    /**
     Mutator for basis_ attribute.
     @param[in] basis is assigned to basis_ attribute.
    */
    void setBasis(std::shared_ptr<Basis> basis);


    /**
     Mutator for tomoA_ attribute.
     @param[in] tomoA is assigned to tomoA_ attribute.
    */
    void setTomoA(std::shared_ptr<std::vector<double>> tomoA);

    /**
     Mutator for tomoB_ attribute.
     @param[in] tomoB is assigned to tomoB_ attribute.
    */
    void setTomoB(std::shared_ptr<std::vector<double>> tomoB);

    /**
     Mutator for buffer_ attribute.
     @param[in] buffer is assigned to buffer_ attribute.
    */
    void setTomoBuffer(std::shared_ptr<std::vector<double>> buffer);

    /**
     Mutator for result_ attribute.
     @param[in] result is assigned to result_ attribute.
    */
    void setResult(std::shared_ptr<std::vector<double>> result);

    /**
     Swap tomoA_ and buffer_ attributes.
    */
    void swapTomoAAndBuffer();


    /**
     Accessor for points_ attribute.
     @return points_ attribute.
    */
    std::shared_ptr<Points> getPoints();

    /**
     Accessor for basis_ attribute.
     @return basis_ attribute.
    */
    std::shared_ptr<Basis> getBasis();


    /**
     Accessor for tomoA_ attribute.
     @return tomoA_ attribute.
    */
    std::shared_ptr<std::vector<double>> getTomoA();

    /**
     Accessor for tomoB_ attribute.
     @return tomoB_ attribute.
    */
    std::shared_ptr<std::vector<double>> getTomoB();

    /**
     Accessor for buffer_ attribute.
     @return buffer_ attribute.
    */
    std::shared_ptr<std::vector<double>> getBuffer();

    /**
     Accessor for result_ attribute.
     @return result_ attribute.
    */
    std::shared_ptr<std::vector<double>> getResult();


    /**
     Generate CGAL boundary box from points_ attribute.
    */
    Bbox generateBbox();

    /**
     Assign subtraction tomoA_ and TomoB attributes to result_ attribute.
    */
    void computeDifferenceAB();

    /**
     Compute Pearson correlation between tomoA_ and TomoB attributes.
     @return Pearson correlation between tomoA_ and TomoB attributes.
    */
    double computePearsonCorrelationAB();

    /**
     Generate square in XY plane inside this class scanGrid.
     @param[in] xCenter is the X center of square.
     @param[in] yCenter is the Y center of square.
     @param[in] zCenter is the Z center of square.
     @param[in] xWidth is the X width of square.
     @param[in] yWidth is the Y width of square.
     @param[in] nX is the number of points_ on X axis.
     @param[in] nY is the number of points_ on Y axis.
    */
    void createXYSquare(const double &xCenter,
                        const double &yCenter,
                        const double &zCenter,
                        const double &xWidth, const double &yWidth,
                        const int &nX, const int &nY);

    /**
     Generate cylinder surface segment in XY plane inside this class scanGrid.
     @param[in] xCylinderBaseCenter is the X center of cylinder base.
     @param[in] yCylinderBaseCenter is the Y center of cylinder base.
     @param[in] zCylinderBaseCenter is the Z center of cylinder base.
     @param[in] R is the radius of cylinder segment.
     @param[in] angleCenter is the orientation of cylinder segment.
     @param[in] zWidth is height of cylinder segment.
     @param[in] angleWidth is the angle width of cylinder segment.
     @param[in] nZ is the number of points_ on Z axis.
     @param[in] nAngle is the number of points_ on nAngle axis.
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

    std::shared_ptr<Points> points_; ///< CGAL points_.

    std::shared_ptr<Basis> basis_; ///< Basis based on CGAL.


    std::shared_ptr<std::vector<double>> tomoA_; ///< Value connected to points_.

    std::shared_ptr<std::vector<double>> tomoB_; ///< Value connected to points_.

    std::shared_ptr<std::vector<double>> buffer_; ///< Value connected to points_.

    std::shared_ptr<std::vector<double>> result_; ///< Value connected to points_.

};


#endif // SCANGRID_H
