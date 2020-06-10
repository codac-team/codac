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

#include "tubex_DynCtc.h"
#include "tubex_CtcFunction.h"
#include "tubex_CtcDist.h"
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
#include "tubex_CtcPicard.h"
#include "tubex_CtcConstell.h"
#include "tubex_Domain.h"
#include "tubex_CtcFunction.h"


// Generated files from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_CtcFunction_docs.h"
#include "tubex_py_CtcDist_docs.h"
#include "tubex_py_DynCtc_docs.h"
#include "tubex_py_CtcDeriv_docs.h"
#include "tubex_py_CtcEval_docs.h"
#include "tubex_py_CtcPicard_docs.h"
#include "tubex_py_CtcConstell_docs.h"


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
using ibex::Ctc;


class pyStaticCtc : public Ctc {
  // protected:
public:
  pyStaticCtc(int v): Ctc(v){}
  /* Inherit the constructors */
  // using Ctc::Ctc;

  /* Trampoline (need one for each virtual function) */
  void contract(IntervalVector& box) override {
    // py::gil_scoped_acquire acquire;
    PYBIND11_OVERLOAD_PURE(
      void,       /* return type */
      Ctc,        /* Parent class */
      contract,   /* Name of function */
      box         /* Argument(s) */
    );
    // py::gil_scoped_release release;
  }
};

class pyDynCtc : public DynCtc {
public:
  // pyCtc(int v): Ctc(v){}
  /* Inherit the constructors */
  using DynCtc::DynCtc;

  /* Trampoline (need one for each virtual function) */
  void contract(std::vector<Domain*>& v_domains) override {
    // py::gil_scoped_acquire acquire;
    PYBIND11_OVERLOAD_PURE(
      void,       /* return type */
      DynCtc,        /* Parent class */
      contract,   /* Name of function */
      v_domains         /* Argument(s) */
    );
    // py::gil_scoped_release release;
  }
};



