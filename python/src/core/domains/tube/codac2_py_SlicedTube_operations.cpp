/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <codac2_Interval.h>
#include <codac2_SlicedTube.h>
#include <codac2_SlicedTube_operations.h>
#include <codac2_TubeBase.h>
#include "codac2_py_matlab.h"
#include "codac2_py_SlicedTube_operations_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

namespace
{
  template<typename T>
  void bind_set_ops(py::class_<SlicedTube<T>,TubeBase>& pyclass)
  {
    if constexpr(!FOR_MATLAB)
    {
      pyclass
        .def(py::self & py::self,
          SLICEDTUBE_T_OPERATORINTER_CONST_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
          py::is_operator())
        .def(py::self &= py::self,
          SLICEDTUBE_T_REF_OPERATORINTEREQ_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
          py::is_operator())
        .def(py::self | py::self,
          SLICEDTUBE_T_OPERATORUNION_CONST_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
          py::is_operator())
        .def(py::self |= py::self,
          SLICEDTUBE_T_REF_OPERATORUNIONEQ_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
          py::is_operator());
    }
    else
    {
      pyclass
        .def("inter", (SlicedTube<T>(*)(const SlicedTube<T>&,const SlicedTube<T>&)) &codac2::operator&,
          SLICEDTUBE_T_OPERATORINTER_CONST_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
          py::is_operator())
        .def("self_inter", (SlicedTube<T>&(*)(SlicedTube<T>&,const SlicedTube<T>&)) &codac2::operator&=,
          SLICEDTUBE_T_REF_OPERATORINTEREQ_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
          py::return_value_policy::reference_internal,
          py::is_operator())
        .def("union", (SlicedTube<T>(*)(const SlicedTube<T>&,const SlicedTube<T>&)) &codac2::operator|,
          SLICEDTUBE_T_OPERATORUNION_CONST_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
          py::is_operator())
        .def("self_union", (SlicedTube<T>&(*)(SlicedTube<T>&,const SlicedTube<T>&)) &codac2::operator|=,
          SLICEDTUBE_T_REF_OPERATORUNIONEQ_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
          py::return_value_policy::reference_internal,
          py::is_operator());
    }
  }

