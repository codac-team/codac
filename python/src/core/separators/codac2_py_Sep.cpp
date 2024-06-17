/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "codac2_py_core.h"
#include "codac2_py_Sep.h"
#include "codac2_py_Sep_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


py::class_<Sep,pySep> export_Sep(py::module& m)
{
  py::class_<BoxPair> exported_BoxPair(m, "BoxPair", BOXPAIR_MAIN);
  exported_BoxPair

    .def_readwrite("inner", &BoxPair::inner)
    .def_readwrite("outer", &BoxPair::outer)

    .def(py::init(
        [](const IntervalVector& inner, const IntervalVector& outer)
        {
          BoxPair bp { inner,outer };
          return std::make_unique<BoxPair>(bp);
        }),
      "inner"_a, "outer"_a)

    .def("__repr__", [](const BoxPair& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_BOXPAIR_REF)
  ;

  py::implicitly_convertible<py::list,BoxPair>();

  py::class_<Sep,pySep> py_sep(m, "Sep");
  py_sep

    .def(py::init<size_t>(),
      SEP_SEP_SIZET)

    .def("size", &Sep::size,
      SIZET_SEP_SIZE_CONST)
  ;

  return py_sep;
}