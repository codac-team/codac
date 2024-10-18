/** 
 *  \file codac2_SepEllipse.h
 *
 *  From: Optimal separator for an ellipse, Application to localization
 *  Luc Jaulin - arXiv, math.NA, 2305.10842
 *  https://www.ensta-bretagne.fr/jaulin/paper_ctcellipse.pdf
 * 
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Sep.h"
#include "codac2_CtcEllipse.h"

namespace codac2
{
  class SepEllipse : public Sep<SepEllipse>
  {
    public:

      // f(q,x) = q_0 + q_1*x_1 + q_2*x_2 + q_3*x_1^2 + q_4*x_1*x_2 + q_5*x_2^2
      SepEllipse(const Vector& q);
      BoxPair separate(const IntervalVector& x) const;

    protected:

      const CtcEllipse _ctc;
  };
}