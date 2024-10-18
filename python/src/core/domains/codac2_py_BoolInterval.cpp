/** 
 *  Codac binding (core)
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Benoît Desrochers, Simon Rohou, Fabrice Le Bars
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_BoolInterval.h>
#include "codac2_py_BoolInterval_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_BoolInterval(py::module& m)
{
  py::enum_<BoolInterval>(m, "BoolInterval")

    .value("EMPTY", BoolInterval::EMPTY)
    .value("FALSE", BoolInterval::FALSE)
    .value("TRUE", BoolInterval::TRUE)
    .value("UNKNOWN", BoolInterval::UNKNOWN)

    .def(py::self & py::self,
      BOOLINTERVAL_OPERATORAND_BOOLINTERVAL_BOOLINTERVAL)

    .def(py::self | py::self,
      BOOLINTERVAL_OPERATOROR_BOOLINTERVAL_BOOLINTERVAL)

    .def("__repr__", [](const BoolInterval& x) {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_BOOLINTERVAL_REF)
  ;
}