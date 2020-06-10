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

#ifndef __TUBEX_PY_CTC_H__
#define __TUBEX_PY_CTC_H__

#include "tubex_DynCtc.h"
#include "tubex_CtcFunction.h"
#include "tubex_CtcDist.h"
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
#include "tubex_CtcPicard.h"
#include "tubex_CtcConstell.h"
#include "tubex_Domain.h"
#include "tubex_CtcFunction.h"

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


class pyCtc : public Ctc {
  // protected:
public:
  pyCtc(int v): Ctc(v){}
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


#endif