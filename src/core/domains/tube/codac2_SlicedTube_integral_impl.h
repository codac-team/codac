/** 
 *  \file codac2_SlicedTube_integral_impl.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"

namespace codac2
{
  template<typename T>
  T SlicedTube<T>::integral(const Interval& t) const
  {
    auto partial_integ = partial_integral(t);

    if(partial_integ.first.is_empty() || partial_integ.second.is_empty())
      return this->empty_codomain();

    else if(partial_integ.first.is_unbounded() || partial_integ.second.is_unbounded())
      return this->all_reals_codomain();

    else
      return T(partial_integ.first.lb()) | partial_integ.second.ub();
  }

  template<typename T>
  T SlicedTube<T>::integral(const Interval& t1, const Interval& t2) const
  {
    auto integ_t1 = partial_integral(t1);
    auto integ_t2 = partial_integral(t2);

    if(integ_t1.first.is_empty() || integ_t1.second.is_empty() ||
       integ_t2.first.is_empty() || integ_t2.second.is_empty())
    {
      return this->empty_codomain();
    }

    else if(integ_t1.first.is_unbounded() || integ_t1.second.is_unbounded() ||
            integ_t2.first.is_unbounded() || integ_t2.second.is_unbounded())
    {
      return this->all_reals_codomain();
    }

    else
    {
      auto lb = (integ_t2.first - integ_t1.first).lb();
      auto ub = (integ_t2.second - integ_t1.second).ub();
      return T(lb) | ub;
    }
  }

  template<typename T>
  std::pair<T,T> SlicedTube<T>::partial_integral(const Interval& t) const
  {
    //if(!t.is_subset(tdomain()->t0_tf()))
    //  return { Interval(), Interval() };

    auto zero = this->all_reals_codomain();
    zero.init(Interval(0.));

    std::pair<T,T> p_integ { zero, zero };
    auto p_integ_uncertain = p_integ;

    for(const auto& si : *this)
    {
      if(si.t0_tf().lb() >= t.ub())
        break;

      if(si.codomain().is_empty())
      {
        auto e = this->empty_codomain();
        return { e, e };
      }

      if(si.codomain().is_unbounded())
      {
        auto u = this->all_reals_codomain();
        return { u, u };
      }

      // From t0 to tlb

        Interval intv_t = si.t0_tf() & Interval(tdomain()->t0_tf().lb(), t.lb());
        if(!intv_t.is_empty())
        {
          p_integ.first += intv_t.diam() * si.codomain().lb();
          p_integ.second += intv_t.diam() * si.codomain().ub();
          p_integ_uncertain = p_integ;

          if(intv_t.ub() == t.ub())
            return p_integ; // end of the integral evaluation
        }

      // From tlb to tub

        intv_t = si.t0_tf() & t;
        if(!intv_t.is_empty())
        {
          auto p_integ_temp = p_integ_uncertain;
          p_integ_uncertain.first += Interval(0., intv_t.diam()) * si.codomain().lb();
          p_integ_uncertain.second += Interval(0., intv_t.diam()) * si.codomain().ub();
        
          p_integ.first |= p_integ_uncertain.first;
          p_integ.second |= p_integ_uncertain.second;

          p_integ_uncertain.first = p_integ_temp.first + intv_t.diam() * si.codomain().lb();
          p_integ_uncertain.second = p_integ_temp.second + intv_t.diam() * si.codomain().ub();
        }
    }

    return p_integ;
  }

  template<typename T>
  std::pair<T,T> SlicedTube<T>::partial_integral(const Interval& t1, const Interval& t2) const
  {
    auto integ_t1 = partial_integral(t1);
    auto integ_t2 = partial_integral(t2);
    return {
      integ_t2.first - integ_t1.first,
      integ_t2.second - integ_t1.second
    };
  }
}