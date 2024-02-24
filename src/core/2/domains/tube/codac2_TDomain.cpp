/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <cassert>
#include "codac2_TSlice.h"
#include "codac2_TDomain.h"
#include "codac2_Slice.h"
#include "codac_predef_values.h"

using namespace std;
using namespace codac;

namespace codac2
{
  TDomain::TDomain(const Interval& t0_tf)
    : _tslices({ TSlice(t0_tf) })
  {

  }

  TDomain::TDomain(const Interval& t0_tf, double dt, bool with_gates)
  {
    assert(!t0_tf.is_unbounded() && !t0_tf.is_degenerated());
    assert(!t0_tf.is_empty());
    assert(dt > 0.);

    for(double t = t0_tf.lb() ; t < t0_tf.ub()+dt ; t=t+dt)
    {
      double t_next = std::min(t0_tf.ub(),t+dt);
      if(with_gates)
        _tslices.push_back(TSlice(Interval(t)));
      _tslices.push_back(TSlice(Interval(t,t_next)));
      if(t_next == t0_tf.ub())
        break;
    }

    if(with_gates)
      _tslices.push_back(TSlice(Interval(t0_tf.ub())));
  }

  const Interval TDomain::t0_tf() const
  {
    return Interval(_tslices.front().t0_tf().lb(),
      prev(_tslices.end())->t0_tf().ub());
  }

  size_t TDomain::nb_tslices() const
  {
    return _tslices.size();
  }

  size_t TDomain::nb_tubes() const
  {
    return _tslices.front().slices().size();
  }

  bool TDomain::all_gates_defined() const
  {
    if(t0_tf().is_degenerated())
      return true;

    else if(nb_tslices() == 1)
      return false;
    
    else
    {
      list<TSlice>::const_iterator it = std::next(_tslices.begin());
      while(it != _tslices.end())
      {
        if(it->t0_tf().is_degenerated())
          return false;

        it++;
        
        if(it != _tslices.end())
        {
          if(!it->t0_tf().is_degenerated())
            return false;
          it++;
        }
      }
      
      return true;
    }
  }

  list<TSlice>::iterator TDomain::iterator_tslice(double t)
  {
    if(!t0_tf().contains(t))
      return _tslices.end();

    list<TSlice>::iterator it;
    for(it = _tslices.begin(); it != _tslices.end(); ++it)
    {
      const Interval& tdom = it->t0_tf();
      if((tdom.is_degenerated() && tdom.lb() == t) // gate
        || (tdom.lb() <= t && tdom.ub() > t)) // slice
        return it;
    }

    it = _tslices.end(); it--;
    return it;
  }
  
  list<TSlice>::iterator TDomain::sample(double t, bool with_gates)
  {
    assert(!isnan(t));
    list<TSlice>::iterator it;

    if(t <= t0_tf().lb()) // if outside the already defined tdomain
    {
      it = _tslices.begin();

      if(it->is_gate() && it->t0_tf().lb() == t)
        return it;

      TSlice ts(*it, Interval(t, t0_tf().lb())); // duplicate with different tdomain
      it = _tslices.insert(it, ts);
      for(auto& [k,s] : it->_slices)
      {
        s->_it_tslice = it;
        s->set_unbounded(); // reinitialization
      }

      if(with_gates)
      {
        if(it->t0_tf().is_degenerated()) // iterator already pointing to a gate
          return it;
        else
          return sample(t, true); // recursive
      }
      else
        return it;
    }

    else if(t > t0_tf().ub()) // if outside the already defined tdomain
    {
      it = _tslices.end();
      TSlice ts(*std::prev(it), Interval(t0_tf().ub(),t)); // duplicate with different tdomain
      it = _tslices.insert(it, ts);
      for(auto& [k,s] : it->_slices)
      {
        s->_it_tslice = it;
        s->set_unbounded(); // reinitialization
      }

      if(with_gates)
        return sample(t, true); // recursive
      else
        return it;
    }

    else // inside [t0,tf]
    {
      it = iterator_tslice(t);
      assert(it != _tslices.end());
      const Interval tdomain_it = it->t0_tf();
      assert(tdomain_it.contains(t));

      if(tdomain_it.is_degenerated()
       || (!with_gates && (tdomain_it.lb() == t || t == t0_tf().ub())))
        return it; // nothing more to do

      // Else, performing sampling
      it->set_tdomain(Interval(tdomain_it.lb(), t));
      bool new_gate_added = it->t0_tf().is_degenerated();
      list<TSlice>::iterator it_gate = it;
      TSlice ts(*it, Interval(t, tdomain_it.ub())); // duplicate with different tdomain
      // (*it) is the TSlice to copy

      // From C++ insert() doc: the container is extended by inserting new elements before the element at the specified position
      ++it; // we will insert the new tslice before the next TSlice [t.ub(),..]
      it = _tslices.insert(it, ts); // then, it points to the newly inserted element
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
    assert(dt >= 0.);
    assert(!t0_tf.is_degenerated());
    for(double t = t0_tf.lb() ; t < t0_tf.ub()+dt ; t=t+dt)
      sample(std::min(t0_tf.ub(),t), with_gates);
  }

  const list<TSlice>& TDomain::tslices() const
  {
    return _tslices;
  }

  list<TSlice>& TDomain::tslices()
  {
    return const_cast<list<TSlice>&>(
      static_cast<const TDomain&>(*this).tslices());
  }

  void TDomain::delete_gates()
  {
    list<TSlice>::iterator it = _tslices.begin();
    while(it != _tslices.end())
    {
      if(it->t0_tf().is_degenerated())
        _tslices.erase(it++);

      else
        ++it;
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
    return make_shared<TDomain>(t0_tf);
  }

  shared_ptr<TDomain> create_tdomain(const Interval& t0_tf, double dt, bool with_gates)
  {
    return make_shared<TDomain>(t0_tf, dt, with_gates);
  }

  bool TDomain::are_same(const shared_ptr<TDomain>& tdom1, const shared_ptr<TDomain>& tdom2)
  {
    if(tdom1 == tdom2)
      return true;
    if(tdom1->nb_tslices() != tdom2->nb_tslices())
      return false;
    list<TSlice>::const_iterator it1 = tdom1->tslices().cbegin(), it2 = tdom2->tslices().cbegin();
    while(it1 != tdom1->tslices().cend())
    {
      if(*it1 != *it2) return false;
      it1++; it2++;
    }
    return true;
  }
} // namespace codac