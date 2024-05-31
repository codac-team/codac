/** 
 *  Interval Python binding
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
#include "codac2_py_core.h"
#include <codac2_IntervalVector.h>

#include "codac2_py_doc.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_IntervalVector_operations(py::module& m, py::class_<IntervalVector>& py_IntervalVector)
{
  // Members functions

  py_IntervalVector

  .def(py::self & py::self,
    DOC_TO_BE_DEFINED,
    //INTERVAL_OPERATORAND_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a)

  // For MATLAB compatibility
  .def("inter", [](const IntervalVector& s, const IntervalVector& y) { return s&y; },
    DOC_TO_BE_DEFINED,
    //INTERVAL_OPERATORAND_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a)

  .def(py::self | py::self,
    DOC_TO_BE_DEFINED,
    //INTERVAL_OPERATOROR_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a)
  
  // For MATLAB compatibility
  .def("union", [](const IntervalVector& s, const IntervalVector& y) { return s|y; },
    DOC_TO_BE_DEFINED,
    //INTERVAL_OPERATOROR_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a)

  .def(py::self + py::self,
    DOC_TO_BE_DEFINED,
    //INTERVAL_OPERATORPLUS_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a)

  //.def(- py::self,
  //  DOC_TO_BE_DEFINED,
  //  //CONST_INTERVAL_REF_OPERATORPLUS_CONST_INTERVAL_REF)
  //  "x"_a)

  .def(py::self - py::self,
    DOC_TO_BE_DEFINED,
    //INTERVAL_OPERATORMINUS_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a)

  //.def(double() * py::self, [](double a, const IntervalVector& v) { return v; },
  //  DOC_TO_BE_DEFINED,
  //  //INTERVAL_OPERATORMUL_DOUBLE_CONST_INTERVAL_REF,
  //  "x"_a)

  ;
}