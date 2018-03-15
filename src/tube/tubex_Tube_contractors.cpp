/* ============================================================================
 *  tubex-lib - Tube class (contractors methods)
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Tube.h"
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
#include "tubex_CtcOut.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    bool Tube::ctcFwd(const Tube& derivative, const Interval& initial_value)
    {
      
    }
    
    bool Tube::ctcBwd(const Tube& derivative)
    {
      
    }
    
    bool Tube::ctcFwdBwd(const Tube& derivative, const Interval& initial_value)
    {
      
    }
    
    bool Tube::ctcEval(const Tube& derivative, Interval& t, Interval& z, bool propagate)
    {
      
    }
    
    bool Tube::ctcEval(const Tube& derivative, Interval& t, const Interval& z, bool propagate)
    {
      
    }
    
    bool Tube::ctcEval(const Tube& derivative, const Interval& t, Interval& z, bool propagate)
    {
      
    }
    
    bool Tube::ctcEval(const Tube& derivative, const Interval& t, const Interval& z, bool propagate)
    {
      
    }
    
    bool Tube::ctcOut(const Interval& t, const Interval& z)
    {
      
    }
    
    bool Tube::ctcIntertemporal(Interval& t1, Interval& t2) const
    {
      
    }

    bool Tube::ctcIntertemporal(Interval& z, Interval& t1, Interval& t2) const
    {
      
    }

    bool Tube::ctcPeriodic(const Interval& period)
    {
      
    }

  // Protected methods

    void Tube::ctcEval_computeIndex(const Interval& z, const Interval& t, int& index_lb, int& index_ub)
    {

    }
}