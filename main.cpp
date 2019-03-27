#include <iostream>
#include <string>

#include <NcCt.h>
#include <VtpCt.h>

int main() {

    auto ncFileName = std::string("/Volumes/ElkData/CT/samples/10.nc");

    NcCt ncCt(ncFileName);

    std::cout << ncCt << std::endl;

    ncCt.setRegion({500, 600, 600}, {30, 400, 500});

    ncCt.saveRegion("/Volumes/ElkData/CT/tmp/10Cut.nc");

    return EXIT_SUCCESS;
}