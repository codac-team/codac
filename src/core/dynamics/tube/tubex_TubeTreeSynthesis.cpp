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
  TubeTreeSynthesis::TubeTreeSynthesis(const Tube* tube)
  {

  }

  TubeTreeSynthesis::~TubeTreeSynthesis()
  {
    // todo
  }

  const Interval TubeTreeSynthesis::domain()
  {
    return m_domain;
  }

  void TubeTreeSynthesis::request_updates()
  {
    m_update_needed = true;
    if(m_parent != NULL) // todo: check request_updates of parent? (avoid unnecessary request)
      m_parent->request_updates();
    // todo: is it useful to flag the slices before the one updated?
  }

  bool TubeTreeSynthesis::is_leaf() const
  {
    return m_first_subtree == NULL && m_second_subtree == NULL;
  }

  bool TubeTreeSynthesis::is_root() const
  {
    return m_parent == NULL;
  }

  TubeTreeSynthesis* TubeTreeSynthesis::get_root()
  {
    if(m_parent == NULL)
      return this;
    else
      return m_parent->get_root();
  }

  Slice* TubeTreeSynthesis::get_first_slice()
  {
    TubeTreeSynthesis *subtree = this;
    while(!subtree->is_leaf())
      subtree = subtree->m_first_subtree;
    return subtree->m_slice_ref;
  }

  void TubeTreeSynthesis::update_values()
  {
    if(m_update_needed)
    {
      // 1. Updating leafs values (leaf nodes)

      if(m_parent == NULL) // if root
      {
        // This part can only be executed from the root
        // (because computation starts from 0)

        Interval sum = Interval(0);
        for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        {
          double dt = s->domain().diam();
          Interval slice_value = s->codomain();
          Interval integrale = sum + slice_value * Interval(0., dt);
          s->m_synthesis_reference->m_partial_primitive =
                make_pair(Interval(integrale.lb(), integrale.lb() + fabs(slice_value.lb() * dt)),
                          Interval(integrale.ub() - fabs(slice_value.ub() * dt), integrale.ub()));
          s->m_synthesis_reference->m_update_needed = false;
          sum += slice_value * dt;
        }
      }

      // 2. Upper synthesis (tree nodes)

        assert(!is_leaf());
        m_first_subtree->update_values();
        m_second_subtree->update_values();

        pair<Interval,Interval> pp_past = m_first_subtree->m_partial_primitive;
        pair<Interval,Interval> pp_future = m_second_subtree->m_partial_primitive;
        m_partial_primitive = make_pair(pp_past.first | pp_future.first,
                                        pp_past.second | pp_future.second);
        m_update_needed = false;
    }
  }

  pair<Interval,Interval> TubeTreeSynthesis::partial_integral(const Interval& t)
  {
    assert(is_root());

    if(m_update_needed)
      get_root()->update_values();
    
    int index_lb = m_tube_ref->input2index(t.lb());
    int index_ub = m_tube_ref->input2index(t.ub());

    Interval integrale_lb = Interval::EMPTY_SET;
    Interval integrale_ub = Interval::EMPTY_SET;

    Slice *slice_lb = m_tube_ref->slice(index_lb);
    Slice *slice_ub = m_tube_ref->slice(index_ub);

    Interval intv_t_lb = slice_lb->domain();
    Interval intv_t_ub = slice_ub->domain();

    // Part A
    {
      pair<Interval,Interval> partial_primitive_first = make_pair(slice_lb->codomain(), slice_lb->codomain());
      Interval primitive_lb = Interval(partial_primitive_first.first.lb(), partial_primitive_first.second.ub());

      Interval y_first = slice_lb->codomain();
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
      pair<Interval,Interval> partial_primitive = partial_primitive_bounds(Interval(intv_t_lb.ub(), intv_t_ub.lb()));
      integrale_lb |= partial_primitive.first;
      integrale_ub |= partial_primitive.second;
    }

    // Part C
    if(index_lb != index_ub)
    {
      pair<Interval,Interval> partial_primitive_second = make_pair(slice_ub->codomain(), slice_ub->codomain());
      Interval primitive_ub = Interval(partial_primitive_second.first.lb(), partial_primitive_second.second.ub());

      Interval y_second = slice_ub->codomain();
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

  const pair<Interval,Interval> TubeTreeSynthesis::partial_primitive_bounds(const Interval& t)
  {
    if(m_update_needed)
      get_root()->update_values();

    //assert(!m_update_needed);
    // ? assert(domain().is_superset(t));

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