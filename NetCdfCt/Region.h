/**
 @file
 @brief File for Region class.
 It is header which contains Region class.
*/

#ifndef REGIONCT_H
#define REGIONCT_H

#include <vector>

#include <netcdf>

#include <Basis.h>


/// This class provide access to region of NetCDF library data file.
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
     @param[in] _start is start point of RegionCt in terms of voxels.
     @param[in] _width is widths of RegionCt in terms of voxels.
    */
    void initiate(const std::vector<size_t> &_start,
                  const std::vector<size_t> &_width);

    std::vector<size_t> start; ///< Start point of region in terms of voxels.
    std::vector<size_t> width; ///< Widths of region in terms of voxels.
    std::vector<std::vector<float>> dimArrays; ///< Axes X, Y and Z of region.
    std::vector<short> value; ///< Region array of general nc value.


    /**
     Mutator for points and pointsValue attributes.
     @param[in] _points is assigned to points attribute.
     @param[in] _pointsValue is assigned to pointsValue attribute.
    */
    void setPoints(std::shared_ptr<Points> _points,
                   std::shared_ptr<std::vector<double>> _pointsValue) {
        points = _points;
        pointsValue = _pointsValue;
    }


    /**
     Compute value for one point.
     @param[in] point is CGAL point value computation.
     @return value is computed.
    */
    double computePointValue(Point point);

    /**
     Compute value for all array of CGAL points.
     Points are taken from the points attribute.
    */
    void computePointsValue();

    /**
     Compute xInit, yInit, zInit, xStep, yStep and zStep attribute.
    */
    void computeInitsAndSteps();


private:

    std::shared_ptr<Points> points; ///< CGAL points for data extraction.

    std::shared_ptr<std::vector<double>> pointsValue; ///< CGAL points value.


    double xInit; ///< Initial x of region nc data.

    double yInit; ///< Initial y of region nc data.

    double zInit; ///< Initial z of region nc data.


    double xStep; ///< Step x of region nc data.

    double yStep; ///< Step y of region nc data.

    double zStep; ///< Step z of region nc data.

};


#endif // REGIONCT_H