  template<typename T>
  void bind_add_sub_ops(py::class_<SlicedTube<T>,TubeBase>& pyclass)
  {
    pyclass
      .def("__pos__",
        (const SlicedTube<T>&(*)(const SlicedTube<T>&)) &codac2::operator+,
        CONST_SLICEDTUBE_T_REF_OPERATORPLUS_CONST_SLICEDTUBE_T_REF,
        py::return_value_policy::reference_internal,
        py::is_operator())

      .def("__add__", (SlicedTube<T>(*)(const SlicedTube<T>&,const SlicedTube<T>&)) &codac2::operator+,
        SLICEDTUBE_T_OPERATORPLUS_CONST_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
        py::is_operator())

      .def("__add__", (SlicedTube<T>(*)(const SlicedTube<T>&,const T&)) &codac2::operator+,
        SLICEDTUBE_T_OPERATORPLUS_CONST_SLICEDTUBE_T_REF_CONST_Q_REF,
        py::is_operator())

      .def("__radd__",
        [](const SlicedTube<T>& x2, const T& x1) { return x1 + x2; },
        SLICEDTUBE_T_OPERATORPLUS_CONST_Q_REF_CONST_SLICEDTUBE_T_REF,
        py::is_operator())

      .def("__iadd__", (SlicedTube<T>&(*)(SlicedTube<T>&,const T&)) &codac2::operator+=,
        SLICEDTUBE_T_REF_OPERATORPLUSEQ_SLICEDTUBE_T_REF_CONST_Q_REF,
        py::return_value_policy::reference_internal,
        py::is_operator())

      .def("__iadd__", (SlicedTube<T>&(*)(SlicedTube<T>&,const SlicedTube<T>&)) &codac2::operator+=,
        SLICEDTUBE_T_REF_OPERATORPLUSEQ_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
        py::return_value_policy::reference_internal,
        py::is_operator())

      .def("__neg__", (SlicedTube<T>(*)(const SlicedTube<T>&)) &codac2::operator-,
        SLICEDTUBE_T_OPERATORMINUS_CONST_SLICEDTUBE_T_REF,
        py::is_operator())

      .def("__sub__", (SlicedTube<T>(*)(const SlicedTube<T>&,const SlicedTube<T>&)) &codac2::operator-,
        SLICEDTUBE_T_OPERATORMINUS_CONST_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
        py::is_operator())

      .def("__sub__", (SlicedTube<T>(*)(const SlicedTube<T>&,const T&)) &codac2::operator-,
        SLICEDTUBE_T_OPERATORMINUS_CONST_SLICEDTUBE_T_REF_CONST_Q_REF,
        py::is_operator())

      .def("__rsub__",
        [](const SlicedTube<T>& x2, const T& x1) { return x1 - x2; },
        SLICEDTUBE_T_OPERATORMINUS_CONST_Q_REF_CONST_SLICEDTUBE_T_REF,
        py::is_operator())

      .def("__isub__", (SlicedTube<T>&(*)(SlicedTube<T>&,const T&)) &codac2::operator-=,
        SLICEDTUBE_T_REF_OPERATORMINUSEQ_SLICEDTUBE_T_REF_CONST_Q_REF,
        py::return_value_policy::reference_internal,
        py::is_operator())

      .def("__isub__", (SlicedTube<T>&(*)(SlicedTube<T>&,const SlicedTube<T>&)) &codac2::operator-=,
        SLICEDTUBE_T_REF_OPERATORMINUSEQ_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
        py::return_value_policy::reference_internal,
        py::is_operator());
  }

