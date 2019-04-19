/**
 @file
 @brief File for VtpCt class.
 It is header which contains VtpCt class.
*/

#ifndef VTPCT_H
#define VTPCT_H

#include <vector>
#include <string>

#include <Basis.h>
#include <PointsCt.h>

/// This class provides io access for necessary VTK library functionality.
class VtpCt {

public:

    /**
     Constructor by default is deleted.
    */
    VtpCt();

    /**
     Constructor sets instance of PointsCt class.
     @param[in] _pointsCt is instance of PointsCt class for pointsCt attribute.
    */
    VtpCt(std::shared_ptr<PointsCt> _pointsCt);

    /**
     Destructor sets by default.
    */
    virtual ~VtpCt() {}


    /**
     Mutator for pointsCt attribute.
     @param[in] _pointsCt is assigned to _pointsCt attribute.
    */
    void setPointsCt(std::shared_ptr<PointsCt> _pointsCt);

    /**
     Accessor for pointsCt attribute.
     @return pointsCt attribute.
    */
    std::shared_ptr<PointsCt> getPointsCt();


    /**
     Read pointsCt attribute from file.
     @param[in] fileName is the name of file with pointsCt attribute.
    */
    void readPointsFile(const std::string &fileName);

    /**
     Mutator for fileIsBinary attribute.
     @param[in] _fileIsBinary is assigned to fileIsBinary attribute.
    */
    void setFileIsBinary(const bool &_fileIsBinary);

    /**
     Accessor for fileIsBinary attribute.
     @return fileIsBinary attribute.
    */
    bool getFileIsBinary();

    /**
     Save pointsCt attribute into file.
     @param[in] fileName is the name for saving pointsCt attribute.
     @param[in] fileDescription is description for file.
    */
    void savePointsFile(const std::string &fileName,
                        const std::string &fileDescription);

    /**
     Save pointsCt attribute into file. Description for file is default.
     @param[in] fileName is the name for saving pointsCt attribute.
    */
    void savePointsFile(const std::string &fileName);

    /**
     Clear the list of saved pointCt files and the list of its descriptions.
    */
    void clearFilesCollection();

    /**
     Save file for paraview for loading all saved pointsCt files as one.
     @param[in] fileName is the name for collection file.
    */
    void saveCollectionFile(const std::string &fileName);


private:

    std::shared_ptr<PointsCt> pointsCt; ///< Set of points in PointCt instance.


    bool fileIsBinary; ///< Flag for binary format ov saved pointCt files.

    std::vector<std::string> fileNames; ///< The list of saved pointCt files.

    std::vector<std::string> fileDescriptions; ///< The list of descriptions.

};


#endif //VTPCT_H
