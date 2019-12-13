/**
 @file
 @brief File for PointsIO class.
 It is header which contains PointsIO class.
*/

#ifndef VTP_POINTSIO_H
#define VTP_POINTSIO_H

#include <vector>
#include <string>

#include <Points/Points.h>

/// This class provides io access for necessary VTK library functionality.
class PointsIO {

public:

  /**
   Constructor by default.
  */
  PointsIO();

  /**
   Constructor sets instance of Points class.
   @param[in] pointsCt is instance of Points class for pointsCt attribute.
  */
  PointsIO(std::shared_ptr<Points> pointsCt);

  /**
   Destructor sets by default.
  */
  virtual ~PointsIO() {}


  /**
   Mutator for pointsCt attribute.
   @param[in] pointsCt is assigned to _pointsCt attribute.
  */
  void setPointsCt(std::shared_ptr<Points> pointsCt);

  /**
   Accessor for pointsCt attribute.
   @return pointsCt attribute.
  */
  std::shared_ptr<Points> getPointsCt();


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
  /// Set of points_3 of Points type.
  std::shared_ptr<Points> pointsCt_;

  /// Binarity flag for pointsCt files.
  bool formatIsBinary_;

  /// Names of saved pointsCt files.
  std::vector<std::string> pointsCtFileNames_;

  /// Descriptions of saved pointsCt files.
  std::vector<std::string> pointsCtFileDescriptions_;

};


#endif // VTP_POINTSIO_H
