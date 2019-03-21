/** 
 *  DelayFunction class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_DELAYFUNCTION_H__
#define __TUBEX_DELAYFUNCTION_H__

#include "tubex_Fnc.h"

namespace tubex
{
  class DelayFunction : public Fnc
  {
    public:

      DelayFunction(double delay);

      using Fnc::eval; // todo: check this
      const ibex::Interval eval(int slice_id, const TubeVector& x) const;
      const ibex::Interval eval(const ibex::Interval& t, const TubeVector& x) const;
      const ibex::IntervalVector eval_vector(int slice_id, const TubeVector& x) const;
      const ibex::IntervalVector eval_vector(const ibex::Interval& t, const TubeVector& x) const;

    protected:

      double m_delay;
  };
}

#endif