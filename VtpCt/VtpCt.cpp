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
        VtpCt(std::make_shared<PointsCt>()) {};

VtpCt::VtpCt(std::shared_ptr<PointsCt> _pointsCt) :
        pointsCt(_pointsCt),
        fileIsBinary(true) {}

void VtpCt::readPointsFile(const std::string &fileName) {

    auto polyDataReaderVtk = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    polyDataReaderVtk->SetFileName(fileName.c_str());
    polyDataReaderVtk->Update();
    auto polyDataVtk = polyDataReaderVtk->GetOutput();
    auto pointsVtk = polyDataVtk->GetPoints();
    auto tomoAVtk = polyDataVtk->GetPointData()->GetScalars("tomoA");
    auto tomoBVtk = polyDataVtk->GetPointData()->GetScalars("tomoB");
    auto tomoBufferVtk = polyDataVtk->GetPointData()->GetScalars("tomoBuffer");
    auto resultVtk = polyDataVtk->GetPointData()->GetScalars("result");
    auto basisVtk = polyDataVtk->GetFieldData()->GetArray("basis");

    int nPoints = pointsVtk->GetNumberOfPoints();

    auto points = std::make_shared<Points>();
    for (int i = 0; i < nPoints; i++) {
        auto xyz = pointsVtk->GetPoint(i);
        points->push_back(Point(xyz[0], xyz[1], xyz[2]));
    }

    pointsCt->setPoints(points);

    auto tomoA = pointsCt->getTomoA();
    auto tomoB = pointsCt->getTomoB();
    auto tomoBuffer = pointsCt->getTomoBuffer();
    auto result = pointsCt->getResult();

    for (int i = 0; i < nPoints; i++) {
        (*tomoA)[i] =
                tomoAVtk->GetVariantValue(vtkIdType(i)).ToDouble();
        (*tomoB)[i] =
                tomoBVtk->GetVariantValue(vtkIdType(i)).ToDouble();
        (*tomoBuffer)[i] =
                tomoBufferVtk->GetVariantValue(vtkIdType(i)).ToDouble();
        (*result)[i] =
                resultVtk->GetVariantValue(vtkIdType(i)).ToDouble();
    }


    std::vector<double> basisValues;
    for (int i = 0; i < 12; i++)
        basisValues.push_back(
                basisVtk->GetVariantValue(vtkIdType(i)).ToDouble());

    std::vector<Direction> axes;
    for (int i = 0; i < 3; i++)
        axes.push_back(Direction(basisValues[i * 3 + 0],
                                 basisValues[i * 3 + 1],
                                 basisValues[i * 3 + 2]));

    auto origin = Point(basisValues[9],
                        basisValues[10],
                        basisValues[11]);

    pointsCt->setBasis(std::make_shared<Basis>(Basis(axes, origin)));
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
    auto tomoBufferVtk = vtkSmartPointer<vtkDoubleArray>::New();
    auto resultVtk = vtkSmartPointer<vtkDoubleArray>::New();

    tomoAVtk->SetName("tomoA");
    tomoBVtk->SetName("tomoB");
    tomoBufferVtk->SetName("tomoBuffer");
    resultVtk->SetName("result");


    auto points = pointsCt->getPoints();
    auto tomoA = pointsCt->getTomoA();
    auto tomoB = pointsCt->getTomoB();
    auto tomoBuffer = pointsCt->getTomoBuffer();
    auto result = pointsCt->getResult();


    for (int i = 0; i < pointsCt->size(); i++) {
        pointsVtk->InsertNextPoint((*points)[i].x(),
                                   (*points)[i].y(),
                                   (*points)[i].z());
        tomoAVtk->InsertNextValue((*tomoA)[i]);
        tomoBVtk->InsertNextValue((*tomoB)[i]);
        tomoBufferVtk->InsertNextValue((*tomoBuffer)[i]);
        resultVtk->InsertNextValue((*result)[i]);
    }

    auto writerVtk = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    if (fileIsBinary)
        writerVtk->SetDataModeToBinary();
    else
        writerVtk->SetDataModeToAscii();

    vtkSmartPointer<vtkVertex> vertexVtk =
            vtkSmartPointer<vtkVertex>::New();
    vertexVtk->Initialize(pointsCt->size(), pointsVtk);

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
    polyDataVtk->GetPointData()->AddArray(tomoBufferVtk);


    std::vector<double> basisValues;

    auto directions = pointsCt->getBasis()->getAxes();
    for (auto &direction : *directions) {
        basisValues.push_back(direction.dx());
        basisValues.push_back(direction.dy());
        basisValues.push_back(direction.dz());
    }

    auto origin = pointsCt->getBasis()->getOrigin();
    basisValues.push_back(origin->x());
    basisValues.push_back(origin->y());
    basisValues.push_back(origin->z());

    vtkSmartPointer<vtkDoubleArray> basisVtk =
            vtkSmartPointer<vtkDoubleArray>::New();

    basisVtk->SetNumberOfComponents(12);
    basisVtk->SetName("basis");
    basisVtk->InsertNextTuple(basisValues.data());
    polyDataVtk->GetFieldData()->AddArray(basisVtk);

    writerVtk->SetInputData(polyDataVtk);

    writerVtk->SetFileName(fileName.c_str());
    writerVtk->Write();

    fileNames.push_back(fileName);
    fileDescriptions.push_back(fileDescription);

}

void VtpCt::savePointsFile(const std::string &fileName) {
    savePointsFile(fileName, "0");
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

