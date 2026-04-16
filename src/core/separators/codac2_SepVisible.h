/** 
 *  \file codac2_SepVisible.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Sep.h"
#include "codac2_CtcVisible.h"

namespace codac2 {
  class SepVisible : public Sep<SepVisible> {
  public:
    /**
     * \brief Constructor for visibility separation from point 'a' relative to segment 's'.
     */
    SepVisible(const IntervalVector& a, const Segment& s) 
      : Sep<SepVisible>(2), 
        _ctc_visible(a, s), 
        _ctc_novisible(a, s) 
    {}

    /**
     * \brief Constructor for visibility separation from point 'a' relative to a list of segments 's'.
     */
    SepVisible(const IntervalVector& a, const std::vector<Segment>& s) 
      : Sep<SepVisible>(2), 
        _ctc_visible(a, s), 
        _ctc_novisible(a, s) 
    {}

    /**
     * \brief Constructor for visibility separation from point 'a' relative to polygon 'p'.
     */
    SepVisible(const IntervalVector& a, const Polygon& p) 
      : Sep<SepVisible>(2), 
        _ctc_visible(a, p), 
        _ctc_novisible(a, p) 
    {}

    BoxPair separate(const IntervalVector& x) const override {
      IntervalVector x_in(x);
      IntervalVector x_out(x);

      _ctc_novisible.contract(x_in);
      _ctc_visible.contract(x_out);

      assert((x_in | x_out) == x);

      return {x_in, x_out};
    }

  private:
    const CtcVisible _ctc_visible;
    const CtcNoVisible _ctc_novisible;
  };
}