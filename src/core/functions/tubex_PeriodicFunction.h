/* ============================================================================
 *  tubex-lib - PeriodicFunction class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_PERIODICFUNCTION_H__
#define __TUBEX_PERIODICFUNCTION_H__

#include "tubex_Fnc.h"

namespace tubex
{
  class PeriodicFunction : public Fnc
  {
    public:

      PeriodicFunction();

      using Fnc::eval; // todo: is this relevant?
      const ibex::Interval eval(const ibex::Interval& t, const TubeVector& x) const;
      const ibex::IntervalVector eval_vector(const ibex::Interval& t, const TubeVector& x) const;
  };
}

#endif