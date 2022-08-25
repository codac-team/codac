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
  TDomain::TDomain(const Interval& t0_tf, bool with_gates) :
    TDomain(t0_tf, t0_tf.diam(), with_gates)
  {

  }

  TDomain::TDomain(const Interval& t0_tf, double dt, bool with_gates)
  {
    assert(!t0_tf.is_empty());
    assert(dt > 0.);

    if(isinf(dt))
      _tslices.push_back(TSlice(t0_tf));

    else
    {
      double prev_t = -oo;
      for(double t = t0_tf.lb() ; t < t0_tf.ub()+dt ; t+=dt)
      {
        double t_ = min(t, t0_tf.ub());

        _tslices.push_back(TSlice(Interval(prev_t,t_)));
        if(with_gates)
          _tslices.push_back(TSlice(Interval(t_,t_)));

        prev_t = t_;
      }

      _tslices.push_back(TSlice(Interval(t0_tf.ub(),oo)));
    }
  }

  const Interval TDomain::t0_tf() const
  {
    return Interval(_tslices.front().t0_tf().ub(),
      prev(_tslices.end())->t0_tf().lb());
  }

  size_t TDomain::nb_tslices() const
  {
    return _tslices.size();
  }

  size_t TDomain::nb_tubes() const
  {
    return _tslices.front().slices().size();
  }

  list<TSlice>::iterator TDomain::iterator_tslice(double t)
  {
    if(t == -oo) return _tslices.begin();
    if(t == oo) return prev(_tslices.end());

    list<TSlice>::iterator it;
    for(it = _tslices.begin(); it != _tslices.end(); ++it)
    {
      const Interval& tdom = it->t0_tf();
      if((tdom.is_degenerated() && tdom.lb() == t) // gate
        || (tdom.lb() <= t && tdom.ub() > t)) // slice
        return it;
    }
    assert(false && "time must belong to one slice because tdomain is unbounded");
    return _tslices.end();
  }
  
  list<TSlice>::iterator TDomain::sample(double t, bool allow_gate)
  {
    list<TSlice>::iterator it = iterator_tslice(t);
    assert(it != _tslices.end());
    const Interval tdomain = it->t0_tf();
    assert(tdomain.contains(t));

    if((tdomain.lb() == t && !allow_gate) || tdomain.is_degenerated())
      return it;

    it->set_tdomain(Interval(tdomain.lb(), t));
    TSlice ts(*it, Interval(t, tdomain.ub()));

    ++it;
    it = _tslices.insert(it, ts);
    for(auto& [k,s] : it->_slices) // adding the new iterator pointer to the new slices
      s->_it_tslice = it;
    
    return it;
  }

  const list<TSlice>& TDomain::tslices() const
  {
    return _tslices;
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

  shared_ptr<TDomain> create_tdomain(const Interval& t0_tf, bool with_gates)
  {
    return make_shared<TDomain>(t0_tf, with_gates);
  }

  shared_ptr<TDomain> create_tdomain(const Interval& t0_tf, double dt, bool with_gates)
  {
    return make_shared<TDomain>(t0_tf, dt, with_gates);
  }
} // namespace codac