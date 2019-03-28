#include <VtpCt.h>

VtpCt::VtpCt(const std::vector<double> &pointsArray) :
        points(vtkSmartPointer<vtkPoints>::New()),
        nPoints(pointsArray.size() / 3),
        tomoA(vtkSmartPointer<vtkDoubleArray>::New()),
        tomoB(vtkSmartPointer<vtkDoubleArray>::New()),
        result(vtkSmartPointer<vtkDoubleArray>::New()),
        isBinary(true),
        writer(vtkSmartPointer<vtkXMLPolyDataWriter>::New()) {

    tomoA->SetName("tomoA");
    tomoB->SetName("tomoB");
    result->SetName("result");

    for (int i = 0; i < pointsArray.size() / 3; i++) {
        points->InsertNextPoint(&pointsArray[i * 3]);
        tomoA->InsertNextValue(0);
        tomoB->InsertNextValue(0);
        result->InsertNextValue(0);
    }

    vtkSmartPointer<vtkVertex> vertex =
            vtkSmartPointer<vtkVertex>::New();
    vertex->Initialize(pointsArray.size() / 3, points);

    vtkSmartPointer<vtkCellArray> vertices =
            vtkSmartPointer<vtkCellArray>::New();
    vertices->InsertNextCell(vertex);

    vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);

    polyData->GetPointData()->SetScalars(result);
    polyData->SetVerts(vertices);

    polyData->GetPointData()->AddArray(tomoA);
    polyData->GetPointData()->AddArray(tomoB);

    writer->SetInputData(polyData);

}


int VtpCt::size() {
    return nPoints;
}


void VtpCt::setXyzArray(const std::vector<double> &xyzArray) {
    for (int i = 0; i < xyzArray.size() / 3; i++)
        points->SetPoint(vtkIdType(i), &xyzArray[i * 3]);
}

void VtpCt::setTomoAArray(const std::vector<double> &valuesArray) {
    setValues(tomoA, valuesArray);
}

void VtpCt::setTomoBArray(const std::vector<double> &valuesArray) {
    setValues(tomoB, valuesArray);
}

void VtpCt::setResultArray(const std::vector<double> &valuesArray) {
    setValues(result, valuesArray);
}

void VtpCt::setValues(vtkSmartPointer<vtkDoubleArray> values,
                      const std::vector<double> &valuesArray) {
    for (int i = 0; i < valuesArray.size(); i++)
        values->SetVariantValue(vtkIdType(i), valuesArray[i]);
}


double *VtpCt::getXyz(const int &pointId) {
    return points->GetPoint(vtkIdType(pointId));
}

double *VtpCt::getTomoAValue(const int &pointId) {
    return tomoA->GetPointer(vtkIdType(pointId));
}

double *VtpCt::getTomoBValue(const int &pointId) {
    return tomoB->GetPointer(vtkIdType(pointId));
}

double *VtpCt::getResultValue(const int &pointId) {
    return result->GetPointer(vtkIdType(pointId));
}


void VtpCt::setIsBinary(const bool &_isBinary) {

    isBinary = _isBinary;

    if (isBinary)
        writer->SetDataModeToBinary();
    else
        writer->SetDataModeToAscii();

}

bool VtpCt::getIsBinary() {
    return isBinary;
}

void VtpCt::savePoints(const std::string &fileName) {
    writer->SetFileName(fileName.c_str());
    writer->Write();
}

