/* ============================================================================
 *  tubex-lib - Subtube class (integration methods)
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Subtube.h"
#include <iostream>
#include <iomanip> // for setprecision()
#ifdef _OPENMP
  #include <omp.h> // for multithreading
#endif

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    Interval Subtube::integral(double t) const
    {
        return integral(Interval(t));
    }

    Interval Subtube::integral(const Interval& t) const
    {
      pair<Interval,Interval> partial_integrale = partialIntegral(t);
      return Interval(partial_integrale.first.lb(), partial_integrale.second.ub());
    }
    
    Interval Subtube::integral(const Interval& t1, const Interval& t2) const
    {
      pair<Interval,Interval> integrale_t1 = partialIntegral(t1);
      pair<Interval,Interval> integrale_t2 = partialIntegral(t2);
      double lb = (integrale_t2.first - integrale_t1.first).lb();
      double ub = (integrale_t2.second - integrale_t1.second).ub();
      return Interval(min(lb, ub), max(lb, ub));
    }
    
    pair<Interval,Interval> Subtube::partialIntegral(const Interval& t) const
    {
      if(primitiveUpdateNeeded())
        computePartialPrimitive(false);
      
      int index_lb = input2index(t.lb());
      int index_ub = input2index(t.ub());

      Interval integrale_lb = Interval::EMPTY_SET;
      Interval integrale_ub = Interval::EMPTY_SET;

      Interval intv_t_lb = sliceDomain(index_lb);
      Interval intv_t_ub = sliceDomain(index_ub);

      // Part A
      {
        pair<Interval,Interval> partial_primitive_first = getSlice(index_lb)->getPartialPrimitiveValue();
        Interval primitive_lb = Interval(partial_primitive_first.first.lb(), partial_primitive_first.second.ub());

        Interval y_first = (*this)[index_lb];
        Interval ta1 = Interval(intv_t_lb.lb(), t.lb());
        Interval ta2 = Interval(intv_t_lb.lb(), min(t.ub(), intv_t_lb.ub()));
        Interval tb1 = Interval(t.lb(), intv_t_lb.ub());
        Interval tb2 = Interval(min(t.ub(), intv_t_lb.ub()), intv_t_lb.ub());

        if(y_first.lb() < 0)
          integrale_lb |= Interval(primitive_lb.lb() - y_first.lb() * tb2.diam(),
                                   primitive_lb.lb() - y_first.lb() * tb1.diam());

        else if(y_first.lb() > 0)
          integrale_lb |= Interval(primitive_lb.lb() + y_first.lb() * ta1.diam(),
                                   primitive_lb.lb() + y_first.lb() * ta2.diam());

        if(y_first.ub() < 0)
          integrale_ub |= Interval(primitive_lb.ub() + y_first.ub() * ta2.diam(),
                                   primitive_lb.ub() + y_first.ub() * ta1.diam());

        else if(y_first.ub() > 0)
          integrale_ub |= Interval(primitive_lb.ub() - y_first.ub() * tb1.diam(),
                                   primitive_lb.ub() - y_first.ub() * tb2.diam());
      }

      // Part B
      if(index_ub - index_lb > 1)
      {
        pair<Interval,Interval> partial_primitive = getPartialPrimitiveValue(Interval(intv_t_lb.ub(), intv_t_ub.lb()));
        integrale_lb |= partial_primitive.first;
        integrale_ub |= partial_primitive.second;
      }

      // Part C
      if(index_lb != index_ub)
      {
        pair<Interval,Interval> partial_primitive_second = getSlice(index_ub)->getPartialPrimitiveValue();
        Interval primitive_ub = Interval(partial_primitive_second.first.lb(), partial_primitive_second.second.ub());

        Interval y_second = (*this)[index_ub];
        Interval ta = Interval(intv_t_ub.lb(), t.ub());
        Interval tb1 = intv_t_ub;
        Interval tb2 = Interval(t.ub(), intv_t_ub.ub());

        if(y_second.lb() < 0)
          integrale_lb |= Interval(primitive_ub.lb() - y_second.lb() * tb2.diam(),
                                   primitive_ub.lb() - y_second.lb() * tb1.diam());

        else if(y_second.lb() > 0)
          integrale_lb |= Interval(primitive_ub.lb(),
                                   primitive_ub.lb() + y_second.lb() * ta.diam());

        if(y_second.ub() < 0)
          integrale_ub |= Interval(primitive_ub.ub() + y_second.ub() * ta.diam(),
                                   primitive_ub.ub());

        else if(y_second.ub() > 0)
          integrale_ub |= Interval(primitive_ub.ub() - y_second.ub() * tb1.diam(),
                                   primitive_ub.ub() - y_second.ub() * tb2.diam());
      }

      return make_pair(integrale_lb, integrale_ub);
    }
    
    pair<Interval,Interval> Subtube::partialIntegral(const Interval& t1, const Interval& t2) const
    {
      pair<Interval,Interval> integrale_t1 = partialIntegral(t1);
      pair<Interval,Interval> integrale_t2 = partialIntegral(t2);
      return make_pair((integrale_t2.first - integrale_t1.first),
                       (integrale_t2.second - integrale_t1.second));
    }

  // Protected methods

    void Subtube::computePartialPrimitive(bool build_from_leafs) const
    {
      // Warning: this method can only be called from the root
      // (because computation starts from 0)

      if(build_from_leafs)
      {
        if(!isSlice()) // todo: check usefulness of this block
        {
          m_first_subtube->computePartialPrimitive(true);
          m_second_subtube->computePartialPrimitive(true);

          pair<Interval,Interval> pp_past = m_first_subtube->getPartialPrimitiveValue();
          pair<Interval,Interval> pp_future = m_second_subtube->getPartialPrimitiveValue();
          m_partial_primitive = make_pair(pp_past.first | pp_future.first, pp_past.second | pp_future.second);
        }
      }

      else
      {
        Interval sum_max = Interval(0);

        for(int i = 0 ; i < nbSlices() ; i++)
        {
          double dt_ = sliceDomain(i).diam();
          Interval int_value = sum_max + (*this)[i] * Interval(0., dt_);
          getSlice(i)->m_partial_primitive = make_pair(Interval(int_value.lb(), int_value.lb() + fabs((*this)[i].lb() * dt_)),
                                                       Interval(int_value.ub() - fabs((*this)[i].ub() * dt_), int_value.ub()));

          // todo: gates
          sum_max += (*this)[i] * dt_;
        }

        computePartialPrimitive(true);
      }

      m_primitive_computation_needed = false;
    }
    
    void Subtube::flagFuturePrimitiveComputation() const // todo: slice index?
    {
      if(!primitiveUpdateNeeded())
      {
        m_primitive_computation_needed = true;
        if(!isSlice())
        {
          m_first_subtube->flagFuturePrimitiveComputation();
          m_second_subtube->flagFuturePrimitiveComputation();
        }
      }
    }

    bool Subtube::primitiveUpdateNeeded() const
    {
      return m_primitive_computation_needed;
    }
    
    pair<Interval,Interval> Subtube::getPartialPrimitiveValue(const Interval& t) const
    {
      if(t.lb() == t.ub())
        return make_pair(Interval((*this)[t.lb()].lb()), Interval((*this)[t.lb()].ub()));

      Interval intersection = m_domain & t;

      if(intersection.is_empty())
        return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

      else if(isSlice() || t == m_domain || t.is_unbounded() || t.is_superset(m_domain))
        return m_partial_primitive; // pre-computed with computePartialPrimitive()

      else
      {
        Interval inter_firstsubtube = m_first_subtube->domain() & intersection;
        Interval inter_secondsubtube = m_second_subtube->domain() & intersection;

        if(inter_firstsubtube.lb() == inter_firstsubtube.ub() && inter_secondsubtube.lb() == inter_secondsubtube.ub())
          return make_pair(m_first_subtube->getPartialPrimitiveValue().first & m_second_subtube->getPartialPrimitiveValue().first,
                           m_first_subtube->getPartialPrimitiveValue().second & m_second_subtube->getPartialPrimitiveValue().second);

        else if(inter_firstsubtube.is_empty() || inter_firstsubtube.lb() == inter_firstsubtube.ub())
          return m_second_subtube->getPartialPrimitiveValue(inter_secondsubtube);

        else if(inter_secondsubtube.is_empty() || inter_secondsubtube.lb() == inter_secondsubtube.ub())
          return m_first_subtube->getPartialPrimitiveValue(inter_firstsubtube);

        else
        {
          pair<Interval,Interval> pp_past = m_first_subtube->getPartialPrimitiveValue(inter_firstsubtube);
          pair<Interval,Interval> pp_future = m_second_subtube->getPartialPrimitiveValue(inter_secondsubtube);
          return make_pair(pp_past.first | pp_future.first, pp_past.second | pp_future.second);
        }
      }
    }
}