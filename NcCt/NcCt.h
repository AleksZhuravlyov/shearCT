#ifndef NCCT_H
#define NCCT_H

#include <string>

#include <netcdf>


class NcCt {

public:

    NcCt() = delete;

    NcCt(const std::string &_ncFileName);

    virtual ~NcCt() {}

    friend std::ostream &operator<<(std::ostream &stream, const NcCt &ncCt);

    std::shared_ptr<std::vector<std::vector<float>>> getDimArrays() {
        return std::make_shared<std::vector<std::vector<float>>>(dimArrays);
    }


private:

    std::string ncFileName;

    netCDF::NcFile ctFile;

    std::multimap<std::string, netCDF::NcVar> vars;

    std::string valName;

    std::vector<netCDF::NcDim> dimVector;

    std::string units;

    std::vector<std::string> varNames;

    std::vector<std::string> varUnitNames;

    std::vector<int> varDims;

    std::vector<std::string> dimNames;

    std::vector<size_t> dimSize;

    std::vector<std::vector<float>> dimArrays;

};


#endif //NCCT_H
