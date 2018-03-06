/* ============================================================================
 *  tubex-lib - CtcDeriv class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_CtcDeriv.h"
#include "tubex_DomainException.h"
#include "tubex_EmptyException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDeriv::CtcDeriv()
  {

  }

  bool CtcDeriv::contract(Tube& x, const Tube& v, const Interval& x0)
  {
    bool contraction = contractFwd(x, v, x0);
    contraction |= contractBwd(x, v);
    return contraction;
  }

  bool CtcDeriv::contractFwd(Tube& x, const Tube& v, const Interval& x0) // temporal forward
  {
    checkStructures(x, v);
    checkEmptiness(v);

    int size = x.size();
    bool contraction = false;

    Interval next_y = x[0];
    Interval y_front = next_y & x0;

    for(int i = 0 ; i < size ; i++) // from the past to the future
    {
      double dt = x.domain(i).diam();
      Interval y_old = next_y;
      Interval y_new = y_old & (y_front + v[i] * Interval(0., dt));
      contraction |= y_new.diam() < y_old.diam();
      x.set(y_new, i);

      // Discontinuous
      if(y_new.is_empty())
      {
        x.set(Interval::EMPTY_SET);
        contraction = true;
        break;
      }

      // Preparing next slice computation
      if(i < size - 1)
      {
        y_front = y_old & (y_front + v[i] * dt);
        next_y = x[i + 1];
        y_front &= next_y;
      }
    }

    return contraction;
  }

  bool CtcDeriv::contractBwd(Tube& x, const Tube& v) // temporal backward
  {
    checkStructures(x, v);
    checkEmptiness(v);

    int size = x.size();
    bool contraction = false;

    Interval next_y = x[size - 1];
    Interval y_front = next_y & next_y - v[size - 1] * v.domain(size - 1).diam();
    next_y = x[max(0, size - 2)];

    for(int i = max(0, size - 2) ; i >= 0 ; i--) // from the future to the past
    {
      double dt = x.domain(i).diam();
      Interval y_old = x[i];
      Interval y_new = y_old & (y_front - v[i] * Interval(0., dt));
      contraction |= y_new.diam() < y_old.diam();
      x.set(y_new, i);

      // Discontinuous
      if(y_new.is_empty())
      {
        x.set(Interval::EMPTY_SET);
        contraction = true;
        break;
      }

      if(i > 0)
      {
        y_front = y_old & (y_front - v[i] * dt);
        next_y = x[i - 1];
        y_front &= next_y;
      }
    }

    return contraction;
  }
}