//============================================================================
//                                P Y I B E X
// File        : pyIbex_Ctc.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <codac_type_caster.h>
namespace py = pybind11;
using py::self;
using namespace pybind11::literals;

#include "codac_CtcHull.h"
#include <codac_QInterProjF.h>
#include <ibex_LargestFirst.h>
#include "codac_py_Ctc_unsupported_docs.h"
#include "../../core/contractors/static/codac_py_Ctc.h"

using namespace codac;



void export_unsupported_ctc(py::module& m, py::class_<Ctc, pyCtc>& ctc){





  // // Export CtcQInterProjF
  // py::class_<CtcQInterProjF>(m, "CtcQInterProjF", ctc, DOC_CTCQINTERPROJF_TYPE)
  //   .def(py::init<Array<Ctc>, int>(), py::keep_alive<1,2>(), "list"_a, "q"_a)
  //   .def("contract", (void (Ctc::*) (IntervalVector&)) &CtcQInterProjF::contract)
  //   ;


  // Export CtcHull
  py::class_<codac::CtcHull>(m, "CtcHull", ctc, DOC_CTCHULL_TYPE)
    .def(py::init<ibex::Sep&, double, ibex::Bsc&>(),
            py::keep_alive<1,2>(), py::keep_alive<1,4>(),
            "sep"_a, "epsilon"_a, "bsc"_a = ibex::LargestFirst(1e-10)
          )
    .def("contract", &codac::CtcHull::contract, py::arg("box").noconvert())
    ;
}