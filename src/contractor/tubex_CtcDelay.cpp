/* ============================================================================
 *  tubex-lib - CtcDelay class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2017
 * ---------------------------------------------------------------------------- */

#include "tubex_CtcDelay.h"
#include "tubex_TubeSlice.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDelay::CtcDelay()
  {

  }

  bool CtcDelay::contract(Interval& a, Tube& x, Tube& y)
  {
    cout << "CtcDelay: starting" << endl;
    int i = 0;
    double volume;

    do
    {
      // todo: gates
      volume = x.volume() + y.volume() + a.diam(); // todo: optimized version of this?

      /*TubeSlice *x_slice = x.getFirstSlice();
      while(x_slice != NULL)
      {
        Interval inv = y.invert(x_slice->codomain()) & x.domain();

        if(inv.lb() == x.domain().lb() || inv.ub() == x.domain().ub())
          continue;

        a &= inv - x_slice->domain();
        x_slice = x_slice->nextSlice();
      }

      TubeSlice *y_slice = y.getFirstSlice();
      while(y_slice != NULL)
      {
        Interval inv = x.invert(y_slice->codomain()) & x.domain();
        if(inv.lb() == x.domain().lb() || inv.ub() == x.domain().ub())
          continue;

        a &= y_slice->domain() - inv;
        y_slice = y_slice->nextSlice();
      }*/

      TubeSlice *x_slice = x.getFirstSlice();
      while(x_slice != NULL)
      {
        Interval inv = (x_slice->domain() + a) & y.domain();
        if(inv.is_empty() || inv.lb() == y.domain().lb() || inv.ub() == y.domain().ub())
        {
          x_slice = x_slice->nextSlice();
          continue;
        }
        x_slice->set(x_slice->codomain() & y[inv]);
        x_slice = x_slice->nextSlice();
      }

      TubeSlice *y_slice = y.getFirstSlice();
      while(y_slice != NULL)
      {
        Interval inv = (y_slice->domain() - a) & x.domain();
        if(inv.is_empty() || inv.lb() == x.domain().lb() || inv.ub() == x.domain().ub())
        {
          y_slice = y_slice->nextSlice();
          continue;
        }
        y_slice->set(y_slice->codomain() & x[inv]);
        y_slice = y_slice->nextSlice();
      }

      i++;
      cout << "iteration " << i << " \ta=" << a << endl;

      if(a.is_empty())
        break;

    } while(volume != x.volume() + y.volume() + a.diam());

    cout << "CtcDelay: ending" << endl;
    return i != 1;
  }
}