#include <NcCt.h>

NcCt::NcCt(const std::string &_ncFileName) :

        ncFileName(_ncFileName),
        ctFile(_ncFileName, netCDF::NcFile::read),
        vars(ctFile.getVars()),
        valName("tomo"),
        dimVector(ctFile.getVar(valName).getDims()),
        units("units") {

    for (auto &&var : vars) {
        varNames.push_back(var.first);
        varUnitNames.push_back("");
        var.second.getAtt(units).getValues(varUnitNames.back());
        varDims.push_back(var.second.getDimCount());
    }

    for (auto &&dim : dimVector) {
        dimNames.push_back(dim.getName());
        dimSize.push_back(dim.getSize());
    }

    for (auto &&size : dimSize)
        dimArrays.push_back(std::vector<float>(size));

    for (int i = 0; i < dimNames.size(); i++)
        ctFile.getVar(dimNames[i]).getVar(dimArrays[i].data());

}

std::ostream &operator<<(std::ostream &stream, const NcCt &ncCt) {

    stream << "ncFileName:" << std::endl;
    stream << ncCt.ncFileName << std::endl;

    stream << "varName varUnitName varDim:" << std::endl;
    for (int i = 0; i < ncCt.varNames.size(); i++) {
        stream << ncCt.varNames[i] << " ";
        stream << ncCt.varUnitNames[i] << " ";
        stream << ncCt.varDims[i] << std::endl;
    }

    stream << "dimName dimSize dimStep:" << std::endl;
    for (int i = 0; i < ncCt.dimNames.size(); i++) {
        stream << ncCt.dimNames[i] << " ";
        stream << ncCt.dimSize[i] << " ";
        stream << ncCt.dimArrays[i][1] - ncCt.dimArrays[i][0] << std::endl;
    }

    return stream;
}

