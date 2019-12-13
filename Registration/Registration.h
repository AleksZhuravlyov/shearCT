#ifndef SHEARCT_REGISTRATION_H
#define SHEARCT_REGISTRATION_H

#include <vector>
#include <string>
#include <iomanip>
#include <Points/Points.h>
#include <NcOps/Image.h>
#include <Geometry/Transformation.h>
#include <Vtp/PointsIO.h>
#include <dlib/optimization.h>
#include <dlib/global_optimization.h>


#include <StringAndNumber.h>

typedef dlib::matrix<double, 0, 1> ColumnVector;

typedef std::vector<std::shared_ptr<Transformation>> Transformations;

Transformations generateTranslationAnaRotationXYZ();

Transformations generateStretchingXY();

std::vector<double> makeRegistration(
        Image &ncCt, std::shared_ptr<Points> &pointsCt,
        Transformations &transformations,
        const double &accuracy,
        const std::vector<double> &constraintsMin,
        const std::vector<double> &constraintsMax,
        const std::string &fileNamesPrefix,
        const bool &isFilesSaved);

Bbox_3 calculateGeneralBbox(std::shared_ptr<Points> &pointsCt,
                            const Transformations &transformations,
                            const std::vector<double> &constraintsMin,
                            const std::vector<double> &constraintsMax);

class InvCorrelation {

public:

    InvCorrelation(Transformations &_transformations,
                   Points &_pointsCt,
                   Region &_regionCt,
                   const std::string &_fileNamesPrefix,
                   const bool &_isFilesSaved,
                   int &_iteration,
                   PointsIO &_vtpCt);

    virtual ~InvCorrelation() = default;

    double operator()(const ColumnVector &x) const;

    Transformations &transformations;

    Points &pointsCt;

    Region &regionCt;

    std::string fileNamesPrefix;

    bool isFilesSaved;

    int &iteration;

    PointsIO &vtpCt;


    void implementResult(const ColumnVector &x);


};


#endif //SHEARCT_REGISTRATION_H
