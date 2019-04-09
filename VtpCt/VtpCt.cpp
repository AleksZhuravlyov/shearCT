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


VtpCt::VtpCt() :
        VtpCt(std::make_shared<Points>(1, Point(0, 0, 0))) {};

VtpCt::VtpCt(std::shared_ptr<Points> _points) :
        points(_points),
        nPoints(_points->size()),
        tomoA(std::make_shared<std::vector<double>>(_points->size(), 0)),
        tomoB(std::make_shared<std::vector<double>>(_points->size(), 0)),
        result(std::make_shared<std::vector<double>>(_points->size(), 0)),
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

    auto _points = std::make_shared<Points>();
    for (int i = 0; i < _nPoints; i++) {
        auto xyz = pointsVtk->GetPoint(i);
        _points->push_back(Point(xyz[0], xyz[1], xyz[2]));
    }

    setPoints(_points);

    for (int i = 0; i < _nPoints; i++) {
        (*tomoA)[i] = tomoAVtk->GetVariantValue(vtkIdType(i)).ToDouble();
        (*tomoB)[i] = tomoBVtk->GetVariantValue(vtkIdType(i)).ToDouble();
        (*result)[i] = resultVtk->GetVariantValue(vtkIdType(i)).ToDouble();
    }
}


int VtpCt::size() {
    return nPoints;
}

void VtpCt::setPoints(std::shared_ptr<Points> _points) {
    points = _points;
    nPoints = points->size();
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


std::shared_ptr<Points> VtpCt::getPoints() {
    return points;
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
        pointsVtk->InsertNextPoint((*points)[i].x(),
                                   (*points)[i].y(),
                                   (*points)[i].z());
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

