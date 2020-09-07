/**
 @file
 @brief File for Region class.
 It is header which contains Region class.
*/

#ifndef REGION_H
#define REGION_H

#include <vector>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Point_3.h>


typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point;
typedef std::vector<Point> Points;


/// This class provide access to region of NetCDF nc data.
class Region {

public:

    /**
     Constructor sets by default.
    */
    Region();

    /**
     Destructor sets by default.
    */
    virtual ~Region() {}

    /**
     Initiate the state of this class. Method sets start and width attributes
     and defines sizes of arrays.
     @param[in] _start is start point for region in terms of voxels.
     @param[in] _width is width for region in terms of voxels.
    */
    void initiate(const std::vector<size_t> &_start,
                  const std::vector<size_t> &_width);

    std::vector<size_t> start; ///< Start point of region in terms of voxels.
    std::vector<size_t> width; ///< Width of region in terms of voxels.
    std::vector<std::vector<float>> dimArrays; ///< Axes X, Y and Z of region.
    // ToDo: type of vector depends on type of microCT data
    std::vector<uint8_t> value; ///< Value array of region.


    /**
     Mutator for points_ and pointsValue_ attributes.
     @param[in] points is assigned to points_ attribute.
     @param[in] pointsValue is assigned to pointsValue_ attribute.
    */
    void setPoints(std::shared_ptr<Points> points,
                   std::shared_ptr<std::vector<double>> pointsValue);


    /**
     Compute value for one point.
     @param[in] point is point for value computation.
     @return value is computed.
    */
    double computePointValue(Point point);

    /**
     Compute value for points_.
    */
    void computePointsValue();

    /**
     Compute xInit_, yInit_, zInit_, xStep_, yStep_ and zStep_ attributes.
    */
    void computeInitsAndSteps();


private:

    std::shared_ptr<Points> points_; ///< Points for data extraction.

    std::shared_ptr<std::vector<double>> pointsValue_; ///< Points value.


    double xInit_; ///< Initial x of region nc data.

    double yInit_; ///< Initial y of region nc data.

    double zInit_; ///< Initial z of region nc data.


    double xStep_; ///< Step x of region nc data.

    double yStep_; ///< Step y of region nc data.

    double zStep_; ///< Step z of region nc data.

};


#endif // REGION_H
