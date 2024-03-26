/** 
 *  \file
 *  Interval Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Benoît Desrochers, Simon Rohou, Fabrice Le Bars
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Interval.h>
#include <codac2_Interval_operations.h>
#include "codac2_py_namespace_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Interval_operations(py::module& m, py::class_<Interval>& py_Interval)
{
  // Members functions

  py_Interval

  .def(py::self & py::self,
    _INTERVAL_OPERATORINT_CONSTINTERVAL_CONSTINTERVAL,
    "x"_a)

  // For MATLAB compatibility
  .def("inter", (Interval(*)(const Interval&,const Interval&)) &codac2::operator&,
    _INTERVAL_OPERATORINT_CONSTINTERVAL_CONSTINTERVAL,
    "x"_a)

  .def(py::self | py::self,
    _INTERVAL_OPERATORUNI_CONSTINTERVAL_CONSTINTERVAL,
    "x"_a)
  
  // For MATLAB compatibility
  .def("union", (Interval(*)(const Interval&,const Interval&)) &codac2::operator|,
    _INTERVAL_OPERATORUNI_CONSTINTERVAL_CONSTINTERVAL,
    "x"_a)

  .def(+ py::self,
    _CONSTINTERVAL_OPERATORADD_CONSTINTERVAL)

  .def(py::self + double(),
    _INTERVAL_OPERATORADD_CONSTINTERVAL_DOUBLE,
    "x"_a)

  .def(double() + py::self,
    _INTERVAL_OPERATORADD_DOUBLE_CONSTINTERVAL,
    "x"_a)

  .def(py::self + py::self,
    _INTERVAL_OPERATORADD_CONSTINTERVAL_CONSTINTERVAL,
    "x"_a)

  .def(py::self - double(),
    _INTERVAL_OPERATORMIN_CONSTINTERVAL_DOUBLE,
    "x"_a)

  .def(double() - py::self,
    _INTERVAL_OPERATORMIN_DOUBLE_CONSTINTERVAL,
    "x"_a)

  .def(py::self - py::self,
    _INTERVAL_OPERATORMIN_CONSTINTERVAL_CONSTINTERVAL,
    "x"_a)

  .def(py::self * double(),
    _INTERVAL_OPERATORMUL_CONSTINTERVAL_DOUBLE,
    "x"_a)

  .def(double() * py::self,
    _INTERVAL_OPERATORMUL_DOUBLE_CONSTINTERVAL,
    "x"_a)

  .def(py::self * py::self,
    _INTERVAL_OPERATORMUL_CONSTINTERVAL_CONSTINTERVAL,
    "x"_a)

  .def(py::self / double(),
    _INTERVAL_OPERATORDIV_CONSTINTERVAL_DOUBLE,
    "x"_a)

  .def(double() / py::self,
    _INTERVAL_OPERATORDIV_DOUBLE_CONSTINTERVAL,
    "x"_a)

  .def(py::self / py::self,
    _INTERVAL_OPERATORDIV_CONSTINTERVAL_CONSTINTERVAL,
    "x"_a);

  // External functions outside the Interval class
  
  m

  .def("sqr", &codac2::sqr,
    _INTERVAL_SQR_CONSTINTERVAL,
    "x"_a)

  .def("sqrt", &codac2::sqrt,
    _INTERVAL_SQRT_CONSTINTERVAL,
    "x"_a)

  .def("pow", (Interval(*)(const Interval&,int)) &codac2::pow,
    _INTERVAL_POW_CONSTINTERVAL_INT,
    "x"_a, "n"_a)

  .def("pow", (Interval(*)(const Interval&,double)) &codac2::pow,
    _INTERVAL_POW_CONSTINTERVAL_DOUBLE,
    "x"_a, "d"_a)

  .def("pow", (Interval(*)(const Interval&,const Interval&)) &codac2::pow,
    _INTERVAL_POW_CONSTINTERVAL_CONSTINTERVAL,
    "x"_a, "y"_a)

  .def("root", (Interval(*)(const Interval&,int)) &codac2::root,
    _INTERVAL_ROOT_CONSTINTERVAL_INT,
    "x"_a, "n"_a)

  .def("exp", &codac2::exp,
    _INTERVAL_EXP_CONSTINTERVAL,
    "x"_a)

  .def("log", &codac2::log,
    _INTERVAL_LOG_CONSTINTERVAL,
    "x"_a)

  .def("cos", &codac2::cos,
    _INTERVAL_COS_CONSTINTERVAL,
    "x"_a)

  .def("sin", &codac2::sin,
    _INTERVAL_SIN_CONSTINTERVAL,
    "x"_a)

  .def("tan", &codac2::tan,
    _INTERVAL_TAN_CONSTINTERVAL,
    "x"_a)

  .def("atan", &codac2::atan,
    _INTERVAL_ACOS_CONSTINTERVAL,
    "x"_a)

  .def("asin", &codac2::asin,
    _INTERVAL_ASIN_CONSTINTERVAL,
    "x"_a)

  .def("atan", &codac2::atan,
    _INTERVAL_ATAN_CONSTINTERVAL,
    "x"_a)

  .def("atan2", &codac2::atan2,
    _INTERVAL_ATAN2_CONSTINTERVAL_CONSTINTERVAL,
    "x"_a, "y"_a)

  .def("cosh", &codac2::cosh,
    _INTERVAL_COSH_CONSTINTERVAL,
    "x"_a)

  .def("asinh", &codac2::asinh,
    _INTERVAL_SINH_CONSTINTERVAL,
    "x"_a)

  .def("tanh", &codac2::tanh,
    _INTERVAL_TANH_CONSTINTERVAL,
    "x"_a)

  .def("acosh", &codac2::acosh,
    _INTERVAL_ACOSH_CONSTINTERVAL,
    "x"_a)

  .def("asinh", &codac2::asinh,
    _INTERVAL_ASINH_CONSTINTERVAL,
    "x"_a)

  .def("atanh", &codac2::atanh,
    _INTERVAL_ATANH_CONSTINTERVAL,
    "x"_a)

  .def("abs", &codac2::abs,
    _INTERVAL_ABS_CONSTINTERVAL,
    "x"_a)

  .def("min", &codac2::min,
    _INTERVAL_MIN_CONSTINTERVAL_CONSTINTERVAL,
    "x"_a, "y"_a)

  .def("max", &codac2::max,
    _INTERVAL_MAX_CONSTINTERVAL_CONSTINTERVAL,
    "x"_a, "y"_a)

  .def("sign", &codac2::sign,
    _INTERVAL_SIGN_CONSTINTERVAL,
    "x"_a)

  .def("integer", &codac2::integer,
    _INTERVAL_INTEGER_CONSTINTERVAL,
    "x"_a)

  .def("floor", &codac2::floor,
    _INTERVAL_FLOOR_CONSTINTERVAL,
    "x"_a)

  .def("ceil", &codac2::ceil,
    _INTERVAL_CEIL_CONSTINTERVAL,
    "x"_a)

  .def("bwd_add", &codac2::bwd_add,
    _VOID_BWD_ADD_CONSTINTERVAL_INTERVAL_INTERVAL,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_sub", &codac2::bwd_sub,
    _VOID_BWD_SUB_CONSTINTERVAL_INTERVAL_INTERVAL,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_mul", &codac2::bwd_mul,
    _VOID_BWD_MUL_CONSTINTERVAL_INTERVAL_INTERVAL,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_div", &codac2::bwd_div,
    _VOID_BWD_DIV_CONSTINTERVAL_INTERVAL_INTERVAL,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_sqr", &codac2::bwd_sqr,
    _VOID_BWD_SQR_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_sqrt", &codac2::bwd_sqrt,
    _VOID_BWD_SQRT_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_pow", (void(*)(const Interval&,Interval&,int)) &codac2::bwd_pow,
    _VOID_BWD_POW_CONSTINTERVAL_INTERVAL_INT,
    "y"_a, "x"_a, "n"_a)

  .def("bwd_pow", (void(*)(const Interval&,Interval&,Interval&)) &codac2::bwd_pow,
    _VOID_BWD_POW_CONSTINTERVAL_INTERVAL_INTERVAL,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_root", &codac2::bwd_root,
    _VOID_BWD_ROOT_CONSTINTERVAL_INTERVAL_INT,
    "y"_a, "x"_a, "n"_a)

  .def("bwd_exp", &codac2::bwd_exp,
    _VOID_BWD_EXP_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_log", &codac2::bwd_log,
    _VOID_BWD_LOG_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_cos", &codac2::bwd_cos,
    _VOID_BWD_COS_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_sin", &codac2::bwd_sin,
    _VOID_BWD_SIN_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_tan", &codac2::bwd_tan,
    _VOID_BWD_TAN_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_acos", &codac2::bwd_acos,
    _VOID_BWD_ACOS_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_asin", &codac2::bwd_asin,
    _VOID_BWD_ASIN_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_atan", &codac2::bwd_atan,
    _VOID_BWD_ATAN_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_atan2", &codac2::bwd_atan2,
    _VOID_BWD_ATAN2_CONSTINTERVAL_INTERVAL_INTERVAL,
    "y"_a, "x1"_a, "x2"_a)
  
  .def("bwd_cosh", &codac2::bwd_cosh,
    _VOID_BWD_COSH_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)
  
  .def("bwd_sinh", &codac2::bwd_sinh,
    _VOID_BWD_SINH_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)
  
  .def("bwd_tanh", &codac2::bwd_tanh,
    _VOID_BWD_TANH_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)
  
  .def("bwd_acosh", &codac2::bwd_acosh,
    _VOID_BWD_ACOSH_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)
  
  .def("bwd_asinh", &codac2::bwd_asinh,
    _VOID_BWD_ASINH_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)
  
  .def("bwd_atanh", &codac2::bwd_atanh,
    _VOID_BWD_ATANH_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)
  
  .def("bwd_abs", &codac2::bwd_abs,
    _VOID_BWD_ABS_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_min", &codac2::bwd_min,
    _VOID_BWD_MIN_CONSTINTERVAL_INTERVAL_INTERVAL,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_max", &codac2::bwd_max,
    _VOID_BWD_MAX_CONSTINTERVAL_INTERVAL_INTERVAL,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_sign", &codac2::bwd_sign,
    _VOID_BWD_SIGN_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_floor", &codac2::bwd_floor,
    _VOID_BWD_FLOOR_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_ceil", &codac2::bwd_ceil,
    _VOID_BWD_CEIL_CONSTINTERVAL_INTERVAL,
    "y"_a, "x"_a)

  .def("bwd_imod", &codac2::bwd_imod,
    _VOID_BWD_IMOD_INTERVAL_INTERVAL_DOUBLE,
    "y"_a, "x"_a, "p"_a)

  ;
};