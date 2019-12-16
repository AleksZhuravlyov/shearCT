/**
 @file
 @brief File for Image class.
 It is header which contains NcCt class.
*/

#ifndef NETCDF_IMAGE_H
#define NETCDF_IMAGE_H

#include <string>
#include <vector>

#include <netcdf>

#include <NcOps/Region.h>

#include <CGAL/Bbox_3.h>


typedef CGAL::Bbox_3 Bbox;


/// This struct represents dims of NetCDF library.
struct Dim {
    std::string name; ///< The name of dim.
    size_t size; ///< The size of dim.
};

/// This struct represents vars of NetCDF library.
struct Var {
    std::string name; ///< The name of var.
    std::string unitName; ///< The unit name of var.
    int dim; ///< The dim of var.
};

/// This class provides io access for necessary NetCDF library functionality.
class Image {

public:

    /**
     Constructor by default is deleted.
    */
    Image() = delete;

    /**
     Constructor sets the name of nc file.
     @param[in] fileName is the name of nc file.
    */
    Image(const std::string &fileName);

    /**
     Destructor sets by default.
    */
    virtual ~Image() {}


    /**
     Overload insertion operator.
     @param[in, out] stream is the instance of class std::ostream.
     @param[in] ncCt is the instance of class NcCt.
    */
    friend std::ostream &operator<<(std::ostream &stream, const Image &ncCt);


    /**
     Accessor for xInit attribute.
     @return xInit attribute.
    */
    double getXInit();

    /**
     Accessor for yInit attribute.
     @return yInit attribute.
    */
    double getYInit();

    /**
     Accessor for zInit attribute.
     @return zInit attribute.
    */
    double getZInit();


    /**
     Accessor for nX attribute.
     @return nX attribute.
    */
    int getNX();

    /**
     Accessor for nY attribute.
     @return nY attribute.
    */
    int getNY();

    /**
     Accessor for nZ attribute.
     @return nZ attribute.
    */
    int getNZ();


    /**
     Accessor for xStep attribute.
     @return xStep attribute.
    */
    double getXStep();

    /**
     Accessor for yStep attribute.
     @return yStep attribute.
    */
    double getYStep();

    /**
     Accessor for zStep attribute.
     @return zStep attribute.
    */
    double getZStep();


    /**
     Accessor for dims attribute.
     @return shared pointer of dims attribute.
    */
    std::shared_ptr<std::vector<Dim>> getDims();

    /**
     Accessor for vars attribute.
     @return shared pointer of vars attribute.
    */
    std::shared_ptr<std::vector<Var>> getVars();

    /**
     Accessor for value attribute.
     @return shared pointer of value attribute.
    */
    std::shared_ptr<std::vector<short>> getValue();


    /**
     Mutator for region attribute. This method initiates
     the regionCt attribute and computes sate of region attribute.
     @param[in] start is start point of RegionCt in terms of voxels.
     @param[in] width is widths of RegionCt in terms of voxels.
    */
    void setRegion(const std::vector<size_t> &start,
                   const std::vector<size_t> &width);

    /**
     Mutator for region attribute. This method initiates
     the regionCt attribute and computes sate of region attribute.
     @param[in] bbox is CGAL library boundary box in terms of meters.
    */
    void setRegion(const Bbox &bbox);

    /**
     Save region attribute as nc file.
     @param[in] fileName is name of nc file for the regionCt attribute.
    */
    void saveRegion(const std::string &fileName);

    Region region; ///< The region of general nc data.


private:

    netCDF::NcFile file; ///< General nc data of NetCFD library.


    std::string path; ///< Path to general nc file.

    std::string valueName; ///< Name to value in general nc file.

    std::string units; ///< Name of the units name.


    std::vector<Var> vars; ///< Array of NetCDF vars from general nc file.

    std::vector<Dim> dims; ///< Array of NetCDF dims from general nc file.

    std::vector<std::vector<float>> dimArrays; ///< Axes X, Y and Z.


    double xInit; ///< Initial x of general nc data.

    double yInit; ///< Initial y of general nc data.

    double zInit; ///< Initial z of general nc data.


    int nX; ///< Number of x elements of general nc data.

    int nY; ///< Number of y elements of general nc data.

    int nZ; ///< Number of z elements of general nc data.


    double xStep; ///< Step x of general nc data.

    double yStep; ///< Step y of general nc data.

    double zStep; ///< Step z of general nc data.

};


#endif // NETCDF_IMAGE_H
