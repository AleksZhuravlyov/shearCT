#ifndef VTPCT_H
#define VTPCT_H

#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkVertex.h>
#include <vtkXMLPolyDataReader.h>


class VtpCt {

public:

    VtpCt();

    VtpCt(std::shared_ptr<std::vector<double>> _xyzArray);

    VtpCt(const std::string &fileName);


    virtual ~VtpCt() {}


    int size();

    void setXyzArray(std::shared_ptr<std::vector<double>> _xyzArray);

    void setTomoA(std::shared_ptr<std::vector<double>> value);

    void setTomoB(std::shared_ptr<std::vector<double>> value);

    void setResult(std::shared_ptr<std::vector<double>> value);


    std::shared_ptr<std::vector<double>> getXyzArray();

    std::shared_ptr<std::vector<double>> getTomoA();

    std::shared_ptr<std::vector<double>> getTomoB();

    std::shared_ptr<std::vector<double>> getResult();


    void setFileIsBinary(const bool &_fileIsBinary);

    bool getFileIsBinary();

    void saveIntoFile(const std::string &fileName);


private:

    std::shared_ptr<std::vector<double>> xyzArray;

    std::shared_ptr<std::vector<double>> tomoA;

    std::shared_ptr<std::vector<double>> tomoB;

    std::shared_ptr<std::vector<double>> result;


    int nPoints;

    bool fileIsBinary;

};


#endif //VTPCT_H
