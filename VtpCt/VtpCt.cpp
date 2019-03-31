#include <VtpCt.h>

#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkVertex.h>
#include <vtkXMLPolyDataReader.h>
#include <pugixml.hpp>


VtpCt::VtpCt() : VtpCt(1) {}

VtpCt::VtpCt(const int &_nPoints) :
        VtpCt(std::make_shared<std::vector<double>>(_nPoints * 3, 0)) {}

VtpCt::VtpCt(std::shared_ptr<std::vector<double>> _xyzArray) :
        xyzArray(_xyzArray),
        nPoints(_xyzArray->size() / 3),
        tomoA(std::make_shared<std::vector<double>>(_xyzArray->size() / 3, 0)),
        tomoB(std::make_shared<std::vector<double>>(_xyzArray->size() / 3, 0)),
        result(std::make_shared<std::vector<double>>(_xyzArray->size() / 3, 0)),
        fileIsBinary(true) {}

VtpCt::VtpCt(const std::string &fileName) : VtpCt() {

    auto polyDataReaderVtk = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    polyDataReaderVtk->SetFileName(fileName.c_str());
    polyDataReaderVtk->Update();
    auto polyDataVtk = polyDataReaderVtk->GetOutput();
    auto pointsVtk = polyDataVtk->GetPoints();
    auto tomoAVtk = polyDataVtk->GetPointData()->GetScalars("tomoA");
    auto tomoBVtk = polyDataVtk->GetPointData()->GetScalars("tomoB");
    auto resultVtk = polyDataVtk->GetPointData()->GetScalars("result");

    int _nPoints = pointsVtk->GetNumberOfPoints();

    auto _xyzArray = std::make_shared<std::vector<double>>(_nPoints, 0);
    for (int i = 0; i < _nPoints; i++) {
        auto xyz = pointsVtk->GetPoint(i);
        for (int j = 0; j < 3; j++)
            _xyzArray->push_back(xyz[j]);
    }

    setXyzArray(_xyzArray);

    for (int i = 0; i < _nPoints; i++) {
        (*tomoA)[i] = tomoAVtk->GetVariantValue(vtkIdType(i)).ToDouble();
        (*tomoB)[i] = tomoBVtk->GetVariantValue(vtkIdType(i)).ToDouble();
        (*result)[i] = resultVtk->GetVariantValue(vtkIdType(i)).ToDouble();
    }
}


int VtpCt::size() {
    return nPoints;
}

void VtpCt::setXyzArray(std::shared_ptr<std::vector<double>> _xyzArray) {
    xyzArray = _xyzArray;
    nPoints = xyzArray->size() / 3;
    tomoA->resize(nPoints, 0);
    tomoB->resize(nPoints, 0);
    result->resize(nPoints, 0);
}

void VtpCt::setTomoA(std::shared_ptr<std::vector<double>> value) {
    tomoA = value;
}

void VtpCt::setTomoB(std::shared_ptr<std::vector<double>> value) {
    tomoB = value;
}

void VtpCt::setResult(std::shared_ptr<std::vector<double>> value) {
    result = value;
}


std::shared_ptr<std::vector<double>> VtpCt::getXyzArray() {
    return xyzArray;
}

std::shared_ptr<std::vector<double>> VtpCt::getTomoA() {
    return tomoA;
}

std::shared_ptr<std::vector<double>> VtpCt::getTomoB() {
    return tomoB;
}

std::shared_ptr<std::vector<double>> VtpCt::getResult() {
    return result;
}


void VtpCt::setFileIsBinary(const bool &_fileIsBinary) {
    fileIsBinary = _fileIsBinary;
}

bool VtpCt::getFileIsBinary() {
    return fileIsBinary;
}

void VtpCt::savePointsFile(const std::string &fileName,
                           const std::string &fileDescription) {

    auto pointsVtk = vtkSmartPointer<vtkPoints>::New();
    auto tomoAVtk = vtkSmartPointer<vtkDoubleArray>::New();
    auto tomoBVtk = vtkSmartPointer<vtkDoubleArray>::New();
    auto resultVtk = vtkSmartPointer<vtkDoubleArray>::New();

    tomoAVtk->SetName("tomoA");
    tomoBVtk->SetName("tomoB");
    resultVtk->SetName("result");

    for (int i = 0; i < nPoints; i++) {
        pointsVtk->InsertNextPoint(&(*xyzArray)[i * 3]);
        tomoAVtk->InsertNextValue((*tomoA)[i]);
        tomoBVtk->InsertNextValue((*tomoB)[i]);
        resultVtk->InsertNextValue((*result)[i]);
    }

    auto writerVtk = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    if (fileIsBinary)
        writerVtk->SetDataModeToBinary();
    else
        writerVtk->SetDataModeToAscii();

    vtkSmartPointer<vtkVertex> vertexVtk =
            vtkSmartPointer<vtkVertex>::New();
    vertexVtk->Initialize(nPoints, pointsVtk);

    vtkSmartPointer<vtkCellArray> verticesVtk =
            vtkSmartPointer<vtkCellArray>::New();
    verticesVtk->InsertNextCell(vertexVtk);

    vtkSmartPointer<vtkPolyData> polyDataVtk =
            vtkSmartPointer<vtkPolyData>::New();
    polyDataVtk->SetPoints(pointsVtk);

    polyDataVtk->GetPointData()->SetScalars(resultVtk);
    polyDataVtk->SetVerts(verticesVtk);

    polyDataVtk->GetPointData()->AddArray(tomoAVtk);
    polyDataVtk->GetPointData()->AddArray(tomoBVtk);

    writerVtk->SetInputData(polyDataVtk);

    writerVtk->SetFileName(fileName.c_str());
    writerVtk->Write();

    fileNames.push_back(fileName);
    fileDescriptions.push_back(fileDescription);

}


void VtpCt::clearFilesCollection() {
    fileNames.clear();
    fileDescriptions.clear();
}


void VtpCt::saveCollectionFile(const std::string &fileName) {

    pugi::xml_document doc;

    pugi::xml_node vtkFile = doc.append_child("VTKFile");
    vtkFile.append_attribute("type") = "Collection";
    vtkFile.append_attribute("version") = "1.0";
    vtkFile.append_attribute("byte_order") = "LittleEndian";
    vtkFile.append_attribute("header_type") = "UInt64";

    pugi::xml_node collection = vtkFile.append_child("Collection");

    for (int i = 0; i < fileNames.size(); i++) {

        collection.append_child("DataSet");

        collection.last_child().append_attribute("timestep").set_value(
                fileDescriptions[i].c_str());

        collection.last_child().append_attribute("file").set_value(
                fileNames[i].c_str());

    }

    doc.save_file(fileName.c_str());

}

