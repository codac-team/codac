/** 
 *  \file codac2_CtcEval.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"
#include "codac2_SlicedTube.h"

namespace codac2
{
  class CtcEval
  {
    public:

      CtcEval()
      { }

      template<typename T>
      void contract(Interval& t, T& z, SlicedTube<T>& x, const SlicedTube<T>& v) const
        requires std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>
      {
        assert_release(TDomain::are_same(x.tdomain(), v.tdomain()));
        assert_release(x.size() == v.size());
        assert_release(z.size() == x.size());

        if(t.is_empty() || z.is_empty() || x.is_empty() || v.is_empty())
        {
          t.set_empty();
          z.set_empty();
          x.set(x.empty_value());
          return;
        }

        t &= x.invert(z, v, t);

        if(!t.is_empty())
        {
          z &= x(t, v);

          if(t.is_degenerated())
          {
            x.set(x(t.lb()) & z, t.lb());
            // This will create a new gate at t
            return;
          }

          // Sampling the tube with the creation of two gates
          auto it_t_beg = x.tdomain()->sample(t.lb(),true);
          auto it_t = it_t_beg;
          auto it_t_end = std::next(x.tdomain()->sample(t.ub(),true));

          // For each t in [t]
          while(it_t != it_t_end)
          {
            if(it_t->is_gate())
            {
              T slices_union = x.empty_value();

              // For each t1 in [t]
              auto it_t1 = x.tdomain()->sample(t.lb(),true);
              auto it_t1_end = it_t_end;
              while(it_t1 != it_t1_end)
              {
                if(it_t1->is_gate())
                {
                  T y1 = x(it_t1->lb()) & z;

                  if(!y1.is_empty())
                  {
                    // Transporting y1 to t
                    auto it_t1_t = it_t1;
                    auto it_t1_t_end = it_t;
                    while(it_t1_t != it_t1_t_end)
                    {
                      if(!it_t1_t->is_gate())
                      {
                        // /!\ .diam() method is not reliable (floating point result)
                        // -> We need to compute the diameter with intervals
                        Interval d = Interval(it_t1_t->ub())-Interval(it_t1_t->lb());
                        if(it_t1->lb() > it_t->lb())
                          d *= -1; // going backward
                        y1 += d*v.slice(it_t1_t)->codomain();
                      }

                      if(it_t1->lb() < it_t->lb())
                        it_t1_t++;
                      else
                        it_t1_t--;
                    }

                    slices_union |= y1;
                  }
                }

                it_t1++;
              }

              x.slice(it_t)->set(slices_union);
            }

            it_t++;
          }

          CtcDeriv ctc_deriv(TimePropag::FWD_BWD, true);
          ctc_deriv.restrict_tdomain(t);
          ctc_deriv.contract(x,v);
        }

        if(t.is_empty() || z.is_empty() || x.is_empty())
        {
          t.set_empty();
          z.set_empty();
          x.set(x.empty_value());
        }
      }
  };
}