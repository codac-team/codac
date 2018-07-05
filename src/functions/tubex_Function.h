/* ============================================================================
 *  tubex-lib - Function class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_FUNCTION_H__
#define __TUBEX_FUNCTION_H__

#include "ibex_Function.h"
#include "tubex_Fnc.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  class TubeVector;

  class Function : public Fnc
  {
    public:

      Function(const char* y);
      Function(const char* x1, const char* y);
      Function(const tubex::Function& f);
      ~Function();
      const Function& operator=(const Function& f);

      const ibex::IntervalVector eval(double t, const ibex::IntervalVector& x = ibex::IntervalVector(1)) const;
      const ibex::IntervalVector eval(const ibex::Interval& t, const ibex::IntervalVector& x = ibex::IntervalVector(1)) const;
      const TubeVector eval(const TubeVector& x) const;

    protected:

      ibex::Function *m_ibex_f = NULL;
  };
}

#endif