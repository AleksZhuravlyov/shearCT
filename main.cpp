#include "iostream"

// #include <procedures/demoSchema.h>
#include <procedures/swellingSchema.h>


int main(int nArg, char **args) {

    std::map<std::string,
            std::variant<int, double, std::string, std::vector<double>>> params;

    params["imageAFileName"] = "/Users/bigelk/tmp/microct/zak/CO2Z0al_01.nc";
    params["imageBFileName"] = "/Users/bigelk/tmp/microct/zak/CO2Z0al_10.nc";
    params["valueName"] = "array";

    params["shiftZ"] = double(0.001186);
    params["initZ"] = double(0.0008895);
    params["xCenter"] = double(0.00360544);
    params["yCenter"] = double(0.00364102);
    params["xWidth"] = double(0.0047);
    params["yWidth"] = double(0.0047);
    params["nX"] = int(300);
    params["nY"] = int(300);

    params["baseConstraintsMin"] = std::vector<double>{-30e-6, -30e-6, -30e-6,
                                                       -0.0785, -0.0785, -0.0785, 0.99};
    params["baseConstraintsMax"] = std::vector<double>{30e-6, 30e-6, 30e-6,
                                                       0.0785, 0.0785, 0.0785, 1.01};

    params["topConstraintsMin"] = std::vector<double>{-30e-6, -30e-6, -30e-6,
                                                       -0.2094, -0.2094, -0.2094, 0.99};
    params["topConstraintsMax"] = std::vector<double>{30e-6, 30e-6, 30e-6,
                                                       0.2094, 0.2094, 0.2094, 1.01};


    auto answer = swellingSchema(params);

    std::cout << "stretchZ " << answer["stretchZ"] <<
              std::endl;
    std::cout << "pearsonCorrelationBase " << answer["pearsonCorrelationBase"] <<
              std::endl;
    std::cout << "pearsonCorrelationTop " << answer["pearsonCorrelationTop"] <<
              std::endl;

    // demoSchema();

    return EXIT_SUCCESS;

}
