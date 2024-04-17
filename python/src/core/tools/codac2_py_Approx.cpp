/** 
 *  \file
 *  Approx Python binding
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <codac2_Interval.h>
#include <codac2_Approx.h>
#include "codac2_py_Approx_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_doc.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Approx(py::module& m)
{
  py::class_<Approx<Interval>> exported_class(m, "Approx", APPROX_MAIN);
  exported_class
  
    .def(py::init<const Interval&>(),
      APPROX_APPROX_CONSTT,
      "x"_a)

    .def(py::init<const Interval&,double>(),
      APPROX_APPROX_CONSTT_DOUBLE,
      "x"_a, "eps"_a)

    .def(Interval() == py::self,
      APPROX_BOOL_OPERATOREQ_CONSTT_CONSTAPPROXT,
      "x"_a)

    .def(py::self == Interval(),
      APPROX_BOOL_OPERATOREQ_CONSTAPPROXT_CONSTT,
      "x"_a)
    
    .def(Interval() != py::self,
      APPROX_BOOL_OPERATORNEQ_CONSTT_CONSTAPPROXT,
      "x"_a)

    .def(py::self != Interval(),
      APPROX_BOOL_OPERATORNEQ_CONSTAPPROXT_CONSTT,
      "x"_a)

    .def("__repr__", [](const Approx<Interval>& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        },
      APPROX_OSTREAM_OPERATOR_OSTREAM_CONSTAPPROXT)
  ;
};