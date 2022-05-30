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
#include "codac2_TDomain.h"
#include "codac2_TubeVector.h"
#include "codac_predef_values.h"

using namespace std;
using namespace codac;

namespace codac2
{
  TDomain::TDomain(const Interval& t0_tf, double dt, bool with_gates)
  {
    assert(!t0_tf.is_empty());
    assert(dt > 0.);

    _tslices.push_back(TSlice(Interval(-oo,oo)));
    for(double t = t0_tf.lb() ; t < t0_tf.ub()+dt ; t+=dt)
    {
      double t_ = min(t, t0_tf.ub());

      sample(t_);
      if(with_gates)
        sample(t_); // second sampling
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
    return _tslices.front()._slices.size();
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
      s._it_tslice = it;
    
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
} // namespace codac