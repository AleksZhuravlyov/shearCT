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


class PointsCt {

public:

    PointsCt(const std::vector<double> &pointsArray);

    virtual ~PointsCt() {}


    void setTomoA(const std::vector<double> &valuesArray);

    void setTomoB(const std::vector<double> &valuesArray);

    void setResult(const std::vector<double> &valuesArray);



private:

    vtkSmartPointer<vtkPoints> points;

    vtkSmartPointer<vtkDoubleArray> tomoA;

    vtkSmartPointer<vtkDoubleArray> tomoB;

    vtkSmartPointer<vtkDoubleArray> result;


    void setValues(vtkSmartPointer<vtkDoubleArray> values,
            const std::vector<double> &valuesArray);

};


#endif //VTPCT_H
