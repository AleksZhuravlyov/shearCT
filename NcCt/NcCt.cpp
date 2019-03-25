#include <vector>
#include <algorithm>

#include <NcCt.h>

Region::Region() : start({0, 0, 0}),
                   width({1, 1, 1}),
                   dimArrays(std::vector<std::vector<float>>(3)),
                   val(std::vector<short>(1)) {

    for (int i = 0; i < width.size(); i++)
        dimArrays[i].resize(width[i]);

    val.resize(width[0] * width[1] * width[2]);
}

void Region::initiateRegion(const std::vector<size_t> &_start,
                            const std::vector<size_t> &_width) {
    start = _start;

    if (width != _width) {
        width = _width;

        for (int i = 0; i < width.size(); i++)
            dimArrays[i].resize(width[i]);

        val.resize(width[0] * width[1] * width[2]);
    }
}


NcCt::NcCt(const std::string &fileName) : file(fileName, netCDF::NcFile::read),
                                          path(fileName),
                                          valName("tomo"),
                                          units("units"),
                                          region(Region()) {

    for (auto &&varData : file.getVars()) {
        vars.push_back(Var());
        vars.back().name = varData.first;
        varData.second.getAtt(units).getValues(vars.back().unitName);
        vars.back().dim = varData.second.getDimCount();
    }

    std::reverse(vars.begin(), vars.end() - 1);

    for (auto &&dimData : file.getVar(valName).getDims()) {
        dims.push_back(Dim());
        dims.back().name = dimData.getName();
        dims.back().size = dimData.getSize();
        dimArrays.push_back(std::vector<float>(dims.back().size));
        file.getVar(dims.back().name).getVar(dimArrays.back().data());
    }


    for (int i = 0; i < dimArrays.size(); i++)
        std::copy(dimArrays[i].begin() + region.start[i],
                  dimArrays[i].begin() + region.start[i]
                  + region.width[i], region.dimArrays[i].begin());

    file.getVar(valName).getVar(region.start, region.width,
                                region.val.data());

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


std::shared_ptr<std::vector<Dim>> NcCt::getDims() {
    return std::make_shared<std::vector<Dim>>(dims);
}

std::shared_ptr<std::vector<Var>> NcCt::getVars() {
    return std::make_shared<std::vector<Var>>(vars);
}

std::shared_ptr<std::vector<short>> NcCt::getVal() {
    return std::make_shared<std::vector<short>>(region.val);
}


void NcCt::setRegion(const std::vector<size_t> &start,
                     const std::vector<size_t> &width) {

    region.initiateRegion(start, width);

    for (int i = 0; i < dimArrays.size(); i++)
        std::copy(dimArrays[i].begin() + region.start[i],
                  dimArrays[i].begin() + region.start[i]
                  + region.width[i], region.dimArrays[i].begin());

    file.getVar(valName).getVar(region.start, region.width, region.val.data());

}

void NcCt::saveRegion(const std::string &fileName) {

    netCDF::NcFile regionFile(fileName, netCDF::NcFile::replace);

    std::vector<netCDF::NcDim> ncDims(dims.size());
    for (int i = 0; i < dims.size(); i++)
        ncDims[i] = regionFile.addDim(dims[i].name, region.width[i]);

    std::vector<netCDF::NcVar> ncVars(dims.size());
    for (int i = 0; i < dims.size(); i++)
        ncVars[i] = regionFile.addVar(vars[i].name, netCDF::ncFloat, ncDims[i]);
    netCDF::NcVar valVar = regionFile.addVar(valName, netCDF::ncShort, ncDims);

    for (int i = 0; i < dims.size(); i++)
        ncVars[i].putAtt(units, vars[i].unitName);
    valVar.putAtt(units, vars[dims.size()].unitName);

    for (int i = 0; i < dims.size(); i++)
        ncVars[i].putVar(region.dimArrays[i].data());
    valVar.putVar(region.val.data());

}