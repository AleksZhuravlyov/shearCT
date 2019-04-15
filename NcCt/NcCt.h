#ifndef NCCT_H
#define NCCT_H

#include <string>

#include <netcdf>

#include <Basis.h>
#include <RegionCt.h>

struct Dim {
    std::string name;
    size_t size;
};

struct Var {
    std::string name;
    std::string unitName;
    int dim;
};


class NcCt {

public:

    NcCt() = delete;

    NcCt(const std::string &fileName);

    virtual ~NcCt() {}


    friend std::ostream &operator<<(std::ostream &stream, const NcCt &ncCt);


    std::shared_ptr<std::vector<Dim>> getDims();

    std::shared_ptr<std::vector<Var>> getVars();

    std::shared_ptr<std::vector<short>> getVal();


    void setRegionCt(const std::vector<size_t> &start,
                     const std::vector<size_t> &width);

    void setRegionCt(const Bbox &bbox);

    void saveRegionCt(const std::string &fileName);

    RegionCt regionCt;


private:

    netCDF::NcFile file;


    std::string path;

    std::string valName;

    std::string units;


    std::vector<Var> vars;

    std::vector<Dim> dims;

    std::vector<std::vector<float>> dimArrays;




};


#endif //NCCT_H
