/**
 @file
 @brief File for Image class.
 It is header which contains Image class.
*/


#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>

#include <netcdf>
#include <CGAL/Bbox_3.h>

#include "Region.h"


typedef CGAL::Bbox_3 Bbox;


/// This structure represents dims for NetCDF lib.
struct Dim {
  std::string name; ///< Name of dim.
  size_t size; ///< Size of dim.
};

/// This structure represents vars for NetCDF lib.
struct Var {
  std::string name; ///< Name of var.
  std::string unitName; ///< Unit name of var.
  int dim; ///< Dim of var.
};

/// This class provides io and other processing of NetCDF lib.
class Image {

public:

  /**
   Constructor by default is deleted.
  */
  Image() = delete;

  /**
   Constructor sets name of nc file.
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
   @param[in] image is the instance of this class.
  */
  friend std::ostream &operator<<(std::ostream &stream, const Image &image);


  /**
   Accessor for xInit_ attribute.
   @return xInit_ attribute.
  */
  double getXInit();

  /**
   Accessor for yInit_ attribute.
   @return yInit_ attribute.
  */
  double getYInit();

  /**
   Accessor for zInit_ attribute.
   @return zInit_ attribute.
  */
  double getZInit();


  /**
   Accessor for nX_ attribute.
   @return nX_ attribute.
  */
  int getNX();

  /**
   Accessor for nY_ attribute.
   @return nY_ attribute.
  */
  int getNY();

  /**
   Accessor for nZ_ attribute.
   @return nZ_ attribute.
  */
  int getNZ();


  /**
   Accessor for xStep_ attribute.
   @return xStep_ attribute.
  */
  double getXStep();

  /**
   Accessor for yStep_ attribute.
   @return yStep_ attribute.
  */
  double getYStep();

  /**
   Accessor for zStep_ attribute.
   @return zStep_ attribute.
  */
  double getZStep();


  /**
   Accessor for dims_ attribute.
   @return shared pointer of dims_ attribute.
  */
  std::shared_ptr<std::vector<Dim>> getDims();

  /**
   Accessor for vars_ attribute.
   @return shared pointer of vars_ attribute.
  */
  std::shared_ptr<std::vector<Var>> getVars();

  /**
   Accessor for value attribute.
   @return shared pointer of value attribute.
  */
  std::shared_ptr<std::vector<short>> getValue();


  /**
   Mutator for region attribute. This method initiates
   the region_ attribute and computes its attributes.
   @param[in] start is start point of region_ in terms of voxels.
   @param[in] width is widths of region_ in terms of voxels.
  */
  void setRegion(const std::vector<size_t> &start,
                 const std::vector<size_t> &width);

  /**
   Mutator for region attribute. This method initiates
   the region_ attribute and computes its attributes.
   @param[in] bbox is CGAL lib boundary box in terms of meters.
  */
  void setRegion(const Bbox &bbox);

  /**
   Save region attribute as nc file.
   @param[in] fileName is name of nc file for the region_ attribute.
  */
  void saveRegion(const std::string &fileName);

  Region region; ///< Region of general nc data.


private:

  netCDF::NcFile ncFile_; ///< General nc data of NetCFD lib.


  std::string path_; ///< Path to general nc file.

  std::string valueName_; ///< Name of value in general nc data.

  std::string units_; ///< Units name.


  std::vector<Var> vars_; ///< Array of NetCDF vars from general nc data.

  std::vector<Dim> dims_; ///< Array of NetCDF dims from general nc data.

  std::vector<std::vector<float>> dimArrays_; ///< Axes X, Y and Z.


  double xInit_; ///< Initial x of general nc data.

  double yInit_; ///< Initial y of general nc data.

  double zInit_; ///< Initial z of general nc data.


  int nX_; ///< Number of x elements of general nc data.

  int nY_; ///< Number of y elements of general nc data.

  int nZ_; ///< Number of z elements of general nc data.


  double xStep_; ///< Step x of general nc data.

  double yStep_; ///< Step y of general nc data.

  double zStep_; ///< Step z of general nc data.

};


#endif // IMAGE_H
