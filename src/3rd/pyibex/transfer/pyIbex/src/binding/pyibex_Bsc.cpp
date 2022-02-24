//============================================================================
//                                P Y I B E X
// File        : pyIbex_Bsc.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Oct 31, 2014
//============================================================================

#include <ibex_Bsc.h>
#include <ibex_LargestFirst.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pyIbex_type_caster.h>
#include "pyibex_doc_Bsc.h"

namespace py = pybind11;
using py::class_;
using py::init;
using py::keep_alive;

using ibex::LargestFirst;
using ibex::Bsc;
using ibex::IntervalVector;
using ibex::Cell;
using ibex::Vector;


typedef std::pair<IntervalVector,IntervalVector> ItvPair;

class pyBsc : public Bsc {
public:
  /* Inherit the constructors */
  using Bsc::Bsc;
  /* Trampoline (need one for each virtual function) */
  virtual ItvPair bisect(const IntervalVector& box){
    PYBIND11_OVERLOAD_PURE(
      ItvPair,       /* return type */
      Bsc,           /* Parent class */
      bisect,        /* Name of function */
      box            /* Argument(s) */
    );
  }
  // virtual ItvPair bisect(Cell& cell){
  //   PYBIND11_OVERLOAD(
  //     ItvPair,       /* return type */
  //     Bsc,           /* Parent class */
  //     bisect,        /* Name of function */
  //     cell           /* Argument(s) */
  //   );
  // }

  // virtual void add_backtrackable(Cell& root){
  //   PYBIND11_OVERLOAD(
  //     void,               /* return type */
  //     Bsc,                /* Parent class */
  //     add_backtrackable,  /* Name of function */
  //     root                /* Argument(s) */
  //   );
  // }
};


void export_Bsc(py::module& m){
	// Overloading bisection Function
	typedef std::pair<IntervalVector,IntervalVector> (Bsc::*bisect_1) (const IntervalVector&);
	// typedef std::pair<IntervalVector,IntervalVector> (Bsc::*bisect_2) (Cell&);

	// Bsc Class binding
	class_<Bsc, std::unique_ptr<Bsc>, pyBsc> bsc(m, "Bsc", DOCS_BSC_TYPE);
    bsc.def("bisect", (ItvPair (Bsc::*) (const IntervalVector& box) )(&Bsc::bisect), DOCS_BSC_BISECT, py::arg("x"))
    // .def("bisect", bisect_2(&Bsc::bisect))
    // .def("add_backtrackable", &Bsc::add_backtrackable)
		.def( "default_ratio", &Bsc::default_ratio , DOCS_BSC_DEFAULT_RATIO)
		.def( "uniform_prec", &Bsc::uniform_prec, DOCS_BSC_UNIFORM_PREC)
		.def( "prec", &Bsc::prec, DOCS_BSC_PREC)
		.def( "too_small", &Bsc::too_small,DOCS_BSC_TOO_SMALL )
    ;

    // // LargestFirst Bisector binding
    class_<LargestFirst>(m, "LargestFirst", bsc, DOCS_LARGEST_FIRST)
      .def(init<double, double>(), py::arg("prec")=0, py::arg("ratio")=Bsc::default_ratio())
      .def(init<Vector&, double>(), py::arg("prec"), py::arg("ratio")=Bsc::default_ratio())
    	.def("bisect", (ItvPair (LargestFirst::*) (const IntervalVector& box) ) &LargestFirst::bisect)
    	;
}
