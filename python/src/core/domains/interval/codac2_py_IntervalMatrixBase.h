/** 
 *  IntervalMatrixBase binding
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "codac2_py_MatrixBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename S,typename V,bool VECTOR_INHERITANCE>
void export_IntervalMatrixBase(py::module& m, py::class_<S>& pyclass)
{
  export_MatrixBase<S,Interval,VECTOR_INHERITANCE>(m, pyclass);

  pyclass

    .def("volume", &S::volume,
      DOUBLE_INTERVALMATRIXBASE_SV_VOLUME_CONST)

    .def("is_empty", &S::is_empty,
      BOOL_INTERVALMATRIXBASE_SV_IS_EMPTY_CONST)

    .def("set_empty", &S::set_empty,
      VOID_INTERVALMATRIXBASE_SV_SET_EMPTY)

    .def("lb", &S::lb,
      V_INTERVALMATRIXBASE_SV_LB_CONST)

    .def("ub", &S::ub,
      V_INTERVALMATRIXBASE_SV_UB_CONST)

    .def("mid", &S::mid,
      V_INTERVALMATRIXBASE_SV_MID_CONST)

    .def("rand", &S::rand,
      V_INTERVALMATRIXBASE_SV_RAND_CONST)

    .def("rad", &S::rad,
      V_INTERVALMATRIXBASE_SV_RAD_CONST)

    .def("diam", &S::diam,
      V_INTERVALMATRIXBASE_SV_DIAM_CONST)

    .def("min_diam", &S::min_diam,
      DOUBLE_INTERVALMATRIXBASE_SV_MIN_DIAM_CONST)

    .def("max_diam", &S::max_diam,
      DOUBLE_INTERVALMATRIXBASE_SV_MAX_DIAM_CONST)

    .def("min_diam_index", [](const S& x)
        {
          return matlab::output_index(x.min_diam_index());
        },
      SIZET_INTERVALMATRIXBASE_SV_MIN_DIAM_INDEX_CONST)

    .def("max_diam_index", [](const S& x)
        {
          return matlab::output_index(x.max_diam_index());
        },
      SIZET_INTERVALMATRIXBASE_SV_MAX_DIAM_INDEX_CONST)

    .def("extr_diam_index", [](const S& x, bool min)
        {
          return matlab::output_index(x.extr_diam_index(min));
        },
      SIZET_INTERVALMATRIXBASE_SV_EXTR_DIAM_INDEX_BOOL_CONST,
      "min"_a)

    .def("__contains__", &S::contains,
      BOOL_INTERVALMATRIXBASE_SV_CONTAINS_CONST_V_REF_CONST)

    .def("contains", &S::contains,
      BOOL_INTERVALMATRIXBASE_SV_CONTAINS_CONST_V_REF_CONST)

    .def("interior_contains", &S::interior_contains,
      BOOL_INTERVALMATRIXBASE_SV_INTERIOR_CONTAINS_CONST_V_REF_CONST)

    .def("is_unbounded", &S::is_unbounded,
      BOOL_INTERVALMATRIXBASE_SV_IS_UNBOUNDED_CONST)

    .def("is_degenerated", &S::is_degenerated,
      BOOL_INTERVALMATRIXBASE_SV_IS_DEGENERATED_CONST)

    .def("is_flat", &S::is_flat,
      BOOL_INTERVALMATRIXBASE_SV_IS_FLAT_CONST)

    .def("intersects", &S::intersects,
      BOOL_INTERVALMATRIXBASE_SV_INTERSECTS_CONST_S_REF_CONST)

    .def("is_disjoint", &S::is_disjoint,
      BOOL_INTERVALMATRIXBASE_SV_IS_DISJOINT_CONST_S_REF_CONST)

    .def("overlaps", &S::overlaps,
      BOOL_INTERVALMATRIXBASE_SV_OVERLAPS_CONST_S_REF_CONST)

    .def("is_subset", &S::is_subset,
      BOOL_INTERVALMATRIXBASE_SV_IS_SUBSET_CONST_S_REF_CONST)

    .def("is_strict_subset", &S::is_strict_subset,
      BOOL_INTERVALMATRIXBASE_SV_IS_STRICT_SUBSET_CONST_S_REF_CONST)

    .def("is_interior_subset", &S::is_interior_subset,
      BOOL_INTERVALMATRIXBASE_SV_IS_INTERIOR_SUBSET_CONST_S_REF_CONST)

    .def("is_strict_interior_subset", &S::is_strict_interior_subset,
      BOOL_INTERVALMATRIXBASE_SV_IS_STRICT_INTERIOR_SUBSET_CONST_S_REF_CONST)

    .def("is_superset", &S::is_superset,
      BOOL_INTERVALMATRIXBASE_SV_IS_SUPERSET_CONST_S_REF_CONST)

    .def("is_strict_superset", &S::is_strict_superset,
      BOOL_INTERVALMATRIXBASE_SV_IS_STRICT_SUPERSET_CONST_S_REF_CONST)

    .def("is_bisectable", &S::is_bisectable,
      BOOL_INTERVALMATRIXBASE_SV_IS_BISECTABLE_CONST)

    .def("inflate", (S&(S::*)(double))&S::inflate,
      S_REF_INTERVALMATRIXBASE_SV_INFLATE_DOUBLE,
      "r"_a)

    .def("inflate", (S&(S::*)(const V&))&S::inflate,
      S_REF_INTERVALMATRIXBASE_SV_INFLATE_CONST_V_REF,
      "r"_a)

    .def("bisect", [](const S& x, size_t_type i, double ratio)
        {
          matlab::test_integer(i);
          return x.bisect(matlab::input_index(i),ratio);
        },
      PAIR_SS_INTERVALMATRIXBASE_SV_BISECT_SIZET_FLOAT_CONST,
      "i"_a, "ratio"_a = 0.49)

    .def("bisect_largest", &S::bisect_largest,
      PAIR_SS_INTERVALMATRIXBASE_SV_BISECT_LARGEST_FLOAT_CONST,
      "ratio"_a = 0.49)

    .def(py::self &= py::self,
      S_REF_INTERVALMATRIXBASE_SV_OPERATORANDEQ_CONST_S_REF
      "x"_a)

    .def(py::self |= py::self,
      S_REF_INTERVALMATRIXBASE_SV_OPERATOROREQ_CONST_S_REF
      "x"_a)

    .def(py::self & py::self,
      S_INTERVALMATRIXBASE_SV_OPERATORAND_CONST_S_REF_CONST
      "x"_a)

    .def(py::self | py::self,
      S_INTERVALMATRIXBASE_SV_OPERATOROR_CONST_S_REF_CONST,
      "x"_a)
  ;

  py::implicitly_convertible<V,S>();
}