#include <string>
#include <iostream>

#include <stretchZvsShift.h>
#include <demoSchema.h>


int main() {

    // demoSchema();

    double shiftZ = 0.0522;
    double initZ = 0.0032;

    auto stretchLength = stretchZVsShift(1.e-8,
                                         shiftZ,
                                         0.45,
                                         0.5,
                                         initZ,
                                         20,
                                         20,
                                         200, 200);

    std::cout << "stretchLength " << stretchLength << std::endl;

    return EXIT_SUCCESS;

}
