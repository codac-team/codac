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

namespace codac2
{
  template<class T>
  class Slice;

  template<typename T>
  class SlicedTube;
  
  class CtcDeriv
  {
    public:

      CtcDeriv();
      void contract(Slice<Interval>& x, const Slice<Interval>& v) const;
      void contract(Slice<IntervalVector>& x, const Slice<IntervalVector>& v) const;

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
  };
}