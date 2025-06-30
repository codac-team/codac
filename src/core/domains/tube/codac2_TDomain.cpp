/** 
 *  \file codac2_TDomain.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <cassert>
#include <iostream>
#include "codac2_Interval.h"
#include "codac2_TDomain.h"
#include "codac2_SliceBase.h"

using namespace std;

namespace codac2
{
  TDomain::TDomain(const Interval& t0_tf)
    : std::list<TSlice>({ TSlice(t0_tf) })
  { }

  TDomain::TDomain(const Interval& t0_tf, double dt, bool with_gates)
  {
    assert_release(!t0_tf.is_unbounded() && !t0_tf.is_degenerated());
    assert_release(!t0_tf.is_empty());
    assert_release(dt > 0.);

    for(double t = t0_tf.lb() ; t < t0_tf.ub()+dt ; t=t+dt)
    {
      double t_next = std::min(t0_tf.ub(),t+dt);
      if(with_gates)
        this->push_back(TSlice(t));
      this->push_back(TSlice({t,t_next}));
      if(t_next == t0_tf.ub())
        break;
    }

    if(with_gates)
      this->push_back(TSlice(t0_tf.ub()));
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
    if(t0_tf().is_degenerated())
      return true;

    else if(nb_tslices() == 1)
      return false;
    
    else
    {
      list<TSlice>::const_iterator it = std::next(this->begin());
      while(it != this->end())
      {
        if(it->is_gate())
          return false;

        it++;
        
        if(it != this->end())
        {
          if(!it->is_gate())
            return false;
          it++;
        }
      }
      
      return true;
    }
  }
  
  std::vector<TSlice> TDomain::tslices_vector() const
  {
    return {
      make_move_iterator(this->cbegin()),
      make_move_iterator(this->cend())
    };
  }

  list<TSlice>::iterator TDomain::tslice(double t)
  {
    if(!t0_tf().contains(t))
      return this->end();

    list<TSlice>::iterator it;
    for(it = this->begin(); it != this->end(); ++it)
    {
      if((it->is_gate() && it->lb() == t) // gate
        || (it->lb() <= t && it->ub() > t)) // slice
        return it;
    }

    it = this->end(); it--;
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
      it = this->insert(it, ts);
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
      it = this->insert(it, ts);
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
      it = this->insert(it, ts); // then, it points to the newly inserted element
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
    assert_release(dt >= 0.);
    assert_release(!t0_tf.is_degenerated());
    for(double t = t0_tf.lb() ; t < t0_tf.ub()+dt ; t=t+dt)
      sample(std::min(t0_tf.ub(),t), with_gates);
  }

  void TDomain::delete_gates()
  {
    list<TSlice>::iterator it = this->begin();
    while(it != this->end())
    {
      if(it->is_gate()) this->erase(it++);
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
      if(*it1 != *it2) return false;
      it1++; it2++;
    }
    return true;
  }
}