/* ============================================================================
 *  tubex-lib - CtcOut class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_CtcOut.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcOut::CtcOut()
  {

  }

  bool CtcOut::contract(const Interval& t, const Interval& z, Tube& y)
  {
    Interval intersected_t = t & y.domain();
    bool contraction = false;
    pair<Interval,Interval> enc_bounds = y.eval(intersected_t);

    if(z.intersects(enc_bounds.first))
      #pragma omp parallel num_threads(omp_get_num_procs())
      {
        #pragma omp for
        for(int i = y.input2index(intersected_t.lb()) ; i < y.input2index(intersected_t.ub()) ; i++)
        {
          Interval old_y = y[i];
          Interval new_y(max(z.ub(), old_y.lb()), old_y.ub());
          contraction |= new_y.diam() < old_y.diam();
          y.set(new_y, i);
        }
      }

    else if(z.intersects(enc_bounds.second))
      #pragma omp parallel num_threads(omp_get_num_procs())
      {
        #pragma omp for
        for(int i = y.input2index(intersected_t.lb()) ; i < y.input2index(intersected_t.ub()) ; i++)
        {
          Interval old_y = y[i];
          Interval new_y(old_y.lb(), min(old_y.ub(), z.lb()));
          contraction |= new_y.diam() < old_y.diam();
          y.set(new_y, i);
        }
      }

    else
      return false; // surely no contraction

    return contraction;
  }
}