/** 
 *  \file
 *  Ctc Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "codac_type_caster.h"

#include "codac_py_Ctc.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_Ctc_docs.h" // todo: generate this file from Doxygen doc

#include <ibex_CmpOp.h>
#include <ibex_CtcPropag.h>
#include <ibex_CtcQInter.h>
#include <ibex_CtcFwdBwd.h>
#include <ibex_CtcInverse.h>
#include <ibex_CtcNotIn.h>
#include <ibex_CtcFixPoint.h>
#include <ibex_CtcExist.h>
#include <ibex_CtcForAll.h>
#include "codac_CtcUnion.h"
#include "codac_CtcCompo.h"
#include "codac_Function.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


py::class_<Ctc,pyCtc> export_Ctc(py::module& m)
{
  // Export Ctc
  py::class_<Ctc,pyCtc> ctc(m, "Ctc", DOCS_CTC_TYPE);
  ctc

    .def(py::init<int>())
    .def_readonly("nb_var", &Ctc::nb_var, DOC_CTC_NB_VAR)

    .def("__or__", [](Ctc& c1, Ctc& c2)
      {
        return new CtcUnion(c1, c2);
        // todo: manage delete
      },
      DOC_CTC_OR,
      py::return_value_policy::take_ownership,
      py::keep_alive<0,1>(), py::keep_alive<0,2>())

    .def("__and__", [](Ctc& c1, Ctc& c2)
      {
        return new CtcCompo(c1, c2);
        // todo: manage delete
      },
      DOC_CTC_AND,
      py::return_value_policy::take_ownership,
      py::keep_alive<0,1>(), py::keep_alive<0,2>())
    ;

  // Export comparaison constant
  py::enum_<ibex::CmpOp>(m, "CmpOp", DOC_CMPOP_TYPE)
    .value("LT",  ibex::LT)
    .value("LEQ", ibex::LEQ)
    .value("EQ",  ibex::EQ)
    .value("GEQ", ibex::GEQ)
    .value("GT",  ibex::GT)
    .export_values()
  ;

  // Export CtcUnion
  py::class_<CtcUnion>(m, "CtcUnion", ctc, DOC_CTCUNION_TYPE)
    .def(py::init<ibex::Array<Ctc>>(), py::keep_alive<1,2>(), "list"_a)
    .def("contract", (void (Ctc::*)(IntervalVector&)) &CtcUnion::contract)
    ;

  // Export CtcCompo
  py::class_<CtcCompo>(m, "CtcCompo", ctc, DOC_CTCCOMPO_TYPE)
    .def(py::init<ibex::Array<Ctc>>(), py::keep_alive<1,2>(), "list"_a)
    .def("contract", (void (Ctc::*)(IntervalVector&)) &CtcCompo::contract)
    ;

  // Export CtcPropag
  py::class_<ibex::CtcPropag>(m, "CtcPropag", ctc, "todo")
    .def(py::init<ibex::Array<Ctc>,double,bool>(), py::keep_alive<1,2>(), "list"_a, "ratio"_a=0.1, "incr"_a=false)
    .def("contract", (void (Ctc::*)(IntervalVector&)) &ibex::CtcPropag::contract)
    ;

  // Export CtcQInter
  py::class_<ibex::CtcQInter>(m, "CtcQInter", ctc, DOC_CTCQINTER_TYPE)
    .def(py::init<ibex::Array<Ctc>,int>(), py::keep_alive<1,2>(), "list"_a, "q"_a)
    .def("contract", (void (Ctc::*)(IntervalVector&)) &ibex::CtcQInter::contract)
    ;

  // Export CtcFwdBwd
  py::class_<ibex::CtcFwdBwd>(m, "CtcFwdBwd", ctc, DOC_CTCFWDBWD_TYPE)
    .def(py::init<Function&,ibex::CmpOp>(), py::keep_alive<1,2>(), "f"_a, "op"_a=ibex::EQ)
    .def(py::init<Function&,Interval&>(), py::keep_alive<1,2>(), "f"_a, "itv_y"_a)
    .def(py::init<Function&,IntervalVector&>(), py::keep_alive<1,2>(), "f"_a, "box_y"_a)
    .def(py::init([](ibex::Function& f,const std::array<double,2>& itv)
      {
        return std::unique_ptr<ibex::CtcFwdBwd>(new ibex::CtcFwdBwd(f, Interval(itv[0], itv[1])));
      }),
      py::keep_alive<1,2>(), py::arg("f"), py::arg("itv_y"))
    .def("contract", (void (Ctc::*)(IntervalVector&)) &ibex::CtcFwdBwd::contract)
    ;

  // Export CtcInverse
  py::class_<ibex::CtcInverse>(m, "CtcInverse", ctc, DOC_CTCINVERSE_TYPE)
    .def(py::init<Ctc&,Function&>(),py::keep_alive<1,2>(), py::keep_alive<1,3>(), "ctc"_a, "f"_a)
    .def("contract", (void (Ctc::*)(IntervalVector&)) &ibex::CtcInverse::contract)
    ;

  // Export CtcNotIn
  py::class_<ibex::CtcNotIn>(m, "CtcNotIn", ctc, DOC_CTCNOTIN_TYPE)
    .def(py::init<Function&,Interval&>(), py::keep_alive<1,2>())
    .def(py::init<Function&,IntervalVector&>(), py::keep_alive<1,2>())
    .def("contract", (void (Ctc::*)(IntervalVector&)) &ibex::CtcNotIn::contract)
    ;

  // Export CtcFixPoint
  py::class_<ibex::CtcFixPoint>(m, "CtcFixPoint", ctc, DOC_CTCFIXPOINT_TYPE)
    .def(py::init<Ctc&,double>(), py::keep_alive<1,2>(), "ctc"_a, "ratio"_a=1e-3)
    .def("contract", (void (Ctc::*)(IntervalVector&)) &ibex::CtcFixPoint::contract)
    ;

  // Export CtcExist
  py::class_<ibex::CtcExist>(m, "CtcExist", ctc, DOC_CTCEXISTS_TYPE)
    .def(py::init([](Ctc& c, const IntervalVector& y, double prec)
      {
        ibex::BitSet vars = ibex::BitSet::empty(c.nb_var);
        vars.fill(0, c.nb_var-y.size()-1);
        ibex::CtcExist *instance = new ibex::CtcExist(c, vars, y, prec);
        return instance;
        // todo: manage delete
      }),
      py::keep_alive<1,2>(), "ctc"_a, "y"_a, "prec"_a = 1e-3)  
    .def("contract", (void (Ctc::*)(IntervalVector&)) &ibex::CtcExist::contract)
    ;

  // Export CtcForAll
  py::class_<ibex::CtcForAll>(m, "CtcForAll", ctc, DOC_CTCEXISTS_TYPE)
    .def(py::init([](Ctc& c, const IntervalVector& y, double prec)
      {
        ibex::BitSet vars = ibex::BitSet::empty(c.nb_var);
        vars.fill(0, c.nb_var-y.size()-1);
        ibex::CtcForAll *instance = new ibex::CtcForAll(c, vars, y, prec);
        return instance;
        // todo: manage delete
      }),
      py::keep_alive<1,2>(), "ctc"_a, "y"_a, "prec"_a = 1e-3)  
    .def("contract", (void (Ctc::*)(IntervalVector&)) &ibex::CtcForAll::contract)
    ;

  return ctc;
}