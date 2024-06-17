/** 
 *  \file codac2_Paver.h
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
#include "codac2_Collection.h"

namespace codac2
{
  class Paver
  {
    public:

      Paver(const IntervalVector& x0);

      std::list<IntervalVector> pave(const Ctc_<IntervalVector>& c, double eps);
      std::list<IntervalVector> pave(const std::shared_ptr<Ctc_<IntervalVector>>& c, double eps);
      void pave(const Sep& s, double eps);
      void pave(const std::shared_ptr<Sep>& s, double eps);

      void set_figure(Figure2D *g);

      // Public attributes freely editable by the user
      StyleProperties inner_style = StyleProperties::inner();
      StyleProperties outer_style = StyleProperties::outer();
      StyleProperties boundary_style = StyleProperties::boundary();

    protected:

      Figure2D *_figure = nullptr;
      void init_figure();

      const IntervalVector _x0;
  };

  template<typename O>
  void pave(const IntervalVector& x0, const O& o, double eps)
  {
    Paver p(x0);
    p.set_figure(DefaultView::selected_fig());
    p.pave(o,eps);
  }

  template<typename O>
  void pave_test(const IntervalVector& x0, const O& o, double eps)
  {
    Paver p(x0);
    p.set_figure(DefaultView::selected_fig());
    p.pave(o,eps);
  }
}