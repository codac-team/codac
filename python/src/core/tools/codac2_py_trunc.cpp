/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <streambuf>
#include <istream>
#include <codac2_trunc.h>
#include "codac2_py_trunc_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_cast.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_trunc(py::module& m)
{
  m

    .def("trunc", (double (*)(double))&codac2::trunc,
      DOUBLE_TRUNC_DOUBLE,
      "x"_a)

    .def("trunc", (Interval (*)(const Interval&))&codac2::trunc,
      INTERVAL_TRUNC_CONST_INTERVAL_REF,
      "x"_a)

    .def("untrunc", (double (*)(double))&codac2::untrunc,
      DOUBLE_UNTRUNC_DOUBLE,
      "x"_a)

    .def("untrunc", (Interval (*)(const Interval&))&codac2::untrunc,
      INTERVAL_UNTRUNC_CONST_INTERVAL_REF,
      "x"_a)

    .def("trunc", [](const IntervalVector& x) { return codac2::trunc(x); },
      AUTO_TRUNC_CONST_EIGEN_MATRIXBASE_OTHERDERIVED_REF,
      "x"_a)

    .def("untrunc", [](const IntervalVector& x) { return codac2::untrunc(x); },
      AUTO_UNTRUNC_CONST_EIGEN_MATRIXBASE_OTHERDERIVED_REF,
      "x"_a)

    .def("trunc", [](const IntervalMatrix& x) { return codac2::trunc(x); },
      AUTO_TRUNC_CONST_EIGEN_MATRIXBASE_OTHERDERIVED_REF,
      "x"_a)

    .def("untrunc", [](const IntervalMatrix& x) { return codac2::untrunc(x); },
      AUTO_UNTRUNC_CONST_EIGEN_MATRIXBASE_OTHERDERIVED_REF,
      "x"_a)
  ;
}