#include <VtpCt.h>

PointsCt::PointsCt(const std::vector<double> &pointsArray) :
        points(vtkSmartPointer<vtkPoints>::New()),
        tomoA(vtkSmartPointer<vtkDoubleArray>::New()),
        tomoB(vtkSmartPointer<vtkDoubleArray>::New()),
        result(vtkSmartPointer<vtkDoubleArray>::New()) {

    tomoA->SetName("tomoA");
    tomoB->SetName("tomoB");
    result->SetName("result");
    for (int i = 0; i < pointsArray.size() / 3; i++) {

        points->InsertNextPoint(
                pointsArray[i * 3],
                pointsArray[i * 3 + 1],
                pointsArray[i * 3 + 2]);

        tomoA->InsertNextValue(0);
        tomoB->InsertNextValue(0);
        result->InsertNextValue(0);

    }

}

void PointsCt::setValues(vtkSmartPointer<vtkDoubleArray> values,
                         const std::vector<double> &valuesArray) {
    ///TODO start here

}

void PointsCt::setTomoA(const std::vector<double> &valuesArray) {
    setValues(tomoA, valuesArray);
}

void PointsCt::setTomoB(const std::vector<double> &valuesArray) {
    setValues(tomoB, valuesArray);
}

void PointsCt::setResult(const std::vector<double> &valuesArray) {
    setValues(result, valuesArray);
}


