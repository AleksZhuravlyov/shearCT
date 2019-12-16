#ifndef SHEARCT_REGISTRATION_H
#define SHEARCT_REGISTRATION_H

#include <vector>
#include <string>
#include <iomanip>
#include <ScanGrid/ScanGrid.h>
#include <NcOps/Image.h>
#include <Geometry/TransformationFunctor.h>
#include <Vtp/ScanGridIO.h>
#include <dlib/optimization.h>
#include <dlib/global_optimization.h>


#include <StringAndNumber.h>

typedef dlib::matrix<double, 0, 1> ColumnVector;

typedef std::vector<std::shared_ptr<TransformationFunctor>> Transformations;

Transformations generateTranslationAnaRotationXYZ();

Transformations generateStretchingXY();

std::vector<double> makeRegistration(
        Image &ncCt, std::shared_ptr<ScanGrid> &pointsCt,
        Transformations &transformations,
        const double &accuracy,
        const std::vector<double> &constraintsMin,
        const std::vector<double> &constraintsMax,
        const std::string &fileNamesPrefix,
        const bool &isFilesSaved);

Bbox calculateGeneralBbox(std::shared_ptr<ScanGrid> &pointsCt,
                          const Transformations &transformations,
                          const std::vector<double> &constraintsMin,
                          const std::vector<double> &constraintsMax);

class InvCorrelation {

public:

    InvCorrelation(Transformations &_transformations,
                   ScanGrid &_pointsCt,
                   Region &_regionCt,
                   const std::string &_fileNamesPrefix,
                   const bool &_isFilesSaved,
                   int &_iteration,
                   ScanGridIO &_vtpCt);

    virtual ~InvCorrelation() = default;

    double operator()(const ColumnVector &x) const;

    Transformations &transformations;

    ScanGrid &pointsCt;

    Region &regionCt;

    std::string fileNamesPrefix;

    bool isFilesSaved;

    int &iteration;

    ScanGridIO &vtpCt;


    void implementResult(const ColumnVector &x);


};


#endif //SHEARCT_REGISTRATION_H
