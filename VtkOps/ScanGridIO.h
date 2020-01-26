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
   @param[in] scanGrid is instance of ScanGrid class for scanGrid_ attribute.
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
   @return scanGrid_ attribute.
  */
  std::shared_ptr<ScanGrid> getScanGrid();


  /**
   Read scanGrid_ attribute from paraview file.
   @param[in] fileName is the name of paraview file with scanGrid.
  */
  void loadScanGridFromFile(const std::string &fileName);

  /**
   Mutator for fileIsBinary_ attribute.
   @param[in] formatIsBinary is assigned to fileIsBinary_ attribute.
  */
  void setFormatIsBinary(const bool &formatIsBinary);

  /**
   Accessor for fileIsBinary_ attribute.
   @return fileIsBinary_ attribute.
  */
  bool getFormatIsBinary();

  /**
   Save scanGrid_ attribute into paraview file.
   @param[in] fileName is the name for saving scanGrid_ attribute.
   @param[in] fileDescription is description for scanGrid file.
  */
  void saveScanGridToFile(const std::string &fileName,
                          const std::string &fileDescription);

  /**
   Save scanGrid_ attribute into paraview file. Description for scanGrid
   is default.
   @param[in] fileName is the name for saving scanGrid attribute.
  */
  void saveScanGridToFile(const std::string &fileName);

  /**
   Clear names and descriptions of saved scanGrid files.
  */
  void clearFilesCollection();

  /**
   Save scanGrid files names for loading all saved scanGrid files as one.
   @param[in] fileName is the name for collection of scanGrid files.
  */
  void saveFilesCollectionToFile(const std::string &fileName);


private:
  /// Set of points of ScanGrid type.
  std::shared_ptr<ScanGrid> scanGrid_;

  /// Binarity flag for scanGrid files.
  bool formatIsBinary_;

  /// Names of saved scanGrid files.
  std::vector<std::string> scanGridFileNames_;

  /// Descriptions of saved scanGrid files.
  std::vector<std::string> scanGridFileDescriptions_;

};


#endif // VTP_SCANGRIDIO_H
