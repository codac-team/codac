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
#include "codac2_py_Interval_operations_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
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

  .def("bwd_add", &codac2::bwd_add,
    VOID_BWD_ADD_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_sub", &codac2::bwd_sub,
    VOID_BWD_SUB_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_mul", &codac2::bwd_mul,
    VOID_BWD_MUL_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_div", &codac2::bwd_div,
    VOID_BWD_DIV_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_sqr", &codac2::bwd_sqr,
    VOID_BWD_SQR_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_sqrt", &codac2::bwd_sqrt,
    VOID_BWD_SQRT_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_pow", [](const Interval& y, Interval& x, int_type p) { return codac2::bwd_pow(y,x,p); },
    VOID_BWD_POW_CONST_INTERVAL_REF_INTERVAL_REF_INT,
    "y"_a, "x"_a, "n"_a)

  .def("bwd_pow", (void(*)(const Interval&,Interval&,Interval&)) &codac2::bwd_pow,
    VOID_BWD_POW_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_root", &codac2::bwd_root,
    VOID_BWD_ROOT_CONST_INTERVAL_REF_INTERVAL_REF_INT,
    "y"_a, "x"_a, "n"_a)

  .def("bwd_exp", &codac2::bwd_exp,
    VOID_BWD_EXP_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_log", &codac2::bwd_log,
    VOID_BWD_LOG_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_cos", &codac2::bwd_cos,
    VOID_BWD_COS_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_sin", &codac2::bwd_sin,
    VOID_BWD_SIN_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_tan", &codac2::bwd_tan,
    VOID_BWD_TAN_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_acos", &codac2::bwd_acos,
    VOID_BWD_ACOS_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_asin", &codac2::bwd_asin,
    VOID_BWD_ASIN_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_atan", &codac2::bwd_atan,
    VOID_BWD_ATAN_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_atan2", &codac2::bwd_atan2,
    VOID_BWD_ATAN2_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x1"_a, "x2"_a)
  
  .def("bwd_cosh", &codac2::bwd_cosh,
    VOID_BWD_COSH_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)
  
  .def("bwd_sinh", &codac2::bwd_sinh,
    VOID_BWD_SINH_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)
  
  .def("bwd_tanh", &codac2::bwd_tanh,
    VOID_BWD_TANH_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)
  
  .def("bwd_acosh", &codac2::bwd_acosh,
    VOID_BWD_ACOSH_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)
  
  .def("bwd_asinh", &codac2::bwd_asinh,
    VOID_BWD_ASINH_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)
  
  .def("bwd_atanh", &codac2::bwd_atanh,
    VOID_BWD_ATANH_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)
  
  .def("bwd_abs", &codac2::bwd_abs,
    VOID_BWD_ABS_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_min", &codac2::bwd_min,
    VOID_BWD_MIN_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_max", &codac2::bwd_max,
    VOID_BWD_MAX_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x1"_a, "x2"_a)

  .def("bwd_sign", &codac2::bwd_sign,
    VOID_BWD_SIGN_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_floor", &codac2::bwd_floor,
    VOID_BWD_FLOOR_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_ceil", &codac2::bwd_ceil,
    VOID_BWD_CEIL_CONST_INTERVAL_REF_INTERVAL_REF,
    "y"_a, "x"_a)

  .def("bwd_imod", &codac2::bwd_imod,
    VOID_BWD_IMOD_INTERVAL_REF_INTERVAL_REF_DOUBLE,
    "y"_a, "x"_a, "p"_a)

  ;
}