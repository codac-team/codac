/** 
 *  CtcPointCloud.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcPointCloud.h"

using namespace std;

namespace codac2
{
  CtcPointCloud::CtcPointCloud(const vector<IntervalVector>& p)
    : Ctc<CtcPointCloud,IntervalVector>([&]() {
      assert_release(!p.empty());
      assert_release([&]() {
        Index n = p[0].size();
        for(const auto& pi : p)
          if(n != pi.size())
            return false;
        return true;
      }() && "all the points should be of same dimension");
      return p[0].size();
    }()), _p(p)
  { }

  void CtcPointCloud::contract(IntervalVector& x) const
  {
    assert_release(this->size() == x.size());
    auto u = IntervalVector::empty(x.size());

    for(const auto& pi : _p)
      u |= x & pi;
    x = u;
  }
}