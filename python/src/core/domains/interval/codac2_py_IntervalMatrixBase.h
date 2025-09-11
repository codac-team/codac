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

    .def("init", [](S& x) { return x.init(); },
      MATRIX_ADDONS_INTERVALMATRIXBASE_AUTO_REF_INIT)

    .def("volume", [](const S& x) { return x.volume(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_DOUBLE_VOLUME_CONST)

    .def("is_empty", [](const S& x) { return x.is_empty(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_IS_EMPTY_CONST)

    .def("set_empty", [](S& x) { x.set_empty(); },
      MATRIX_ADDONS_INTERVALMATRIXBASE_VOID_SET_EMPTY)

    .def("lb", [](const S& x) { return x.lb(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_AUTO_LB_CONST)

    .def("ub", [](const S& x) { return x.ub(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_AUTO_UB_CONST)

    .def("mid", [](const S& x) { return x.mid(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_AUTO_MID_CONST)

    .def("mag", [](const S& x) { return x.mag(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_AUTO_MAG_CONST)

    .def("mig", [](const S& x) { return x.mig(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_AUTO_MIG_CONST)

    .def("rand", [](const S& x) { return x.rand(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_AUTO_RAND_CONST)

    .def("rad", [](const S& x) { return x.rad(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_AUTO_RAD_CONST)

    .def("diam", [](const S& x) { return x.diam(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_AUTO_DIAM_CONST)

    .def("min_rad", [](const S& x) { return x.min_rad(); },
      MATRIX_ADDONS_INTERVALMATRIXBASE_DOUBLE_MIN_RAD_CONST)

    .def("max_rad", [](const S& x) { return x.max_rad(); },
      MATRIX_ADDONS_INTERVALMATRIXBASE_DOUBLE_MAX_RAD_CONST)

    .def("min_diam", [](const S& x) { return x.min_diam(); },
      MATRIX_ADDONS_INTERVALMATRIXBASE_DOUBLE_MIN_DIAM_CONST)

    .def("max_diam", [](const S& x) { return x.max_diam(); },
      MATRIX_ADDONS_INTERVALMATRIXBASE_DOUBLE_MAX_DIAM_CONST)

    .def("min_diam_index", [](const S& x)
        {
          return matlab::output_index(x.min_diam_index());
        },
      MATRIX_ADDONS_INTERVALMATRIXBASE_INDEX_MIN_DIAM_INDEX_CONST)

    .def("max_diam_index", [](const S& x)
        {
          return matlab::output_index(x.max_diam_index());
        },
      MATRIX_ADDONS_INTERVALMATRIXBASE_INDEX_MAX_DIAM_INDEX_CONST)

    .def("extr_diam_index", [](const S& x, bool min)
        {
          return matlab::output_index(x.extr_diam_index(min));
        },
      MATRIX_ADDONS_INTERVALMATRIXBASE_INDEX_EXTR_DIAM_INDEX_BOOL_CONST,
      "min"_a)

    .def("__contains__", [](const S& x, const V& y) { return x.contains(y); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_CONTAINS_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST)

    .def("contains", [](const S& x, const V& y) { return x.contains(y); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_CONTAINS_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST)

    .def("interior_contains", [](const S& x, const V& y) { return x.interior_contains(y); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_INTERIOR_CONTAINS_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST)

    .def("is_unbounded", [](const S& x) { return x.is_unbounded(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_IS_UNBOUNDED_CONST)

    .def("is_degenerated", [](const S& x) { return x.is_degenerated(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_IS_DEGENERATED_CONST)

    .def("is_flat", [](const S& x) { return x.is_flat(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_IS_FLAT_CONST)

    .def("intersects", [](const S& x, const S& y) { return x.intersects(y); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_INTERSECTS_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST)

    .def("is_disjoint", [](const S& x, const S& y) { return x.is_disjoint(y); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_IS_DISJOINT_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST)

    .def("overlaps", [](const S& x, const S& y) { return x.overlaps(y); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_OVERLAPS_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST)

    .def("is_subset", [](const S& x, const S& y) { return x.is_subset(y); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_IS_SUBSET_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST)

    .def("is_strict_subset", [](const S& x, const S& y) { return x.is_strict_subset(y); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_IS_STRICT_SUBSET_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST)

    .def("is_interior_subset", [](const S& x, const S& y) { return x.is_interior_subset(y); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_IS_INTERIOR_SUBSET_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST)

    .def("is_strict_interior_subset", [](const S& x, const S& y) { return x.is_strict_interior_subset(y); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_IS_STRICT_INTERIOR_SUBSET_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST)

    .def("is_superset", [](const S& x, const S& y) { return x.is_superset(y); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_IS_SUPERSET_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST)

    .def("is_strict_superset", [](const S& x, const S& y) { return x.is_strict_superset(y); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_IS_STRICT_SUPERSET_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST)

    .def("is_bisectable", [](const S& x) { return x.is_bisectable(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_IS_BISECTABLE_CONST)

    .def("has_integer_bounds", [](const S& x) { return x.has_integer_bounds(); },
      MATRIXBASE_ADDONS_INTERVALMATRIXBASE_BOOL_HAS_INTEGER_BOUNDS_CONST)

    .def("inflate", [](S& x, double r) { return x.inflate(r); },
      MATRIX_ADDONS_INTERVALMATRIXBASE_AUTO_REF_INFLATE_DOUBLE,
      "r"_a)

    .def("inflate", [](S& x, const V& r) { return x.inflate(r); },
      MATRIX_ADDONS_INTERVALMATRIXBASE_AUTO_REF_INFLATE_CONST_MATRIXBASE_OTHERDERIVED_REF,
      "r"_a)

    .def("bisect", [](const S& x, Index_type i, double ratio)
        {
          matlab::test_integer(i);
          return x.bisect(matlab::input_index(i),ratio);
        },
      MATRIX_ADDONS_INTERVALMATRIXBASE_AUTO_BISECT_INDEX_FLOAT_CONST,
      "i"_a, "ratio"_a = 0.49)

    .def("bisect_largest", [](const S& x, double ratio = 0.49) { return x.bisect_largest(ratio); },
      MATRIX_ADDONS_INTERVALMATRIXBASE_AUTO_BISECT_LARGEST_FLOAT_CONST,
      "ratio"_a = 0.49)

    .def(py::self &= py::self,
      MATRIX_ADDONS_INTERVALMATRIXBASE_AUTO_REF_OPERATORANDEQ_CONST_MATRIXBASE_OTHERDERIVED_REF
      "x"_a)

    .def(py::self |= py::self,
      MATRIX_ADDONS_INTERVALMATRIXBASE_AUTO_REF_OPERATOROREQ_CONST_MATRIXBASE_OTHERDERIVED_REF
      "x"_a)

    .def(py::self & py::self,
      MATRIX_ADDONS_INTERVALMATRIXBASE_AUTO_OPERATORAND_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST
      "x"_a)

    .def(py::self | py::self,
      MATRIX_ADDONS_INTERVALMATRIXBASE_AUTO_OPERATOROR_CONST_MATRIXBASE_OTHERDERIVED_REF_CONST,
      "x"_a)
  ;

  py::implicitly_convertible<V,S>();
}
