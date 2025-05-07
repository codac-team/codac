/** 
 *  \file codac2_atan2.h
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
#include "codac2_atan2.h"
#include "codac2_arith_div.h"

namespace codac2
{
  struct Atan2Op
  {
    template<typename X1,typename X2>
    static std::string str(const X1& x1, const X2& x2)
    {
      return "atan2(" + x1->str() + "," + x2->str() + ")";
    }

    template<typename X1, typename X2>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1, [[maybe_unused]] const X2& s2)
    {
      return {1,1};
    }

    static Interval fwd(const Interval& x1, const Interval& x2);
    static ScalarType fwd_natural(const ScalarType& x1, const ScalarType& x2);
    static ScalarType fwd_centered(const ScalarType& x1, const ScalarType& x2);
    static void bwd(const Interval& y, Interval& x1, Interval& x2);
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  inline ScalarExpr
  atan2(const ScalarExpr& x1, const ScalarExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<Atan2Op,ScalarType,ScalarType,ScalarType>>(x1,x2) };
  }
  
  // Inline functions

  inline Interval Atan2Op::fwd(const Interval& x1, const Interval& x2)
  {
    return atan2(x1,x2);
  }

  inline ScalarType Atan2Op::fwd_natural(const ScalarType& x1, const ScalarType& x2)
  {
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain
      && !(x1.a == 0. && x2.a == 0.) // def domain of the derivative of atan2
    };
  }

  inline ScalarType Atan2Op::fwd_centered(const ScalarType& x1, const ScalarType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);

    assert(x1.da.rows() == 1);
    assert(x1.da.rows() == x2.da.rows() && x1.da.cols() == x2.da.cols());

    IntervalMatrix d(1,x1.da.cols());
    for(Index i = 0 ; i < d.size() ; i++)
      d(0,i) = (-x1.a*x2.da(0,i)/(sqr(x2.a)+sqr(x1.a)))+(x2.a*x1.da(0,i)/(sqr(x2.a)+sqr(x1.a)));

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain
      && !(x1.a == 0. && x2.a == 0.) // def domain of the derivative of atan2
    };
  }

  inline void Atan2Op::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    // The content of this function comes from the IBEX library.
    // See ibex::Interval (IBEX lib, main author: Gilles Chabert)
    //   https://ibex-lib.readthedocs.io

    // Probably not the most efficient implementation ever...

    // half-plane x2>0
    Interval y_x2pos = y & Interval(-1,1)*Interval::half_pi();
    // upper left quadrant
    Interval y_x2neg_x1pos = y & (Interval::half_pi() | Interval::pi());
    // lower left quadrant
    Interval y_x2neg_x1neg = y & -(Interval::half_pi() | Interval::pi());

    Interval xres = Interval::empty();
    Interval yres = Interval::empty();

    if(!y_x2pos.is_empty())
    {
      Interval xpos = x2 & Interval(0,oo);
      Interval yall = x1;

      if(y_x2neg_x1pos.is_empty() || y_x2neg_x1neg.is_empty())
      {
        Interval z = yall/xpos;
        AtanOp::bwd(y_x2pos,z);
        DivOp::bwd(z,yall,xpos);
      } // Otherwise, all is valid (useless to contract)

      xres |= xpos;
      yres |= yall;

      // Because atan is not defined for pi/2 and -pi/2
      if(y_x2pos.lb() >= Interval::half_pi().lb())
      {
        xres |= (x2 & Interval::zero());
        yres |= (x1 & Interval(0,oo));
      }
      else if(y_x2pos.ub() <= -Interval::half_pi().lb())
      {
        xres |= (x2 & Interval::zero());
        yres |= (x1 & Interval(-oo,0));
      }
    }

    if(!y_x2neg_x1pos.is_empty())
    {
      Interval xneg = x2 & Interval(-oo,0);
      Interval ypos = x1 & Interval(0,oo);
      Interval z = ypos/xneg;
      AtanOp::bwd(y_x2neg_x1pos - Interval::pi(),z);
      DivOp::bwd(z,ypos,xneg);
      xres |= xneg;
      yres |= ypos;
    }

    if(!y_x2neg_x1neg.is_empty())
    {
      Interval xneg = x2 & Interval(-oo,0);
      Interval yneg = x1 & Interval(-oo,0);
      Interval z = yneg/xneg;
      AtanOp::bwd(y_x2neg_x1neg + Interval::pi(),z);
      DivOp::bwd(z,yneg,xneg);
      xres |= xneg;
      yres |= yneg;
    }

    x2 = xres;
    x1 = yres;
  }
}
