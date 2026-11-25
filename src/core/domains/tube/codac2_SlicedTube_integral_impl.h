/** 
 *  \file codac2_SlicedTube_integral_impl.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <tuple>
#include <utility>
#include "codac2_Interval.h"

namespace codac2
{
  template<typename T>
  T SlicedTube<T>::integral(const Interval& t) const
  {
    auto partial_integ = partial_integral(t);

    if(partial_integ.first.is_empty() || partial_integ.second.is_empty())
      return this->empty_value();

    else if(partial_integ.first.is_unbounded() || partial_integ.second.is_unbounded())
      return this->all_reals_value();

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
      return this->empty_value();
    }

    else if(integ_t1.first.is_unbounded() || integ_t1.second.is_unbounded() ||
            integ_t2.first.is_unbounded() || integ_t2.second.is_unbounded())
    {
      return this->all_reals_value();
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

    auto zero = this->all_reals_value();
    zero.init(Interval(0.));

    std::pair<T,T> p_integ { zero, zero };
    auto p_integ_uncertain = p_integ;

    for(const auto& si : *this)
    {
      if(si.t0_tf().lb() >= t.ub())
        break;

      if(si.codomain().is_empty())
      {
        auto e = this->empty_value();
        return { e, e };
      }

      if(si.codomain().is_unbounded())
      {
        auto u = this->all_reals_value();
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

  template<typename T>
  std::pair<SlicedTube<T>,SlicedTube<T>> SlicedTube<T>::partial_primitives() const
  {
    const Interval t = this->t0_tf();
    assert_release(!t.is_empty() && !t.is_unbounded());

    for(const auto& s : *this)
      if(!s.is_gate())
        assert_release(!s.codomain().is_empty() && !s.codomain().is_unbounded());
    auto zero = this->all_reals_value();
    zero.init(Interval(0.));

    std::pair<SlicedTube<T>,SlicedTube<T>> primitives(
      std::piecewise_construct,
      std::forward_as_tuple(this->tdomain(), zero),
      std::forward_as_tuple(this->tdomain(), zero));

    std::pair<T,T> p { zero, zero };

    for(auto it = _tdomain->begin(); it != _tdomain->end(); ++it)
    {
      auto pm = primitives.first.slice(it);
      auto pp = primitives.second.slice(it);

      if(it->is_gate())
      {
        // Gates have zero duration and do not contribute to the integral.
        pm->set(p.first, false);
        pp->set(p.second, false);
        continue;
      }

      const auto& x = this->slice(it)->codomain();
      auto q = p;

      // The interval duration encloses the exact difference of the endpoints.
      const Interval dt = Interval(it->ub()) - Interval(it->lb());
      q.first += dt * T(x.lb());
      q.second += dt * T(x.ub());

      // Each partial primitive is affine on a non-gate slice.
      // Its range is enclosed by the hull of its endpoint enclosures.
      pm->set(p.first | q.first, false);
      pp->set(p.second | q.second, false);
      p = q;
    }

    return primitives;
  }

  template<typename T>
  T SlicedTube<T>::integral(const Interval& t,
    const std::pair<SlicedTube<T>,SlicedTube<T>>& primitives) const
  {
    auto p = partial_integral(t, primitives);
    if(p.first.is_empty() || p.second.is_empty())
      return this->empty_value();
    if(p.first.is_unbounded() || p.second.is_unbounded())
      return this->all_reals_value();
    return T(p.first.lb()) | p.second.ub();
  }

  template<typename T>
  T SlicedTube<T>::integral(const Interval& t1, const Interval& t2,
    const std::pair<SlicedTube<T>,SlicedTube<T>>& primitives) const
  {
    auto p = partial_integral(t1, t2, primitives);
    if(p.first.is_empty() || p.second.is_empty())
      return this->empty_value();
    if(p.first.is_unbounded() || p.second.is_unbounded())
      return this->all_reals_value();

    if(t1.ub() <= t2.lb())
      return T(p.first.lb()) | p.second.ub();
    if(t2.ub() <= t1.lb())
      return T(p.second.lb()) | p.first.ub();

    // Both integration directions are possible when the time intervals overlap.
    return p.first | p.second;
  }

  template<typename T>
  std::pair<T,T> SlicedTube<T>::partial_integral(const Interval& t,
    const std::pair<SlicedTube<T>,SlicedTube<T>>& primitives) const
  {
    if(t.is_empty())
      return { this->empty_value(), this->empty_value() };
    if(!t.is_subset(this->t0_tf()))
      return { this->all_reals_value(), this->all_reals_value() };

    assert_release(primitives.first.tdomain() == this->tdomain() &&
                   primitives.second.tdomain() == this->tdomain());

    auto it = _tdomain->tslice(t.lb());
    assert(it != _tdomain->end() && it->contains(t.lb()));

    std::pair<T,T> p { this->empty_value(), this->empty_value() };

    for(; it != _tdomain->cend() && it->lb() <= t.ub(); ++it)
    {
      const Interval ti = t & it->t0_tf();
      const auto pm = primitives.first.slice(it);
      const auto pp = primitives.second.slice(it);
      std::pair<T,T> q { pm->codomain(), pp->codomain() };

      if(!it->is_gate())
      {
        const auto& x = this->slice(it)->codomain();

        if(!q.first.is_unbounded() && !q.second.is_unbounded() && ti != it->t0_tf())
        {
          // Reconstruct the affine primitives on the queried part of the slice.
          // With explicit gates, the endpoint enclosures are the cached sums.
          const Interval dt = ti - Interval(it->lb());
          auto pm0 = pm->input_gate();
          auto pp0 = pp->input_gate();
          if(it->lb() == this->t0_tf().lb())
          {
            pm0.init(Interval(0.));
            pp0.init(Interval(0.));
          }
          q.first &= pm0 + dt * T(x.lb());
          q.second &= pp0 + dt * T(x.ub());

          // Also use the right endpoint, notably when gates are implicit.
          const Interval dtf = Interval(it->ub()) - ti;
          q.first &= pm->output_gate() - dtf * T(x.lb());
          q.second &= pp->output_gate() - dtf * T(x.ub());
        }
      }

      p.first |= q.first;
      p.second |= q.second;

      if(it->ub() >= t.ub())
        break;
    }

    return p;
  }

  template<typename T>
  std::pair<T,T> SlicedTube<T>::partial_integral(const Interval& t1, const Interval& t2,
    const std::pair<SlicedTube<T>,SlicedTube<T>>& primitives) const
  {
    auto p1 = partial_integral(t1, primitives);
    auto p2 = partial_integral(t2, primitives);
    return { p2.first - p1.first, p2.second - p1.second };
  }
}
