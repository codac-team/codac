/** 
 *  \file
 *  Ctc Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_PY_TFNC_H__
#define __TUBEX_PY_TFNC_H__

#include <pybind11/pybind11.h>
#include "pyIbex_type_caster.h"

#include "tubex_Tube.h"
#include "tubex_TubeVector.h"
#include "tubex_TFnc.h"


namespace tubex
{
  class pyTFnc : public TFnc
  {
    public:

      using TFnc::TFnc;

      // Trampoline (need one for each virtual function)

      const Tube eval(const TubeVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const Tube, TFnc, eval, x);
      }

      const ibex::Interval eval(const ibex::IntervalVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const ibex::Interval, TFnc, eval, x);
      }

      const ibex::Interval eval(int slice_id,const TubeVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const ibex::Interval, TFnc, eval, slice_id, x);
      }

      const ibex::Interval eval(const ibex::Interval &t,const TubeVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const ibex::Interval, TFnc, eval, t, x);
      }

      const TubeVector eval_vector(const TubeVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const TubeVector, TFnc, eval_vector, x);
      }

      const ibex::IntervalVector eval_vector(const ibex::IntervalVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const ibex::IntervalVector, TFnc, eval_vector, x);
      }

      const ibex::IntervalVector eval_vector(int slice_id,const TubeVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const ibex::IntervalVector, TFnc, eval_vector, slice_id, x);
      }

      const ibex::IntervalVector eval_vector(const ibex::Interval &t,const TubeVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const ibex::IntervalVector, TFnc, eval_vector, t, x);
      }
  };
}

#endif