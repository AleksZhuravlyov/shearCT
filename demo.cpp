#include <demo.h>


PointsCt createInitBaseSquare(NcCt &ncCt) {

    double xCenter = ncCt.getXInit() + (ncCt.getXStep() * 2399) / 2;
    double yCenter = ncCt.getYInit() + (ncCt.getYStep() * 2399) / 2;

    double zCenter = ncCt.getZInit() + 90 * ncCt.getZStep();

    double xWidth = 100 * ncCt.getXStep();
    double yWidth = 100 * ncCt.getYStep();

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
        vtpCt.saveCollectionFile(fileNamesPrefix + "_" +
                                 transformation->getName() + ".pvd");


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


    std::string plotLine =
            "plot \"-\" using 1:2 with points pt \"*\" notitle";
    /*std::string plotLine =
            "plot \"-\" using 1:2 with linespoint lw 2 pt 8 notitle";*/

    for (int i = 0; i < absoluteValues.size(); i++) {
        plotLine += "\n" + toString(absoluteValues[i], 20, 5, true);
        plotLine += " " + toString(correlations[i], 20, 5, true);
    }

    GnuplotPipe gp;
    gp.sendLine("set title '" +
                fileNamesPrefix + " " + transformation->getName() +
                ": max " + toString(absoluteValues[indMaxCorrelation]) + "'");
    gp.sendLine("set term dumb");
    gp.sendLine(plotLine);


    return absoluteValues[indMaxCorrelation];

}


double processVariation(std::shared_ptr<PointsCt> pointsCt, NcCt &ncCt,
                        std::shared_ptr<Transformation> transformation,
                        const double &valueWidth, const int &nValues,
                        const std::string &fileNamesPrefix,
                        const bool &isFilesSaved) {

    auto valuesStart = -valueWidth / 2;
    auto valuesStep = valueWidth / (nValues - 1);
    std::vector<double> valuesRelative;
    std::vector<double> valuesAbsolute;
    valuesRelative.push_back(valuesStart);
    valuesAbsolute.push_back(valuesStart);
    for (int i = 1; i < nValues; i++) {
        valuesRelative.push_back(valuesStep);
        valuesAbsolute.push_back(valuesStart + valuesStep * i);
    }


    pointsCt->transform((*transformation)(valuesAbsolute.front()));
    auto bBoxFront = pointsCt->generateBbox();
    pointsCt->transform((*transformation)(-valuesAbsolute.front()));

    pointsCt->transform((*transformation)(valuesAbsolute.back()));
    auto bBoxBack = pointsCt->generateBbox();
    pointsCt->transform((*transformation)(-valuesAbsolute.back()));

    ncCt.setRegionCt(bBoxFront + bBoxBack);
    ncCt.regionCt.setPoints(pointsCt->getPoints(), pointsCt->getTomoB());

    auto value = variatePoints(pointsCt, ncCt.regionCt,
                               transformation,
                               valuesRelative, valuesAbsolute,
                               fileNamesPrefix, isFilesSaved);

    /*std::cout << "computed" << " " << transformation->getName() <<
              " " << value << std::endl << std::endl;*/

    return value;

}


std::shared_ptr<std::vector<double>> computeTomoATopFirstCt(
        std::shared_ptr<PointsCt> pointsCt,
        NcCt &ncCt, const double &shiftZ) {

    auto pointsTopFirstCt = std::make_shared<PointsCt>(*pointsCt);

    pointsTopFirstCt->transform(TranslationZ()(shiftZ));

    ncCt.setRegionCt(pointsTopFirstCt->generateBbox());
    ncCt.regionCt.setPoints(pointsTopFirstCt->getPoints(),
                            pointsTopFirstCt->getTomoA());
    ncCt.regionCt.computePointsValue();

    auto vtpCt = VtpCt(pointsTopFirstCt);

    vtpCt.savePointsFile("tmpTop.vtp", "0");

    return pointsTopFirstCt->getTomoA();

}
