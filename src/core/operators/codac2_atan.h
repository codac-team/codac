/** 
 *  \file codac2_atan.h
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

namespace codac2
{
  struct AtanOp
  {
    template<typename X1>
    static std::string str(const X1& x1)
    {
      return "atan(" + x1->str() + ")";
    }

    template<typename X1>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1)
    {
      return {1,1};
    }

    static Interval fwd(const Interval& x1);
    static ScalarType fwd_natural(const ScalarType& x1);
    static ScalarType fwd_centered(const ScalarType& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  inline ScalarExpr
  atan(const ScalarExpr& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<AtanOp,ScalarType,ScalarType>>(x1) };
  }
  
  // Inline functions

  inline Interval AtanOp::fwd(const Interval& x1)
  {
    return atan(x1);
  }

  inline ScalarType AtanOp::fwd_natural(const ScalarType& x1)
  {
    return {
      fwd(x1.a),
      x1.def_domain
    };
  }

  inline ScalarType AtanOp::fwd_centered(const ScalarType& x1)
  {
    if(centered_form_not_available_for_args(x1))
      return fwd_natural(x1);

    IntervalMatrix d(1,x1.da.size());
    for(Index i = 0 ; i < d.size() ; i++)
      d(0,i) = x1.da(0,i)/(1.+sqr(x1.a));

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain
    };
  }

  inline void AtanOp::bwd(const Interval& y, Interval& x1)
  {
    // The content of this function comes from the IBEX library.
    // See ibex::Interval (IBEX lib, main author: Gilles Chabert)
    //   https://ibex-lib.readthedocs.io

    if(y.is_empty())
      x1.set_empty();

    else
    {
      // Note: if y.ub>pi/2 or y.lb<-pi/2, tan(y) gives (-oo,oo).
      // so the implementation is not as simple as x1 &= tan(y).

      Interval z = y;
      double pi2l = (Interval::pi()/2).lb();
      double pi2u = (Interval::pi()/2).ub();

      if(z.ub() >= pi2l) // not pi2u. See comments below.
      {
        if(z.lb() >= pi2u)
          x1.set_empty();

        else
        {
          if(z.lb() > -pi2l)
          {
            // Note 1: tan(z^-) can give an interval (-oo,+oo) if
            // z^- is close to -pi/2. Even in this case we keep the
            // lower bound -oo.
            //
            // Note 2: if we had used z.lb()<-pi2u (with pi2u>pi/2)
            // instead of z.lb()<-pi2l, it may be possible, in theory,
            // that the calculated lower bound is a high value close to +oo, which would be incorrect.
            //
            // Note 3: if z.lb() is close to pi/2, the lower bound of tan(z.lb()) can be -oo. There
            // is nothing we can do about it (the lower bound cannot be evaluated in this case)
            //
            // Note 4: tan(z.lb()) cannot be an empty set since z.lb() cannot be exactly pi/2.
            x1 &= Interval(tan(Interval(z.lb())).lb(),oo);
          }

          // else do nothing
        }
      }

      else
      {
        if(z.ub() <= -pi2u)
          x1.set_empty();

        else if(z.lb() < -pi2l)
          // Same comments as above
          x1 &= Interval(-oo,tan(Interval(z.ub())).ub());

        else
          x1 &= Interval(tan(Interval(z.lb())).lb(), tan(Interval(z.ub())).ub());
      }
    }
  }
}
