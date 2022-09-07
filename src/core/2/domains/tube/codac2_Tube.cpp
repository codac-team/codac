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

#include "codac2_Tube.h"

using namespace std;
using namespace codac;

namespace codac2
{
  codac::Tube to_codac1(const Tube<Interval>& x)
  {
    codac::Tube x_(x.t0_tf());
    for(const auto& s : x)
      if(!s.t0_tf().is_unbounded())
        x_.set(s.codomain(), s.t0_tf());
    for(const auto& s : x) // setting gate (were overwritten)
      if(s.t0_tf().is_degenerated())
        x_.set(s.codomain(), s.t0_tf());
    return x_;
  }

  codac::TubeVector to_codac1(const Tube<IntervalVector>& x)
  {
    codac::TubeVector x_(x.t0_tf(), x.size());
    for(const auto& s : x)
      if(!s.t0_tf().is_unbounded())
        x_.set(s.codomain(), s.t0_tf());
    for(const auto& s : x) // setting gate (were overwritten)
      if(s.t0_tf().is_degenerated())
        x_.set(s.codomain(), s.t0_tf());
    return x_;
  }
} // namespace codac