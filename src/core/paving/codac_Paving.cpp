/** 
 *  Paving class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include <iostream>
#include "codac_Paving.h"
#include "ibex_LargestFirst.h"

using namespace std;
using namespace ibex;

namespace codac
{
  // Basics
  
  Paving::Paving(const IntervalVector& box, SetValue value)
    : Set(box, value), m_flag(false), m_root(this), m_first_subpaving(nullptr), m_second_subpaving(nullptr)
  {

  }
  
  Paving::Paving(const Paving& p)
    : Set(p), m_flag(p.m_flag), m_root(p.m_root), m_first_subpaving(nullptr), m_second_subpaving(nullptr)
  {
    if(p.m_first_subpaving)
    {
      m_first_subpaving = new Paving(p.m_first_subpaving->m_box, p.m_first_subpaving->m_value);
      *m_first_subpaving = *p.m_first_subpaving;
      m_second_subpaving = new Paving(p.m_second_subpaving->m_box, p.m_second_subpaving->m_value);
      *m_second_subpaving = *p.m_second_subpaving;
    }
  }

  Paving::~Paving()
  {
    if(m_first_subpaving)
    {
      delete m_first_subpaving;
      delete m_second_subpaving;
    }
  }
  
  Paving& Paving::operator=(const Paving& p)
  {
    Set::operator = (p);      
    m_flag = p.m_flag; 
    m_root = p.m_root;
    m_first_subpaving = nullptr;
    m_second_subpaving = nullptr;
    if(p.m_first_subpaving)
    {
      m_first_subpaving = new Paving(p.m_first_subpaving->m_box, p.m_first_subpaving->m_value);
      *m_first_subpaving = *p.m_first_subpaving;
      m_second_subpaving = new Paving(p.m_second_subpaving->m_box, p.m_second_subpaving->m_value);
      *m_second_subpaving = *p.m_second_subpaving;
    }
    return *this;
  }

  // Binary tree structure

  Paving* Paving::get_first_subpaving()
  {
    return const_cast<Paving*>(static_cast<const Paving&>(*this).get_first_subpaving());
  }

  const Paving* Paving::get_first_subpaving() const
  {
    return m_first_subpaving;
  }

  Paving* Paving::get_second_subpaving()
  {
    return const_cast<Paving*>(static_cast<const Paving&>(*this).get_second_subpaving());
  }

  const Paving* Paving::get_second_subpaving() const
  {
    return m_second_subpaving;
  }

  Paving* Paving::get_root()
  {
    return const_cast<Paving*>(static_cast<const Paving&>(*this).get_root());
  }

  const Paving* Paving::get_root() const
  {
    return m_root;
  }

  Paving* Paving::get_first_leaf(SetValue val, bool without_flag)
  {
    return const_cast<Paving*>(static_cast<const Paving&>(*this).get_first_leaf(val, without_flag));
  }

  const Paving* Paving::get_first_leaf(SetValue val, bool without_flag) const
  {
    if(is_leaf() && (value() & val) && (!without_flag || (without_flag && !flag())))
      return this;

    const Paving* p = nullptr;

    if(m_first_subpaving) p = m_first_subpaving->get_first_leaf(val, without_flag);
    if(p) return p;

    if(m_second_subpaving) p = m_second_subpaving->get_first_leaf(val, without_flag);
    return p;
  }

  void Paving::bisect(float ratio)
  {
    assert(Interval(0.,1.).interior_contains(ratio));
    assert(is_leaf() && "only leaves can be bisected");

    LargestFirst bisector(0., ratio);
    pair<IntervalVector,IntervalVector> subboxes = bisector.bisect(m_box);
    m_first_subpaving = new Paving(subboxes.first, m_value);
    m_first_subpaving->m_root = m_root;
    m_second_subpaving = new Paving(subboxes.second, m_value);
    m_second_subpaving->m_root = m_root;
  }

  bool Paving::is_leaf() const
  {
    return m_first_subpaving == nullptr;
  }

  // Flags

  bool Paving::flag() const
  {
    bool flag = m_flag;
    if(!flag && m_first_subpaving) flag |= m_first_subpaving->flag();
    if(!flag && m_second_subpaving) flag |= m_second_subpaving->flag();
    return flag;
  }

  void Paving::set_flag() const
  {
    m_flag = true;
    if(m_first_subpaving) m_first_subpaving->set_flag();
    if(m_second_subpaving) m_second_subpaving->set_flag();
  }

  void Paving::reset_flags() const
  {
    m_flag = false;
    if(m_first_subpaving) m_first_subpaving->reset_flags();
    if(m_second_subpaving) m_second_subpaving->reset_flags();
  }

  void Paving::reset_paving(SetValue value)
  {
    m_value = value;
    if(m_first_subpaving)
    {
      delete m_first_subpaving; m_first_subpaving = nullptr;
      delete m_second_subpaving; m_second_subpaving = nullptr;
    }
  }

  // Extract methods
  
  void Paving::get_boxes(list<IntervalVector>& l_subpavings, SetValue val, SetValue neg_val) const
  {
    assert(!(val & neg_val) && "val and neg_val intersect");

    if(neg_val != SetValue::DEFAULT && (m_value & neg_val) && !(m_value & val))
    {
      // The current node and its leaves do not contain solutions
      // So we stop here
    }

    else
    {
      if(is_leaf())
      {
        if(m_value & val)
          l_subpavings.push_back(box());
      }

      else
      {
        m_first_subpaving->get_boxes(l_subpavings, val, neg_val);
        m_second_subpaving->get_boxes(l_subpavings, val, neg_val);
      }
    }
  }

  void Paving::get_pavings_intersecting(SetValue val, const IntervalVector& box_to_intersect, vector<const Paving*>& v_subpavings, bool no_degenerated_intersection) const
  {
    // todo: use the negation of val for a faster reading of the tree?
    // See Paving::get_boxes

    assert(box_to_intersect.size() == 2);
    IntervalVector inter = box_to_intersect & m_box;

    if(inter.is_empty() || (no_degenerated_intersection && inter[0].is_degenerated() && inter[1].is_degenerated()))
      return;

    if(is_leaf())
    {
      if(m_value & val)
        v_subpavings.push_back(this);
    }

    else
    {
      m_first_subpaving->get_pavings_intersecting(val, box_to_intersect, v_subpavings, no_degenerated_intersection);
      m_second_subpaving->get_pavings_intersecting(val, box_to_intersect, v_subpavings, no_degenerated_intersection);
    }
  }

  void Paving::get_neighbours(vector<const Paving*>& v_neighbours, SetValue val, bool without_flag) const
  {
    v_neighbours.clear();
    vector<const Paving*> v;
    m_root->get_pavings_intersecting(val, m_box, v);

    for(const auto& p : v)
    {
      if(p != this && (p->value() & val) && (!without_flag || (without_flag && !p->flag())))
        v_neighbours.push_back(p);
    }
  }

  // todo: bool compare_subset(const ConnectedSubset* x1, const ConnectedSubset* x2)
  // todo: {
  // todo:   return x1->get_items().size() > x2->get_items().size();
  // todo: }

  vector<ConnectedSubset> Paving::get_connected_subsets(bool sort_by_size, SetValue val) const
  {
    reset_flags();

    const Paving *p;
    vector<ConnectedSubset> v_connected_subsets;

    while((p = get_first_leaf(val, true)))
    {
      vector<const Paving*> v_subset_items;
      list<const Paving*> l;
      l.push_back(p);

      while(!l.empty())
      {
        const Paving *e = l.front();
        l.pop_front();

        v_subset_items.push_back(e);
        e->set_flag();

        vector<const Paving*> v_neighbours;
        e->get_neighbours(v_neighbours, val, true);

        for(const auto& neighb : v_neighbours)
        {
          neighb->set_flag();
          l.push_back(neighb);
        }
      }

      v_connected_subsets.push_back(ConnectedSubset(v_subset_items));
    }

    // todo: if(sort_by_size)
    // todo:   sort(v_connected_subsets.begin(), v_connected_subsets.end(), compare_subset);

    return v_connected_subsets;
  }
}