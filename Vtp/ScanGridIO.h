/**
 @file
 @brief File for ScanGridIO class.
 It is header which contains ScanGridIO class.
*/

#ifndef VTP_SCANGRIDIO_H
#define VTP_SCANGRIDIO_H

#include <vector>
#include <string>

#include <ScanGrid/ScanGrid.h>

/// This class provides io access for necessary VTK library functionality.
class ScanGridIO {

public:

  /**
   Constructor by default.
  */
  ScanGridIO();

  /**
   Constructor sets instance of ScanGrid class.
   @param[in] scanGrid is instance of ScanGrid class for scanGrid attribute.
  */
  ScanGridIO(std::shared_ptr<ScanGrid> scanGrid);

  /**
   Destructor sets by default.
  */
  virtual ~ScanGridIO() {}


  /**
   Mutator for scanGrid_ attribute.
   @param[in] scanGrid is assigned to scanGrid_ attribute.
  */
  void setScanGrid(std::shared_ptr<ScanGrid> scanGrid);

  /**
   Accessor for scanGrid_ attribute.
   @return pointsCt attribute.
  */
  std::shared_ptr<ScanGrid> getScanGrid();


  /**
   Read pointsCt attribute from ncFile_.
   @param[in] fileName is the name of ncFile_ with pointsCt attribute.
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
   Save pointsCt attribute into ncFile_.
   @param[in] fileName is the name for saving pointsCt attribute.
   @param[in] fileDescription is description for ncFile_.
  */
  void savePointsCtToFile(const std::string &fileName,
                          const std::string &fileDescription);

  /**
   Save pointsCt attribute into ncFile_. Description for ncFile_ is default.
   @param[in] fileName is the name for saving pointsCt attribute.
  */
  void savePointsCtToFile(const std::string &fileName);

  /**
   Clear names and descriptions of saved pointsCt files.
  */
  void clearFilesCollection();

  /**
   Save ncFile_ for paraview for loading all saved pointsCt files as one.
   @param[in] fileName is the name for collection ncFile_.
  */
  void saveFilesCollectionToFile(const std::string &fileName);


private:
  /// Set of points_ of ScanGrid type.
  std::shared_ptr<ScanGrid> scanGrid_;

  /// Binarity flag for scanGrid files.
  bool formatIsBinary_;

  /// Names of saved scanGrid files.
  std::vector<std::string> scanGridFileNames_;

  /// Descriptions of saved scanGrid files.
  std::vector<std::string> scanGridFileDescriptions_;

};


#endif // VTP_SCANGRIDIO_H
