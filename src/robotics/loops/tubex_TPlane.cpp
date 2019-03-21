/** 
 *  TPlane class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_TPlane.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  TPlane::TPlane(const IntervalVector& tbox)
    : Paving(tbox, VALUE_MAYBE)
  {
    assert(tbox.size() == 2);
  }

  void TPlane::compute(float precision, const TubeVector& x, const TubeVector& v)
  {
    assert(precision > 0.);
    assert(x.size() == 2);
    assert(v.size() == 2);

    if(value() == VALUE_OUT)
      return;

    else if(!is_leaf())
    {
      ((TPlane*)m_first_subpaving)->compute(precision, x, v);
      ((TPlane*)m_second_subpaving)->compute(precision, x, v);
    }

    else
    {
      const Interval t1 = m_box[0], t2 = m_box[1];
      const IntervalVector box_neg_reals(2, Interval::NEG_REALS);
      const IntervalVector box_pos_reals(2, Interval::POS_REALS);

      // Based on derivative information
      
        const pair<IntervalVector, IntervalVector> partial_integ = v.partial_integral(t1, t2);
        const IntervalVector integ = IntervalVector(partial_integ.first.lb()) | partial_integ.second.ub();

        bool derivative_out = Interval::POS_REALS.is_strict_superset(t1 - t2)
                           || !integ.interior_contains(Vector(2, 0.)) 
                           || !v(t1 | t2).interior_contains(Vector(2, 0.));

        bool derivative_in = Interval::NEG_REALS.is_strict_superset(t1 - t2)
                          && box_neg_reals.is_strict_superset(partial_integ.first)
                          && box_pos_reals.is_strict_superset(partial_integ.second);
      
      // Based on primitive information (<=> kernel)

        const pair<IntervalVector,IntervalVector> uy1 = x.eval(t1), uy2 = x.eval(t2);
        const pair<IntervalVector,IntervalVector> enc_bounds =
          make_pair(IntervalVector(uy1.first.lb() - uy2.second.ub()) | ( uy1.first.ub() - uy2.second.lb()),
                    IntervalVector(uy1.second.lb() - uy2.first.ub()) | (uy1.second.ub() - uy2.first.lb()));

        bool primitive_out = Interval::POS_REALS.is_strict_superset(t1 - t2)
                          || box_pos_reals.is_strict_superset(enc_bounds.first)
                          || box_neg_reals.is_strict_superset(enc_bounds.second);

        bool primitive_in = Interval::NEG_REALS.is_strict_superset(t1 - t2)
                         && box_neg_reals.is_strict_superset(enc_bounds.first)
                         && box_pos_reals.is_strict_superset(enc_bounds.second);

      // Conclusion

        if(derivative_out || primitive_out)
          set_value(VALUE_OUT);

        else if(derivative_in && primitive_in)
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