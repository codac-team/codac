/** 
 *  \file
 *  CtcFunction class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCCENTEREDFORM__
#define __CODAC2_CTCCENTEREDFORM__

#include <map>
#include "codac2_CtcFunction.h"
#include "codac2_Expr_operations.h"

namespace codac2
{
  class CtcCenteredForm
  {
    public:

      CtcCenteredForm(const Function<IntervalVector>& f, const Function<IntervalMatrix>& J, const IntervalVector& y)
        : _ctc_f(CtcFunction<IntervalVector>(f,y))
      {
        ArgVector x(3), z(3), m(3);
        _ctc_g = std::make_shared<CtcFunction<IntervalVector>>(
          Function<IntervalVector>({x,z,m}, f(m)+J(z)*(x-m)), y);
      }

      void contract(IntervalVector& x) const
      {
        IntervalVector prev_x(x);

        do
        {
          prev_x = x;
          IntervalVector z(x), m(x.mid());
          _ctc_f.contract(x);
          _ctc_g->contract(x,z,m);

        // Fixed point is considered as reached if less than (100*_ratio)% of contraction is obtained
        } while(!x.is_empty() && prev_x.volume()-x.volume() > _ratio*prev_x.volume());
      }

      void set_fixed_point_ratio(double r)
      {
        _ratio = r;
      }

    protected:

      double _ratio = 1.;
      CtcFunction<IntervalVector> _ctc_f;
      std::shared_ptr<CtcFunction<IntervalVector>> _ctc_g;
  };
}

#endif