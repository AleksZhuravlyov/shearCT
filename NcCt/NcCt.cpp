#include <algorithm>

#include <NcCt.h>


NcCt::NcCt(const std::string &fileName) : file(fileName, netCDF::NcFile::read),
                                          path(fileName),
                                          valueName("tomo"),
                                          units("units"),
                                          regionCt(RegionCt()) {

    for (auto &&varData : file.getVars()) {
        vars.push_back(Var());
        vars.back().name = varData.first;
        varData.second.getAtt(units).getValues(vars.back().unitName);
        vars.back().dim = varData.second.getDimCount();
    }

    std::reverse(vars.begin(), vars.end() - 1);

    for (auto &&dimData : file.getVar(valueName).getDims()) {
        dims.push_back(Dim());
        dims.back().name = dimData.getName();
        dims.back().size = dimData.getSize();
        dimArrays.push_back(std::vector<float>(dims.back().size));
        file.getVar(dims.back().name).getVar(dimArrays.back().data());
    }


    for (int i = 0; i < dimArrays.size(); i++)
        std::copy(dimArrays[i].begin() + regionCt.start[i],
                  dimArrays[i].begin() + regionCt.start[i]
                  + regionCt.width[i], regionCt.dimArrays[i].begin());

    file.getVar(valueName).getVar(regionCt.start, regionCt.width,
                                  regionCt.value.data());

    xInit = dimArrays[2][0];
    yInit = dimArrays[1][0];
    zInit = dimArrays[0][0];

    xStep = dimArrays[2][1] - xInit;
    yStep = dimArrays[1][1] - yInit;
    zStep = dimArrays[0][1] - zInit;

}


std::ostream &operator<<(std::ostream &stream, const NcCt &ncCt) {

    stream << "path: " << ncCt.path << std::endl;

    stream << "vars:" << std::endl;
    stream << "name unitName dim:" << std::endl;
    for (auto &&var : ncCt.vars) {
        stream << var.name << " ";
        stream << var.unitName << " ";
        stream << var.dim << std::endl;
    }

    stream << "dims:" << std::endl;
    stream << "name size step:" << std::endl;
    for (int i = 0; i < ncCt.dims.size(); i++) {
        stream << ncCt.dims[i].name << " ";
        stream << ncCt.dims[i].size << " ";
        stream << ncCt.dimArrays[i][1] - ncCt.dimArrays[i][0] << std::endl;
    }

    return stream;

}


double NcCt::getXInit() {
    return xInit;
}

double NcCt::getYInit() {
    return yInit;
}

double NcCt::getZInit() {
    return zInit;
}


double NcCt::getXStep() {
    return xStep;
}

double NcCt::getYStep() {
    return yStep;
}

double NcCt::getZStep() {
    return zStep;
}


std::shared_ptr<std::vector<Dim>> NcCt::getDims() {
    return std::make_shared<std::vector<Dim>>(dims);
}

std::shared_ptr<std::vector<Var>> NcCt::getVars() {
    return std::make_shared<std::vector<Var>>(vars);
}

std::shared_ptr<std::vector<short>> NcCt::getValue() {
    return std::make_shared<std::vector<short>>(regionCt.value);
}


void NcCt::setRegionCt(const std::vector<size_t> &start,
                       const std::vector<size_t> &width) {

    regionCt.initiateRegionCt(start, width);

    for (int i = 0; i < dimArrays.size(); i++)
        std::copy(dimArrays[i].begin() + regionCt.start[i],
                  dimArrays[i].begin() + regionCt.start[i]
                  + regionCt.width[i], regionCt.dimArrays[i].begin());

    regionCt.computeInitsAndSteps();

    file.getVar(valueName).getVar(regionCt.start, regionCt.width,
                                  regionCt.value.data());

}


void NcCt::setRegionCt(const Bbox &bbox) {

    auto xStart = size_t((bbox.xmin() - xInit) / xStep) - 1;
    auto yStart = size_t((bbox.ymin() - yInit) / yStep) - 1;
    auto zStart = size_t((bbox.zmin() - zInit) / zStep) - 1;

    auto xWidth = size_t((bbox.xmax() - bbox.xmin()) / xStep) + 5;
    auto yWidth = size_t((bbox.ymax() - bbox.ymin()) / yStep) + 5;
    auto zWidth = size_t((bbox.zmax() - bbox.zmin()) / zStep) + 5;


    std::string errorMessage;
    if ((xStart + xWidth) > dims[2].size)
        errorMessage += " x ";
    if ((yStart + yWidth) > dims[1].size)
        errorMessage += " y ";
    if ((zStart + zWidth) > dims[0].size)
        errorMessage += " z ";

    if (errorMessage != "") {

        std::cerr << "PointsCt are out range for " <<
                  errorMessage << std::endl;

        std::exit(EXIT_FAILURE);

    }


    std::vector<size_t> start = {zStart, yStart, xStart};
    std::vector<size_t> width = {zWidth, yWidth, xWidth};

    setRegionCt(start, width);

}


void NcCt::saveRegionCt(const std::string &fileName) {

    netCDF::NcFile regionCtFile(fileName, netCDF::NcFile::replace);

    std::vector<netCDF::NcDim> ncDims(dims.size());
    for (int i = 0; i < dims.size(); i++)
        ncDims[i] = regionCtFile.addDim(dims[i].name, regionCt.width[i]);

    std::vector<netCDF::NcVar> ncVars(dims.size());
    for (int i = 0; i < dims.size(); i++)
        ncVars[i] = regionCtFile.addVar(vars[i].name,
                                        netCDF::ncFloat, ncDims[i]);
    netCDF::NcVar valVar = regionCtFile.addVar(valueName,
                                               netCDF::ncShort, ncDims);

    for (int i = 0; i < dims.size(); i++)
        ncVars[i].putAtt(units, vars[i].unitName);
    valVar.putAtt(units, vars[dims.size()].unitName);

    for (int i = 0; i < dims.size(); i++)
        ncVars[i].putVar(regionCt.dimArrays[i].data());
    valVar.putVar(regionCt.value.data());

}