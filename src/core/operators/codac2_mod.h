/** 
 *  \file codac2_mod.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"
#include "codac2_arith_sub.h"

namespace codac2
{
  struct ModOp
  {
    template<typename X1,typename X2,typename P>
    static std::string str(const X1& x1, const X2& x2, const P& p)
    {
      return "mod(" + x1->str() + "," + x2->str() + "," + p->str() + ")";
    }

    template<typename X1, typename X2>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1, [[maybe_unused]] const X2& s2)
    {
      return {1,1};
    }

    static void bwd(Interval& x1, Interval& x2, double p);
    static void bwd(Interval& x1, Interval& x2, Interval& p);
  };

  // Inline functions

  inline void ModOp::bwd(Interval& x1, Interval& x2, double p) // x1 = x2 mod(p)
  {
    // The content of this function comes from the IBEX library.
    // See ibex::Interval (IBEX lib, main author: Gilles Chabert)
    //   https://ibex-lib.readthedocs.io

    assert_release(p > 0. && "Modulo needs a strictly positive period p.");

    if(!(x2.diam() > p || x1.diam() > p))
    {
      Interval r = (x1-x2)/p;
      Interval ir = integer(r);

      if(ir.is_empty()) // additional protection because an empty interval is considered degenerated.
      {
        x1.set_empty();
        x2.set_empty();
      }

      else
      {
        if(ir.is_degenerated())
          SubOp::bwd(ir*p,x1,x2);

        else if(ir.diam() == 1.)
        {
          double ir1 = ir.lb();
          double ir2 = ir.ub();
          Interval x1_1 = x1; Interval x1_2 = x1;
          Interval x2_1 = x2; Interval x2_2 = x2;
          SubOp::bwd(Interval(ir1*p),x1_1,x2_1);
          SubOp::bwd(Interval(ir2*p),x1_2,x2_2);
          x1 = x1_1 | x1_2;
          x2 = x2_1 | x2_2;
        }

        else
        {
          assert_release(false && "Modulo diameter error.");
        }
      }
    }
  }

  inline void ModOp::bwd(Interval& x1, Interval& x2, Interval& p) // x = y mod(p)
  {
    assert_release(p.is_degenerated() && "ModOp::bwd(y,x1,x2) (with x1 and x2 intervals) not implemented yet");
    ModOp::bwd(x1, x2, p.mid());
  }
}
