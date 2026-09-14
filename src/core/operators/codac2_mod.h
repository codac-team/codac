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
  static inline double canonical_mod(double x, double p)
  {
    double r = std::fmod(x, p);
    if(r < 0.0) r += p;
    return r;
  }

  struct ModOp
  {
    template<typename X1,typename P>
    static std::string str(const X1& x1, const P& p)
    {
      return "mod(" + x1->str() + "," + p->str() + ")";
    }

    template<typename X1,typename P>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1, [[maybe_unused]] const P& p)
    {
      return {1,1};
    }

    static Interval fwd(const Interval& x1, const Interval& p);
    static ScalarType fwd_natural(const ScalarType& x1, const ScalarType& p);
    static ScalarType fwd_centered(const ScalarType& x1, const ScalarType& p);
    static void bwd(const Interval& y, Interval& x1, Interval& p);

    static void fwd_bwd(Interval& y, Interval& x1, double p);
    static void fwd_bwd(Interval& y, Interval& x1, Interval& p);
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  inline ScalarExpr
  mod(const ScalarExpr& x1, const ScalarExpr& p)
  {
    return { std::make_shared<AnalyticOperationExpr<ModOp,ScalarType,ScalarType,ScalarType>>(x1,p) };
  }

  // Inline functions

  inline Interval ModOp::fwd(const Interval& x1, const Interval& p)
  {
    if(x1.is_empty() || p.is_empty())
      return Interval::empty();

    Interval pp = p & Interval(next_float(0),oo);
    if(pp.is_empty())
      return Interval::empty();

    // Exact case: singleton / singleton
    if(x1.is_degenerated() && pp.is_degenerated())
      return { canonical_mod(x1.lb(), pp.lb()) };

    // Exact case: for every admissible p, x1 is already in the canonical band [0,p)
    if(x1.lb() >= 0.0 && x1.ub() < pp.lb())
      return x1;

    // Exact case: scalar period and constant quotient over the whole x1
    if(pp.lb() == pp.ub())
    {
      const double q = pp.lb();
      const double k1 = std::floor(x1.lb() / q);
      const double k2 = std::floor(x1.ub() / q);

      if(k1 == k2)
      {
        // y = x1 - k*q
        return x1 - (Interval(k1) * q);
      }
    }

    // Safe but intentionally simple enclosure
    double yub = pp.ub();
    if(x1.lb() >= 0.0)
      yub = std::min(yub, x1.ub());

    return { 0.0, yub };
  }

  inline ScalarType ModOp::fwd_natural(const ScalarType& x1, const ScalarType& p)
  {
    return {
      fwd(x1.a, p.a),
      x1.def_domain && p.def_domain
    };
  }

  inline ScalarType ModOp::fwd_centered(const ScalarType& x1, const ScalarType& p)
  {
    if(centered_form_not_available_for_args(x1,p))
      return fwd_natural(x1,p);

    return {
      fwd(x1.m,p.m),
      fwd(x1.a,p.a),
      IntervalMatrix(0,0), // not supported yet for auto diff
      x1.def_domain && p.def_domain
    };
  }

  inline void ModOp::bwd(const Interval& y, Interval& x1, Interval& p)
  {
    auto set_empty = [&]()
    {
      x1.set_empty();
      p.set_empty();
    };

    if(y.is_empty() || x1.is_empty() || p.is_empty())
    {
      set_empty();
      return;
    }

    // Modulo is defined only for p > 0
    p &= Interval(next_float(0),oo);
    if(p.is_empty())
    {
      set_empty();
      return;
    }

    // Since y is not contracted here (const), we only keep the part
    // potentially compatible with a canonical remainder: 0 <= y <= p.ub()
    Interval y0 = y & Interval(0.0, p.ub());
    if(y0.is_empty())
    {
      set_empty();
      return;
    }

    // Candidate integer quotients:
    // x1 = y + k p  <=>  k = (x1 - y)/p
    Interval r = (x1-y0)/p;

    const double kmin_d = std::ceil(r.lb());
    const double kmax_d = std::floor(r.ub());

    if(!(kmin_d <= kmax_d))
    {
      set_empty();
      return;
    }

    Interval K(kmin_d, kmax_d);

    // Coarse hull contraction
    x1 &= y0+K*p;
    if(x1.is_empty())
    {
      set_empty();
      return;
    }

    // If 0 is not in K, we can also contract p approximately
    if(!K.contains(0.))
    {
      p &= (x1-y0)/K;
      p &= Interval(next_float(0),oo);

      if(p.is_empty())
      {
        set_empty();
        return;
      }

      y0 &= Interval(0.0, p.ub());
      if(y0.is_empty())
      {
        set_empty();
        return;
      }

      x1 &= y0+K*p;
      if(x1.is_empty())
      {
        set_empty();
        return;
      }
    }
  }

  inline void ModOp::fwd_bwd(Interval& y, Interval& x1, double p)
  {
    Interval ip(p);
    ModOp::fwd_bwd(y,x1,ip);
  }

  inline void ModOp::fwd_bwd(Interval& y, Interval& x1, Interval& p)
  {
    y &= ModOp::fwd(x1,p);
    ModOp::bwd(y,x1,p);
  }
}
