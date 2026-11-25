/** 
 *  \file codac2_TDomain.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include "codac2_Interval.h"
#include "codac2_TDomain.h"
#include "codac2_SliceBase.h"

using namespace std;

namespace codac2
{
  TDomain::TDomain(const Interval& t0_tf)
  {
    insert_tslice(this->end(), TSlice(t0_tf));
  }

  TDomain::TDomain(const Interval& t0_tf, double dt, bool with_gates)
  {
    assert_release(!t0_tf.is_unbounded() && !t0_tf.is_degenerated());
    assert_release(!t0_tf.is_empty());
    assert_release(dt > 0.);

    for(double t = t0_tf.lb() ; t < t0_tf.ub()+dt ; t=t+dt)
    {
      double t_next = std::min(t0_tf.ub(),t+dt);
      if(with_gates)
        insert_tslice(this->end(), TSlice(t));
      insert_tslice(this->end(), TSlice({t,t_next}));
      if(t_next == t0_tf.ub())
        break;
    }

    if(with_gates)
      insert_tslice(this->end(), TSlice(t0_tf.ub()));
  }

  Interval TDomain::t0_tf() const
  {
    return { this->front().lb(), prev(this->end())->ub() };
  }

  size_t TDomain::nb_tslices() const
  {
    return this->size();
  }

  size_t TDomain::nb_tubes() const
  {
    return this->front().slices().size();
  }

  bool TDomain::all_gates_defined() const
  {
    if(this->empty())
      return false;

    bool expect_gate = true;

    for(const auto& s : *this)
    {
      if(s.is_gate() != expect_gate)
        return false;

      expect_gate = !expect_gate;
    }

    // We must end on a gate:
    // valid patterns are [gate] or [gate, slice, ..., gate]
    return !expect_gate;
  }
  
  std::vector<TSlice> TDomain::tslices_vector() const
  {
    return {
      make_move_iterator(this->cbegin()),
      make_move_iterator(this->cend())
    };
  }

  list<TSlice>::iterator TDomain::insert_tslice(list<TSlice>::iterator pos, const TSlice& ts)
  {
    auto it = this->insert(pos, ts);
    if(!it->is_empty())
    {
      // If inserted immediately before an existing node with the same
      // lower bound, the new node takes precedence (e.g. a new gate).
      if(pos == this->end())
      {
        // Appending in temporal order: constant amortized insertion.
        if(_time_index.empty() || _time_index.rbegin()->first != it->lb())
          _time_index.emplace_hint(_time_index.end(), it->lb(), it);
      }
      else if(pos->lb() == it->lb())
        _time_index.insert_or_assign(it->lb(), it);
      else
        _time_index.try_emplace(it->lb(), it);
    }
    return it;
  }

  list<TSlice>::iterator TDomain::erase_tslice(list<TSlice>::iterator it)
  {
    if(!it->is_empty())
    {
      auto index_it = _time_index.find(it->lb());
      if(index_it != _time_index.end() && index_it->second == it)
      {
        auto next_it = std::next(it);
        if(next_it != this->end() && !next_it->is_empty() && next_it->lb() == it->lb())
          index_it->second = next_it;
        else
          _time_index.erase(index_it);
      }
    }
    return this->erase(it);
  }

  list<TSlice>::iterator TDomain::tslice(double t)
  {
    if(_time_index.empty() || std::isnan(t) || !t0_tf().contains(t))
      return this->end();

    if(t == t0_tf().ub())
      return std::prev(this->end());

    auto it = std::prev(_time_index.upper_bound(t))->second;
    if(it->is_gate() && it->lb() != t)
      ++it;

    return it;
  }
  
  list<TSlice>::iterator TDomain::sample(double t, bool with_gates)
  {
    assert_release(!isnan(t));
    list<TSlice>::iterator it;

    if(t <= t0_tf().lb()) // if outside the already defined tdomain
    {
      it = this->begin();

      if(it->is_gate() && it->lb() == t)
        return it;

      TSlice ts(*it, Interval(t, t0_tf().lb())); // duplicate with different tdomain
      it = insert_tslice(it, ts);
      for(auto& [k,s] : it->_slices)
      {
        s->_it_tslice = it;
        s->init(); // reinitialization to unbounded set
      }

      if(with_gates)
      {
        if(it->is_gate()) // iterator already pointing to a gate
          return it;
        else
          return sample(t, true); // recursive
      }
      else
        return it;
    }

    else if(t > t0_tf().ub()) // if outside the already defined tdomain
    {
      it = this->end();
      TSlice ts(*std::prev(it), Interval(t0_tf().ub(),t)); // duplicate with different tdomain
      it = insert_tslice(it, ts);
      for(auto& [k,s] : it->_slices)
      {
        s->_it_tslice = it;
        s->init(); // reinitialization to unbounded set
      }

      if(with_gates)
        return sample(t, true); // recursive
      else
        return it;
    }

    else // inside [t0,tf]
    {
      it = tslice(t);
      assert(it != this->end());
      assert(it->contains(t));

      if(it->is_gate()
       || (!with_gates && (it->lb() == t || t == t0_tf().ub())))
        return it; // nothing more to do

      // Else, performing sampling
      TSlice ts(*it, Interval(t, it->ub())); // duplicate with different tdomain
      it->init({it->lb(), t});
      bool new_gate_added = it->is_gate();
      list<TSlice>::iterator it_gate = it;
      // (*it) is the TSlice to copy

      // From C++ insert() doc: the container is extended by inserting new elements before the element at the specified position
      ++it; // we will insert the new tslice before the next TSlice [t.ub(),..]
      it = insert_tslice(it, ts); // then, it points to the newly inserted element
      for(auto& [k,s] : it->_slices) // adding the new iterator pointer to the new slices
        s->_it_tslice = it;
      
      // In case the sampling includes the creation of a gate, the method is called again at same t
      if(new_gate_added)
        return it_gate;
      else if(with_gates && !new_gate_added)
        return sample(t, true);
      else
        return it;
    }
  }

  void TDomain::sample(const Interval& t0_tf, double dt, bool with_gates)
  {
    assert_release(dt > 0.);
    assert_release(!t0_tf.is_degenerated());
    for(double t = t0_tf.lb() ; t < t0_tf.ub()+dt ; t=t+dt)
      sample(std::min(t0_tf.ub(),t), with_gates);
  }

  void TDomain::delete_gates()
  {
    list<TSlice>::iterator it = this->begin();
    while(it != this->end())
    {
      if(it->is_gate()) it = erase_tslice(it);
      else ++it;
    }
  }
  
  void TDomain::truncate(const Interval& new_tdomain)
  {
    assert_release(new_tdomain.is_subset(t0_tf()));
    sample(new_tdomain.lb());
    sample(new_tdomain.ub());
    for(auto it = this->begin(); it != this->end(); )
    {
      if(!it->intersects(new_tdomain)) it = erase_tslice(it);
      else ++it;
    }
  }

  ostream& operator<<(ostream& os, const TDomain& x)
  {
    os << x.t0_tf()
       << ", " << x.nb_tslices()
       << " slice" << (x.nb_tslices() > 1 ? "s" : "")
       << ", " << x.nb_tubes()
       << " tube" << (x.nb_tubes() > 1 ? "s" : "")
       << flush;
    return os;
  }

  shared_ptr<TDomain> create_tdomain(const Interval& t0_tf)
  {
    return std::shared_ptr<TDomain>(new TDomain(t0_tf));
  }

  shared_ptr<TDomain> create_tdomain(const Interval& t0_tf, double dt, bool with_gates)
  {
    return std::shared_ptr<TDomain>(new TDomain(t0_tf, dt, with_gates));
  }

  bool TDomain::are_same(const shared_ptr<const TDomain>& tdom1, const shared_ptr<const TDomain>& tdom2)
  {
    if(tdom1 == tdom2)
      return true;

    if(tdom1->nb_tslices() != tdom2->nb_tslices())
      return false;

    list<TSlice>::const_iterator it1 = tdom1->cbegin(), it2 = tdom2->cbegin();
    while(it1 != tdom1->cend())
    {
      if(!((*it1) == (*it2))) return false;
      it1++; it2++;
    }
    return true;
  }
}