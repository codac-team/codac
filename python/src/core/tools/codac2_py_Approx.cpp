/** 
 *  Approx Python binding
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
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
      APPROX_T_APPROX_CONST_T_REF,
      "x"_a)

    .def(py::init<const Interval&,double>(),
      APPROX_T_APPROX_CONST_T_REF_DOUBLE,
      "x"_a, "eps"_a)

    .def(Interval() == py::self,
      BOOL_OPERATOREQ_CONST_T_REF_CONST_APPROX_T_REF,
      "x"_a)

    .def(py::self == Interval(),
      BOOL_OPERATOREQ_CONST_APPROX_T_REF_CONST_T_REF,
      "x"_a)
    
    .def(Interval() != py::self,
      BOOL_OPERATORNEQ_CONST_T_REF_CONST_APPROX_T_REF,
      "x"_a)

    .def(py::self != Interval(),
      BOOL_OPERATORNEQ_CONST_APPROX_T_REF_CONST_T_REF,
      "x"_a)

    .def("__repr__", [](const Approx<Interval>& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_APPROX_T_REF)
  ;
};