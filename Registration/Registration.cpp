#include <Registration.h>

#include <CgalAliases.h>

#include <iomanip>


Transformations generateTranslationAnaRotationXYZ() {
    Transformations transformations;
    transformations.push_back(std::make_shared<TranslationX>());
    transformations.push_back(std::make_shared<TranslationY>());
    transformations.push_back(std::make_shared<TranslationZ>());
    transformations.push_back(std::make_shared<RotationX>());
    transformations.push_back(std::make_shared<RotationY>());
    transformations.push_back(std::make_shared<RotationZ>());
    return transformations;
}

Transformations generateStretchingXY() {
    Transformations transformations;
    transformations.push_back(std::make_shared<StretchingXY>());
    return transformations;
}


std::vector<double> makeRegistration(
        Image &ncCt, std::shared_ptr<PointsCt> &pointsCt,
        Transformations &transformations,
        const double &accuracy,
        const std::vector<double> &constraintsMin,
        const std::vector<double> &constraintsMax,
        const std::string &fileNamesPrefix,
        const bool &isFilesSaved) {

    auto bBoxGeneral = calculateGeneralBbox(pointsCt, transformations,
                                            constraintsMin, constraintsMax);

    ncCt.setRegion(bBoxGeneral);
    int iteration = -1;
    VtpCt vtpCt;


    InvCorrelation invCorrelation(
            transformations, *pointsCt,
            ncCt.region,
            fileNamesPrefix, isFilesSaved,
            iteration, vtpCt);

    int iterationForDerivative = -1;
    InvCorrelation invCorrelationForDerivative(
            transformations, *pointsCt,
            ncCt.region,
            fileNamesPrefix, false,
            iterationForDerivative, vtpCt);


    ColumnVector searchVector(transformations.size());
    ColumnVector lowerConstraint(transformations.size());
    ColumnVector upperConstraint(transformations.size());
    for (int i = 0; i < transformations.size(); i++) {

        if (typeid(*transformations[i]).name() == typeid(StretchingXY).name())
            searchVector(i) = 1;
        else
            searchVector(i) = 0;

        lowerConstraint(i) = constraintsMin[i];
        upperConstraint(i) = constraintsMax[i];

    }


    std::cout << std::endl;
    auto result = find_min_box_constrained(
            dlib::lbfgs_search_strategy(10),
            dlib::objective_delta_stop_strategy(accuracy).be_verbose(),
            invCorrelation, dlib::derivative(invCorrelationForDerivative),
            searchVector, lowerConstraint, upperConstraint);


    invCorrelation.implementResult(searchVector);

    std::cout << std::endl;
    std::cout << "correlation " << 1. - result << std::endl;
    std::cout << "searchVector:\n" << searchVector << std::endl;

    std::vector<double> answerVector;
    for (int i = 0; i < transformations.size(); i++)
        answerVector.push_back(searchVector(i));

    return answerVector;

}


Bbox calculateGeneralBbox(std::shared_ptr<PointsCt> &pointsCt,
                          const Transformations &transformations,
                          const std::vector<double> &constraintsMin,
                          const std::vector<double> &constraintsMax) {

    auto bBoxIni = pointsCt->generateBbox();
    std::vector<Bbox> bBoxesRotation;
    auto pointsCtBboxIni = PointsCt(bBoxIni);

    for (int i = 0; i < transformations.size(); i++)

        if (typeid(*transformations[i]).name() == typeid(RotationX).name() ||
            typeid(*transformations[i]).name() == typeid(RotationY).name() ||
            typeid(*transformations[i]).name() == typeid(RotationZ).name()) {

            auto pointsCtMin = pointsCtBboxIni;
            pointsCtMin.transform((*transformations[i])(constraintsMin[i]));
            bBoxesRotation.push_back(pointsCtMin.generateBbox());

            auto pointsCtMax = pointsCtBboxIni;
            pointsCtMax.transform((*transformations[i])(constraintsMax[i]));
            bBoxesRotation.push_back(pointsCtMax.generateBbox());

        }

    auto bBoxRotation = bBoxIni;
    for (auto &bBox : bBoxesRotation)
        bBoxRotation += bBox;


    std::vector<Bbox> bBoxesTranslation;
    auto pointsCtBboxRotation = PointsCt(bBoxRotation);

    for (int i = 0; i < transformations.size(); i++)

        if (typeid(*transformations[i]).name() != typeid(RotationX).name() &&
            typeid(*transformations[i]).name() != typeid(RotationY).name() &&
            typeid(*transformations[i]).name() != typeid(RotationZ).name()) {

            auto pointsCtMin = pointsCtBboxRotation;
            pointsCtMin.transform((*transformations[i])(constraintsMin[i]));
            bBoxesTranslation.push_back(pointsCtMin.generateBbox());

            auto pointsCtMax = pointsCtBboxRotation;
            pointsCtMax.transform((*transformations[i])(constraintsMax[i]));
            bBoxesTranslation.push_back(pointsCtMax.generateBbox());

        }

    auto bBoxGeneral = bBoxRotation;
    for (auto &bBox : bBoxesTranslation)
        bBoxGeneral += bBox;

    return bBoxGeneral;

}


double InvCorrelation::operator()(const ColumnVector &x) const {

    ++iteration;

    auto pointsCtCurr = pointsCt;
    regionCt.setPoints(pointsCtCurr.getPoints(), pointsCtCurr.getTomoB());

    std::vector<Aff_transformation> aff_transformations;
    for (int i = 0; i < transformations.size(); i++)
        aff_transformations.push_back((*transformations[i])(x(i)));

    pointsCtCurr.transform(aff_transformations);
    regionCt.computePointsValue();
    pointsCtCurr.computeResult();

    if (isFilesSaved) {
        vtpCt.setPointsCt(std::make_shared<PointsCt>(pointsCtCurr));
        vtpCt.savePointsFile(fileNamesPrefix + "_" + toString(iteration) +
                             ".vtp", toString(iteration));
    }

    return 1. - pointsCtCurr.computePearsonCorrelation();
}


void InvCorrelation::implementResult(const ColumnVector &x) {

    if (isFilesSaved)
        vtpCt.saveCollectionFile(fileNamesPrefix + ".pvd");

    regionCt.setPoints(pointsCt.getPoints(), pointsCt.getTomoB());

    std::vector<Aff_transformation> aff_transformations;
    for (int i = 0; i < transformations.size(); i++)
        aff_transformations.push_back((*transformations[i])(x(i)));

    pointsCt.transform(aff_transformations);

    regionCt.computePointsValue();
    pointsCt.computeResult();

}


InvCorrelation::InvCorrelation(Transformations &_transformations,
                               PointsCt &_pointsCt,
                               Region &_regionCt,
                               const std::string &_fileNamesPrefix,
                               const bool &_isFilesSaved,
                               int &_iteration,
                               VtpCt &_vtpCt) :
        transformations(_transformations),
        pointsCt(_pointsCt),
        regionCt(_regionCt),
        fileNamesPrefix(_fileNamesPrefix),
        isFilesSaved(_isFilesSaved),
        iteration(_iteration),
        vtpCt(_vtpCt) {}


