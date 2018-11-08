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
  TubeTreeSynthesis::TubeTreeSynthesis(const Tube* tube, int k0, int kf, const vector<const Slice*>& v_slices)
    : m_tube_ref(tube), m_parent(NULL), m_update_needed(true)
  {
    assert(tube != NULL);
    assert(k0 >= 0 && k0 < v_slices.size());
    assert(kf >= 0 && kf < v_slices.size());

    if(k0 == kf) // leaf, pointer to a slice
    {
      m_first_subtree = NULL;
      m_second_subtree = NULL;
      m_slice_ref = v_slices[k0];
      m_slice_ref->m_synthesis_reference = this;
      m_domain = m_slice_ref->domain();
      m_nb_slices = 1;
    }

    else
    {
      // In the first subtree: [t0,thalf[
      // In the second subtree: [thalf,tf]

      m_nb_slices = kf - k0 + 1;
      int kmid = k0 + floor(m_nb_slices / 2.);

      m_first_subtree = new TubeTreeSynthesis(tube, k0, kmid - 1, v_slices);
      m_first_subtree->m_parent = this;

      if(kmid - 1 < kf)
      {
        m_second_subtree = new TubeTreeSynthesis(tube, kmid, kf, v_slices);
        m_second_subtree->m_parent = this;
      }

      else
        m_second_subtree = NULL;

      m_domain = m_first_subtree->domain() | m_second_subtree->domain();
      m_slice_ref = NULL;
    }
  }

  TubeTreeSynthesis::~TubeTreeSynthesis()
  {
    if(m_first_subtree != NULL)
      delete m_first_subtree;
    if(m_second_subtree != NULL)
      delete m_second_subtree;
  }

  const Interval TubeTreeSynthesis::domain() const
  {
    return m_domain;
  }

  int TubeTreeSynthesis::nb_slices() const
  {
    return m_nb_slices;
  }

  double TubeTreeSynthesis::volume() const
  {
    assert(false);
    // todo
    return 0.;
  }

  const Interval TubeTreeSynthesis::operator()(const Interval& t) const
  {
    assert(false);
    // todo
    return Interval(0.);
  }
  
  const Interval TubeTreeSynthesis::codomain() const
  {
    assert(false);
    // todo: check update needed
    return Interval(0.);
  }

  int TubeTreeSynthesis::input2index(double t) const
  {
    assert(domain().contains(t));

    if(t == m_domain.ub())
      return m_nb_slices - 1;

    if(is_leaf())
      return 0;

    if(t < m_first_subtree->domain().ub())
      return m_first_subtree->input2index(t);

    else
      return m_second_subtree->input2index(t) + m_first_subtree->nb_slices();
  }

  Slice* TubeTreeSynthesis::slice(int slice_id)
  {
    assert(slice_id >= 0 && slice_id < nb_slices());
    return const_cast<Slice*>(static_cast<const TubeTreeSynthesis&>(*this).slice(slice_id));
  }

  const Slice* TubeTreeSynthesis::slice(int slice_id) const
  {
    assert(slice_id >= 0 && slice_id < nb_slices());

    if(is_leaf())
      return m_slice_ref;

    else
    {
      int mid_id = ceil(nb_slices() / 2.);

      if(slice_id < mid_id)
        return m_first_subtree->slice(slice_id);

      else
        return m_second_subtree->slice(slice_id - mid_id);
    }
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
    bool is_leaf_ = (m_first_subtree == NULL && m_second_subtree == NULL);
    if(is_leaf_)
      assert(m_slice_ref != NULL);
    return is_leaf_;
  }

  bool TubeTreeSynthesis::is_root() const
  {
    bool is_root_ = (m_parent == NULL);
    if(is_root_)
      assert(m_slice_ref == NULL);
    return is_root_;
  }

  TubeTreeSynthesis* TubeTreeSynthesis::root()
  {
    if(m_parent == NULL)
      return this;
    else
      return m_parent->root();
  }

  void TubeTreeSynthesis::update_values()
  {
    if(m_update_needed)
    {
      // 1. Updating leafs values (leaf nodes)

      if(is_root())
      {
        // This part can only be executed from the root
        // (because integral computation starts from k=0)

        Interval sum = Interval(0);
        for(const Slice *s = m_tube_ref->first_slice() ; s != NULL ; s = s->next_slice())
        {
          double dt = s->domain().diam();
          Interval slice_value = s->codomain();
          Interval integrale = sum + slice_value * Interval(0., dt);
          assert(s->m_synthesis_reference != NULL);
          s->m_synthesis_reference->m_partial_primitive =
                make_pair(Interval(integrale.lb(), integrale.lb() + fabs(slice_value.lb() * dt)),
                          Interval(integrale.ub() - fabs(slice_value.ub() * dt), integrale.ub()));
          s->m_synthesis_reference->m_update_needed = false;
          sum += slice_value * dt;
        }
      }

      // 2. Upper synthesis (tree nodes)

        assert(!is_leaf()); // flag already set to false in step 1.
        m_first_subtree->update_values();
        if(m_second_subtree != NULL)
          m_second_subtree->update_values();

        m_partial_primitive = m_first_subtree->m_partial_primitive;

        if(m_second_subtree != NULL)
        {
          pair<Interval,Interval> pp_second = m_second_subtree->m_partial_primitive;
          m_partial_primitive.first |= pp_second.first;
          m_partial_primitive.second |= pp_second.second;
        }
        
        // todo: update the following only once?
        m_domain = m_first_subtree->domain();
        if(m_second_subtree != NULL)
          m_domain |= m_second_subtree->domain();

        m_update_needed = false;
    }
  }

  pair<Interval,Interval> TubeTreeSynthesis::partial_integral(const Interval& t)
  {
    assert(is_root());

    if(m_update_needed)
      root()->update_values();

    int index_lb = m_tube_ref->input2index(t.lb());
    int index_ub = m_tube_ref->input2index(t.ub());

    Interval integrale_lb = Interval::EMPTY_SET;
    Interval integrale_ub = Interval::EMPTY_SET;

    const Slice *slice_lb = m_tube_ref->slice(index_lb);
    const Slice *slice_ub = m_tube_ref->slice(index_ub);

    Interval intv_t_lb = slice_lb->domain();
    Interval intv_t_ub = slice_ub->domain();

    // Part A: integral along the temporal domain [t]&[intv_t_lb]
    {
      pair<Interval,Interval> partial_primitive_first = slice_lb->m_synthesis_reference->m_partial_primitive;
      
      if(partial_primitive_first.first.is_empty() || partial_primitive_first.second.is_empty())
        return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

      if(partial_primitive_first.first.is_unbounded() || partial_primitive_first.second.is_unbounded())
        return make_pair(Interval::ALL_REALS, Interval::ALL_REALS);

      if(intv_t_lb.is_subset(t)) // slice entirely considered
      {
        integrale_lb |= partial_primitive_first.first;
        integrale_ub |= partial_primitive_first.second;
      }
      
      else // partial integral (on [t]&[intv_t_lb]) rebuilt from precomputation
      {
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
    }

    // Part B: in between, integral along the temporal domain [ub(intv_t_lb),lb(intv_t_ub)]
    if(index_ub - index_lb > 1)
    {
      pair<Interval,Interval> partial_primitive = partial_primitive_bounds(Interval(intv_t_lb.ub(), intv_t_ub.lb()));
      integrale_lb |= partial_primitive.first;
      integrale_ub |= partial_primitive.second;
    }

    // Part C: integral along the temporal domain [t]&[intv_t_ub]
    if(index_lb != index_ub)
    {
      pair<Interval,Interval> partial_primitive_second = slice_ub->m_synthesis_reference->m_partial_primitive;
      
      if(partial_primitive_second.first.is_empty() || partial_primitive_second.second.is_empty())
        return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

      if(partial_primitive_second.first.is_unbounded() || partial_primitive_second.second.is_unbounded())
        return make_pair(Interval::ALL_REALS, Interval::ALL_REALS);
      
      if(intv_t_ub.is_subset(t)) // slice entirely considered
      {
        integrale_lb |= partial_primitive_second.first;
        integrale_ub |= partial_primitive_second.second;
      }
      
      else // partial integral (on [t]&[intv_t_ub]) rebuilt from precomputation
      {
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
    }

    return make_pair(integrale_lb, integrale_ub);
  }

  const pair<Interval,Interval> TubeTreeSynthesis::partial_primitive_bounds(const Interval& t)
  {
    if(m_update_needed)
      root()->update_values();

    if(t == Interval::ALL_REALS)
      return m_partial_primitive; // pre-computed values

    //assert(!m_update_needed);
    // ? assert(domain().is_superset(t));

    //if(t.is_degenerated()) // todo: check this:
    //  return make_pair(Interval((*m_tube_ref)(t.lb()).lb()), Interval((*m_tube_ref)(t.lb()).ub()));

    Interval intersection = domain() & t;

    if(intersection.is_empty())
      return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

    else if(is_leaf() || t == domain() || t.is_superset(domain()))
      return m_partial_primitive; // pre-computed values

    else
    {
      Interval inter_firstsubtree = m_first_subtree->domain() & intersection;
      Interval inter_secondsubtree = m_second_subtree->domain() & intersection;
      
      if(inter_firstsubtree.is_degenerated() && inter_secondsubtree.is_degenerated())
        return make_pair(m_first_subtree->partial_primitive_bounds().first & m_second_subtree->partial_primitive_bounds().first,
                         m_first_subtree->partial_primitive_bounds().second & m_second_subtree->partial_primitive_bounds().second);
     
      else if(inter_firstsubtree.is_empty() || inter_firstsubtree.is_degenerated())
        return m_second_subtree->partial_primitive_bounds(inter_secondsubtree);
      
      else if(inter_secondsubtree.is_empty() || inter_secondsubtree.is_degenerated())
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