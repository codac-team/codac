/** 
 *  \file codac2_fixpoint.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

namespace codac2
{
  template<typename F, typename... X>
  void fixpoint(const F& contract, const X&... x)
  {
    double vol = -1., prev_vol;

    do
    {
      prev_vol = vol;
      contract();
      vol = 0.;
      ((vol += [&x]() {
        // As infinity is absorbent, this would not
        // allow us to identify a contraction, so we
        // exclude these cases:
        double w = x.volume(); return w == oo ? 0. : w;
      }()), ...);

      if((x.is_empty(), ...))
        return;

    } while(prev_vol != vol);
  }
}