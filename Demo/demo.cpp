#include <demo.h>


double variatePoints(std::shared_ptr<PointsCt> pointsCt, RegionCt &regionCt,
                     std::shared_ptr<Transformation> transformation,
                     const std::vector<double> &valuesRelative,
                     const std::vector<double> &valuesAbsolute,
                     const std::vector<double> &valuesAbsoluteReverse,
                     const std::string &fileNamesPrefix,
                     const bool &isFilesSaved) {

    std::vector<double> correlations;

    auto vtpCt = VtpCt(pointsCt);

    for (int i = 0; i < valuesRelative.size(); i++) {

        pointsCt->transform((*transformation)(valuesRelative[i]));
        regionCt.computePointsValue();
        pointsCt->computeResult();
        correlations.push_back(pointsCt->computePearsonCorrelation());

        if (isFilesSaved)
            vtpCt.savePointsFile(
                    fileNamesPrefix + "_" +
                    typeid(*transformation).name() +
                    toString(valuesAbsolute[i]) + ".vtp",
                    toString(valuesAbsolute[i]));

    }

    if (isFilesSaved)
        vtpCt.saveCollectionFile(fileNamesPrefix + "_" +
                                 typeid(*transformation).name() + ".pvd");


    int indMaxCorrelation = 0;
    double maxCorrelation = correlations[indMaxCorrelation];

    for (int i = 0; i < correlations.size(); i++)
        if (maxCorrelation < correlations[i]) {
            maxCorrelation = correlations[i];
            indMaxCorrelation = i;
        }

    pointsCt->transform((*transformation)(valuesAbsoluteReverse.back()));
    pointsCt->transform((*transformation)(valuesAbsolute[indMaxCorrelation]));
    regionCt.computePointsValue();
    pointsCt->computeResult();


    std::string plotLine =
            "plot \"-\" using 1:2 with points pt \"*\" notitle";
    /*std::string plotLine =
            "plot \"-\" using 1:2 with linespoint lw 2 pt 8 notitle";*/

    for (int i = 0; i < valuesAbsolute.size(); i++) {
        plotLine += "\n" + toString(valuesAbsolute[i], 20, 5, true);
        plotLine += " " + toString(correlations[i], 20, 5, true);
    }

    GnuplotPipe gp;
    gp.sendLine("set title '" +
                fileNamesPrefix + " " + typeid(*transformation).name() +
                ": max " + toString(valuesAbsolute[indMaxCorrelation]) + "'");
    gp.sendLine("set term dumb");
    gp.sendLine(plotLine);


    return valuesAbsolute[indMaxCorrelation];

}


double processVariation(std::shared_ptr<PointsCt> pointsCt, NcCt &ncCt,
                        std::shared_ptr<Transformation> transformation,
                        const double &valueWidth, const int &nValues,
                        const std::string &fileNamesPrefix,
                        const bool &isFilesSaved) {

    std::vector<double> valuesRelative;
    std::vector<double> valuesAbsolute;
    std::vector<double> valuesAbsoluteReverse;

    int _nValues = nValues;
    if (nValues % 2 == 0)
        _nValues += 1;

    if (typeid(*transformation).name() == typeid(StretchingXY).name()) {

        double valuesStart = 1 - valueWidth / 2;
        double valuesStop = 1 + valueWidth / 2;
        double deltaValue = (valuesStop - valuesStart) / (_nValues - 1);

        valuesRelative.push_back(valuesStart);
        valuesAbsolute.push_back(valuesStart);
        valuesAbsoluteReverse.push_back(1. / valuesStart);

        for (int i = 1; i < _nValues; i++) {
            valuesRelative.push_back((deltaValue * i + valuesStart) /
                                     (deltaValue * (i - 1) + valuesStart));
            valuesAbsolute.push_back(valuesAbsolute[i - 1] * valuesRelative[i]);
            valuesAbsoluteReverse.push_back(1. / valuesAbsolute[i]);
        }

    } else {

        double valuesStart = -valueWidth / 2;
        double valuesStep = valueWidth / (_nValues - 1);

        valuesRelative.push_back(valuesStart);
        valuesAbsolute.push_back(valuesStart);

        for (int i = 1; i < _nValues; i++) {
            valuesRelative.push_back(valuesStep);
            valuesAbsolute.push_back(valuesStart + valuesStep * i);
            valuesAbsoluteReverse.push_back(-valuesAbsolute[i]);
        }
    }


    pointsCt->transform((*transformation)(valuesAbsolute.front()));
    auto bBoxFront = pointsCt->generateBbox();
    pointsCt->transform((*transformation)(valuesAbsoluteReverse.front()));

    pointsCt->transform((*transformation)(valuesAbsolute.back()));
    auto bBoxBack = pointsCt->generateBbox();
    pointsCt->transform((*transformation)(valuesAbsoluteReverse.back()));

    ncCt.setRegionCt(bBoxFront + bBoxBack);
    ncCt.regionCt.setPoints(pointsCt->getPoints(), pointsCt->getTomoB());

    auto value = variatePoints(pointsCt, ncCt.regionCt,
                               transformation,
                               valuesRelative,
                               valuesAbsolute,
                               valuesAbsoluteReverse,
                               fileNamesPrefix,
                               isFilesSaved);


    return value;

}


