/** 
 *  \file
 *  Codac binding (sympy)
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <codac2_AnalyticFunction.h>
#include <codac2_AnalyticType.h>
#include <codac2_sympy.h>
#include "codac2_py_sympy_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_sympy(py::module& m)
{
  // sympy_simplify

  m.def("sympy_simplify", (AnalyticFunction<ScalarType> (*)(const AnalyticFunction<ScalarType>&))&codac2::sympy_simplify,
    ANALYTICFUNCTION_SCALARTYPE_SYMPY_SIMPLIFY_CONST_ANALYTICFUNCTION_SCALARTYPE_REF,
    "f"_a);
  
  m.def("sympy_simplify", (AnalyticFunction<VectorType> (*)(const AnalyticFunction<VectorType>&))&codac2::sympy_simplify,
    ANALYTICFUNCTION_VECTORTYPE_SYMPY_SIMPLIFY_CONST_ANALYTICFUNCTION_VECTORTYPE_REF,
    "f"_a);
  
  m.def("sympy_simplify", (AnalyticFunction<MatrixType> (*)(const AnalyticFunction<MatrixType>&))&codac2::sympy_simplify,
    ANALYTICFUNCTION_MATRIXTYPE_SYMPY_SIMPLIFY_CONST_ANALYTICFUNCTION_MATRIXTYPE_REF,
    "f"_a);

  // sympy_horner

  m.def("sympy_horner", (AnalyticFunction<ScalarType> (*)(const AnalyticFunction<ScalarType>&))&codac2::sympy_horner,
    ANALYTICFUNCTION_SCALARTYPE_SYMPY_HORNER_CONST_ANALYTICFUNCTION_SCALARTYPE_REF,
    "f"_a);
  
  m.def("sympy_horner", (AnalyticFunction<VectorType> (*)(const AnalyticFunction<VectorType>&))&codac2::sympy_horner,
    ANALYTICFUNCTION_VECTORTYPE_SYMPY_HORNER_CONST_ANALYTICFUNCTION_VECTORTYPE_REF,
    "f"_a);
  
  m.def("sympy_horner", (AnalyticFunction<MatrixType> (*)(const AnalyticFunction<MatrixType>&))&codac2::sympy_horner,
    ANALYTICFUNCTION_MATRIXTYPE_SYMPY_HORNER_CONST_ANALYTICFUNCTION_MATRIXTYPE_REF,
    "f"_a);

  // sympy_partial_diff

  m.def("sympy_partial_diff", (AnalyticFunction<ScalarType> (*)(const AnalyticFunction<ScalarType>&,const ScalarVar&))&codac2::sympy_partial_diff,
    ANALYTICFUNCTION_SCALARTYPE_SYMPY_PARTIAL_DIFF_CONST_ANALYTICFUNCTION_SCALARTYPE_REF_CONST_SCALARVAR_REF,
    "f"_a, "x"_a);

  m.def("sympy_partial_diff", (AnalyticFunction<ScalarType> (*)(const AnalyticFunction<ScalarType>&,const ScalarExpr&))&codac2::sympy_partial_diff,
    ANALYTICFUNCTION_SCALARTYPE_SYMPY_PARTIAL_DIFF_CONST_ANALYTICFUNCTION_SCALARTYPE_REF_CONST_SCALARVAR_REF,
    "f"_a, "x"_a);

  // sympy_diff

  m.def("sympy_diff", (AnalyticFunction<ScalarType> (*)(const AnalyticFunction<ScalarType>&))&codac2::sympy_diff,
    ANALYTICFUNCTION_SCALARTYPE_SYMPY_DIFF_CONST_ANALYTICFUNCTION_SCALARTYPE_REF,
    "f"_a);

  m.def("sympy_diff", (AnalyticFunction<ScalarType> (*)(const AnalyticFunction<ScalarType>&,const ScalarVar&))&codac2::sympy_diff,
    ANALYTICFUNCTION_SCALARTYPE_SYMPY_DIFF_CONST_ANALYTICFUNCTION_SCALARTYPE_REF_CONST_SCALARVAR_REF,
    "f"_a, "x"_a);

  m.def("sympy_diff", (AnalyticFunction<ScalarType> (*)(const AnalyticFunction<ScalarType>&,const ScalarExpr&))&codac2::sympy_diff,
    ANALYTICFUNCTION_SCALARTYPE_SYMPY_DIFF_CONST_ANALYTICFUNCTION_SCALARTYPE_REF_CONST_SCALARVAR_REF,
    "f"_a, "x"_a);

  m.def("sympy_diff", (AnalyticFunction<ScalarType> (*)(const AnalyticFunction<ScalarType>&,Index))&codac2::sympy_diff,
    ANALYTICFUNCTION_SCALARTYPE_SYMPY_DIFF_CONST_ANALYTICFUNCTION_SCALARTYPE_REF_INDEX,
    "f"_a, "order"_a);

  m.def("sympy_diff", (AnalyticFunction<ScalarType> (*)(const AnalyticFunction<ScalarType>&,const ScalarVar&,Index))&codac2::sympy_diff,
    ANALYTICFUNCTION_SCALARTYPE_SYMPY_DIFF_CONST_ANALYTICFUNCTION_SCALARTYPE_REF_CONST_SCALARVAR_REF_INDEX,
    "f"_a, "x"_a, "order"_a);

  m.def("sympy_diff", (AnalyticFunction<ScalarType> (*)(const AnalyticFunction<ScalarType>&,const ScalarExpr&,Index))&codac2::sympy_diff,
    ANALYTICFUNCTION_SCALARTYPE_SYMPY_DIFF_CONST_ANALYTICFUNCTION_SCALARTYPE_REF_CONST_SCALARVAR_REF_INDEX,
    "f"_a, "x"_a, "order"_a);

  m.def("sympy_diff", (AnalyticFunction<MatrixType> (*)(const AnalyticFunction<VectorType>&))&codac2::sympy_diff,
    ANALYTICFUNCTION_MATRIXTYPE_SYMPY_DIFF_CONST_ANALYTICFUNCTION_VECTORTYPE_REF,
    "f"_a);

  // sympy_gradient

  m.def("sympy_gradient", (AnalyticFunction<VectorType> (*)(const AnalyticFunction<ScalarType>&))&codac2::sympy_gradient,
    ANALYTICFUNCTION_VECTORTYPE_SYMPY_GRADIENT_CONST_ANALYTICFUNCTION_SCALARTYPE_REF,
    "f"_a);

  // sympy_hessian

  m.def("sympy_hessian", (AnalyticFunction<MatrixType> (*)(const AnalyticFunction<ScalarType>&))&codac2::sympy_hessian,
    ANALYTICFUNCTION_MATRIXTYPE_SYMPY_HESSIAN_CONST_ANALYTICFUNCTION_SCALARTYPE_REF,
    "f"_a);

  // sympy_series

  m.def("sympy_series", (AnalyticFunction<ScalarType> (*)(const AnalyticFunction<ScalarType>&,double,Index))&codac2::sympy_series,
    ANALYTICFUNCTION_SCALARTYPE_SYMPY_SERIES_CONST_ANALYTICFUNCTION_SCALARTYPE_REF_DOUBLE_INDEX,
    "f"_a, "center"_a, "order"_a);
  
  m.def("sympy_series", (AnalyticFunction<ScalarType> (*)(const AnalyticFunction<ScalarType>&,const ScalarVar&,double,Index))&codac2::sympy_series,
    ANALYTICFUNCTION_SCALARTYPE_SYMPY_SERIES_CONST_ANALYTICFUNCTION_SCALARTYPE_REF_CONST_SCALARVAR_REF_DOUBLE_INDEX,
    "f"_a, "x"_a, "center"_a, "order"_a);
  
  m.def("sympy_series", (AnalyticFunction<ScalarType> (*)(const AnalyticFunction<ScalarType>&,const ScalarExpr&,double,Index))&codac2::sympy_series,
    ANALYTICFUNCTION_SCALARTYPE_SYMPY_SERIES_CONST_ANALYTICFUNCTION_SCALARTYPE_REF_CONST_SCALARVAR_REF_DOUBLE_INDEX,
    "f"_a, "x"_a, "center"_a, "order"_a);

  // sympy_equal

  m.def("sympy_equal", (bool (*)(const AnalyticFunction<ScalarType>&,const AnalyticFunction<ScalarType>&))&codac2::sympy_equal,
    BOOL_SYMPY_EQUAL_CONST_ANALYTICFUNCTION_SCALARTYPE_REF_CONST_ANALYTICFUNCTION_SCALARTYPE_REF,
    "f"_a, "g"_a);
  
  m.def("sympy_equal", (bool (*)(const AnalyticFunction<VectorType>&,const AnalyticFunction<VectorType>&))&codac2::sympy_equal,
    BOOL_SYMPY_EQUAL_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_CONST_ANALYTICFUNCTION_VECTORTYPE_REF,
    "f"_a, "g"_a);
  
  m.def("sympy_equal", (bool (*)(const AnalyticFunction<MatrixType>&,const AnalyticFunction<MatrixType>&))&codac2::sympy_equal,
    BOOL_SYMPY_EQUAL_CONST_ANALYTICFUNCTION_MATRIXTYPE_REF_CONST_ANALYTICFUNCTION_MATRIXTYPE_REF,
    "f"_a, "g"_a);
}