void export_contractors(py::module& m){


  py::enum_<TimePropag>(m, "TimePropag", "todo")
      .value("FORWARD", TimePropag::FORWARD)
      .value("BACKWARD", TimePropag::BACKWARD)
  ;
    
  py::class_<Ctc, pyStaticCtc> static_ctc(m, "StaticCtc", py::module_local());
  /*static_ctc
    .def(py::init<>(),DOCS_CTC_CTC)
    // .def("contract", &DynCtc::contract,
        // DOCS_CTC_CONTRACT_VECTOR_ABSTRACTDOMAIN, "v_domains"_a)
    .def("preserve_slicing", &DynCtc::preserve_slicing,
        DOCS_CTC_PRESERVE_SLICING_BOOL, "preserve"_a=true)
    .def("set_fast_mode", &DynCtc::set_fast_mode,
        DOCS_CTC_SET_FAST_MODE_BOOL, "fast_mode"_a=true)
    .def("restrict_tdomain", &DynCtc::restrict_tdomain,
        DOCS_CTC_RESTRICT_TDOMAIN_INTERVAL, "domain"_a)
  ;*/
    
  py::class_<DynCtc, pyDynCtc> dyn_ctc(m, "DynCtc");
  dyn_ctc
    .def(py::init<>(),DYNCTC_DYNCTC_BOOL)
    // .def("contract", &DynCtc::contract,
        // DOCS_CTC_CONTRACT_VECTOR_ABSTRACTDOMAIN, "v_domains"_a)
    .def("preserve_slicing", &DynCtc::preserve_slicing,
        DYNCTC_VOID_PRESERVE_SLICING_BOOL, "preserve"_a=true)
    .def("set_fast_mode", &DynCtc::set_fast_mode,
        DYNCTC_VOID_SET_FAST_MODE_BOOL, "fast_mode"_a=true)
    .def("restrict_tdomain", &DynCtc::restrict_tdomain,
        DYNCTC_VOID_RESTRICT_TDOMAIN_INTERVAL, "domain"_a)
  ;

  py::class_<CtcEval> ctc_eval(m, "CtcEval", dyn_ctc);
  ctc_eval
    .def(py::init<>(),CTCEVAL_CTCEVAL)
    //.def("contract", (void (CtcEval::*)(std::vector<Domain *> &) )&CtcEval::contract,
    //    DOCS_CTCEVAL_CONTRACT_VECTOR_a.noconvert()BSTRACTDOMAIN, "v_domains"_a.noconvert())
    .def("contract", (void (CtcEval::*)(double,ibex::Interval &,Tube &,Tube &) )&CtcEval::contract,
        CTCEVAL_VOID_CONTRACT_DOUBLE_INTERVAL_TUBE_TUBE, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    .def("contract", (void (CtcEval::*)(ibex::Interval &,ibex::Interval &,Tube &,Tube &) )&CtcEval::contract,
        CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVAL_TUBE_TUBE, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    //.def("contract", (void (CtcEval::*)(const ibex::Interval &,const ibex::Interval &,Tube &,Tube &) )&CtcEval::contract,
    //    CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVAL_TUBE_TUBE, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    .def("contract", (void (CtcEval::*)(double,ibex::IntervalVector &,TubeVector &,TubeVector &) )&CtcEval::contract,
        CTCEVAL_VOID_CONTRACT_DOUBLE_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    .def("contract", (void (CtcEval::*)(ibex::Interval &,ibex::IntervalVector &,TubeVector &,TubeVector &) )&CtcEval::contract,
        CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    //.def("contract", (void (CtcEval::*)(const ibex::Interval &,const ibex::IntervalVector &,TubeVector &,TubeVector &) )&CtcEval::contract,
    //    DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR1, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    .def("contract", (void (CtcEval::*)(ibex::Interval &,ibex::Interval &,const Tube &) )&CtcEval::contract,
        CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVAL_TUBE, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert())
    .def("contract", (void (CtcEval::*)(ibex::Interval &,ibex::IntervalVector &,const TubeVector &) )&CtcEval::contract,
        CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert())
    .def("enable_time_propag", &CtcEval::enable_time_propag,
        CTCEVAL_VOID_ENABLE_TIME_PROPAG_BOOL, "enable_propagation"_a)
  ;

    py::class_<CtcDeriv> ctc_deriv(m, "CtcDeriv", dyn_ctc, "todo");
    ctc_deriv
        .def(py::init<>(),CTCDERIV_CTCDERIV)
        // .def("contract", (void (CtcDeriv::*)(std::vector<Domain> &) )&CtcDeriv::contract,
            // DOCS_CTCDERIV_CONTRACT_VECTOR_ABSTRACTDOMAIN, "v_domains"_a)
        .def("contract", (void (CtcDeriv::*)(Tube &,const Tube &,TimePropag) )&CtcDeriv::contract,
            CTCDERIV_VOID_CONTRACT_TUBE_TUBE_TIMEPROPAG, "x"_a.noconvert(), "v"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
        .def("contract", (void (CtcDeriv::*)(TubeVector &,const TubeVector &,TimePropag) )&CtcDeriv::contract,
            CTCDERIV_VOID_CONTRACT_TUBEVECTOR_TUBEVECTOR_TIMEPROPAG, "x"_a.noconvert(), "v"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
        .def("contract", (void (CtcDeriv::*)(Slice &,const Slice &,TimePropag) )&CtcDeriv::contract,
            CTCDERIV_VOID_CONTRACT_SLICE_SLICE_TIMEPROPAG, "x"_a.noconvert(), "v"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
    ;

    py::class_<CtcPicard> ctc_picard(m, "CtcPicard", dyn_ctc, "todo");
    ctc_picard
      .def(py::init<float>(),CTCPICARD_CTCPICARD_FLOAT, "delta"_a=1.1)
      //.def("contract", (void (CtcPicard::*)(std::vector<Domain *> &) )&CtcPicard::contract,
      //    DOCS_CTCPICARD_CONTRACT_VECTOR_DOMAIN, "v_domains"_a)
      .def("contract", (void (CtcPicard::*)(const TFnc &,Tube &,TimePropag) )&CtcPicard::contract,
          CTCPICARD_VOID_CONTRACT_TFNC_TUBE_TIMEPROPAG, "f"_a, "x"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
      .def("contract", (void (CtcPicard::*)(const TFnc &,TubeVector &,TimePropag) )&CtcPicard::contract,
          CTCPICARD_VOID_CONTRACT_TFNC_TUBEVECTOR_TIMEPROPAG, "f"_a, "x"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
      .def("picard_iterations", &CtcPicard::picard_iterations,
          CTCPICARD_INT_PICARD_ITERATIONS)
    ;




  /*py::class_<Ctc, std::unique_ptr<Ctc>, pyStaticCtc > static_ctc(m, "Ctc", "todo");
  static_ctc.def(py::init<int>())
    // .def("contract",(void (ibex::Ctc::*)(IntervalVector&)) &Ctc::contract, DOC_CTC_CONTRACT)
    .def_readonly("nb_var", &Ctc::nb_var, "todo")
    //.def("__or__", &__or, "todo", py::return_value_policy::take_ownership, py::keep_alive<0,1>(),py::keep_alive<0,2>()  )
    //.def("__and__", &__and, "todo", py::return_value_policy::take_ownership, py::keep_alive<0,1>(),py::keep_alive<0,2>())
  ;*/

    py::class_<CtcFunction> ctc_function(m, "CtcFunction", static_ctc, "todo");
    ctc_function
      .def(py::init<const ibex::Function &>(),
          CTCFUNCTION_CTCFUNCTION_FUNCTION, "f"_a)
      .def(py::init<const ibex::Function &,const ibex::Domain &>(),
          CTCFUNCTION_CTCFUNCTION_FUNCTION_DOMAIN, "f"_a, "y"_a)
      .def(py::init<const ibex::Function &,const ibex::Interval &>(),
          CTCFUNCTION_CTCFUNCTION_FUNCTION_INTERVAL, "f"_a, "y"_a)
      .def(py::init<const ibex::Function &,const ibex::IntervalVector &>(),
          CTCFUNCTION_CTCFUNCTION_FUNCTION_INTERVALVECTOR, "f"_a, "y"_a)
      .def("contract", (void (CtcFunction::*)(ibex::IntervalVector &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_INTERVALVECTOR, "x"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(TubeVector &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBEVECTOR, "x"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(Tube &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBE, "x1"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(Tube &,Tube &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE, "x1"_a.noconvert(), "x2"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(Tube &,Tube &,Tube &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE_TUBE, "x1"_a.noconvert(), "x2"_a.noconvert(), "x3"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(Tube &,Tube &,Tube &,Tube &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE_TUBE_TUBE, "x1"_a.noconvert(), "x2"_a.noconvert(), "x3"_a.noconvert(), "x4"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(Tube &,Tube &,Tube &,Tube &,Tube &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE_TUBE_TUBE_TUBE, "x1"_a.noconvert(), "x2"_a.noconvert(), "x3"_a.noconvert(), "x4"_a.noconvert(), "x5"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(Tube &,Tube &,Tube &,Tube &,Tube &,Tube &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE_TUBE_TUBE_TUBE_TUBE, "x1"_a.noconvert(), "x2"_a.noconvert(), "x3"_a.noconvert(), "x4"_a.noconvert(), "x5"_a.noconvert(), "x6"_a.noconvert())
      //.def("contract", (void (CtcFunction::*)(Slice * *) )&CtcFunction::contract,
      //    CTCFUNCTION_VOID_CONTRACT_SLICE, "v_x_slices"_a)
    ;

    py::class_<CtcDist> ctc_dist(m, "CtcDist", static_ctc, "todo");
    ctc_dist
      .def(py::init<>(),CTCDIST_CTCDIST)
      .def("contract", &CtcDist::contract,
          CTCDIST_VOID_CONTRACT_INTERVALVECTOR_INTERVALVECTOR_INTERVAL, "a"_a.noconvert(), "b"_a.noconvert(), "d"_a.noconvert())

    ;


    py::class_<CtcConstell> ctc_constell(m, "CtcConstell", static_ctc, "todo");
    ctc_constell
      .def(py::init<const std::list<ibex::IntervalVector> &>(),
          CTCCONSTELL_CTCCONSTELL_VECTORINTERVALVECTOR, "map"_a)
      .def("contract", &CtcConstell::contract,
          CTCCONSTELL_VOID_CONTRACT_INTERVALVECTOR, "beacon_box"_a.noconvert())
    ;
}