std::shared_ptr<PointsCt> getPointCtFromFile(const std::string &fileName) {
    auto pointsCt = std::make_shared<PointsCt>();
    auto vtpCt = VtpCt(pointsCt);
    vtpCt.readPointsFile(fileName);
    return pointsCt;
}


std::shared_ptr<PointsCt> getBaseSquareFromCtA(NcCt &ncCt) {

    double xCenter = ncCt.getXInit() + (ncCt.getXStep() * 2399) / 2;
    double yCenter = ncCt.getYInit() + (ncCt.getYStep() * 2399) / 2;

    double zCenter = ncCt.getZInit() + 90 * ncCt.getZStep();

    double xWidth = 100 * ncCt.getXStep();
    double yWidth = 100 * ncCt.getYStep();

    auto pointsCt = std::make_shared<PointsCt>();
    pointsCt->createXYSquare(xCenter, yCenter, zCenter,
                             xWidth, yWidth, 1000, 1000);

    ncCt.setRegionCt(pointsCt->generateBbox());
    ncCt.regionCt.setPoints(pointsCt->getPoints(), pointsCt->getTomoA());
    ncCt.regionCt.computePointsValue();

    return pointsCt;

}


void getBaseSquareFromCtAWithTop(
        NcCt &ncCt, const double &shiftZ, std::shared_ptr<PointsCt> &pointsCt) {

    pointsCt->transform(TranslationZ()(shiftZ));
    ncCt.setRegionCt(pointsCt->generateBbox());
    ncCt.regionCt.setPoints(pointsCt->getPoints(), pointsCt->getTomoBuffer());
    ncCt.regionCt.computePointsValue();
    pointsCt->transform(TranslationZ()(-shiftZ));

}

std::shared_ptr<PointsCt> getBaseSquareFromCtAWithTop(
        NcCt &ncCt, const double &shiftZ, const std::string &fileName) {
    auto pointsCt = getPointCtFromFile(fileName);
    getBaseSquareFromCtAWithTop(ncCt, shiftZ, pointsCt);
    return pointsCt;
}


void getBaseSquareFromCtB(NcCt &ncCt, std::shared_ptr<PointsCt> &pointsCt) {

    processVariation(pointsCt, ncCt, std::make_shared<TranslationZ>(),
                     15e-5, 31, "bottom", false);
    processVariation(pointsCt, ncCt, std::make_shared<TranslationX>(),
                     10e-5, 21, "bottom", false);
    processVariation(pointsCt, ncCt, std::make_shared<TranslationY>(),
                     10e-5, 21, "bottom", false);
    processVariation(pointsCt, ncCt, std::make_shared<RotationX>(),
                     M_PI / 140, 31, "bottom", false);
    processVariation(pointsCt, ncCt, std::make_shared<RotationY>(),
                     M_PI / 200, 21, "bottom", false);
    processVariation(pointsCt, ncCt, std::make_shared<TranslationZ>(),
                     1e-5, 21, "bottom", false);

}

std::shared_ptr<PointsCt> getBaseSquareFromCtB(NcCt &ncCt,
                                               const std::string &fileName) {
    auto pointsCt = getPointCtFromFile(fileName);
    getBaseSquareFromCtB(ncCt, pointsCt);
    return pointsCt;
}


void getTopSquareFromCtB(NcCt &ncCt, const double &shiftZ,
                         std::shared_ptr<PointsCt> &pointsCt) {

    pointsCt->swapAAndBuffer();
    pointsCt->transform(TranslationZ()(shiftZ));


    processVariation(pointsCt, ncCt, std::make_shared<TranslationY>(),
                     40e-5, 41, "top1", false);
    processVariation(pointsCt, ncCt, std::make_shared<TranslationX>(),
                     35e-5, 31, "top1", false);
    processVariation(pointsCt, ncCt, std::make_shared<RotationX>(),
                     M_PI / 30, 41, "top1", false);
    processVariation(pointsCt, ncCt, std::make_shared<RotationY>(),
                     M_PI / 50, 41, "top1", false);
    processVariation(pointsCt, ncCt, std::make_shared<TranslationZ>(),
                     19e-5, 51, "top1", false);
    processVariation(pointsCt, ncCt, std::make_shared<TranslationZ>(),
                     1.5e-5, 41, "top1", false);


    processVariation(pointsCt, ncCt, std::make_shared<TranslationY>(),
                     15e-5, 41, "top2", false);
    processVariation(pointsCt, ncCt, std::make_shared<TranslationX>(),
                     5e-5, 31, "top2", false);
    processVariation(pointsCt, ncCt, std::make_shared<RotationX>(),
                     M_PI / 70, 100, "top2", false);
    processVariation(pointsCt, ncCt, std::make_shared<RotationY>(),
                     M_PI / 150, 41, "top2", false);
    processVariation(pointsCt, ncCt, std::make_shared<TranslationZ>(),
                     9e-5, 61, "top2", false);
    processVariation(pointsCt, ncCt, std::make_shared<TranslationZ>(),
                     .7e-5, 61, "top2", false);

}