#ifndef NCCT_H
#define NCCT_H

#include <string>

#include <netcdf>

struct Dim {
    std::string name;
    size_t size;
    std::vector<float> array;
};

struct Var {
    std::string name;
    std::string unitName;
    int dim;
};

struct Region {
    std::vector<size_t> start;
    std::vector<size_t> width;
};


class NcCt {

public:

    NcCt() = delete;

    NcCt(const std::string &fileName);

    virtual ~NcCt() {}


    friend std::ostream &operator<<(std::ostream &stream, const NcCt &ncCt);


    std::shared_ptr<std::vector<Dim>> getDims();

    std::shared_ptr<std::vector<Var>> getVars();

    std::shared_ptr<std::vector<short>> getValue();


    void setRegion(const Region &_region);

    void saveRegion(const std::string &fileName);


private:

    netCDF::NcFile file;


    std::string path;

    std::string valueName;

    std::string units;


    std::vector<Var> vars;

    std::vector<Dim> dims;


    Region region;

    std::vector<short> value;

};


#endif //NCCT_H
