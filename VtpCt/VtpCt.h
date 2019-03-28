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


class VtpCt {

public:

    VtpCt() = delete;

    VtpCt(const std::vector<double> &pointsArray);

    virtual ~VtpCt() {}


    int size();

    void setXyzArray(const std::vector<double> &xyzArray);

    void setTomoAArray(const std::vector<double> &valuesArray);

    void setTomoBArray(const std::vector<double> &valuesArray);

    void setResultArray(const std::vector<double> &valuesArray);


    double *getXyz(const int &pointId);

    double *getTomoAValue(const int &pointId);

    double *getTomoBValue(const int &pointId);

    double *getResultValue(const int &pointId);


    void setIsBinary(const bool &_isBinary);

    bool getIsBinary();

    void savePoints(const std::string &fileName);


private:

    vtkSmartPointer<vtkPoints> points;

    int nPoints;

    vtkSmartPointer<vtkDoubleArray> tomoA;

    vtkSmartPointer<vtkDoubleArray> tomoB;

    vtkSmartPointer<vtkDoubleArray> result;


    void setValues(vtkSmartPointer<vtkDoubleArray> values,
                   const std::vector<double> &valuesArray);

    bool isBinary;

    vtkSmartPointer<vtkXMLPolyDataWriter> writer;

};


#endif //VTPCT_H
