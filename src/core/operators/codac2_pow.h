/** 
 *  \file codac2_pow.h
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
#include "codac2_sqr.h"

namespace codac2
{
  struct PowOp
  {
    template<typename X1,typename X2>
    static std::string str(const X1& x1, const X2& x2)
    {
      return "(" + x1->str(!x1->is_str_leaf()) + ")^(" + x2->str(!x2->is_str_leaf()) + ")";
    }

    template<typename X1, typename X2>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1, [[maybe_unused]] const X2& s2)
    {
      return {1,1};
    }
    
    static Interval fwd(const Interval& x1, const Interval& x2);
    static ScalarType fwd_natural(const ScalarType& x1, const ScalarType& x2);
    static ScalarType fwd_centered(const ScalarType& x1, const ScalarType& x2);
    static void bwd(const Interval& y, Interval& x1, int x2);
    static void bwd(const Interval& y, Interval& x1, Interval& x2);
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  inline ScalarExpr
  pow(const ScalarExpr& x1, const ScalarExpr& x2)
  {
    auto x2_as_const_value = std::dynamic_pointer_cast<ConstValueExpr<ScalarType>>(x2->copy());
    if(x2_as_const_value && x2_as_const_value->value() == 2.)
    {
      // SqrOp may be better than the generic PowOp with a simple power of 2
      return { std::make_shared<AnalyticOperationExpr<SqrOp,ScalarType,ScalarType>>(x1) };
    }

    return { std::make_shared<AnalyticOperationExpr<PowOp,ScalarType,ScalarType,ScalarType>>(x1,x2) };
  }

  inline ScalarExpr
  operator^(const ScalarExpr& x1, const ScalarExpr& x2)
  {
    std::cout
      << "Operator '^' is discouraged in Codac. Use pow(x,y), or sqr(x) instead."
      << std::endl;
    return pow(x1,x2);
  }

  // Inline functions

  inline Interval PowOp::fwd(const Interval& x1, const Interval& x2)
  {
    return pow(x1,x2);
  }

  inline ScalarType PowOp::fwd_natural(const ScalarType& x1, const ScalarType& x2)
  {
    bool x2isint = x2.a.is_integer();
    bool x2positive = x2.a.lb()>=0.0;
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain 
        && (x2isint || x1.a.lb()>=0.0)
        && (x2positive || !x1.a.contains(0.0))
    };
  }

  inline ScalarType PowOp::fwd_centered(const ScalarType& x1, const ScalarType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);

    bool x2isint = x2.a.is_integer();
    bool x2positive = x2.a.lb()>=0.0;
    IntervalMatrix d(1,x1.da.size());
    
    if (x2.a.is_degenerated())
    { 
      // To avoid calling log(x1.a), as it would return emptyset if x1.a<=0
      for(Index i = 0 ; i < d.size() ; i++)
        d(0,i) = x2.a*x1.da(0,i)*pow(x1.a,x2.a-1.);
    }

    else
    {
      for(Index i = 0 ; i < d.size() ; i++)
      {
        d(0,i) = x2.a*x1.da(0,i)*pow(x1.a,x2.a-1.) 
                + x2.da(0,i)*log(x1.a)*pow(x1.a,x2.a); 
			  // Not good when x1 close to 0
      }
    }

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain 
        && (x2isint || x1.a.lb()>=0.0)
        && (x2positive || !x1.a.contains(0.0))
    };
  }

  inline void PowOp::bwd(const Interval& y, Interval& x1, int x2)
  {
#if CODAC_GAOL_GUARDS
    // The content of this function comes from the IBEX library.
    // See ibex::Interval (IBEX lib, main author: Gilles Chabert)
    //   https://ibex-lib.readthedocs.io

    if(x2 % 2 == 0)
    {
      Interval proj = root(y, x2);
      Interval pos_proj = proj & x1;
      Interval neg_proj = (-proj) & x1;
      x1 = pos_proj | neg_proj;
    }

    else
      x1 &= root(y, x2);
#else
    // nth_root_rel() of GAOL v5, pownRev of IEEE 1788-2015, the reverse of the
    // integer power x^x2 that pow(x,int) computes (pown of IEEE 1788): the hull
    // of the x of x1 with x^x2 defined and in y, for every integer x2.
    //  - x2 >= 1: the same intervals as the code of IBEX above (compared on
    //    every pair of 41 intervals, for x2 from 1 to 5).
    //  - x2 = 0: x^0 is 1 for every x, 0 included, so that x1 is kept where y
    //    holds 1 and emptied elsewhere. The code of IBEX emptied x1 in every
    //    case, the root of order 0 being the empty set: it lost every solution
    //    (x1 = [-oo,+oo] and y = [-oo,+oo] gave the empty set).
    //  - x2 < 0: x^x2 = 1/x^|x2| has no value at 0, which is left out, and y is
    //    cut at 0 before the roots are taken, where the code of IBEX took the
    //    inverse of the root of the whole of y, whose hull may span the gap
    //    between its two parts: for y = [-1,2], x2 = -3 and x1 = [-0.5,0.5],
    //    GAOL gives the empty set (|x^-3| >= 8 on x1) and IBEX [-0.5,0.5].
    x1 = gaol::nth_root_rel(y, x2, x1);
#endif
  }

  inline void PowOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
#if CODAC_GAOL_GUARDS
    assert_release(x2.is_degenerated() && "PowOp::bwd(y,x1,x2) (with x1 and x2 intervals) not implemented yet with Gaol");
    assert_release(x2 == (int)(x2.mid()) && "PowOp::bwd(y,x1,x2) (x2 not integer) not implemented yet with Gaol");
    PowOp::bwd(y, x1, (int)(x2.mid()));
#else
    // pow(x1,x2) is the power gaol::pow(x1,x2) of GAOL v5 (see PowOp::fwd()),
    // the hull over the box of x^v, where x^v is
    //  - for an integer v = n: the integer power x^n, pown of IEEE 1788, which
    //    is defined for a negative x too, is 1 at n = 0 for every x, 0
    //    included, and has no value at x = 0 for n < 0;
    //  - for any other v: the pow of IEEE 1788, x^v = e^(v log x) for x > 0,
    //    0 for x = 0 and v > 0, and no value elsewhere.
    // pow_rel() and pow_exponent_rel() of GAOL v5 are its reverses, for the
    // same x^v: the hull of the bases x of x1 with x^v in y for some v of x2,
    // then of the exponents v of x2 with x^v in y for some x of the x1 just
    // contracted. They cover every exponent: a degenerate integer x2 = [n]
    // gives the reverse of x^n, as bwd(y,x1,int) above, beyond the ints too
    // (n >= 2^31 or n < -2^31), and the negative bases are kept for the
    // integers of x2: (-1,3) is a solution of y = [-1] in the box x1 = [-2,2],
    // x2 = [0.5,3], which x1 = [-1] and x2 = [1,3] hold.
    //
    // This needs GAOL 5.0.1 (https://github.com/Jordan08/GAOL/issues/400) or
    // later: Codac asks for 5.0.2 (CODAC_GAOL_MIN_VERSION, in
    // scripts/CMakeModules/codac_gaol.cmake). In GAOL 5.0.0, gaol::pow() took
    // the pow of IEEE 1788 for any exponent but a degenerate integer, which left
    // out the negative bases of the integers of x2 (gaol::pow([-2,2],[0.5,3])
    // was [0,8], although (-1)^3 = -1), and pow_rel() and pow_exponent_rel()
    // reversed the pow of IEEE 1788 only: x1 = [-3,-1] was emptied for y =
    // [-27,-1] and x2 = [3]. With GAOL 4, Codac only contracted x1 for an x2 [n]
    // within the ints (the assertions above).
    x1 = gaol::pow_rel(y, x2, x1);
    x2 = gaol::pow_exponent_rel(y, x1, x2);

    // No base of x1 with an exponent of x2, or the converse: the box holds no
    // solution
    if(x1.is_empty() || x2.is_empty())
    {
      x1.set_empty();
      x2.set_empty();
    }
#endif
  }
}
