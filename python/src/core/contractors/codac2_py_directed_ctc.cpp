/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_directed_ctc.h>
#include "codac2_py_directed_ctc_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_directed_ctc(py::module& m)
{
  py::class_<AddOp>(m, "AddOp")

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

  py::class_<SubOp>(m, "SubOp")

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

  py::class_<MulOp>(m, "MulOp")

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

  ;

  py::class_<DivOp>(m, "DivOp")

    .def_static("fwd", (Interval(*)(const Interval&,const Interval&)) &DivOp::fwd,
      STATIC_INTERVAL_DIVOP_FWD_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&,Interval&)) &DivOp::bwd,
      STATIC_VOID_DIVOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a, "x2"_a)

  ;

  py::class_<PowOp>(m, "PowOp")

    .def_static("fwd", (Interval(*)(const Interval&,const Interval&)) &PowOp::fwd,
      STATIC_INTERVAL_POWOP_FWD_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&,Interval&)) &PowOp::bwd,
      STATIC_VOID_POWOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a, "x2"_a)

  ;

  py::class_<SqrOp>(m, "SqrOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &SqrOp::fwd,
      STATIC_INTERVAL_SQROP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &SqrOp::bwd,
      STATIC_VOID_SQROP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<SqrtOp>(m, "SqrtOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &SqrtOp::fwd,
      STATIC_INTERVAL_SQRTOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &SqrtOp::bwd,
      STATIC_VOID_SQRTOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<ExpOp>(m, "ExpOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &ExpOp::fwd,
      STATIC_INTERVAL_EXPOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &ExpOp::bwd,
      STATIC_VOID_EXPOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<LogOp>(m, "LogOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &LogOp::fwd,
      STATIC_INTERVAL_LOGOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &LogOp::bwd,
      STATIC_VOID_LOGOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<CosOp>(m, "CosOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &CosOp::fwd,
      STATIC_INTERVAL_COSOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &CosOp::bwd,
      STATIC_VOID_COSOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<SinOp>(m, "SinOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &SinOp::fwd,
      STATIC_INTERVAL_SINOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &SinOp::bwd,
      STATIC_VOID_SINOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<TanOp>(m, "TanOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &TanOp::fwd,
      STATIC_INTERVAL_TANOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &TanOp::bwd,
      STATIC_VOID_TANOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<AcosOp>(m, "AcosOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &AcosOp::fwd,
      STATIC_INTERVAL_ACOSOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &AcosOp::bwd,
      STATIC_VOID_ACOSOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<AsinOp>(m, "AsinOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &AsinOp::fwd,
      STATIC_INTERVAL_ASINOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &AsinOp::bwd,
      STATIC_VOID_ASINOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<AtanOp>(m, "AtanOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &AtanOp::fwd,
      STATIC_INTERVAL_ATANOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &AtanOp::bwd,
      STATIC_VOID_ATANOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<Atan2Op>(m, "Atan2Op")

    .def_static("fwd", (Interval(*)(const Interval&,const Interval&)) &Atan2Op::fwd,
      STATIC_INTERVAL_ATAN2OP_FWD_CONST_INTERVAL_REF_CONST_INTERVAL_REF,
      "x1"_a, "x2"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&,Interval&)) &Atan2Op::bwd,
      STATIC_VOID_ATAN2OP_BWD_CONST_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a, "x2"_a)

  ;

  py::class_<CoshOp>(m, "CoshOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &CoshOp::fwd,
      STATIC_INTERVAL_COSHOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &CoshOp::bwd,
      STATIC_VOID_COSHOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<SinhOp>(m, "SinhOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &SinhOp::fwd,
      STATIC_INTERVAL_SINHOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &SinhOp::bwd,
      STATIC_VOID_SINHOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<TanhOp>(m, "TanhOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &TanhOp::fwd,
      STATIC_INTERVAL_TANHOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &TanhOp::bwd,
      STATIC_VOID_TANHOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<AbsOp>(m, "AbsOp")

    .def_static("fwd", (Interval(*)(const Interval&)) &AbsOp::fwd,
      STATIC_INTERVAL_ABSOP_FWD_CONST_INTERVAL_REF,
      "x1"_a)
    .def_static("bwd", (void(*)(const Interval&,Interval&)) &AbsOp::bwd,
      STATIC_VOID_ABSOP_BWD_CONST_INTERVAL_REF_INTERVAL_REF,
      "y"_a, "x1"_a)

  ;

  py::class_<ComponentOp>(m, "ComponentOp")

    .def_static("fwd", (Interval(*)(const IntervalVector&,size_t)) &ComponentOp::fwd,
      STATIC_INTERVAL_COMPONENTOP_FWD_CONST_INTERVALVECTOR_REF_SIZET,
      "x1"_a, "i"_a)
    .def_static("bwd", (void(*)(const Interval&,IntervalVector&,size_t)) &ComponentOp::bwd,
      STATIC_VOID_COMPONENTOP_BWD_CONST_INTERVAL_REF_INTERVALVECTOR_REF_SIZET,
      "y"_a, "x1"_a, "i"_a)

  ;

  py::class_<VectorOp>(m, "VectorOp")

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

  // todo: MatrixOp

  py::class_<DetOp>(m, "DetOp")

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
}