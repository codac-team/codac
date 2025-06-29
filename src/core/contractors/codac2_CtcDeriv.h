/** 
 *  \file codac2_CtcDeriv.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"
#include "codac2_Slice.h"
#include "codac2_Interval.h"

namespace codac2
{
  class CtcDeriv
  {
    public:

      CtcDeriv()
      { }

      inline void contract(Slice<Interval>& x, const Slice<Interval>& v) const
      {
        assert_release(x.t0_tf() == v.t0_tf());
        
        x.set(this->slice_hull({0,x.t0_tf().diam()},
          x.codomain(),x.input_gate(),x.output_gate(),
          v.codomain()
          ), false); // disable propagation to adjacent slices
        // v remains unchanged

        auto x_next = x.next_slice();
        if(x_next && x_next->is_gate())
        {
          x_next->set(
            x_next->codomain() &
            (x.input_gate()+x.t0_tf().diam()*v.codomain()),
          false); // disable propagation to adjacent slices
          // v remains unchanged
        }

        auto x_prev = x.prev_slice();
        if(x_prev && x_prev->is_gate())
        {
          x_prev->set(
            x_prev->codomain() &
            (x.output_gate()-x.t0_tf().diam()*v.codomain()),
          false); // disable propagation to adjacent slices
          // v remains unchanged
        }
      }

      inline void contract(Slice<IntervalVector>& x, const Slice<IntervalVector>& v) const
      {
        assert_release(x.size() == v.size());
        assert_release(x.t0_tf() == v.t0_tf());

        auto x_prev = x.prev_slice();
        auto x_next = x.next_slice();
        
        auto hull = x.codomain();
        auto input_gate = x.input_gate();
        auto output_gate = x.output_gate();

        for(Index i = 0 ; i < x.size() ; i++)
        {
          hull[i] = this->slice_hull({0,x.t0_tf().diam()},
            x.codomain()[i],x.input_gate()[i],x.output_gate()[i],
            v.codomain()[i]
          );

          if(x_next && x_next->is_gate())
            output_gate[i] &= input_gate[i]+x.t0_tf().diam()*v.codomain()[i];

          if(x_prev && x_prev->is_gate())
            input_gate[i] &= output_gate[i]-x.t0_tf().diam()*v.codomain()[i];
        }

        if(x_prev && x_prev->is_gate())
          x_prev->set(input_gate, false);

        if(x_next && x_next->is_gate())
          x_next->set(output_gate, false);

        x.set(hull, false); // disable propagation to adjacent slices
        // v remains unchanged
      }

      template<typename T>
      inline void contract(SlicedTube<T>& x, const SlicedTube<T>& v) const
      {
        assert_release(x.tdomain()->all_gates_defined() && "not available without gates");
        assert_release(x.tdomain() == v.tdomain());

        auto xi = x.begin(); auto vi = v.begin();
        while(xi != x.end())
        {
          assert(vi != v.end());
          if(!xi->is_gate())
            this->contract(*xi,*vi);
          xi++; vi++;
        }
        assert(vi == v.end());

        auto rxi = x.rbegin(); auto rvi = v.rbegin();
        while(rxi != x.rend())
        {
          assert(rvi != v.rend());
          if(!rxi->is_gate())
            this->contract(*rxi,*rvi);
          rxi++; rvi++;
        }
        assert(rvi == v.rend());
      }

    protected:

      Interval slice_hull(const Interval& dt,
        const Interval& x, const Interval& input_x, const Interval& output_x,
        const Interval& v) const
      {
        Interval hull = x;
        hull &= input_x  + dt*v;
        hull &= output_x - dt*v;
        return hull;
      }
  };
}