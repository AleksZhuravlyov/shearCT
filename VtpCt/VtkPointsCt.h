/**
 @file
 @brief File for VtkPointsCt class.
 It is header which contains VtkPointsCt class.
*/

#ifndef VTKPOINTSCT_H
#define VTKPOINTSCT_H

#include <vector>
#include <string>

#include <PointsCt.h>

/// This class provides io access for necessary VTK library functionality.
class VtkPointsCt {

public:

  /**
   Constructor by default.
  */
  VtkPointsCt();

  /**
   Constructor sets instance of PointsCt class.
   @param[in] pointsCt is instance of PointsCt class for pointsCt attribute.
  */
  VtkPointsCt(std::shared_ptr<PointsCt> pointsCt);

  /**
   Destructor sets by default.
  */
  virtual ~VtkPointsCt() {}


  /**
   Mutator for pointsCt attribute.
   @param[in] pointsCt is assigned to _pointsCt attribute.
  */
  void setPointsCt(std::shared_ptr<PointsCt> pointsCt);

  /**
   Accessor for pointsCt attribute.
   @return pointsCt attribute.
  */
  std::shared_ptr<PointsCt> getPointsCt();


  /**
   Read pointsCt attribute from file.
   @param[in] fileName is the name of file with pointsCt attribute.
  */
  void loadPointsCtFromFile(const std::string &fileName);

  /**
   Mutator for fileIsBinary attribute.
   @param[in] formatIsBinary is assigned to fileIsBinary attribute.
  */
  void setFormatIsBinary(const bool &formatIsBinary);

  /**
   Accessor for fileIsBinary attribute.
   @return fileIsBinary attribute.
  */
  bool getFormatIsBinary();

  /**
   Save pointsCt attribute into file.
   @param[in] fileName is the name for saving pointsCt attribute.
   @param[in] fileDescription is description for file.
  */
  void savePointsCtToFile(const std::string &fileName,
                          const std::string &fileDescription);

  /**
   Save pointsCt attribute into file. Description for file is default.
   @param[in] fileName is the name for saving pointsCt attribute.
  */
  void savePointsCtToFile(const std::string &fileName);

  /**
   Clear names and descriptions of saved pointsCt files.
  */
  void clearFilesCollection();

  /**
   Save file for paraview for loading all saved pointsCt files as one.
   @param[in] fileName is the name for collection file.
  */
  void saveFilesCollectionToFile(const std::string &fileName);


private:
  /// Set of points of PointsCt type.
  std::shared_ptr<PointsCt> pointsCt_;

  /// Binarity flag for pointsCt files.
  bool formatIsBinary_;

  /// Names of saved pointsCt files.
  std::vector<std::string> pointsCtFileNames_;

  /// Descriptions of saved pointsCt files.
  std::vector<std::string> pointsCtFileDescriptions_;

};


#endif //VTKPOINTSCT_H
