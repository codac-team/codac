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
#include <codac2_operators.h>

#include "codac2_py_abs_docs.h"
#include "codac2_py_acos_docs.h"
#include "codac2_py_arith_add_docs.h"
#include "codac2_py_arith_div_docs.h"
#include "codac2_py_arith_mul_docs.h"
#include "codac2_py_arith_sub_docs.h"
#include "codac2_py_asin_docs.h"
#include "codac2_py_atan_docs.h"
#include "codac2_py_atan2_docs.h"
#include "codac2_py_ceil_docs.h"
#include "codac2_py_chi_docs.h"
#include "codac2_py_component_docs.h"
#include "codac2_py_cos_docs.h"
#include "codac2_py_cosh_docs.h"
#include "codac2_py_cross_prod_docs.h"
#include "codac2_py_det_docs.h"
#include "codac2_py_exp_docs.h"
#include "codac2_py_extend_docs.h"
#include "codac2_py_flatten_docs.h"
#include "codac2_py_floor_docs.h"
#include "codac2_py_log_docs.h"
#include "codac2_py_mat_docs.h"
#include "codac2_py_max_docs.h"
#include "codac2_py_min_docs.h"
#include "codac2_py_mod_docs.h"
#include "codac2_py_pow_docs.h"
#include "codac2_py_sign_docs.h"
#include "codac2_py_sin_docs.h"
#include "codac2_py_sinh_docs.h"
#include "codac2_py_sqr_docs.h"
#include "codac2_py_sqrt_docs.h"
#include "codac2_py_subvector_docs.h"
#include "codac2_py_tan_docs.h"
#include "codac2_py_tanh_docs.h"
#include "codac2_py_transpose_docs.h"
#include "codac2_py_vec_docs.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_operators(py::module& m)
{
  // Arithmetic operators

  py::class_<AddOp>(m, "AddOp")
    .def(py::init<>()) // for using static methods in Matlab
    // Unary operations

    .def_static("fwd", (Interval(*)(const Interval&)) &AddOp::fwd,
      STATIC_INTERVAL_ADDOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &AddOp::bwd,
      STATIC_VOID_ADDOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

    .def_static("fwd", (IntervalVector(*)(const IntervalVector&)) &AddOp::fwd,
      STATIC_INTERVALVECTOR_ADDOP_FWD_CONST_INTERVALVECTOR_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,IntervalVector&)) &AddOp::bwd,
      STATIC_VOID_ADDOP_BWD_CONST_INTERVALVECTOR_REF_INTERVALVECTOR_REF,
      "y"_a, "x1"_a)

    .def_static("fwd", (IntervalMatrix(*)(const IntervalMatrix&)) &AddOp::fwd,
      STATIC_INTERVALMATRIX_ADDOP_FWD_CONST_INTERVALMATRIX_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const IntervalMatrix&,IntervalMatrix&)) &AddOp::bwd,
      STATIC_VOID_ADDOP_BWD_CONST_INTERVALMATRIX_REF_INTERVALMATRIX_REF,
      "y"_a, "x1"_a)

    // Binary operations

    .def_static("fwd", (Interval(*)(const Interval&,const Interval&)) &AddOp::fwd,
      STATIC_INTERVAL_ADDOP_FWD_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&,Interval&)) &AddOp::bwd,
      STATIC_VOID_ADDOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a, "x2"_a)

    .def_static("fwd", (IntervalVector(*)(const IntervalVector&,const IntervalVector&)) &AddOp::fwd,
      STATIC_INTERVALVECTOR_ADDOP_FWD_CONST_INTERVALVECTOR_REF_CONST_INTERVALVECTOR_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,IntervalVector&,IntervalVector&)) &AddOp::bwd,
      STATIC_VOID_ADDOP_BWD_CONST_INTERVALVECTOR_REF_INTERVALVECTOR_REF_INTERVALVECTOR_REF,
      "y"_a, "x1"_a, "x2"_a)

    .def_static("fwd", (IntervalMatrix(*)(const IntervalMatrix&,const IntervalMatrix&)) &AddOp::fwd,
      STATIC_INTERVALMATRIX_ADDOP_FWD_CONST_INTERVALMATRIX_REF_CONST_INTERVALMATRIX_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const IntervalMatrix&,IntervalMatrix&,IntervalMatrix&)) &AddOp::bwd,
      STATIC_VOID_ADDOP_BWD_CONST_INTERVALMATRIX_REF_INTERVALMATRIX_REF_INTERVALMATRIX_REF,
      "y"_a, "x1"_a, "x2"_a)
  ;

  py::class_<DivOp>(m, "DivOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&,const Interval&)) &DivOp::fwd,
      STATIC_INTERVAL_DIVOP_FWD_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&,Interval&)) &DivOp::bwd,
      STATIC_VOID_DIVOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a, "x2"_a)
  ;

  py::class_<MulOp>(m, "MulOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&,const Interval&)) &MulOp::fwd,
      STATIC_INTERVAL_MULOP_FWD_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&,Interval&)) &MulOp::bwd,
      STATIC_VOID_MULOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a, "x2"_a)

    .def_static("fwd", (IntervalVector(*)(const Interval&,const IntervalVector&)) &MulOp::fwd,
      STATIC_INTERVALVECTOR_MULOP_FWD_CONST_INTERVAL_REF_CONST_INTERVALVECTOR_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,Interval&,IntervalVector&)) &MulOp::bwd,
      STATIC_VOID_MULOP_BWD_CONST_INTERVALVECTOR_REF_INTERVAL_REF_INTERVALVECTOR_REF,
      "y"_a, "x1"_a, "x2"_a)

    .def_static("fwd", (IntervalVector(*)(const IntervalMatrix&,const IntervalVector&)) &MulOp::fwd,
      STATIC_INTERVALVECTOR_MULOP_FWD_CONST_INTERVALMATRIX_REF_CONST_INTERVALVECTOR_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,IntervalMatrix&,IntervalVector&)) &MulOp::bwd,
      STATIC_VOID_MULOP_BWD_CONST_INTERVALVECTOR_REF_INTERVALMATRIX_REF_INTERVALVECTOR_REF,
      "y"_a, "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const IntervalMatrix&,IntervalMatrix&,IntervalMatrix&)) &MulOp::bwd,
      STATIC_VOID_MULOP_BWD_CONST_INTERVALMATRIX_REF_INTERVALMATRIX_REF_INTERVALMATRIX_REF,
      "y"_a, "x1"_a, "x2"_a)
  ;

  py::class_<SubOp>(m, "SubOp")
    .def(py::init<>()) // for using static methods in Matlab
    // Unary operations

    .def_static("fwd", (Interval(*)(const Interval&)) &SubOp::fwd,
      STATIC_INTERVAL_ADDOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &SubOp::bwd,
      STATIC_VOID_ADDOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

    .def_static("fwd", (IntervalVector(*)(const IntervalVector&)) &SubOp::fwd,
      STATIC_INTERVALVECTOR_ADDOP_FWD_CONST_INTERVALVECTOR_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,IntervalVector&)) &SubOp::bwd,
      STATIC_VOID_ADDOP_BWD_CONST_INTERVALVECTOR_REF_INTERVALVECTOR_REF,
      "y"_a, "x1"_a)

    .def_static("fwd", (IntervalMatrix(*)(const IntervalMatrix&)) &SubOp::fwd,
      STATIC_INTERVALMATRIX_ADDOP_FWD_CONST_INTERVALMATRIX_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const IntervalMatrix&,IntervalMatrix&)) &SubOp::bwd,
      STATIC_VOID_ADDOP_BWD_CONST_INTERVALMATRIX_REF_INTERVALMATRIX_REF,
      "y"_a, "x1"_a)

    // Binary operations

    .def_static("fwd", (Interval(*)(const Interval&,const Interval&)) &SubOp::fwd,
      STATIC_INTERVAL_SUBOP_FWD_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&,Interval&)) &SubOp::bwd,
      STATIC_VOID_SUBOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a, "x2"_a)

    .def_static("fwd", (IntervalVector(*)(const IntervalVector&,const IntervalVector&)) &SubOp::fwd,
      STATIC_INTERVALVECTOR_SUBOP_FWD_CONST_INTERVALVECTOR_REF_CONST_INTERVALVECTOR_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,IntervalVector&,IntervalVector&)) &SubOp::bwd,
      STATIC_VOID_SUBOP_BWD_CONST_INTERVALVECTOR_REF_INTERVALVECTOR_REF_INTERVALVECTOR_REF,
      "y"_a, "x1"_a, "x2"_a)

    .def_static("fwd", (IntervalMatrix(*)(const IntervalMatrix&,const IntervalMatrix&)) &SubOp::fwd,
      STATIC_INTERVALMATRIX_SUBOP_FWD_CONST_INTERVALMATRIX_REF_CONST_INTERVALMATRIX_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const IntervalMatrix&,IntervalMatrix&,IntervalMatrix&)) &SubOp::bwd,
      STATIC_VOID_SUBOP_BWD_CONST_INTERVALMATRIX_REF_INTERVALMATRIX_REF_INTERVALMATRIX_REF,
      "y"_a, "x1"_a, "x2"_a)
  ;

  // Other operators

  py::class_<AbsOp>(m, "AbsOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &AbsOp::fwd,
      STATIC_INTERVAL_ABSOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &AbsOp::bwd,
      STATIC_VOID_ABSOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("abs", [](const ScalarExpr& e1) { return abs(e1); },
    SCALAREXPR_ABS_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<AcosOp>(m, "AcosOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &AcosOp::fwd,
      STATIC_INTERVAL_ACOSOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &AcosOp::bwd,
      STATIC_VOID_ACOSOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("acos", [](const ScalarExpr& e1) { return acos(e1); },
    SCALAREXPR_ACOS_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<AsinOp>(m, "AsinOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &AsinOp::fwd,
      STATIC_INTERVAL_ASINOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &AsinOp::bwd,
      STATIC_VOID_ASINOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("asin", [](const ScalarExpr& e1) { return asin(e1); },
    SCALAREXPR_ASIN_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<AtanOp>(m, "AtanOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &AtanOp::fwd,
      STATIC_INTERVAL_ATANOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &AtanOp::bwd,
      STATIC_VOID_ATANOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("atan", [](const ScalarExpr& e1) { return atan(e1); },
    SCALAREXPR_ATAN_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<Atan2Op>(m, "Atan2Op")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&,const Interval&)) &Atan2Op::fwd,
      STATIC_INTERVAL_ATAN2OP_FWD_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&,Interval&)) &Atan2Op::bwd,
      STATIC_VOID_ATAN2OP_BWD_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a, "x2"_a)
  ;

  m.def("atan2", [](const ScalarExpr& e1, const ScalarExpr& e2) { return atan2(e1,e2); },
    SCALAREXPR_ATAN2_CONST_SCALAREXPR_REF_CONST_SCALAREXPR_REF,
    "y"_a, "x"_a);

  py::class_<CeilOp>(m, "CeilOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &CeilOp::fwd,
      STATIC_INTERVAL_CEILOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &CeilOp::bwd,
      STATIC_VOID_CEILOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("ceil", [](const ScalarExpr& e1) { return ceil(e1); },
    SCALAREXPR_CEIL_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<ChiOp>(m, "ChiOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&,const Interval&,const Interval&)) &ChiOp::fwd,
      STATIC_T_CHIOP_FWD_CONST_INTERVAL_REF_CONST_T_REF_CONST_T_REF,
      "x1"_a, "x2"_a, "x3"_a)
    .def_static("fwd", (IntervalVector(*)(const Interval&,const IntervalVector&,const IntervalVector&)) &ChiOp::fwd,
      STATIC_T_CHIOP_FWD_CONST_INTERVAL_REF_CONST_T_REF_CONST_T_REF,
      "x1"_a, "x2"_a, "x3"_a)
    .def_static("fwd", (IntervalMatrix(*)(const Interval&,const IntervalMatrix&,const IntervalMatrix&)) &ChiOp::fwd,
      STATIC_T_CHIOP_FWD_CONST_INTERVAL_REF_CONST_T_REF_CONST_T_REF,
      "x1"_a, "x2"_a, "x3"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&,Interval&,Interval&)) &ChiOp::bwd,
      STATIC_VOID_CHIOP_BWD_CONST_T_REF_INTERVAL_REF_T_REF_T_REF,
      "y"_a, "x1"_a, "x2"_a, "x3"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,Interval&,IntervalVector&,IntervalVector&)) &ChiOp::bwd,
      STATIC_VOID_CHIOP_BWD_CONST_T_REF_INTERVAL_REF_T_REF_T_REF,
      "y"_a, "x1"_a, "x2"_a, "x3"_a)
    .def_static("bwd", (void(*)(const IntervalMatrix&,Interval&,IntervalMatrix&,IntervalMatrix&)) &ChiOp::bwd,
      STATIC_VOID_CHIOP_BWD_CONST_T_REF_INTERVAL_REF_T_REF_T_REF,
      "y"_a, "x1"_a, "x2"_a, "x3"_a)
  ;

  m.def("chi", [](const ScalarExpr& e1, const ScalarExpr& e2, const ScalarExpr& e3) { return chi(e1,e2,e3); },
    ANALYTICEXPRWRAPPER_T_CHI_CONST_SCALAREXPR_REF_CONST_T2_REF_CONST_T3_REF,
    "x1"_a, "x2"_a, "x3"_a);

  m.def("chi", [](const ScalarExpr& e1, const VectorExpr& e2, const VectorExpr& e3) { return chi(e1,e2,e3); },
    ANALYTICEXPRWRAPPER_T_CHI_CONST_SCALAREXPR_REF_CONST_T2_REF_CONST_T3_REF,
    "x1"_a, "x2"_a, "x3"_a);

  m.def("chi", [](const ScalarExpr& e1, const MatrixExpr& e2, const MatrixExpr& e3) { return chi(e1,e2,e3); },
    ANALYTICEXPRWRAPPER_T_CHI_CONST_SCALAREXPR_REF_CONST_T2_REF_CONST_T3_REF,
    "x1"_a, "x2"_a, "x3"_a);

  py::class_<ComponentOp>(m, "ComponentOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const IntervalVector&,Index)) &ComponentOp::fwd,
      STATIC_INTERVAL_COMPONENTOP_FWD_CONST_INTERVALVECTOR_REF_INDEX,
      "x1"_a, "i"_a)
    .def_static("bwd", (void(*)(const Interval&,IntervalVector&,Index)) &ComponentOp::bwd,
      STATIC_VOID_COMPONENTOP_BWD_CONST_INTERVAL_REF_INTERVALVECTOR_REF_INDEX,
      "y"_a, "x1"_a, "i"_a)

    .def_static("fwd", (Interval(*)(const IntervalMatrix&,Index,Index)) &ComponentOp::fwd,
      STATIC_INTERVAL_COMPONENTOP_FWD_CONST_INTERVALMATRIX_REF_INDEX_INDEX,
      "x1"_a, "i"_a, "j"_a)
    .def_static("bwd", (void(*)(const Interval&,IntervalMatrix&,Index,Index)) &ComponentOp::bwd,
      STATIC_VOID_COMPONENTOP_BWD_CONST_INTERVAL_REF_INTERVALMATRIX_REF_INDEX_INDEX,
      "y"_a, "x1"_a, "i"_a, "j"_a)
  ;

  py::class_<CosOp>(m, "CosOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &CosOp::fwd,
      STATIC_INTERVAL_COSOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &CosOp::bwd,
      STATIC_VOID_COSOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("cos", [](const ScalarExpr& e1) { return cos(e1); },
    SCALAREXPR_COS_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<CoshOp>(m, "CoshOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &CoshOp::fwd,
      STATIC_INTERVAL_COSHOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &CoshOp::bwd,
      STATIC_VOID_COSHOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("cosh", [](const ScalarExpr& e1) { return cosh(e1); },
    SCALAREXPR_COSH_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<CrossProdOp>(m, "CrossProdOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (IntervalVector(*)(const IntervalVector&,const IntervalVector&)) &CrossProdOp::fwd,
      STATIC_INTERVALVECTOR_CROSSPRODOP_FWD_CONST_INTERVALVECTOR_REF_CONST_INTERVALVECTOR_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,IntervalVector&,IntervalVector&)) &CrossProdOp::bwd,
      STATIC_VOID_CROSSPRODOP_BWD_CONST_INTERVALVECTOR_REF_INTERVALVECTOR_REF_INTERVALVECTOR_REF,
      "y"_a, "x1"_a, "x2"_a)
  ;

  m.def("cross_prod", [](const VectorExpr& e1, const VectorExpr& e2) { return cross_prod(e1,e2); },
    VECTOREXPR_CROSS_PROD_CONST_VECTOREXPR_REF_CONST_VECTOREXPR_REF,
    "x1"_a, "x2"_a);

  py::class_<DetOp>(m, "DetOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const IntervalMatrix&)) &DetOp::fwd,
      STATIC_INTERVAL_DETOP_FWD_CONST_INTERVALMATRIX_REF,
      "x"_a)
    .def_static("bwd", (void(*)(const Interval&,IntervalMatrix&)) &DetOp::bwd,
      STATIC_VOID_DETOP_BWD_CONST_INTERVAL_REF_INTERVALMATRIX_REF,
      "y"_a, "x"_a)

    .def_static("fwd", (Interval(*)(const IntervalVector&,const IntervalVector&)) &DetOp::fwd,
      STATIC_INTERVAL_DETOP_FWD_CONST_INTERVALVECTOR_REF_CONST_INTERVALVECTOR_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const Interval&,IntervalVector&,IntervalVector&)) &DetOp::bwd,
      STATIC_VOID_DETOP_BWD_CONST_INTERVAL_REF_INTERVALVECTOR_REF_INTERVALVECTOR_REF,
      "y"_a, "x1"_a, "x2"_a)

    .def_static("fwd", (Interval(*)(const IntervalVector&,const IntervalVector&,const IntervalVector&)) &DetOp::fwd,
      STATIC_INTERVAL_DETOP_FWD_CONST_INTERVALVECTOR_REF_CONST_INTERVALVECTOR_REF_CONST_INTERVALVECTOR_REF,
      "x1"_a, "x2"_a, "x3"_a)
    .def_static("bwd", (void(*)(const Interval&,IntervalVector&,IntervalVector&,IntervalVector&)) &DetOp::bwd,
      STATIC_VOID_DETOP_BWD_CONST_INTERVAL_REF_INTERVALVECTOR_REF_INTERVALVECTOR_REF_INTERVALVECTOR_REF,
      "y"_a, "x1"_a, "x2"_a, "x3"_a)
  ;

  m.def("det", [](const MatrixExpr& e1) { return det(e1); },
    SCALAREXPR_DET_CONST_MATRIXEXPR_REF,
    "x1"_a);

  m.def("det", [](const VectorExpr &x1, const VectorExpr &x2) 
		{ return det(x1,x2); },
    SCALAREXPR_DET_CONST_VECTOREXPR_REF_CONST_VECTOREXPR_REF,
    "x1"_a, "x2"_a);

  py::class_<ExpOp>(m, "ExpOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &ExpOp::fwd,
      STATIC_INTERVAL_EXPOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &ExpOp::bwd,
      STATIC_VOID_EXPOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("exp", [](const ScalarExpr& e1) { return exp(e1); },
    SCALAREXPR_EXP_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<ExtendOp>(m, "ExtendOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&,const Interval&)) &ExtendOp::fwd,
    STATIC_T_EXTENDOP_FWD_CONST_T_REF_CONST_T_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&,Interval&)) &ExtendOp::bwd,
    STATIC_VOID_EXTENDOP_BWD_CONST_T_REF_T_REF_T_REF,
      "y"_a, "x1"_a, "x2"_a)
    .def_static("fwd", (IntervalVector(*)(const IntervalVector&,const IntervalVector&)) &ExtendOp::fwd,
    STATIC_T_EXTENDOP_FWD_CONST_T_REF_CONST_T_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,IntervalVector&,IntervalVector&)) &ExtendOp::bwd,
    STATIC_VOID_EXTENDOP_BWD_CONST_T_REF_T_REF_T_REF,
      "y"_a, "x1"_a, "x2"_a)
    .def_static("fwd", (IntervalMatrix(*)(const IntervalMatrix&,const IntervalMatrix&)) &ExtendOp::fwd,
    STATIC_T_EXTENDOP_FWD_CONST_T_REF_CONST_T_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const IntervalMatrix&,IntervalMatrix&,IntervalMatrix&)) &ExtendOp::bwd,
    STATIC_VOID_EXTENDOP_BWD_CONST_T_REF_T_REF_T_REF,
      "y"_a, "x1"_a, "x2"_a)
  ;

  m.def("extend", [](const ScalarExpr& e1, const ScalarExpr& e2) { return extend(e1,e2); },
    ANALYTICEXPRWRAPPER_T_EXTEND_CONST_T1_REF_CONST_T2_REF,
    "x1"_a, "x2"_a);

  m.def("extend", [](const VectorExpr& e1, const VectorExpr& e2) { return extend(e1,e2); },
    ANALYTICEXPRWRAPPER_T_EXTEND_CONST_T1_REF_CONST_T2_REF,
    "x1"_a, "x2"_a);

  m.def("extend", [](const MatrixExpr& e1, const MatrixExpr& e2) { return extend(e1,e2); },
    ANALYTICEXPRWRAPPER_T_EXTEND_CONST_T1_REF_CONST_T2_REF,
    "x1"_a, "x2"_a);

  py::class_<FloorOp>(m, "FloorOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &FloorOp::fwd,
      STATIC_INTERVAL_FLOOROP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &FloorOp::bwd,
      STATIC_VOID_FLOOROP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("floor", [](const ScalarExpr& e1) { return floor(e1); },
    SCALAREXPR_FLOOR_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<FlattenOp>(m, "FlattenOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (IntervalVector(*)(const IntervalMatrix&)) 
		&FlattenOp::fwd,
      STATIC_INTERVALVECTOR_FLATTENOP_FWD_CONST_INTERVALMATRIX_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,IntervalMatrix&))
		 &FlattenOp::bwd,
      STATIC_VOID_FLATTENOP_BWD_CONST_INTERVALVECTOR_REF_INTERVALMATRIX_REF, 
      "y"_a, "x1"_a)
  ;

  m.def("flatten", [](const MatrixExpr& e1) { return flatten(e1); },
    VECTOREXPR_FLATTEN_CONST_MATRIXEXPR_REF,
    "x1"_a);

  py::class_<LogOp>(m, "LogOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &LogOp::fwd,
      STATIC_INTERVAL_LOGOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &LogOp::bwd,
      STATIC_VOID_LOGOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("log", [](const ScalarExpr& e1) { return log(e1); },
    SCALAREXPR_LOG_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<MatrixOp>(m, "MatrixOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (IntervalMatrix(*)(const IntervalVector&)) &MatrixOp::fwd,
      STATIC_INTERVALMATRIX_MATRIXOP_FWD_CONST_X1_REF_CONST_X_REF_VARIADIC,
      "x1"_a)
    .def_static("fwd", (IntervalMatrix(*)(const IntervalVector&,const IntervalVector&)) &MatrixOp::fwd,
      STATIC_INTERVALMATRIX_MATRIXOP_FWD_CONST_X1_REF_CONST_X_REF_VARIADIC,
      "x1"_a, "x2"_a)
    .def_static("fwd", (IntervalMatrix(*)(const IntervalVector&,const IntervalVector&,const IntervalVector&)) &MatrixOp::fwd,
      STATIC_INTERVALMATRIX_MATRIXOP_FWD_CONST_X1_REF_CONST_X_REF_VARIADIC,
      "x1"_a, "x2"_a, "x3"_a)
    .def_static("fwd", (IntervalMatrix(*)(const IntervalVector&,const IntervalVector&,const IntervalVector&,const IntervalVector&)) &MatrixOp::fwd,
      STATIC_INTERVALMATRIX_MATRIXOP_FWD_CONST_X1_REF_CONST_X_REF_VARIADIC,
      "x1"_a, "x2"_a, "x3"_a, "x4"_a)
    .def_static("fwd", (IntervalMatrix(*)(const IntervalVector&,const IntervalVector&,const IntervalVector&,const IntervalVector&,const IntervalVector&)) &MatrixOp::fwd,
      STATIC_INTERVALMATRIX_MATRIXOP_FWD_CONST_X1_REF_CONST_X_REF_VARIADIC,
      "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a)

    .def_static("bwd", (void(*)(const IntervalMatrix&,IntervalVector&)) &MatrixOp::bwd,
      STATIC_VOID_MATRIXOP_BWD_CONST_INTERVALMATRIX_REF_X_REF_VARIADIC,
      "y"_a, "x1"_a)
    .def_static("bwd", (void(*)(const IntervalMatrix&,IntervalVector&,IntervalVector&)) &MatrixOp::bwd,
      STATIC_VOID_MATRIXOP_BWD_CONST_INTERVALMATRIX_REF_X_REF_VARIADIC,
      "y"_a, "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const IntervalMatrix&,IntervalVector&,IntervalVector&,IntervalVector&)) &MatrixOp::bwd,
      STATIC_VOID_MATRIXOP_BWD_CONST_INTERVALMATRIX_REF_X_REF_VARIADIC,
      "y"_a, "x1"_a, "x2"_a, "x3"_a)
    .def_static("bwd", (void(*)(const IntervalMatrix&,IntervalVector&,IntervalVector&,IntervalVector&,IntervalVector&)) &MatrixOp::bwd,
      STATIC_VOID_MATRIXOP_BWD_CONST_INTERVALMATRIX_REF_X_REF_VARIADIC,
      "y"_a, "x1"_a, "x2"_a, "x3"_a, "x4"_a)
    .def_static("bwd", (void(*)(const IntervalMatrix&,IntervalVector&,IntervalVector&,IntervalVector&,IntervalVector&,IntervalVector&)) &MatrixOp::bwd,
      STATIC_VOID_MATRIXOP_BWD_CONST_INTERVALMATRIX_REF_X_REF_VARIADIC,
      "y"_a, "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a)
  ;

  m.def("mat", [](const VectorExpr& e1)
      { return mat(e1); },
    MATRIXEXPR_MAT_CONST_SHARED_PTR_ANALYTICEXPR_X_REF_VARIADIC,
    "x1"_a);
  m.def("mat", [](const VectorExpr& e1, const VectorExpr& e2)
      { return mat(e1,e2); },
    MATRIXEXPR_MAT_CONST_SHARED_PTR_ANALYTICEXPR_X_REF_VARIADIC,
    "x1"_a, "x2"_a);
  m.def("mat", [](const VectorExpr& e1, const VectorExpr& e2, const VectorExpr& e3)
      { return mat(e1,e2,e3); },
    MATRIXEXPR_MAT_CONST_SHARED_PTR_ANALYTICEXPR_X_REF_VARIADIC,
    "x1"_a, "x2"_a, "x3"_a);
  m.def("mat", [](const VectorExpr& e1, const VectorExpr& e2, const VectorExpr& e3, const VectorExpr& e4)
      { return mat(e1,e2,e3,e4); },
    MATRIXEXPR_MAT_CONST_SHARED_PTR_ANALYTICEXPR_X_REF_VARIADIC,
    "x1"_a, "x2"_a, "x3"_a, "x4"_a);
  m.def("mat", [](const VectorExpr& e1, const VectorExpr& e2, const VectorExpr& e3, const VectorExpr& e4, const VectorExpr& e5)
      { return mat(e1,e2,e3,e4,e5); },
    MATRIXEXPR_MAT_CONST_SHARED_PTR_ANALYTICEXPR_X_REF_VARIADIC,
    "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a);
  m.def("mat", [](const VectorExpr& e1, const VectorExpr& e2, const VectorExpr& e3, const VectorExpr& e4, const VectorExpr& e5, const VectorExpr& e6)
      { return mat(e1,e2,e3,e4,e5,e6); },
    MATRIXEXPR_MAT_CONST_SHARED_PTR_ANALYTICEXPR_X_REF_VARIADIC,
    "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a);
  m.def("mat", [](const VectorExpr& e1, const VectorExpr& e2, const VectorExpr& e3, const VectorExpr& e4, const VectorExpr& e5, const VectorExpr& e6, const VectorExpr& e7)
      { return mat(e1,e2,e3,e4,e5,e6,e7); },
    MATRIXEXPR_MAT_CONST_SHARED_PTR_ANALYTICEXPR_X_REF_VARIADIC,
    "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a, "x7"_a);
  m.def("mat", [](const VectorExpr& e1, const VectorExpr& e2, const VectorExpr& e3, const VectorExpr& e4, const VectorExpr& e5, const VectorExpr& e6, const VectorExpr& e7, const VectorExpr& e8)
      { return mat(e1,e2,e3,e4,e5,e6,e7,e8); },
    MATRIXEXPR_MAT_CONST_SHARED_PTR_ANALYTICEXPR_X_REF_VARIADIC,
    "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a, "x7"_a, "x8"_a);
  m.def("mat", [](const VectorExpr& e1, const VectorExpr& e2, const VectorExpr& e3, const VectorExpr& e4, const VectorExpr& e5, const VectorExpr& e6, const VectorExpr& e7, const VectorExpr& e8, const VectorExpr& e9)
      { return mat(e1,e2,e3,e4,e5,e6,e7,e8,e9); },
    MATRIXEXPR_MAT_CONST_SHARED_PTR_ANALYTICEXPR_X_REF_VARIADIC,
    "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a, "x7"_a, "x8"_a, "x9"_a);
  m.def("mat", [](const VectorExpr& e1, const VectorExpr& e2, const VectorExpr& e3, const VectorExpr& e4, const VectorExpr& e5, const VectorExpr& e6, const VectorExpr& e7, const VectorExpr& e8, const VectorExpr& e9, const VectorExpr& e10)
      { return mat(e1,e2,e3,e4,e5,e6,e7,e8,e9,e10); },
    MATRIXEXPR_MAT_CONST_SHARED_PTR_ANALYTICEXPR_X_REF_VARIADIC,
    "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a, "x7"_a, "x8"_a, "x9"_a, "x10"_a);

  py::class_<MaxOp>(m, "MaxOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&,const Interval&)) &MaxOp::fwd,
      STATIC_INTERVAL_MAXOP_FWD_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&,Interval&)) &MaxOp::bwd,
      STATIC_VOID_MAXOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a, "x2"_a)
  ;

  m.def("max", [](const ScalarExpr& e1, const ScalarExpr& e2) { return max(e1,e2); },
    SCALAREXPR_MAX_CONST_SCALAREXPR_REF_CONST_SCALAREXPR_REF,
    "x1"_a, "x2"_a);

  py::class_<MinOp>(m, "MinOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&,const Interval&)) &MinOp::fwd,
      STATIC_INTERVAL_MINOP_FWD_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&,Interval&)) &MinOp::bwd,
      STATIC_VOID_MINOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a, "x2"_a)
  ;

  m.def("min", [](const ScalarExpr& e1, const ScalarExpr& e2) { return min(e1,e2); },
    SCALAREXPR_MIN_CONST_SCALAREXPR_REF_CONST_SCALAREXPR_REF,
    "x1"_a, "x2"_a);

  py::class_<ModOp>(m, "ModOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("bwd", (void(*)(Interval&,Interval&,double)) &ModOp::bwd,
      STATIC_VOID_MODOP_BWD_INTERVAL_REF_INTERVAL_REF_DOUBLE,
      "x1"_a, "x2"_a, "p"_a)
    .def_static("bwd", (void(*)(Interval&,Interval&,Interval&)) &ModOp::bwd,
      STATIC_VOID_MODOP_BWD_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
      "x1"_a, "x2"_a, "p"_a)
  ;

  py::class_<PowOp>(m, "PowOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&,const Interval&)) &PowOp::fwd,
      STATIC_INTERVAL_POWOP_FWD_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&,Interval&)) &PowOp::bwd,
      STATIC_VOID_POWOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a, "x2"_a)
  ;

  m.def("pow", [](const ScalarExpr& e1, const ScalarExpr& e2) { return pow(e1,e2); },
    SCALAREXPR_POW_CONST_SCALAREXPR_REF_CONST_SCALAREXPR_REF,
    "x1"_a, "x2"_a);

  py::class_<SignOp>(m, "SignOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &SignOp::fwd,
      STATIC_INTERVAL_SIGNOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &SignOp::bwd,
      STATIC_VOID_SIGNOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("sign", [](const ScalarExpr& e1) { return sign(e1); },
    SCALAREXPR_SIGN_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<SinOp>(m, "SinOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &SinOp::fwd,
      STATIC_INTERVAL_SINOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &SinOp::bwd,
      STATIC_VOID_SINOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("sin", [](const ScalarExpr& e1) { return sin(e1); },
    SCALAREXPR_SIN_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<SinhOp>(m, "SinhOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &SinhOp::fwd,
      STATIC_INTERVAL_SINHOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &SinhOp::bwd,
      STATIC_VOID_SINHOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("sinh", [](const ScalarExpr& e1) { return sinh(e1); },
    SCALAREXPR_SINH_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<SqrOp>(m, "SqrOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &SqrOp::fwd,
      STATIC_INTERVAL_SQROP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &SqrOp::bwd,
      STATIC_VOID_SQROP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("sqr", [](const ScalarExpr& e1) { return sqr(e1); },
    SCALAREXPR_SQR_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<SqrtOp>(m, "SqrtOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &SqrtOp::fwd,
      STATIC_INTERVAL_SQRTOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &SqrtOp::bwd,
      STATIC_VOID_SQRTOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("sqrt", [](const ScalarExpr& e1) { return sqrt(e1); },
    SCALAREXPR_SQRT_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<TanOp>(m, "TanOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &TanOp::fwd,
      STATIC_INTERVAL_TANOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &TanOp::bwd,
      STATIC_VOID_TANOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("tan", [](const ScalarExpr& e1) { return tan(e1); },
    SCALAREXPR_TAN_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<TanhOp>(m, "TanhOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (Interval(*)(const Interval&)) &TanhOp::fwd,
      STATIC_INTERVAL_TANHOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &TanhOp::bwd,
      STATIC_VOID_TANHOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)
  ;

  m.def("tanh", [](const ScalarExpr& e1) { return tanh(e1); },
    SCALAREXPR_TANH_CONST_SCALAREXPR_REF,
    "x1"_a);

  py::class_<TransposeOp>(m, "TransposeOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (IntervalMatrix(*)(const IntervalMatrix&)) 
		&TransposeOp::fwd,
      STATIC_INTERVALMATRIX_TRANSPOSEOP_FWD_CONST_INTERVALMATRIX_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const IntervalMatrix&,IntervalMatrix&))
		 &TransposeOp::bwd,
      STATIC_VOID_TRANSPOSEOP_BWD_CONST_INTERVALMATRIX_REF_INTERVALMATRIX_REF,
      "y"_a, "x1"_a)
  ;

  m.def("transpose", [](const MatrixExpr& e1) { return transpose(e1); },
    MATRIXEXPR_TRANSPOSE_CONST_MATRIXEXPR_REF,
    "x1"_a);

  py::class_<VectorOp>(m, "VectorOp")
    .def(py::init<>()) // for using static methods in Matlab
    .def_static("fwd", (IntervalVector(*)(const Interval&)) &VectorOp::fwd,
      STATIC_INTERVALVECTOR_VECTOROP_FWD_CONST_X_REF_VARIADIC,
      "x1"_a)
    .def_static("fwd", (IntervalVector(*)(const Interval&,const Interval&)) &VectorOp::fwd,
      STATIC_INTERVALVECTOR_VECTOROP_FWD_CONST_X_REF_VARIADIC,
      "x1"_a, "x2"_a)
    .def_static("fwd", (IntervalVector(*)(const Interval&,const Interval&,const Interval&)) &VectorOp::fwd,
      STATIC_INTERVALVECTOR_VECTOROP_FWD_CONST_X_REF_VARIADIC,
      "x1"_a, "x2"_a, "x3"_a)
    .def_static("fwd", (IntervalVector(*)(const Interval&,const Interval&,const Interval&,const Interval&)) &VectorOp::fwd,
      STATIC_INTERVALVECTOR_VECTOROP_FWD_CONST_X_REF_VARIADIC,
      "x1"_a, "x2"_a, "x3"_a, "x4"_a)
    .def_static("fwd", (IntervalVector(*)(const Interval&,const Interval&,const Interval&,const Interval&,const Interval&)) &VectorOp::fwd,
      STATIC_INTERVALVECTOR_VECTOROP_FWD_CONST_X_REF_VARIADIC,
      "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,Interval&)) &VectorOp::bwd,
      STATIC_VOID_VECTOROP_BWD_CONST_INTERVALVECTOR_REF_X_REF_VARIADIC,
      "y"_a, "x1"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,Interval&,Interval&)) &VectorOp::bwd,
      STATIC_VOID_VECTOROP_BWD_CONST_INTERVALVECTOR_REF_X_REF_VARIADIC,
      "y"_a, "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,Interval&,Interval&,Interval&)) &VectorOp::bwd,
      STATIC_VOID_VECTOROP_BWD_CONST_INTERVALVECTOR_REF_X_REF_VARIADIC,
      "y"_a, "x1"_a, "x2"_a, "x3"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,Interval&,Interval&,Interval&,Interval&)) &VectorOp::bwd,
      STATIC_VOID_VECTOROP_BWD_CONST_INTERVALVECTOR_REF_X_REF_VARIADIC,
      "y"_a, "x1"_a, "x2"_a, "x3"_a, "x4"_a)
    .def_static("bwd", (void(*)(const IntervalVector&,Interval&,Interval&,Interval&,Interval&,Interval&)) &VectorOp::bwd,
      STATIC_VOID_VECTOROP_BWD_CONST_INTERVALVECTOR_REF_X_REF_VARIADIC,
      "y"_a, "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a)
  ;

  const char* EVAL_VEC_DOC = VECTOREXPR_VEC_CONST_X_REF_VARIADIC;
  using S = const ScalarExpr&;

  m .def("vec", [](S e1) { return vec(e1); }, EVAL_VEC_DOC, "x1"_a)
    .def("vec", [](S e1, S e2) { return vec(e1,e2); }, EVAL_VEC_DOC, "x1"_a,"x2"_a)
    .def("vec", [](S e1, S e2, S e3) { return vec(e1,e2,e3); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a)
    .def("vec", [](S e1, S e2, S e3, S e4) { return vec(e1,e2,e3,e4); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5) { return vec(e1,e2,e3,e4,e5); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6) { return vec(e1,e2,e3,e4,e5,e6); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6, S e7) { return vec(e1,e2,e3,e4,e5,e6,e7); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a,"x7"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6, S e7, S e8) { return vec(e1,e2,e3,e4,e5,e6,e7,e8); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a,"x7"_a,"x8"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6, S e7, S e8, S e9) { return vec(e1,e2,e3,e4,e5,e6,e7,e8,e9); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a,"x7"_a,"x8"_a,"x9"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6, S e7, S e8, S e9, S e10) { return vec(e1,e2,e3,e4,e5,e6,e7,e8,e9,e10); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a,"x7"_a,"x8"_a,"x9"_a,"x10"_a)
  ;
}
