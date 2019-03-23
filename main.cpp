#include <iostream>
#include <string>

#include <NcCt.h>
#include <PointsCt.h>

int main() {

    auto ncFileName = std::string("/Volumes/ElkData/CT/samples/10.nc");

    NcCt ncCt(ncFileName);

    auto dimArrays = ncCt.getDimArrays();

    std::cout << ncCt << std::endl;

    /*std::cout << "dimArrays :" << std::endl;
    for (auto &&dimArray : *dimArrays) {
        std::cout << "-> ";
        for (auto &&value : dimArray)
            std::cout << value << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;*/

    return EXIT_SUCCESS;
}