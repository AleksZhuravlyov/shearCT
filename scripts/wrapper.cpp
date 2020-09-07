#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <procedures/swellingSchema.h>

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(scripts, m) {
      m.def("swelling_schema", &swellingSchema, "params"_a);

}