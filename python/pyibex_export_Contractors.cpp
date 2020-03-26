/** 
 *  \file
 *  Tube binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_Ctc.h"
#include "tubex_CtcEval.h"
#include "tubex_CtcDeriv.h"
#include "pyibex_export_Ctc_docs.h"
#include "pyibex_export_CtcEval_docs.h"
#include "pyibex_export_CtcDeriv_docs.h"


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>

namespace py = pybind11;
using namespace pybind11::literals;
using py::class_;
using py::init;

using namespace tubex;
using ibex::Interval;
using ibex::IntervalVector;


class pyCtc : public Ctc {
public:
  // pyCtc(int v): Ctc(v){}
  /* Inherit the constructors */
  using Ctc::Ctc;

  /* Trampoline (need one for each virtual function) */
  void contract(std::vector<AbstractDomain*>& v_domains) override {
    // py::gil_scoped_acquire acquire;
    PYBIND11_OVERLOAD_PURE(
      void,       /* return type */
      Ctc,        /* Parent class */
      contract,   /* Name of function */
      v_domains         /* Argument(s) */
    );
    // py::gil_scoped_release release;
  }
};



void export_Contractors(py::module& m){

  py::enum_<TimePropag>(m, "TimePropag", DOCS_TIMEPROPAG)
      .value("FORWARD", TimePropag::FORWARD)
      .value("BACKWARD", TimePropag::BACKWARD)
  ;
    
  py::class_<Ctc, pyCtc> ctc(m, "Ctc");
  ctc
    .def(py::init<>(),DOCS_CTC_CTC)
    // .def("contract", &Ctc::contract,
        // DOCS_CTC_CONTRACT_VECTOR_ABSTRACTDOMAIN, "v_domains"_a)
    .def("preserve_slicing", &Ctc::preserve_slicing,
        DOCS_CTC_PRESERVE_SLICING_BOOL, "preserve"_a=true)
    .def("set_fast_mode", &Ctc::set_fast_mode,
        DOCS_CTC_SET_FAST_MODE_BOOL, "fast_mode"_a=true)
    .def("set_restricted_domain", &Ctc::set_restricted_domain,
        DOCS_CTC_SET_RESTRICTED_DOMAIN_INTERVAL, "domain"_a)
  ;

  py::class_<CtcEval> ctceval(m, "CtcEval", ctc);
  ctceval
    .def(py::init<>(),DOCS_CTCEVAL_CTCEVAL)
    .def("contract", (void (CtcEval::*)(std::vector<AbstractDomain *> &) )&CtcEval::contract,
        DOCS_CTCEVAL_CONTRACT_VECTOR_ABSTRACTDOMAIN, "v_domains"_a)
    .def("contract", (void (CtcEval::*)(double,ibex::Interval &,Tube &,Tube &) )&CtcEval::contract,
        DOCS_CTCEVAL_CONTRACT_DOUBLE_INTERVAL_TUBE_TUBE, "t"_a, "z"_a, "y"_a, "w"_a)
    .def("contract", (void (CtcEval::*)(ibex::Interval &,ibex::Interval &,Tube &,Tube &) )&CtcEval::contract,
        DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVAL_TUBE_TUBE, "t"_a, "z"_a, "y"_a, "w"_a)
    .def("contract", (void (CtcEval::*)(const ibex::Interval &,const ibex::Interval &,Tube &,Tube &) )&CtcEval::contract,
        DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVAL_TUBE_TUBE1, "t"_a, "z"_a, "y"_a, "w"_a)
    .def("contract", (void (CtcEval::*)(double,ibex::IntervalVector &,TubeVector &,TubeVector &) )&CtcEval::contract,
        DOCS_CTCEVAL_CONTRACT_DOUBLE_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR, "t"_a, "z"_a, "y"_a, "w"_a)
    .def("contract", (void (CtcEval::*)(ibex::Interval &,ibex::IntervalVector &,TubeVector &,TubeVector &) )&CtcEval::contract,
        DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR, "t"_a, "z"_a, "y"_a, "w"_a)
    .def("contract", (void (CtcEval::*)(const ibex::Interval &,const ibex::IntervalVector &,TubeVector &,TubeVector &) )&CtcEval::contract,
        DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR1, "t"_a, "z"_a, "y"_a, "w"_a)
    .def("contract", (void (CtcEval::*)(ibex::Interval &,ibex::Interval &,const Tube &) )&CtcEval::contract,
        DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVAL_TUBE, "t"_a, "z"_a, "y"_a)
    .def("contract", (void (CtcEval::*)(ibex::Interval &,ibex::IntervalVector &,const TubeVector &) )&CtcEval::contract,
        DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR, "t"_a, "z"_a, "y"_a)
    .def("enable_temporal_propagation", &CtcEval::enable_temporal_propagation,
        DOCS_CTCEVAL_ENABLE_TEMPORAL_PROPAGATION_BOOL, "enable_propagation"_a)
  ;

    py::class_<CtcDeriv> ctcderiv(m, "CtcDeriv", DOCS_CTCDERIV);
    ctcderiv
        .def(py::init<>(),DOCS_CTCDERIV_CTCDERIV)
        // .def("contract", (void (CtcDeriv::*)(std::vector<AbstractDomain> &) )&CtcDeriv::contract,
            // DOCS_CTCDERIV_CONTRACT_VECTOR_ABSTRACTDOMAIN, "v_domains"_a)
        .def("contract", (void (CtcDeriv::*)(Tube &,const Tube &,TimePropag) )&CtcDeriv::contract,
            DOCS_CTCDERIV_CONTRACT_TUBE_TUBE_TIMEPROPAG, "x"_a, "v"_a, "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
        .def("contract", (void (CtcDeriv::*)(TubeVector &,const TubeVector &,TimePropag) )&CtcDeriv::contract,
            DOCS_CTCDERIV_CONTRACT_TUBEVECTOR_TUBEVECTOR_TIMEPROPAG, "x"_a, "v"_a, "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
        .def("contract", (void (CtcDeriv::*)(Slice &,const Slice &,TimePropag) )&CtcDeriv::contract,
            DOCS_CTCDERIV_CONTRACT_SLICE_SLICE_TIMEPROPAG, "x"_a, "v"_a, "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
    ;

}