/* ============================================================================
 *  tubex-lib - TPlane class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : -
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_TPlane.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  TPlane::TPlane(const IntervalVector& tbox)
    : Paving(tbox, VALUE_MAYBE)
  {
    // todo: check tbox dim 2

  }

  void TPlane::compute(float precision, const TubeVector& x, const TubeVector& v)
  {
    // todo: check precision, x, v

    if(value() == VALUE_OUT)
      return;

    else if(!is_leaf())
    {
      ((TPlane*)m_first_subpaving)->compute(precision, x, v);
      ((TPlane*)m_second_subpaving)->compute(precision, x, v);
    }

    else
    {
      Interval t1 = m_box[0], t2 = m_box[1];

      // Based on derivative information
      
        pair<Interval, Interval> partial_integ_x = v[0].partial_integral(t1, t2);
        pair<Interval, Interval> partial_integ_y = v[1].partial_integral(t1, t2);

        Interval integ_x = Interval(partial_integ_x.first.lb(), partial_integ_x.second.ub());
        Interval integ_y = Interval(partial_integ_y.first.lb(), partial_integ_y.second.ub());

        bool is_out__derivative = Interval::POS_REALS.is_strict_superset(t1 - t2)
                               || !integ_x.interior_contains(0) 
                               || !integ_y.interior_contains(0)
                               || !v[0](Interval(t1.lb(), t2.ub())).interior_contains(0) 
                               || !v[1](Interval(t1.lb(), t2.ub())).interior_contains(0);

        bool is_in__derivative = Interval::NEG_REALS.is_strict_superset(t1 - t2)
                              && Interval::NEG_REALS.is_strict_superset(partial_integ_x.first)
                              && Interval::POS_REALS.is_strict_superset(partial_integ_x.second)
                              && Interval::NEG_REALS.is_strict_superset(partial_integ_y.first)
                              && Interval::POS_REALS.is_strict_superset(partial_integ_y.second);
      
      // Based on primitive information (<=> kernel)

        pair<Interval,Interval> uy1_x = x[0].eval(t1);
        pair<Interval,Interval> uy2_x = x[0].eval(t2);
        pair<Interval,Interval> enc_bounds_x = make_pair(Interval(uy1_x.first.lb() - uy2_x.second.ub(), uy1_x.first.ub() - uy2_x.second.lb()),
                                                         Interval(uy1_x.second.lb() - uy2_x.first.ub(), uy1_x.second.ub() - uy2_x.first.lb()));

        pair<Interval,Interval> uy1_y = x[1].eval(t1);
        pair<Interval,Interval> uy2_y = x[1].eval(t2);
        pair<Interval,Interval> enc_bounds_y = make_pair(Interval(uy1_y.first.lb() - uy2_y.second.ub(), uy1_y.first.ub() - uy2_y.second.lb()),
                                                         Interval(uy1_y.second.lb() - uy2_y.first.ub(), uy1_y.second.ub() - uy2_y.first.lb()));

        bool is_out__primitive = Interval::POS_REALS.is_strict_superset(t1 - t2)
                              || Interval::POS_REALS.is_strict_superset(enc_bounds_x.first)
                              || Interval::NEG_REALS.is_strict_superset(enc_bounds_x.second)
                              || Interval::POS_REALS.is_strict_superset(enc_bounds_y.first)
                              || Interval::NEG_REALS.is_strict_superset(enc_bounds_y.second);

        bool is_in__primitive = Interval::NEG_REALS.is_strict_superset(t1 - t2)
                             && Interval::NEG_REALS.is_strict_superset(enc_bounds_x.first)
                             && Interval::POS_REALS.is_strict_superset(enc_bounds_x.second)
                             && Interval::NEG_REALS.is_strict_superset(enc_bounds_y.first)
                             && Interval::POS_REALS.is_strict_superset(enc_bounds_y.second);

      // Conclusion

      if(is_out__derivative || is_out__primitive)
        set_value(VALUE_OUT);

      else if(is_in__derivative && is_in__primitive)
        set_value(VALUE_IN);

      else if(max(t1.diam(), t2.diam()) < precision)
        set_value(VALUE_MAYBE);

      else
      {
        bisect();
        ((TPlane*)m_first_subpaving)->compute(precision, x, v);
        ((TPlane*)m_second_subpaving)->compute(precision, x, v);
      }
    }
  }
}