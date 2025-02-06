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
#include <codac2_Interval.h>
#include <codac2_Interval_operations.h>
#include "codac2_py_Interval_operations_impl_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_Interval_impl_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_matlab.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Interval_operations(py::module& m, py::class_<Interval>& py_Interval)
{
  // Members functions

  py_Interval

  .def(py::self & py::self,
    INTERVAL_OPERATORAND_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a)

  // For MATLAB compatibility
  .def("inter", (Interval(*)(const Interval&,const Interval&)) &codac2::operator&,
    INTERVAL_OPERATORAND_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a)

  .def(py::self | py::self,
    INTERVAL_OPERATOROR_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a)
  
  // For MATLAB compatibility
  .def("union", (Interval(*)(const Interval&,const Interval&)) &codac2::operator|,
    INTERVAL_OPERATOROR_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a)

  .def(+ py::self,
    CONST_INTERVAL_REF_OPERATORPLUS_CONST_INTERVAL_REF)

  .def(py::self + double(),
    INTERVAL_OPERATORPLUS_CONST_INTERVAL_REF_DOUBLE,
    "x"_a)

  .def(double() + py::self,
    INTERVAL_OPERATORPLUS_DOUBLE_CONST_INTERVAL_REF,
    "x"_a)

  .def(py::self + py::self,
    INTERVAL_OPERATORPLUS_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a)

  .def(py::self - double(),
    INTERVAL_OPERATORMINUS_CONST_INTERVAL_REF_DOUBLE,
    "x"_a)

  .def(double() - py::self,
    INTERVAL_OPERATORMINUS_DOUBLE_CONST_INTERVAL_REF,
    "x"_a)

  .def(py::self - py::self,
    INTERVAL_OPERATORMINUS_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a)

  .def(py::self * double(),
    INTERVAL_OPERATORMUL_CONST_INTERVAL_REF_DOUBLE,
    "x"_a)

  .def(double() * py::self,
    INTERVAL_OPERATORMUL_DOUBLE_CONST_INTERVAL_REF,
    "x"_a)

  .def(py::self * py::self,
    INTERVAL_OPERATORMUL_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a)

  .def(py::self / double(),
    INTERVAL_OPERATORDIV_CONST_INTERVAL_REF_DOUBLE,
    "x"_a)

  .def(double() / py::self,
    INTERVAL_OPERATORDIV_DOUBLE_CONST_INTERVAL_REF,
    "x"_a)

  .def(py::self / py::self,
    INTERVAL_OPERATORDIV_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a);

  // External functions outside the Interval class
  
  m

  .def("sqr", &codac2::sqr,
    INTERVAL_SQR_CONST_INTERVAL_REF,
    "x"_a)

  .def("sqrt", &codac2::sqrt,
    INTERVAL_SQRT_CONST_INTERVAL_REF,
    "x"_a)

  .def("pow", (Interval(*)(const Interval&,int)) &codac2::pow,
    INTERVAL_POW_CONST_INTERVAL_REF_INT,
    "x"_a, "n"_a)

  .def("pow", (Interval(*)(const Interval&,double)) &codac2::pow,
    INTERVAL_POW_CONST_INTERVAL_REF_DOUBLE,
    "x"_a, "d"_a)

  .def("pow", (Interval(*)(const Interval&,const Interval&)) &codac2::pow,
    INTERVAL_POW_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a, "y"_a)

  .def("root", (Interval(*)(const Interval&,int)) &codac2::root,
    INTERVAL_ROOT_CONST_INTERVAL_REF_INT,
    "x"_a, "n"_a)

  .def("exp", &codac2::exp,
    INTERVAL_EXP_CONST_INTERVAL_REF,
    "x"_a)

  .def("log", &codac2::log,
    INTERVAL_LOG_CONST_INTERVAL_REF,
    "x"_a)

  .def("cos", &codac2::cos,
    INTERVAL_COS_CONST_INTERVAL_REF,
    "x"_a)

  .def("sin", &codac2::sin,
    INTERVAL_SIN_CONST_INTERVAL_REF,
    "x"_a)

  .def("tan", &codac2::tan,
    INTERVAL_TAN_CONST_INTERVAL_REF,
    "x"_a)

  .def("atan", &codac2::atan,
    INTERVAL_ACOS_CONST_INTERVAL_REF,
    "x"_a)

  .def("asin", &codac2::asin,
    INTERVAL_ASIN_CONST_INTERVAL_REF,
    "x"_a)

  .def("atan", &codac2::atan,
    INTERVAL_ATAN_CONST_INTERVAL_REF,
    "x"_a)

  .def("atan2", &codac2::atan2,
    INTERVAL_ATAN2_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a, "y"_a)

  .def("cosh", &codac2::cosh,
    INTERVAL_COSH_CONST_INTERVAL_REF,
    "x"_a)

  .def("asinh", &codac2::asinh,
    INTERVAL_SINH_CONST_INTERVAL_REF,
    "x"_a)

  .def("tanh", &codac2::tanh,
    INTERVAL_TANH_CONST_INTERVAL_REF,
    "x"_a)

  .def("acosh", &codac2::acosh,
    INTERVAL_ACOSH_CONST_INTERVAL_REF,
    "x"_a)

  .def("asinh", &codac2::asinh,
    INTERVAL_ASINH_CONST_INTERVAL_REF,
    "x"_a)

  .def("atanh", &codac2::atanh,
    INTERVAL_ATANH_CONST_INTERVAL_REF,
    "x"_a)

  .def("abs", &codac2::abs,
    INTERVAL_ABS_CONST_INTERVAL_REF,
    "x"_a)

  .def("min", &codac2::min,
    INTERVAL_MIN_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a, "y"_a)

  .def("max", &codac2::max,
    INTERVAL_MAX_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
    "x"_a, "y"_a)

  .def("sign", &codac2::sign,
    INTERVAL_SIGN_CONST_INTERVAL_REF,
    "x"_a)

  .def("integer", &codac2::integer,
    INTERVAL_INTEGER_CONST_INTERVAL_REF,
    "x"_a)

  .def("floor", &codac2::floor,
    INTERVAL_FLOOR_CONST_INTERVAL_REF,
    "x"_a)

  .def("ceil", &codac2::ceil,
    INTERVAL_CEIL_CONST_INTERVAL_REF,
    "x"_a)

  ;
}