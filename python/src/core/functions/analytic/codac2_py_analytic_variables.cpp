/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_analytic_variables.h>
#include <codac2_AnalyticExpr.h>
#include "codac2_py_analytic_variables_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_AnalyticExprWrapper.h"
#include "codac2_py_matlab.h"

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_ScalarVar(py::module& m)
{
  py::class_<ScalarVar, std::shared_ptr<ScalarVar> /* due to enable_shared_from_this */>
    exported(m, "ScalarVar", SCALARVAR_MAIN);
  exported
  
    .def(py::init<>(),
      SCALARVAR_SCALARVAR)

    .def("size", &ScalarVar::size,
      INDEX_SCALARVAR_SIZE_CONST)

    .def("__pos__",  [](const ScalarVar& e1)                           { return e1;      }, py::is_operator())
    .def("__add__",  [](const ScalarVar& e1, const ScalarVar& e2)      { return e1 + e2; }, py::is_operator())
    .def("__add__",  [](const ScalarVar& e1, const ScalarExpr& e2)     { return e1 + e2; }, py::is_operator())
    .def("__add__",  [](const ScalarVar& e1, const Interval& e2)       { return e1 + e2; }, py::is_operator())
    .def("__radd__", [](const ScalarVar& e1, const Interval& e2)       { return e2 + e1; }, py::is_operator())
    .def("__neg__",  [](const ScalarVar& e1)                           { return -e1;     }, py::is_operator())
    .def("__sub__",  [](const ScalarVar& e1, const ScalarVar& e2)      { return e1 - e2; }, py::is_operator())
    .def("__sub__",  [](const ScalarVar& e1, const ScalarExpr& e2)     { return e1 - e2; }, py::is_operator())
    .def("__sub__",  [](const ScalarVar& e1, const Interval& e2)       { return e1 - e2; }, py::is_operator())
    .def("__rsub__", [](const ScalarVar& e1, const Interval& e2)       { return e2 - e1; }, py::is_operator())
    .def("__mul__",  [](const ScalarVar& e1, const ScalarVar& e2)      { return e1 * e2; }, py::is_operator())
    .def("__mul__",  [](const ScalarVar& e1, const ScalarExpr& e2)     { return e1 * e2; }, py::is_operator())
    .def("__mul__",  [](const ScalarVar& e1, const Interval& e2)       { return e1 * e2; }, py::is_operator())
    .def("__rmul__", [](const ScalarVar& e1, const Interval& e2)       { return e2 * e1; }, py::is_operator())
    .def("__mul__",  [](const ScalarVar& e1, const VectorVar& e2)      { return e1 * e2; }, py::is_operator())
    .def("__mul__",  [](const ScalarVar& e1, const VectorExpr& e2)     { return e1 * e2; }, py::is_operator())
    .def("__mul__",  [](const ScalarVar& e1, const IntervalVector& e2) { return e1 * e2; }, py::is_operator())
    .def("__truediv__",  [](const ScalarVar& e1, const ScalarVar& e2)  { return e1 / e2; }, py::is_operator())
    .def("__truediv__",  [](const ScalarVar& e1, const ScalarExpr& e2) { return e1 / e2; }, py::is_operator())
    .def("__truediv__",  [](const ScalarVar& e1, const Interval& e2)   { return e1 / e2; }, py::is_operator())
    .def("__rtruediv__", [](const ScalarVar& e1, const Interval& e2)   { return e2 / e1; }, py::is_operator())
  ;

  py::implicitly_convertible<ScalarVar,ScalarExpr>();
}

ScalarExpr get_item(const VectorVar& v, Index_type i)
{
  matlab::test_integer(i);
  i = matlab::input_index(i);

  if(i < 0 || i >= static_cast<Index>(v.size()))
    throw py::index_error("index is out of range");

  return v[static_cast<int>(i)];
}

void export_VectorVar(py::module& m)
{
  py::class_<VectorVar,
    std::shared_ptr<VectorVar> /* due to enable_shared_from_this */>
    exported(m, "VectorVar", VECTORVAR_MAIN);
  exported
  
    .def(py::init(
        [](Index_type n)
        {
          matlab::test_integer(n);
          return std::make_unique<VectorVar>(n);
        }),
      VECTORVAR_VECTORVAR_INDEX,
      "n"_a)

    .def("size", &VectorVar::size,
      INDEX_VECTORVAR_SIZE_CONST);

  if(FOR_MATLAB)
    exported.def("__call__", [](const VectorVar& v, Index_type i) -> ScalarExpr
      {
        return get_item(v, i);
      }, ANALYTICEXPRWRAPPER_SCALARTYPE_VECTORVAR_OPERATORCOMPO_INDEX_CONST);

  else
    exported.def("__getitem__", [](const VectorVar& v, Index_type i) -> ScalarExpr
      {
        return get_item(v, i);
      }, ANALYTICEXPRWRAPPER_SCALARTYPE_VECTORVAR_OPERATORCOMPO_INDEX_CONST);

  exported

    .def("subvector", [](const VectorVar& v, Index_type i, Index_type j) -> VectorExpr
      {
        matlab::test_integer(i, j);
        return v.subvector(matlab::input_index(i),matlab::input_index(j));
      }, ANALYTICEXPRWRAPPER_VECTORTYPE_VECTORVAR_SUBVECTOR_INDEX_INDEX_CONST)

    .def("__pos__",  [](const VectorVar& e1)                           { return e1;      }, py::is_operator())
    .def("__add__",  [](const VectorVar& e1, const VectorVar& e2)      { return e1 + e2; }, py::is_operator())
    .def("__add__",  [](const VectorVar& e1, const IntervalVector& e2) { return e1 + e2; }, py::is_operator())
    .def("__add__",  [](const VectorVar& e1, const VectorExpr& e2)     { return e1 + e2; }, py::is_operator())
    .def("__radd__", [](const VectorVar& e1, const IntervalVector& e2) { return e2 + e1; }, py::is_operator())
    .def("__neg__",  [](const VectorVar& e1)                           { return -e1;     }, py::is_operator())
    .def("__sub__",  [](const VectorVar& e1, const VectorVar& e2)      { return e1 - e2; }, py::is_operator())
    .def("__sub__",  [](const VectorVar& e1, const VectorExpr& e2)     { return e1 - e2; }, py::is_operator())
    .def("__sub__",  [](const VectorVar& e1, const IntervalVector& e2) { return e1 - e2; }, py::is_operator())
    .def("__rsub__", [](const VectorVar& e1, const IntervalVector& e2) { return e2 - e1; }, py::is_operator())
  ;
  
  py::implicitly_convertible<VectorVar,VectorExpr>();
}