/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_TimePropag.h>
#include "codac2_py_TimePropag_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_TimePropag(py::module& m)
{
  py::enum_<TimePropag>(m, "TimePropag")

    .value("FWD", TimePropag::FWD)
    .value("BWD", TimePropag::BWD)
    .value("FWD_BWD", TimePropag::FWD_BWD)

    .def(py::self & py::self,
      TIMEPROPAG_OPERATORINTER_TIMEPROPAG_TIMEPROPAG)

    .def(py::self | py::self,
      TIMEPROPAG_OPERATORUNION_TIMEPROPAG_TIMEPROPAG)

    .def("__repr__", [](const TimePropag& x) {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_TIMEPROPAG_REF)
  ;
}