  template<typename T>
  void bind_mul_ops(py::class_<SlicedTube<T>,TubeBase>& pyclass)
  {
    pyclass
      .def("__mul__", (SlicedTube<T>(*)(const SlicedTube<T>&,const Interval&)) &codac2::operator*,
        SLICEDTUBE_T_OPERATORMUL_CONST_SLICEDTUBE_T_REF_CONST_INTERVAL_REF,
        py::is_operator())

      .def("__rmul__",
        [](const SlicedTube<T>& x2, const T& x1) { return x1 * x2; },
        SLICEDTUBE_T_OPERATORMUL_CONST_INTERVAL_REF_CONST_SLICEDTUBE_T_REF,
        py::is_operator())

      .def("__imul__", (SlicedTube<T>&(*)(SlicedTube<T>&,const Interval&)) &codac2::operator*=,
        SLICEDTUBE_T_REF_OPERATORMULEQ_SLICEDTUBE_T_REF_CONST_Q_REF,
        py::return_value_policy::reference_internal,
        py::is_operator());

    if constexpr(std::is_same_v<T,Interval>)
    {
      pyclass
        .def("__mul__", (SlicedTube<Interval>(*)(const SlicedTube<Interval>&,const SlicedTube<Interval>&)) &codac2::operator*,
          SLICEDTUBE_INTERVAL_OPERATORMUL_CONST_SLICEDTUBE_INTERVAL_REF_CONST_SLICEDTUBE_INTERVAL_REF,
          py::is_operator())

        .def("__imul__", (SlicedTube<Interval>&(*)(SlicedTube<Interval>&,const SlicedTube<Interval>&)) &codac2::operator*=,
          SLICEDTUBE_T_REF_OPERATORMULEQ_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
          py::return_value_policy::reference_internal,
          py::is_operator());
    }
    else
    {
      pyclass
        .def("__mul__", (SlicedTube<T>(*)(const SlicedTube<T>&,const SlicedTube<T>&)) &codac2::operator*,
          std::is_same_v<T,IntervalVector>
            ? SLICEDTUBE_INTERVALVECTOR_OPERATORMUL_CONST_SLICEDTUBE_INTERVALVECTOR_REF_CONST_SLICEDTUBE_INTERVALVECTOR_REF
            : SLICEDTUBE_INTERVALMATRIX_OPERATORMUL_CONST_SLICEDTUBE_INTERVALMATRIX_REF_CONST_SLICEDTUBE_INTERVALMATRIX_REF,
          py::is_operator())

        .def("__mul__", (SlicedTube<T>(*)(const SlicedTube<T>&,const SlicedTube<Interval>&)) &codac2::operator*,
          SLICEDTUBE_T_OPERATORMUL_CONST_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_INTERVAL_REF,
          py::is_operator())

        .def("__rmul__",
          [](const SlicedTube<T>& x2, const SlicedTube<Interval>& x1) { return x1 * x2; },
          SLICEDTUBE_T_OPERATORMUL_CONST_SLICEDTUBE_INTERVAL_REF_CONST_SLICEDTUBE_T_REF,
          py::is_operator())

        .def("__mul__", (SlicedTube<T>(*)(const SlicedTube<T>&,const T&)) &codac2::operator*,
          SLICEDTUBE_T_OPERATORMUL_CONST_SLICEDTUBE_T_REF_CONST_Q_REF,
          py::is_operator())

        .def("__rmul__",
          [](const SlicedTube<T>& x2, const T& x1) { return x1 * x2; },
          SLICEDTUBE_T_OPERATORMUL_CONST_Q_REF_CONST_SLICEDTUBE_T_REF,
          py::is_operator())

        .def("__imul__", (SlicedTube<T>&(*)(SlicedTube<T>&,const T&)) &codac2::operator*=,
          SLICEDTUBE_T_REF_OPERATORMULEQ_SLICEDTUBE_T_REF_CONST_Q_REF,
          py::return_value_policy::reference_internal,
          py::is_operator())

        .def("__imul__", (SlicedTube<T>&(*)(SlicedTube<T>&,const SlicedTube<T>&)) &codac2::operator*=,
          SLICEDTUBE_T_REF_OPERATORMULEQ_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
          py::return_value_policy::reference_internal,
          py::is_operator())

        .def("__imul__", (SlicedTube<T>&(*)(SlicedTube<T>&,const SlicedTube<Interval>&)) &codac2::operator*=,
          SLICEDTUBE_T_REF_OPERATORMULEQ_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_INTERVAL_REF,
          py::return_value_policy::reference_internal,
          py::is_operator());
    }
  }

  template<typename T>
  void bind_div_ops(py::class_<SlicedTube<T>,TubeBase>& pyclass)
  {
    pyclass
      .def("__truediv__", (SlicedTube<T>(*)(const SlicedTube<T>&,const Interval&)) &codac2::operator/,
        SLICEDTUBE_T_OPERATORDIV_CONST_SLICEDTUBE_T_REF_CONST_INTERVAL_REF,
        py::is_operator())

      .def("__itruediv__", (SlicedTube<T>&(*)(SlicedTube<T>&,const Interval&)) &codac2::operator/=,
        SLICEDTUBE_T_REF_OPERATORDIVEQ_SLICEDTUBE_T_REF_CONST_Q_REF,
        py::return_value_policy::reference_internal,
        py::is_operator());

    if constexpr(std::is_same_v<T,Interval>)
    {
      pyclass
        .def("__truediv__", (SlicedTube<Interval>(*)(const SlicedTube<Interval>&,const SlicedTube<Interval>&)) &codac2::operator/,
          SLICEDTUBE_INTERVAL_OPERATORDIV_CONST_SLICEDTUBE_INTERVAL_REF_CONST_SLICEDTUBE_INTERVAL_REF,
          py::is_operator())

        .def("__rtruediv__",
          [](const SlicedTube<Interval>& x2, const Interval& x1) { return x1 / x2; },
          SLICEDTUBE_T_OPERATORDIV_CONST_Q_REF_CONST_SLICEDTUBE_T_REF,
          py::is_operator())

        .def("__itruediv__", (SlicedTube<Interval>&(*)(SlicedTube<Interval>&,const SlicedTube<Interval>&)) &codac2::operator/=,
          SLICEDTUBE_T_REF_OPERATORDIVEQ_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF,
          py::return_value_policy::reference_internal,
          py::is_operator());
    }
    else
    {
      pyclass
        .def("__truediv__", (SlicedTube<T>(*)(const SlicedTube<T>&,const SlicedTube<Interval>&)) &codac2::operator/,
          SLICEDTUBE_T_OPERATORDIV_CONST_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_INTERVAL_REF,
          py::is_operator())

        .def("__itruediv__", (SlicedTube<T>&(*)(SlicedTube<T>&,const SlicedTube<Interval>&)) &codac2::operator/=,
          SLICEDTUBE_T_REF_OPERATORDIVEQ_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_INTERVAL_REF,
          py::return_value_policy::reference_internal,
          py::is_operator());
    }
  }

