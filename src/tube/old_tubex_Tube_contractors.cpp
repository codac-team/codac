/* ============================================================================
 *  tubex-lib - Tube class
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
  bool Tube::ctcFwd(const Tube& derivative_tube, const ibex::Interval& initial_value)
  {
    CtcDeriv ctc;
    return ctc.contractFwd(*this, derivative_tube, initial_value);
  }

  bool Tube::ctcBwd(const Tube& derivative_tube)
  {
    CtcDeriv ctc;
    return ctc.contractBwd(*this, derivative_tube);
  }
  
  bool Tube::ctcFwdBwd(const Tube& derivative_tube, const ibex::Interval& initial_value)
  {
    CtcDeriv ctc;
    return ctc.contract(*this, derivative_tube, initial_value);
  }

  bool Tube::ctcEval(const Tube& derivative_tube, Interval& t, Interval& y, bool fwd_bwd)
  {
    CtcEval ctc;
    return ctc.contract(t, y, *this, derivative_tube, fwd_bwd);
  }

  bool Tube::ctcEval(const Tube& derivative_tube, Interval& t, const Interval& y, bool fwd_bwd)
  {
    CtcEval ctc;
    Interval y_temp = y;
    return ctc.contract(t, y_temp, *this, derivative_tube, fwd_bwd);
  }

  bool Tube::ctcEval(const Tube& derivative_tube, const Interval& t, Interval& y, bool fwd_bwd)
  {
    CtcEval ctc;
    Interval t_temp = t;
    return ctc.contract(t_temp, y, *this, derivative_tube, fwd_bwd);
  }

  bool Tube::ctcEval(const Tube& derivative_tube, const Interval& t, const Interval& y, bool fwd_bwd)
  {
    CtcEval ctc;
    Interval y_temp = y;
    Interval t_temp = t;
    return ctc.contract(t_temp, y_temp, *this, derivative_tube, fwd_bwd);
  }

  bool Tube::ctcOut(const Interval& t, const Interval& y)
  {
    CtcOut ctc;
    return ctc.contract(t, y, *this);
  }

  bool Tube::ctcIntertemporal(Interval& t1, Interval& t2) const
  {
    bool contraction = false;
    double t1_diam, t2_diam;

    do
    {
      t1_diam = t1.diam();
      t2_diam = t2.diam();
      t1 &= invert((*this)[t2], t1);
      t2 &= invert((*this)[t1], t2);
      contraction |= t1.diam() < t1_diam || t2.diam() < t2_diam;
    } while(t1.diam() < t1_diam || t2.diam() < t2_diam);

    return contraction;
  }

  bool Tube::ctcIntertemporal(Interval& y, Interval& t1, Interval& t2) const
  {
    bool contraction = false;
    double y_diam = y.diam(), t1_diam, t2_diam;

    y &= (*this)[t1] | (*this)[t2];

    if(y.is_empty())
    {
      t1.set_empty();
      t2.set_empty();
      return true;
    }

    else
    {
      contraction |= y.diam() < y_diam;

      do
      {
        t1_diam = t1.diam();
        t2_diam = t2.diam();
        t1 &= invert(y, t1);
        t2 &= invert(y, t2);
        contraction |= t1.diam() < t1_diam || t2.diam() < t2_diam;
      } while(t1.diam() < t1_diam || t2.diam() < t2_diam);

      return contraction;
    }
  }

  bool Tube::ctcPeriodic(const Interval& period)
  {
    bool contraction, at_least_one_contraction = false;

    do
    {
      contraction = false;
      double vol = volume();

      for(int i = 0 ; i < size() ; i++)
      {
        int k;
        Interval shift_dom, dom = domain(i);

        k = 0; // Fwd
        do
        {
          k++;
          shift_dom = domain() & (dom + k*period);

          if(!shift_dom.is_empty())
            set((*this)[shift_dom] & (*this)[dom], dom);

        } while(!shift_dom.is_empty());

        k = 0; // Bwd
        do
        {
          k--;
          shift_dom = domain() & (dom + k*period);

          if(!shift_dom.is_empty())
            set((*this)[shift_dom] & (*this)[dom], dom);

        } while(!shift_dom.is_empty());
      }

      contraction = vol > volume();
      at_least_one_contraction |= contraction;
    } while(contraction);

    return at_least_one_contraction;
  }
}