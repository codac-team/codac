/* ============================================================================
 *  tubex-lib - TubeTreeSynthesis class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_TubeTreeSynthesis.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  bool TubeTreeSynthesis::is_leaf() const
  {
    return m_first_subtree == NULL && m_second_subtree == NULL;
  }

  const Interval TubeTreeSynthesis::domain()
  {
    return m_domain;
  }

  void TubeTreeSynthesis::request_updates()
  {
    m_update_needed = true;
    if(!is_leaf())
    {
      m_first_subtree->request_updates();
      m_second_subtree->request_updates();
    }
  }

  void TubeTreeSynthesis::update_values()
  {
    // Warning: this method can only be called from the root
    // (because computation starts from 0)

    //if(build_from_leafs)
    {
      if(!is_leaf())
      {
        m_first_subtree->update_values();
        m_second_subtree->update_values();

        pair<Interval,Interval> pp_past = m_first_subtree->partial_primitive_bounds();
        pair<Interval,Interval> pp_future = m_second_subtree->partial_primitive_bounds();
        m_partial_primitive = make_pair(pp_past.first | pp_future.first, pp_past.second | pp_future.second);
      }
    }

    //else
    {
      Interval sum = Interval(0);

      for(int i = 0 ; i < m_slices_number ; i++)
      {
        //double dt_ = domain(i).diam();
        //Interval integrale_value = sum + (*this)[i] * Interval(0., dt_);
        //getSlice(i)->m_partial_primitive = make_pair(Interval(integrale_value.lb(), integrale_value.lb() + fabs((*this)[i].lb() * dt_)),
        //                                             Interval(integrale_value.ub() - fabs((*this)[i].ub() * dt_), integrale_value.ub()));
        //sum += (*this)[i] * dt_;
      }

      //computePartialPrimitive(true);
    }

    //m_primitive_computation_needed = false;
  }

  const pair<Interval,Interval> TubeTreeSynthesis::partial_primitive_bounds(const Interval& t)
  {
    assert(domain().is_superset(t));

    /*if(t.is_degenerated()) // todo: check this:
      return make_pair(Interval((*this)(t.lb()).lb()), Interval((*this)(t.lb()).ub()));*/

    Interval intersection = domain() & t;

    if(intersection.is_empty())
      return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

    else if(is_leaf() || t == domain() || t.is_unbounded() || t.is_superset(domain()))
      return m_partial_primitive; // pre-computed values

    else
    {
      Interval inter_firstsubtree = m_first_subtree->domain() & intersection;
      Interval inter_secondsubtree = m_second_subtree->domain() & intersection;

      if(inter_firstsubtree.lb() == inter_firstsubtree.ub() && inter_secondsubtree.lb() == inter_secondsubtree.ub())
        return make_pair(m_first_subtree->partial_primitive_bounds().first & m_second_subtree->partial_primitive_bounds().first,
                         m_first_subtree->partial_primitive_bounds().second & m_second_subtree->partial_primitive_bounds().second);

      else if(inter_firstsubtree.is_empty() || inter_firstsubtree.lb() == inter_firstsubtree.ub())
        return m_second_subtree->partial_primitive_bounds(inter_secondsubtree);

      else if(inter_secondsubtree.is_empty() || inter_secondsubtree.lb() == inter_secondsubtree.ub())
        return m_first_subtree->partial_primitive_bounds(inter_firstsubtree);

      else
      {
        pair<Interval,Interval> pp_past = m_first_subtree->partial_primitive_bounds(inter_firstsubtree);
        pair<Interval,Interval> pp_future = m_second_subtree->partial_primitive_bounds(inter_secondsubtree);
        return make_pair(pp_past.first | pp_future.first, pp_past.second | pp_future.second);
      }
    }
  }
}