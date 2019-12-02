#ifndef SHEARCT_REGISTRATION_H
#define SHEARCT_REGISTRATION_H

#include <vector>
#include <string>
#include <iomanip>
#include <PointsCt.h>
#include <Image.h>
#include <Transformation.h>
#include <VtpCt.h>
#include <dlib/optimization.h>
#include <dlib/global_optimization.h>

#include <CgalAliases.h>
#include <StringAndNumber.h>

typedef dlib::matrix<double, 0, 1> ColumnVector;

typedef std::vector<std::shared_ptr<Transformation>> Transformations;

Transformations generateTranslationAnaRotationXYZ();

Transformations generateStretchingXY();

std::vector<double> makeRegistration(
        Image &ncCt, std::shared_ptr<PointsCt> &pointsCt,
        Transformations &transformations,
        const double &accuracy,
        const std::vector<double> &constraintsMin,
        const std::vector<double> &constraintsMax,
        const std::string &fileNamesPrefix,
        const bool &isFilesSaved);

Bbox calculateGeneralBbox(std::shared_ptr<PointsCt> &pointsCt,
                          const Transformations &transformations,
                          const std::vector<double> &constraintsMin,
                          const std::vector<double> &constraintsMax);

class InvCorrelation {

public:

    InvCorrelation(Transformations &_transformations,
                   PointsCt &_pointsCt,
                   Region &_regionCt,
                   const std::string &_fileNamesPrefix,
                   const bool &_isFilesSaved,
                   int &_iteration,
                   VtpCt &_vtpCt);

    virtual ~InvCorrelation() = default;

    double operator()(const ColumnVector &x) const;

    Transformations &transformations;

    PointsCt &pointsCt;

    Region &regionCt;

    std::string fileNamesPrefix;

    bool isFilesSaved;

    int &iteration;

    VtpCt &vtpCt;


    void implementResult(const ColumnVector &x);


};


#endif //SHEARCT_REGISTRATION_H
