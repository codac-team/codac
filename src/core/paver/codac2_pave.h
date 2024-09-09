/** 
 *  \file codac2_pave.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Paving.h"
#include "codac2_Ctc.h"
#include "codac2_Sep.h"
#include "codac2_Figure2D.h"

namespace codac2
{
  using PavingOut = Paving<IntervalVector>;
  using PavingOut_Node = PavingNode<PavingOut>;
  PavingOut pave(const IntervalVector& x, const CtcBase<IntervalVector>& c, double eps);

  using PavingInOut = Paving<IntervalVector,IntervalVector>;
  using PavingInOut_Node = PavingNode<PavingInOut>;
  PavingInOut pave(const IntervalVector& x, const SepBase& s, double eps);

  void draw_while_paving(const IntervalVector& x0, const CtcBase<IntervalVector>& c, double eps, Figure2D *g = DefaultView::selected_fig());
  void draw_while_paving(const IntervalVector& x0, const SepBase& s, double eps, Figure2D *g = DefaultView::selected_fig());
}