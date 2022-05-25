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
    _tslices.push_back(make_shared<TSlice>(Interval(-oo,oo)));
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
    return Interval(_tslices.front()->tdomain().ub(),
      prev(_tslices.end())->get()->tdomain().lb());
  }

  size_t TDomain::nb_tslices() const
  {
    return _tslices.size();
  }

  size_t TDomain::nb_tubes() const
  {
    return _tslices.front().get()->_slices.size();
  }

  list<shared_ptr<TSlice>>::iterator TDomain::iterator_tslice(double t)
  {
    list<shared_ptr<TSlice>>::iterator it;
    for(it = _tslices.begin(); it != _tslices.end(); ++it)
      if(it->get()->tdomain().contains(t)) return it;
    return _tslices.end();
  }
  
  void TDomain::sample(double t)
  {
    std::list<std::shared_ptr<TSlice>>::iterator it = iterator_tslice(t);
    assert(it != _tslices.end());
    const Interval tdomain = it->get()->tdomain();
    assert(tdomain.contains(t));

    it->get()->set_tdomain(Interval(tdomain.lb(), t));
    ++it;
    
    if(t == tdomain.ub())
      assert(!it->get()->tdomain().is_degenerated() &&
        "degenerated tslice (gate) already existing at this time");

    _tslices.insert(it, make_shared<TSlice>(Interval(t, tdomain.ub())));
  }

  const list<shared_ptr<TSlice>> TDomain::tslices() const
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