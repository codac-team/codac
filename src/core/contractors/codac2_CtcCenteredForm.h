/** 
 *  \file codac2_CtcCenteredForm.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_CtcInverse.h"
#include "codac2_analytic_operations.h"
#include "codac2_Collection.h"

namespace codac2
{
  class CtcCenteredForm : public Ctc_<IntervalVector>
  {
    public:

      CtcCenteredForm(const AnalyticFunction<VectorOpValue>& f, const AnalyticFunction<MatrixOpValue>& J, const IntervalVector& y)
        : Ctc_<IntervalVector>(f.args()[0]->size() /* f must have only one arg, see following assert */),
          _ctc_f(CtcInverse<IntervalVector>(f,y))
      {
        assert(f.args().size() == 1);
        
        VectorVar x(3), z(3), m(3);
        _ctc_g.add(CtcInverse<IntervalVector>(
          AnalyticFunction<VectorOpValue>({x,z,m}, f(m)+J(z)*(x-m)),
          y
        ));
      }

      std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<CtcCenteredForm>(*this);
      }

      void contract(IntervalVector& x) const
      {
        IntervalVector prev_x(x);

        do
        {
          prev_x = x;
          IntervalVector z(x), m(x.mid());
          _ctc_f.contract(x);
          _ctc_g.front().contract(x,z,m);

        // Fixed point is considered as reached if less than (100*_ratio)% of contraction is obtained
        } while(!x.is_empty() && prev_x.volume()-x.volume() > _ratio*prev_x.volume());
      }

      void set_fixpt_ratio(double r)
      {
        _ratio = r;
      }

    protected:

      double _ratio = 1.;
      CtcInverse<IntervalVector> _ctc_f;
      Collection<CtcInverse<IntervalVector>> _ctc_g;
  };
}