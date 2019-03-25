#ifndef NCCT_H
#define NCCT_H

#include <string>

#include <netcdf>

struct Dim {
    std::string name;
    size_t size;
};

struct Var {
    std::string name;
    std::string unitName;
    int dim;
};

class Region {

public:

    Region();
    virtual ~Region() {}

    void initiateRegion(const std::vector<size_t> &_start,
                        const std::vector<size_t> &_width);

    std::vector<size_t> start;
    std::vector<size_t> width;
    std::vector<std::vector<float>> dimArrays;
    std::vector<short> val;
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


    void setRegion(const std::vector<size_t> &start,
                   const std::vector<size_t> &width);

    void saveRegion(const std::string &fileName);


private:

    netCDF::NcFile file;


    std::string path;

    std::string valName;

    std::string units;


    std::vector<Var> vars;

    std::vector<Dim> dims;

    std::vector<std::vector<float>> dimArrays;


    Region region;

};


#endif //NCCT_H
