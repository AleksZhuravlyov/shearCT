#include <RegionCt.h>
#include <vector>

RegionCt::RegionCt() : start({0, 0, 0}),
                   width({1, 1, 1}),
                   dimArrays(std::vector<std::vector<float>>(3)),
                   val(std::vector<short>(1)) {

    for (int i = 0; i < width.size(); i++)
        dimArrays[i].resize(width[i]);

    val.resize(width[0] * width[1] * width[2]);
}

void RegionCt::initiateRegionCt(const std::vector<size_t> &_start,
                            const std::vector<size_t> &_width) {
    start = _start;

    if (width != _width) {
        width = _width;

        for (int i = 0; i < width.size(); i++)
            dimArrays[i].resize(width[i]);

        val.resize(width[0] * width[1] * width[2]);
    }
}
