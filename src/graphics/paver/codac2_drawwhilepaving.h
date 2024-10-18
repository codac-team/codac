/** 
 *  \file codac2_drawwhilepaving.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"
#include "codac2_Sep.h"
#include "codac2_Figure2D.h"

namespace codac2
{
  // eps: accuracy of the paving algorithm, the undefined boxes will have their max_diam <= eps
  
  void draw_while_paving(const IntervalVector& x0, std::shared_ptr<const CtcBase<IntervalVector>> c, double eps, std::shared_ptr<Figure2D> fig = nullptr);
  void draw_while_paving(const IntervalVector& x0, const CtcBase<IntervalVector>& c, double eps, std::shared_ptr<Figure2D> fig = nullptr);

  void draw_while_paving(const IntervalVector& x0, std::shared_ptr<const SepBase> s, double eps, std::shared_ptr<Figure2D> fig = nullptr);
  void draw_while_paving(const IntervalVector& x0, const SepBase& s, double eps, std::shared_ptr<Figure2D> fig = nullptr);
}