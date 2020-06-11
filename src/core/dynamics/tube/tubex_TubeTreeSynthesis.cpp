/** 
 *  TubeTreeSynthesis class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_TubeTreeSynthesis.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  TubeTreeSynthesis::TubeTreeSynthesis(const Tube* tube, int k0, int kf, const vector<const Slice*>& v_tube_slices)
    : m_tube_ref(tube), m_parent(NULL)
  {
    assert(tube != NULL);
    assert(k0 >= 0 && k0 < (int)v_tube_slices.size()); // todo: use size_t
    assert(kf >= 0 && kf < (int)v_tube_slices.size()); // todo: use size_t

    if(k0 == kf) // leaf, pointer to a slice
    {
      m_first_subtree = NULL;
      m_second_subtree = NULL;
      m_slice_ref = v_tube_slices[k0];
      m_slice_ref->m_synthesis_reference = this;
      m_tdomain = m_slice_ref->tdomain();
      m_nb_slices = 1;
    }

    else
    {
      // In the first subtree: [t0,thalf[
      // In the second subtree: [thalf,tf]

      m_nb_slices = kf - k0 + 1;
      int kmid = k0 + ceil(m_nb_slices / 2.) - 1;

      m_first_subtree = new TubeTreeSynthesis(tube, k0, kmid, v_tube_slices);
      m_first_subtree->m_parent = this;

      if(kmid + 1 <= kf)
      {
        m_second_subtree = new TubeTreeSynthesis(tube, kmid + 1, kf, v_tube_slices);
        m_second_subtree->m_parent = this;
      }

      else
        m_second_subtree = NULL;

      m_tdomain = m_first_subtree->tdomain() | m_second_subtree->tdomain();
      m_slice_ref = NULL;
    }
  }

  TubeTreeSynthesis::~TubeTreeSynthesis()
  {
    if(m_slice_ref != NULL)
      m_slice_ref->m_synthesis_reference = NULL; // removing reference from slice's part

    if(m_first_subtree != NULL)
      delete m_first_subtree;

    if(m_second_subtree != NULL)
      delete m_second_subtree;
  }

  const Interval TubeTreeSynthesis::tdomain() const
  {
    return m_tdomain;
  }

  int TubeTreeSynthesis::nb_slices() const
  {
    return m_nb_slices;
  }

  const Interval TubeTreeSynthesis::operator()(const Interval& t)
  {
    assert(!t.is_degenerated());
    assert(tdomain().is_superset(t));

    Interval inter = m_tdomain & t;

    if(inter.is_empty())
      return Interval::EMPTY_SET;

    else if(is_leaf() || inter == m_tdomain)
      return codomain();

    else
    {
      Interval inter_firstsubtree = m_first_subtree->tdomain() & inter;
      Interval inter_secondsubtree = m_second_subtree->tdomain() & inter;

      assert(inter_firstsubtree != inter_secondsubtree); // both degenerated

      if(inter_firstsubtree.is_degenerated() && !inter_secondsubtree.is_degenerated())
        return (*m_second_subtree)(inter_secondsubtree);

      else if(inter_secondsubtree.is_degenerated() && !inter_firstsubtree.is_degenerated())
        return (*m_first_subtree)(inter_firstsubtree);

      else
        return (*m_first_subtree)(inter_firstsubtree) | (*m_second_subtree)(inter_secondsubtree);
    }
  }
  
  const Interval TubeTreeSynthesis::invert(const Interval& y, const Interval& search_tdomain)
  {
    Interval inter = m_tdomain & search_tdomain;

    if(inter.is_empty())
      return Interval::EMPTY_SET;

    else if(!codomain().intersects(y))
      return Interval::EMPTY_SET;

    else if(codomain_bounds().first.ub() < y.lb() && codomain_bounds().second.lb() > y.ub())
      return inter;

    else
    {
      if(is_leaf())
        return inter;

      else
        return m_first_subtree->invert(y, inter) | m_second_subtree->invert(y, inter);
    }
  }
  
  const Interval TubeTreeSynthesis::codomain()
  {
    if(m_values_update_needed)
      root()->update_values();
    return m_codomain;
  }
  
  const pair<Interval,Interval> TubeTreeSynthesis::codomain_bounds()
  {
    if(m_values_update_needed)
      root()->update_values();
    return m_codomain_bounds;
  }

  const pair<Interval,Interval> TubeTreeSynthesis::eval(const Interval& t)
  {
    if(t.is_degenerated()) // faster to perform the evaluation over the related slice
      return slice(time_to_index(t.lb()))->eval(t);

    Interval inter = m_tdomain & t;

    if(inter.is_empty())
      return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

    else if(is_leaf() || inter == m_tdomain) // todo: this last condition useful?
    {
      if(inter == m_tdomain)
        return codomain_bounds();

      else
        return m_slice_ref->eval(inter);
    }

    else
    {
      Interval inter_firstsubtree = m_first_subtree->tdomain() & inter;
      Interval inter_secondsubtree = m_second_subtree->tdomain() & inter;

      assert(inter_firstsubtree != inter_secondsubtree); // both degenerated

      if(inter_firstsubtree.is_degenerated() && !inter_secondsubtree.is_degenerated())
        return m_second_subtree->eval(inter_secondsubtree);

      else if(inter_secondsubtree.is_degenerated() && !inter_firstsubtree.is_degenerated())
        return m_first_subtree->eval(inter_firstsubtree);

      else
      {
        pair<Interval,Interval> p_first = m_first_subtree->eval(inter_firstsubtree);
        pair<Interval,Interval> p_second = m_second_subtree->eval(inter_secondsubtree);
        return make_pair(p_first.first | p_second.first, p_first.second | p_second.second);
      }
    }
  }

  int TubeTreeSynthesis::time_to_index(double t) const
  {
    assert(tdomain().contains(t));

    if(t == m_tdomain.ub())
      return m_nb_slices - 1;

    if(is_leaf())
      return 0;

    if(t < m_first_subtree->tdomain().ub())
      return m_first_subtree->time_to_index(t);

    else
      return m_second_subtree->time_to_index(t) + m_first_subtree->nb_slices();
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

  void TubeTreeSynthesis::request_values_update()
  {
    if(m_values_update_needed)
      return;
    
    m_values_update_needed = true;

    if(m_parent != NULL && !m_parent->m_values_update_needed)
      m_parent->request_values_update();
  }

  void TubeTreeSynthesis::request_integrals_update(bool propagate_to_other_slices)
  {
    if(m_integrals_update_needed)
      return;

    m_integrals_update_needed = true;
    
    if(m_parent != NULL && !m_parent->m_integrals_update_needed)
      m_parent->request_integrals_update();

    if(m_slice_ref != NULL && propagate_to_other_slices)
    {
      // Update everything after the updated slice
      for(const Slice *s = m_slice_ref->next_slice() ; s != NULL ; s = s->next_slice())
      {
        assert(s->m_synthesis_reference != NULL);
        s->m_synthesis_reference->request_integrals_update(false);
      }
    }
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
    if(m_values_update_needed)
    {
      if(is_leaf())
      {
        m_codomain = m_slice_ref->codomain();
        m_codomain_bounds = make_pair(m_codomain.lb(), m_codomain.ub());
        m_codomain_bounds.first |= m_slice_ref->input_gate().lb();
        m_codomain_bounds.first |= m_slice_ref->output_gate().lb();
        m_codomain_bounds.second |= m_slice_ref->input_gate().ub();
        m_codomain_bounds.second |= m_slice_ref->output_gate().ub();
        m_values_update_needed = false;
      }

      else
      {
        m_first_subtree->update_values();
        m_second_subtree->update_values();

        m_codomain = m_first_subtree->m_codomain | m_second_subtree->m_codomain;
        pair<Interval,Interval> p_first = m_first_subtree->m_codomain_bounds;
        pair<Interval,Interval> p_second = m_second_subtree->m_codomain_bounds;
        m_codomain_bounds = make_pair(p_first.first | p_second.first, p_first.second | p_second.second);

        m_values_update_needed = false;
      }
    }
  }

  void TubeTreeSynthesis::update_integrals()
  {
    if(m_values_update_needed)
    {
      // 1. Updating leafs values (leaf nodes)

      if(is_root())
      {
        // This part can only be executed from the root
        // (because integral computation starts from k=0)

        Interval sum = Interval(0);
        for(const Slice *s = m_tube_ref->first_slice() ; s != NULL ; s = s->next_slice())
        {
          double dt = s->tdomain().diam();
          Interval slice_value = s->codomain();
          Interval integral = sum + slice_value * Interval(0., dt);
          assert(s->m_synthesis_reference != NULL);
          s->m_synthesis_reference->m_partial_primitive =
                make_pair(Interval(integral.lb(), integral.lb() + fabs(slice_value.lb() * dt)),
                          Interval(integral.ub() - fabs(slice_value.ub() * dt), integral.ub()));
          s->m_synthesis_reference->m_integrals_update_needed = false;
          sum += slice_value * dt;
        }
      }

      // 2. Upper synthesis (tree nodes)

      if(!is_leaf()) // flag already set to false in step 1
      {
        m_first_subtree->update_integrals();
        m_second_subtree->update_integrals();

        m_partial_primitive = m_first_subtree->m_partial_primitive;
        m_partial_primitive.first |= m_second_subtree->m_partial_primitive.first;
        m_partial_primitive.second |= m_second_subtree->m_partial_primitive.second;
        
        m_integrals_update_needed = false;
      }
    }
  }

  pair<Interval,Interval> TubeTreeSynthesis::partial_integral(const Interval& t)
  {
    assert(is_root());

    if(m_integrals_update_needed)
      root()->update_integrals();

    int index_lb = m_tube_ref->time_to_index(t.lb());
    int index_ub = m_tube_ref->time_to_index(t.ub());

    Interval integral_lb = Interval::EMPTY_SET;
    Interval integral_ub = Interval::EMPTY_SET;

    const Slice *s_lb = m_tube_ref->slice(index_lb);
    const Slice *s_ub = m_tube_ref->slice(index_ub);

    Interval intv_t_lb = s_lb->tdomain();
    Interval intv_t_ub = s_ub->tdomain();

    // Part A: integral along the temporal domain [t]&[intv_t_lb]
    {
      pair<Interval,Interval> partial_primitive_first = s_lb->m_synthesis_reference->m_partial_primitive;
      
      if(partial_primitive_first.first.is_empty() || partial_primitive_first.second.is_empty())
        return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

      if(partial_primitive_first.first.is_unbounded() || partial_primitive_first.second.is_unbounded())
        return make_pair(Interval::ALL_REALS, Interval::ALL_REALS);

      if(intv_t_lb.is_subset(t)) // slice entirely considered
      {
        integral_lb |= partial_primitive_first.first;
        integral_ub |= partial_primitive_first.second;
      }
      
      else // partial integral (on [t]&[intv_t_lb]) rebuilt from pre-computation
      {
        Interval primitive_lb = Interval(partial_primitive_first.first.lb(), partial_primitive_first.second.ub());

        Interval y_first = s_lb->codomain();
        Interval ta1 = Interval(intv_t_lb.lb(), t.lb());
        Interval ta2 = Interval(intv_t_lb.lb(), min(t.ub(), intv_t_lb.ub()));
        Interval tb1 = Interval(t.lb(), intv_t_lb.ub());
        Interval tb2 = Interval(min(t.ub(), intv_t_lb.ub()), intv_t_lb.ub());

        if(y_first.lb() < 0)
          integral_lb |= Interval(primitive_lb.lb() - y_first.lb() * tb2.diam(),
                                   primitive_lb.lb() - y_first.lb() * tb1.diam());

        else if(y_first.lb() > 0)
          integral_lb |= Interval(primitive_lb.lb() + y_first.lb() * ta1.diam(),
                                   primitive_lb.lb() + y_first.lb() * ta2.diam());

        if(y_first.ub() < 0)
          integral_ub |= Interval(primitive_lb.ub() + y_first.ub() * ta2.diam(),
                                   primitive_lb.ub() + y_first.ub() * ta1.diam());

        else if(y_first.ub() > 0)
          integral_ub |= Interval(primitive_lb.ub() - y_first.ub() * tb1.diam(),
                                   primitive_lb.ub() - y_first.ub() * tb2.diam());
      }
    }

    // Part B: in between, integral along the temporal domain [ub(intv_t_lb),lb(intv_t_ub)]
    if(index_ub - index_lb > 1)
    {
      pair<Interval,Interval> partial_primitive = partial_primitive_bounds(Interval(intv_t_lb.ub(), intv_t_ub.lb()));
      integral_lb |= partial_primitive.first;
      integral_ub |= partial_primitive.second;
    }

    // Part C: integral along the temporal domain [t]&[intv_t_ub]
    if(index_lb != index_ub)
    {
      pair<Interval,Interval> partial_primitive_second = s_ub->m_synthesis_reference->m_partial_primitive;
      
      if(partial_primitive_second.first.is_empty() || partial_primitive_second.second.is_empty())
        return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

      if(partial_primitive_second.first.is_unbounded() || partial_primitive_second.second.is_unbounded())
        return make_pair(Interval::ALL_REALS, Interval::ALL_REALS);
      
      if(intv_t_ub.is_subset(t)) // slice entirely considered
      {
        integral_lb |= partial_primitive_second.first;
        integral_ub |= partial_primitive_second.second;
      }
      
      else // partial integral (on [t]&[intv_t_ub]) rebuilt from pre-computation
      {
        Interval primitive_ub = Interval(partial_primitive_second.first.lb(), partial_primitive_second.second.ub());

        Interval y_second = s_ub->codomain();
        Interval ta = Interval(intv_t_ub.lb(), t.ub());
        Interval tb1 = intv_t_ub;
        Interval tb2 = Interval(t.ub(), intv_t_ub.ub());

        if(y_second.lb() < 0)
          integral_lb |= Interval(primitive_ub.lb() - y_second.lb() * tb2.diam(),
                                   primitive_ub.lb() - y_second.lb() * tb1.diam());

        else if(y_second.lb() > 0)
          integral_lb |= Interval(primitive_ub.lb(),
                                   primitive_ub.lb() + y_second.lb() * ta.diam());

        if(y_second.ub() < 0)
          integral_ub |= Interval(primitive_ub.ub() + y_second.ub() * ta.diam(),
                                   primitive_ub.ub());

        else if(y_second.ub() > 0)
          integral_ub |= Interval(primitive_ub.ub() - y_second.ub() * tb1.diam(),
                                   primitive_ub.ub() - y_second.ub() * tb2.diam());
      }
    }

    return make_pair(integral_lb, integral_ub);
  }

  const pair<Interval,Interval> TubeTreeSynthesis::partial_primitive_bounds(const Interval& t)
  {
    if(m_integrals_update_needed)
      root()->update_integrals();

    if(t == Interval::ALL_REALS)
      return m_partial_primitive; // pre-computed values

    Interval intersection = tdomain() & t;

    if(intersection.is_empty())
      return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

    else if(is_leaf() || t == tdomain() || t.is_superset(tdomain()))
      return m_partial_primitive; // pre-computed values

    else
    {
      Interval inter_firstsubtree = m_first_subtree->tdomain() & intersection;
      Interval inter_secondsubtree = m_second_subtree->tdomain() & intersection;
      
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