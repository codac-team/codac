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

#include "codac2_TDomain.h"
#include "codac_predef_values.h"

using namespace std;
using namespace codac;

namespace codac2
{
  TDomain::TDomain(const Interval& t0_tf, double dt, bool with_gates)
  {
    _tslices.push_back(make_shared<TSlice>(Interval(-oo,t0_tf.lb())));

    for(double t = t0_tf.lb() ; t < t0_tf.ub() ; t+=dt)
    {
      if(with_gates)
        _tslices.push_back(make_shared<TSlice>(Interval(t)));
      _tslices.push_back(make_shared<TSlice>(Interval(t,std::min(t0_tf.ub(),t+dt))));
    }

    if(with_gates)
      _tslices.push_back(make_shared<TSlice>(Interval(t0_tf.ub())));
    _tslices.push_back(make_shared<TSlice>(Interval(t0_tf.ub(),oo)));
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

  const list<std::shared_ptr<TSlice>> TDomain::tslices() const
  {
    return _tslices;
  }

  ostream& operator<<(ostream& os, const TDomain& x)
  {
    os << x.t0_tf()
       << ", " << x.nb_tslices()
       << " slice" << (x.nb_tslices() > 1 ? "s" : "")
       << flush;
    return os;
  }
} // namespace codac