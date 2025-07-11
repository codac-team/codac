#include <limits>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_math.h>
#include "codac2_py_math_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_doc.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_math(py::module& m)
{
  m.def("round_up", 
    &round_up, 
    VOID_ROUND_UP);

  m.def("round_down", 
    &round_down, 
    VOID_ROUND_DOWN);

  m.def("get_rounding_mode", 
    &get_rounding_mode, 
    STRING_GET_ROUNDING_MODE);
}