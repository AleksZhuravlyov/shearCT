#include <demo.h>


PointsCt createInitBaseSquare(NcCt &ncCt) {

    double xCenter = ncCt.getXInit() + (ncCt.getXStep() * 2399) / 2;
    double yCenter = ncCt.getYInit() + (ncCt.getYStep() * 2399) / 2;

    double zCenter = ncCt.getZInit() + 200 * ncCt.getZStep();

    double xWidth = 1000 * ncCt.getXStep();
    double yWidth = 1000 * ncCt.getYStep();

    PointsCt pointsCt;
    pointsCt.createXYSquare(xCenter, yCenter, zCenter,
                            xWidth, yWidth, 1000, 1000);

    return pointsCt;
}


void takeBaseDataFromFirstCt(std::shared_ptr<PointsCt> pointsCt, NcCt &ncCt) {
    ncCt.setRegionCt(pointsCt->generateBbox());
    ncCt.regionCt.setPoints(pointsCt->getPoints(), pointsCt->getTomoA());
    ncCt.regionCt.computePointsValue();
}


double variatePoints(std::shared_ptr<PointsCt> pointsCt, RegionCt &regionCt,
                     std::shared_ptr<Transformation> transformation,
                     const std::vector<double> &relativeValues,
                     const std::vector<double> &absoluteValues,
                     const std::string &fileNamesPrefix,
                     const bool &isFilesSaved) {

    std::cout << transformation->getName() << ":" << std::endl << std::endl;

    std::vector<double> correlations;

    auto vtpCt = VtpCt(pointsCt);

    for (int i = 0; i < relativeValues.size(); i++) {

        pointsCt->transform((*transformation)(relativeValues[i]));
        regionCt.computePointsValue();
        pointsCt->computeResult();
        correlations.push_back(pointsCt->computePearsonCorrelation());

        if (isFilesSaved)
            vtpCt.savePointsFile(
                    fileNamesPrefix + "_" + transformation->getName() +
                    toString(absoluteValues[i]) + ".vtp",
                    toString(absoluteValues[i]));

    }

    if (isFilesSaved)
        vtpCt.saveCollectionFile(fileNamesPrefix + ".pvd");


    for (int i = 0; i < absoluteValues.size(); i++) {
        std::cout << "offset " << absoluteValues[i];
        std::cout << "   pearsonCorrelation " << correlations[i]
                  << std::endl;
    }

    int indMaxCorrelation = 0;
    double maxCorrelation = correlations[indMaxCorrelation];

    for (int i = 0; i < correlations.size(); i++)
        if (maxCorrelation < correlations[i]) {
            maxCorrelation = correlations[i];
            indMaxCorrelation = i;
        }

    pointsCt->transform((*transformation)(-absoluteValues.back()));
    pointsCt->transform((*transformation)(absoluteValues[indMaxCorrelation]));
    regionCt.computePointsValue();
    pointsCt->computeResult();

    return absoluteValues[indMaxCorrelation];

}


void variateBaseOffsetZ(std::shared_ptr<PointsCt> pointsCt, NcCt &ncCt) {

    int nOffsetsZ = 21;
    auto offsetsZWidth = ncCt.getZStep() * 2;
    auto offsetsZStart = -offsetsZWidth / 2;
    auto offsetsZStep = offsetsZWidth / (nOffsetsZ - 1);
    std::vector<double> offsetsZRelative;
    std::vector<double> offsetsZAbsolute;
    offsetsZRelative.push_back(offsetsZStart);
    offsetsZAbsolute.push_back(offsetsZStart);
    for (int i = 1; i < nOffsetsZ; i++) {
        offsetsZRelative.push_back(offsetsZStep);
        offsetsZAbsolute.push_back(offsetsZStart + offsetsZStep * i);
    }

    auto translationZ = std::make_shared<TranslationZ>();

    pointsCt->transform((*translationZ)(offsetsZAbsolute.front()));
    auto bBoxFront = pointsCt->generateBbox();
    pointsCt->transform((*translationZ)(-offsetsZAbsolute.front()));

    pointsCt->transform((*translationZ)(offsetsZAbsolute.back()));
    auto bBoxBack = pointsCt->generateBbox();
    pointsCt->transform((*translationZ)(-offsetsZAbsolute.back()));

    ncCt.setRegionCt(bBoxFront + bBoxBack);
    ncCt.regionCt.setPoints(pointsCt->getPoints(), pointsCt->getTomoB());

    auto OffsetZ = variatePoints(pointsCt, ncCt.regionCt,
                                 translationZ,
                                 offsetsZRelative, offsetsZAbsolute,
                                 "base", true);

    std::cout << std::endl << "computed OffsetZ " << OffsetZ << std::endl;

}


std::shared_ptr<PointsCt>
createTopFirstCt(std::shared_ptr<PointsCt> pointsCt, NcCt &ncCt) {

    auto pointsTopFirstCt = *pointsCt;

    auto offsetZ = ncCt.getZStep() * 1000;

    TranslationZ translationZ;
    pointsTopFirstCt.transform(translationZ(offsetZ));

    ncCt.setRegionCt(pointsTopFirstCt.generateBbox());
    ncCt.regionCt.setPoints(pointsTopFirstCt.getPoints(),
                            pointsTopFirstCt.getTomoA());
    ncCt.regionCt.computePointsValue();

    return std::make_shared<PointsCt>(pointsTopFirstCt);

}