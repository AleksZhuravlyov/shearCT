#include <Registration.h>

#include <CgalAliases.h>

void makeRegistration(NcCt &ncCt, std::shared_ptr<PointsCt> &pointsCt,
                      const Transformations &transformations,
                      const std::vector<double> &constraintsMin,
                      const std::vector<double> &constraintsMax) {

    auto bBoxGeneral = calculateGeneralBbox(pointsCt, transformations,
                                            constraintsMin, constraintsMax);


    std::cout << bBoxGeneral << std::endl;

    std::cout << "test" << std::endl;

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