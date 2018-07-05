/* ============================================================================
 *  tubex-lib - Fnc class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_FNC_H__
#define __TUBEX_FNC_H__

#include "ibex_IntervalVector.h"

namespace tubex
{
  class TubeVector;
  
  class Fnc
  {
    public:

      Fnc();
      Fnc(int n, int m);
      ~Fnc();

      int nbVars() const;
      int imageDim() const;

      virtual const ibex::IntervalVector eval(double t, const ibex::IntervalVector& x = ibex::IntervalVector(1)) const = 0;
      virtual const ibex::IntervalVector eval(const ibex::Interval& t, const ibex::IntervalVector& x = ibex::IntervalVector(1)) const = 0;
      virtual const TubeVector eval(const TubeVector& x) const = 0;

    protected:

      int m_nb_vars = -1, m_img_dim = -1;
  };
}

#endif