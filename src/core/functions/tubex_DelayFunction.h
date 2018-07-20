/* ============================================================================
 *  tubex-lib - DelayFunction class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_DELAYFUNCTION_H__
#define __TUBEX_DELAYFUNCTION_H__

#include "tubex_Fnc.h"

namespace tubex
{
  class DelayFunction : public Fnc
  {
    public:

      DelayFunction(double delay);

      using Fnc::eval;
      const ibex::IntervalVector eval(const ibex::Interval& t, const TubeVector& x) const;

    protected:

      double m_delay;
  };
}

#endif