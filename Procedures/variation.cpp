#include <variation.h>

#include <string>
#include <vector>

#include <StringAndNumber.h>
#include <gnuplot.h>

#include <Vtp/ScanGridIO.h>
#include <NcOps/Image.h>
#include <ScanGrid/ScanGrid.h>
#include <Geometry/Translation.h>
#include <Geometry/Stretch.h>


double variatePoints(std::shared_ptr<ScanGrid> pointsCt, Region &regionCt,
                     std::shared_ptr<TransformationFunctor> transformationFunctor,
                     const std::vector<double> &valuesRelative,
                     const std::vector<double> &valuesAbsolute,
                     const std::vector<double> &valuesAbsoluteReverse,
                     const std::string &fileNamesPrefix,
                     const bool &isFilesSaved) {

    std::vector<double> correlations;

    auto vtpCt = ScanGridIO(pointsCt);

    for (int i = 0; i < valuesRelative.size(); i++) {

        pointsCt->transform((*transformationFunctor)(valuesRelative[i]));
        regionCt.computePointsValue();
      pointsCt->computeDifferenceAB();
        correlations.push_back(pointsCt->computePearsonCorrelationAB());

        if (isFilesSaved)
            vtpCt.savePointsCtToFile(
                fileNamesPrefix + "_" +
                typeid(decltype(transformationFunctor)::element_type).name() +
                toString(valuesAbsolute[i]) + ".vtp",
                    toString(valuesAbsolute[i]));

    }

    if (isFilesSaved)
        vtpCt.saveFilesCollectionToFile(
            fileNamesPrefix + "_" +
            typeid(decltype(transformationFunctor)::element_type).name() + ".pvd");


    int indMaxCorrelation = 0;
    double maxCorrelation = correlations[indMaxCorrelation];

    for (int i = 0; i < correlations.size(); i++)
        if (maxCorrelation < correlations[i]) {
            maxCorrelation = correlations[i];
            indMaxCorrelation = i;
        }

    pointsCt->transform((*transformationFunctor)(valuesAbsoluteReverse.back()));
    pointsCt->transform((*transformationFunctor)(valuesAbsolute[indMaxCorrelation]));
    regionCt.computePointsValue();
  pointsCt->computeDifferenceAB();


    std::string plotLine =
            "plot \"-\" using 1:2 with cgalPoints_ pt \"*\" notitle";
    /*std::string plotLine =
            "plot \"-\" using 1:2 with linespoint lw 2 pt 8 notitle";*/

    for (int i = 0; i < valuesAbsolute.size(); i++) {
        plotLine += "\n" + toString(valuesAbsolute[i], 20, 5, true);
        plotLine += " " + toString(correlations[i], 20, 5, true);
    }

    GnuplotPipe gp;
    gp.sendLine("set title '" +
                fileNamesPrefix + " " +
                typeid(decltype(transformationFunctor)::element_type).name() +
                ": max " + toString(valuesAbsolute[indMaxCorrelation]) + "'");
    gp.sendLine("set term dumb");
    //set term tikz latex size 17.9,6
    //set output '6_compare1D_C_H_4_2.txt'
    gp.sendLine(plotLine);


    return valuesAbsolute[indMaxCorrelation];

}


double processVariation(std::shared_ptr<ScanGrid> pointsCt, Image &ncCt,
                        std::shared_ptr<TransformationFunctor> transformationFunctor,
                        const double &valueWidth, const int &nValues,
                        const std::string &fileNamesPrefix,
                        const bool &isFilesSaved) {

    std::vector<double> valuesRelative;
    std::vector<double> valuesAbsolute;
    std::vector<double> valuesAbsoluteReverse;

    int _nValues = nValues;
    if (nValues % 2 == 0)
        _nValues += 1;

    if (typeid(decltype(transformationFunctor)::element_type).name() ==
        typeid(StretchXY).name()) {

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
        valuesAbsoluteReverse.push_back(-valuesStart);

        for (int i = 1; i < _nValues; i++) {
            valuesRelative.push_back(valuesStep);
            valuesAbsolute.push_back(valuesStart + valuesStep * i);
            valuesAbsoluteReverse.push_back(-valuesAbsolute[i]);
        }
    }


    pointsCt->transform((*transformationFunctor)(valuesAbsolute.front()));
    auto bBoxFront = pointsCt->generateBbox();
    pointsCt->transform((*transformationFunctor)(valuesAbsoluteReverse.front()));

    pointsCt->transform((*transformationFunctor)(valuesAbsolute.back()));
    auto bBoxBack = pointsCt->generateBbox();
    pointsCt->transform((*transformationFunctor)(valuesAbsoluteReverse.back()));

    ncCt.setRegion(bBoxFront + bBoxBack);
    ncCt.region.setPoints(pointsCt->getPoints(), pointsCt->getTomoB());

    auto value = variatePoints(pointsCt, ncCt.region,
                               transformationFunctor,
                               valuesRelative,
                               valuesAbsolute,
                               valuesAbsoluteReverse,
                               fileNamesPrefix,
                               isFilesSaved);


    return value;

}