  void bind_scalar_slicedtube_functions(py::module& m)
  {
    m
      .def("sqr", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::sqr,
        SLICEDTUBE_INTERVAL_SQR_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("sqrt", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::sqrt,
        SLICEDTUBE_INTERVAL_SQRT_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("pow", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&,int)) &codac2::pow,
        SLICEDTUBE_INTERVAL_POW_CONST_SLICEDTUBE_INTERVAL_REF_INT,
        "x1"_a, "x2"_a, py::is_operator())

      .def("pow", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&,const Interval&)) &codac2::pow,
        SLICEDTUBE_INTERVAL_POW_CONST_SLICEDTUBE_INTERVAL_REF_CONST_INTERVAL_REF,
        "x1"_a, "x2"_a, py::is_operator())

      .def("exp", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::exp,
        SLICEDTUBE_INTERVAL_EXP_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("log", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::log,
        SLICEDTUBE_INTERVAL_LOG_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("cos", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::cos,
        SLICEDTUBE_INTERVAL_COS_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("sin", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::sin,
        SLICEDTUBE_INTERVAL_SIN_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("tan", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::tan,
        SLICEDTUBE_INTERVAL_TAN_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("acos", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::acos,
        SLICEDTUBE_INTERVAL_ACOS_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("asin", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::asin,
        SLICEDTUBE_INTERVAL_ASIN_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("atan", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::atan,
        SLICEDTUBE_INTERVAL_ATAN_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("atan2", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&,const SlicedTube<Interval>&)) &codac2::atan2,
        SLICEDTUBE_INTERVAL_ATAN2_CONST_SLICEDTUBE_INTERVAL_REF_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, "x2"_a, py::is_operator())

      .def("atan2", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&,const Interval&)) &codac2::atan2,
        SLICEDTUBE_INTERVAL_ATAN2_CONST_SLICEDTUBE_INTERVAL_REF_CONST_INTERVAL_REF,
        "x1"_a, "x2"_a, py::is_operator())

      .def("atan2", (SlicedTube<Interval> (*)(const Interval&,const SlicedTube<Interval>&)) &codac2::atan2,
        SLICEDTUBE_INTERVAL_ATAN2_CONST_INTERVAL_REF_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, "x2"_a, py::is_operator())

      .def("cosh", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::cosh,
        SLICEDTUBE_INTERVAL_COSH_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("sinh", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::sinh,
        SLICEDTUBE_INTERVAL_SINH_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("tanh", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::tanh,
        SLICEDTUBE_INTERVAL_TANH_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("acosh", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::acosh,
        SLICEDTUBE_INTERVAL_ACOSH_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("asinh", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::asinh,
        SLICEDTUBE_INTERVAL_ASINH_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("atanh", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::atanh,
        SLICEDTUBE_INTERVAL_ATANH_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("abs", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::abs,
        SLICEDTUBE_INTERVAL_ABS_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("min", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&,const SlicedTube<Interval>&)) &codac2::min,
        SLICEDTUBE_INTERVAL_MIN_CONST_SLICEDTUBE_INTERVAL_REF_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, "x2"_a, py::is_operator())

      .def("min", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&,const Interval&)) &codac2::min,
        SLICEDTUBE_INTERVAL_MIN_CONST_SLICEDTUBE_INTERVAL_REF_CONST_INTERVAL_REF,
        "x1"_a, "x2"_a, py::is_operator())

      .def("min", (SlicedTube<Interval> (*)(const Interval&,const SlicedTube<Interval>&)) &codac2::min,
        SLICEDTUBE_INTERVAL_MIN_CONST_INTERVAL_REF_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, "x2"_a, py::is_operator())

      .def("max", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&,const SlicedTube<Interval>&)) &codac2::max,
        SLICEDTUBE_INTERVAL_MAX_CONST_SLICEDTUBE_INTERVAL_REF_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, "x2"_a, py::is_operator())

      .def("max", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&,const Interval&)) &codac2::max,
        SLICEDTUBE_INTERVAL_MAX_CONST_SLICEDTUBE_INTERVAL_REF_CONST_INTERVAL_REF,
        "x1"_a, "x2"_a, py::is_operator())

      .def("max", (SlicedTube<Interval> (*)(const Interval&,const SlicedTube<Interval>&)) &codac2::max,
        SLICEDTUBE_INTERVAL_MAX_CONST_INTERVAL_REF_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, "x2"_a, py::is_operator())

      .def("sign", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::sign,
        SLICEDTUBE_INTERVAL_SIGN_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("integer", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::integer,
        SLICEDTUBE_INTERVAL_INTEGER_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("floor", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::floor,
        SLICEDTUBE_INTERVAL_FLOOR_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator())

      .def("ceil", (SlicedTube<Interval> (*)(const SlicedTube<Interval>&)) &codac2::ceil,
        SLICEDTUBE_INTERVAL_CEIL_CONST_SLICEDTUBE_INTERVAL_REF,
        "x1"_a, py::is_operator());
  }
}

void export_SlicedTube_operations(
  py::module& m,
  py::class_<SlicedTube<Interval>,TubeBase>& py_SlicedTube_Interval,
  py::class_<SlicedTube<IntervalVector>,TubeBase>& py_SlicedTube_IntervalVector,
  py::class_<SlicedTube<IntervalMatrix>,TubeBase>& py_SlicedTube_IntervalMatrix)
{
  bind_set_ops(py_SlicedTube_Interval);
  bind_set_ops(py_SlicedTube_IntervalVector);
  bind_set_ops(py_SlicedTube_IntervalMatrix);

  bind_add_sub_ops(py_SlicedTube_Interval);
  bind_add_sub_ops(py_SlicedTube_IntervalVector);
  bind_add_sub_ops(py_SlicedTube_IntervalMatrix);

  bind_mul_ops(py_SlicedTube_Interval);
  bind_mul_ops(py_SlicedTube_IntervalVector);
  bind_mul_ops(py_SlicedTube_IntervalMatrix);

  bind_div_ops(py_SlicedTube_Interval);
  bind_div_ops(py_SlicedTube_IntervalVector);
  bind_div_ops(py_SlicedTube_IntervalMatrix);

  py_SlicedTube_IntervalMatrix
    .def("__mul__", (SlicedTube<IntervalVector>(*)(const SlicedTube<IntervalMatrix>&,const SlicedTube<IntervalVector>&)) &codac2::operator*,
      SLICEDTUBE_INTERVALVECTOR_OPERATORMUL_CONST_SLICEDTUBE_INTERVALMATRIX_REF_CONST_SLICEDTUBE_INTERVALVECTOR_REF,
      py::is_operator());

  bind_scalar_slicedtube_functions(m);
}
