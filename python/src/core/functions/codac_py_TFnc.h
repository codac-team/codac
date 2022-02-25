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

#ifndef __CODAC_PY_TFNC_H__
#define __CODAC_PY_TFNC_H__

#include <pybind11/pybind11.h>
#include "codac_type_caster.h"

#include "codac_Tube.h"
#include "codac_TubeVector.h"
#include "codac_TFnc.h"


namespace codac
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

      const Interval eval(const IntervalVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const Interval, TFnc, eval, x);
      }

      const Interval eval(int slice_id,const TubeVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const Interval, TFnc, eval, slice_id, x);
      }

      const Interval eval(const Interval &t,const TubeVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const Interval, TFnc, eval, t, x);
      }

      const TubeVector eval_vector(const TubeVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const TubeVector, TFnc, eval_vector, x);
      }

      const IntervalVector eval_vector(const IntervalVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const IntervalVector, TFnc, eval_vector, x);
      }

      const IntervalVector eval_vector(int slice_id,const TubeVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const IntervalVector, TFnc, eval_vector, slice_id, x);
      }

      const IntervalVector eval_vector(const Interval &t,const TubeVector &x) const override
      {
        PYBIND11_OVERLOAD_PURE(const IntervalVector, TFnc, eval_vector, t, x);
      }
  };
}

#endif