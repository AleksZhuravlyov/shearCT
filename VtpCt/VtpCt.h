#ifndef VTPCT_H
#define VTPCT_H

#include <vector>
#include <string>

#include <Basis.h>
#include <PointsCt.h>


class VtpCt {

public:

    VtpCt();

    VtpCt(std::shared_ptr<PointsCt> _pointsCt);


    virtual ~VtpCt() {}


    void readPointsFile(const std::string &fileName);

    void setFileIsBinary(const bool &_fileIsBinary);

    bool getFileIsBinary();

    void savePointsFile(const std::string &fileName,
                        const std::string &fileDescription);

    void clearFilesCollection();

    void saveCollectionFile(const std::string &fileName);


private:

    std::shared_ptr<PointsCt> pointsCt;


    bool fileIsBinary;

    std::vector<std::string> fileNames;

    std::vector<std::string> fileDescriptions;

};


#endif //VTPCT_H
