#ifndef VTPCT_H
#define VTPCT_H

#include <vector>
#include <string>

#include <Geom.h>


class VtpCt {

public:

    VtpCt();

    VtpCt(std::shared_ptr<Points> _points);

    VtpCt(const std::string &fileName);


    virtual ~VtpCt() {}


    int size();

    void setPoints(std::shared_ptr<Points> _points);

    void setTomoA(std::shared_ptr<std::vector<double>> value);

    void setTomoB(std::shared_ptr<std::vector<double>> value);

    void setResult(std::shared_ptr<std::vector<double>> value);


    std::shared_ptr<Points> getPoints();

    std::shared_ptr<std::vector<double>> getTomoA();

    std::shared_ptr<std::vector<double>> getTomoB();

    std::shared_ptr<std::vector<double>> getResult();


    void setFileIsBinary(const bool &_fileIsBinary);

    bool getFileIsBinary();

    void savePointsFile(const std::string &fileName,
                        const std::string &fileDescription);

    void clearFilesCollection();

    void saveCollectionFile(const std::string &fileName);


private:

    std::shared_ptr<Points> points;

    std::shared_ptr<std::vector<double>> tomoA;

    std::shared_ptr<std::vector<double>> tomoB;

    std::shared_ptr<std::vector<double>> result;


    int nPoints;

    bool fileIsBinary;

    std::vector<std::string> fileNames;

    std::vector<std::string> fileDescriptions;

};


#endif //VTPCT_H
