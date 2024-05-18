/** 
 *  \file
 *  Paver class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include "codac2_Ctc.h"
#include "codac2_Sep.h"
#include "codac2_WithGraphicOutput.h"

namespace codac2
{
  class Paver : public WithGraphicOutput
  {
    public:

      Paver(const IntervalVector& x0);
      void pave(const Ctc_<IntervalVector>& c, double eps);
      void pave(const Sep& s, double eps);

    protected:

      void init_figure();

      const IntervalVector _x0;
  };

  template<typename O>
  void pave(const IntervalVector& x0, const O& o, double eps)
  {
    Paver p(x0);
    p.set_figure(DefaultFigure::selected_fig());
    p.pave(o,eps);
  }
}