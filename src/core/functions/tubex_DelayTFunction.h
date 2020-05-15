/** 
 *  DelayTFunction class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_DELAYTFUNCTION_H__
#define __TUBEX_DELAYTFUNCTION_H__

#include "tubex_TFnc.h"

namespace tubex
{
  class DelayTFunction : public TFnc
  {
    public:

      DelayTFunction(double delay);

      using TFnc::eval; // todo: check this
      const ibex::Interval eval(int slice_id, const TubeVector& x) const;
      const ibex::Interval eval(const ibex::Interval& t, const TubeVector& x) const;
      const ibex::IntervalVector eval_vector(int slice_id, const TubeVector& x) const;
      const ibex::IntervalVector eval_vector(const ibex::Interval& t, const TubeVector& x) const;

    protected:

      double m_delay;
  };
}

#endif