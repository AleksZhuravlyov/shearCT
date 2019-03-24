#include <NcCt.h>

NcCt::NcCt(const std::string &fileName) : file(fileName, netCDF::NcFile::read),
                                          path(fileName),
                                          valueName("tomo"),
                                          units("units"),
                                          value(std::vector<short>(1)),
                                          region({{0, 0, 0},
                                                  {1, 1, 1}}) {

    file.getVar(valueName).getVar(region.start, region.width, value.data());

    for (auto &&varData : file.getVars()) {
        vars.push_back(Var());
        vars.back().name = varData.first;
        varData.second.getAtt(units).getValues(vars.back().unitName);
        vars.back().dim = varData.second.getDimCount();
    }

    for (auto &&dimData : file.getVar(valueName).getDims()) {
        dims.push_back(Dim());
        dims.back().name = dimData.getName();
        dims.back().size = dimData.getSize();
        dims.back().array.resize(dims.back().size);
        file.getVar(dims.back().name).getVar(dims.back().array.data());
    }

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
    for (auto &&dim : ncCt.dims) {
        stream << dim.name << " ";
        stream << dim.size << " ";
        stream << dim.array[1] - dim.array[0] << std::endl;
    }

    return stream;

}


std::shared_ptr<std::vector<Dim>> NcCt::getDims() {
    return std::make_shared<std::vector<Dim>>(dims);
}

std::shared_ptr<std::vector<Var>> NcCt::getVars() {
    return std::make_shared<std::vector<Var>>(vars);
}

std::shared_ptr<std::vector<short>> NcCt::getValue() {
    return std::make_shared<std::vector<short>>(value);
}


void NcCt::setRegion(const Region &_region) {

    region = _region;

    auto nPoints = region.width[0] * region.width[1] * region.width[2];
    if (nPoints != value.size())
        value.resize(nPoints);

    file.getVar(valueName).getVar(region.start, region.width, value.data());

}

void NcCt::saveRegion(const std::string &fileName) {

    netCDF::NcFile regionFile(fileName, netCDF::NcFile::replace);


    netCDF::NcDim lvlDim = regionFile.addDim(dims[0].name, region.width[0]);
    netCDF::NcDim ltdDim = regionFile.addDim(dims[1].name, region.width[1]);
    netCDF::NcDim lngDim = regionFile.addDim(dims[2].name, region.width[2]);


    netCDF::NcVar lvlVar = regionFile.addVar(vars[2].name,
                                             netCDF::ncFloat, lvlDim);
    netCDF::NcVar ltdVar = regionFile.addVar(vars[1].name,
                                             netCDF::ncFloat, ltdDim);
    netCDF::NcVar lngVar = regionFile.addVar(vars[0].name,
                                             netCDF::ncFloat, lngDim);


    std::vector<netCDF::NcDim> dimVector;
    dimVector.push_back(lvlDim);
    dimVector.push_back(ltdDim);
    dimVector.push_back(lngDim);
    netCDF::NcVar valVar = regionFile.addVar(valueName,
                                             netCDF::ncShort, dimVector);


    lvlVar.putAtt(units, vars[0].unitName);
    ltdVar.putAtt(units, vars[1].unitName);
    lngVar.putAtt(units, vars[2].unitName);
    valVar.putAtt(units, vars[3].unitName);


    auto lvls = std::vector<float>(dims[0].array.begin() + region.start[0],
                                   dims[0].array.begin() + region.start[0]
                                   + region.width[0]);

    auto ltds = std::vector<float>(dims[1].array.begin() + region.start[1],
                                   dims[1].array.begin() + region.start[1]
                                   + region.width[1]);

    auto lngs = std::vector<float>(dims[2].array.begin() + region.start[2],
                                   dims[2].array.begin() + region.start[2]
                                   + region.width[2]);

    lvlVar.putVar(lvls.data());
    ltdVar.putVar(ltds.data());
    lngVar.putVar(lngs.data());
    valVar.putVar(value.data());

}