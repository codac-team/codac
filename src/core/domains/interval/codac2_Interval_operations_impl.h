/** 
 *  \file codac2_Interval_operations_impl.h
 * 
 *  This class reuses several functions developed for ibex::Interval.
 *  See ibex::Interval (IBEX lib, main author: Gilles Chabert)
 *    https://ibex-lib.readthedocs.io
 *  
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Gilles Chabert, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

// Inline functions

namespace codac2
{
  inline Interval sqr(const Interval& x)
  {
    return gaol::sqr(x);
  }

  // ---------------------------------------------------------------------------
  // The elementary functions, whose code depends on the GAOL Codac is built on
  // ---------------------------------------------------------------------------
  //
  // CODAC_GAOL_GUARDS (see codac2_Interval.h) chooses between two versions of
  // them, as in the block at the end of codac2_Interval_impl.h: the code Codac
  // had before GAOL v5, with its guards around GAOL 4, unchanged, and the code
  // that calls GAOL v5 directly.
#if CODAC_GAOL_GUARDS

  // With the guards, for GAOL 4, the GAOL that IBEX bundles (WITH_IBEX):
  // gaol::round_upward() after each function, which GAOL 4 may leave rounding
  // in another direction (see codac2_Interval_impl.h); pow(x,p) empty for an
  // infinite p; log of an interval without positive numbers empty, where GAOL 4
  // gives [-oo,-DBL_MAX] for x.ub() == 0; cosh of an unbounded interval made to
  // reach +oo; asinh computed from nonnegative arguments, by symmetry; the
  // roots of odd order of negative numbers computed by symmetry too, the
  // nth_root of GAOL 4 leaving negative numbers out of its domain; atan2
  // computed by Codac; and sign(), which GAOL 4 does not have, computed by
  // Codac, a sign of 0 being any of [-1,1].

  inline Interval sqrt(const Interval& x)
  {
    Interval y = gaol::sqrt(x);
    gaol::round_upward();
    return y;
  }

  inline Interval pow(const Interval& x, int p)
  {
    Interval y = gaol::pow(x,p);
    //gaol::round_upward(); // not necessary?
    return y;
  }

  inline Interval pow(const Interval& x, double p)
  {
    if(p == -oo || p == oo)
      return Interval::empty();

    else
    {
      Interval y = gaol::pow(x,p);
      gaol::round_upward();
      return y;
    }
  }

  inline Interval pow(const Interval& x, const Interval& p)
  {
    Interval y = gaol::pow(x,p);
    gaol::round_upward();
    return y;
  }

  inline Interval root(const Interval& x, int p)
  {
    // Get the root of the positive part (gaol does
    // not consider negative values to be in the definition
    // domain of the root function)

    gaol::interval y = gaol::nth_root(x, p>=0 ? p : -p);

    if(p%2 == 1 && x.lb() < 0)
      y |= -gaol::nth_root(-x, p >= 0 ? p : -p);

    if(p < 0)
      y = 1.0/y;

    gaol::round_upward();
    return y;
  }

  inline Interval exp(const Interval& x)
  {
    Interval y = gaol::exp(x);
    gaol::round_upward();
    return y;
  }

  inline Interval log(const Interval& x)
  {
    if(x.ub() <= 0) // gaol returns (-oo,-DBL_MAX) if x.ub()==0, instead of empty set
      return Interval::empty();

    else
    {
      Interval y = gaol::log(x);
      gaol::round_upward();
      return y;
    }
  }

  inline Interval cos(const Interval& x)
  {
    Interval y = gaol::cos(x);
    gaol::round_upward();
    return y;
  }

  inline Interval sin(const Interval& x)
  {
    Interval y = gaol::sin(x);
    gaol::round_upward();
    return y;
  }

  inline Interval tan(const Interval& x)
  {
    Interval y = gaol::tan(x);
    gaol::round_upward();
    return y;
  }

  inline Interval acos(const Interval& x)
  {
    Interval y = gaol::acos(x);
    gaol::round_upward();
    return y;
  }

  inline Interval asin(const Interval& x)
  {
    Interval y = gaol::asin(x);
    gaol::round_upward();
    return y;
  }

  inline Interval atan(const Interval& x)
  {
    Interval y = gaol::atan(x);
    gaol::round_upward();
    return y;
  }

  inline Interval atan2(const Interval& y, const Interval& x)
  {
    if(y.is_empty() || x.is_empty())
      return Interval::empty();

    // We handle the special case x=[0,0] separately
    else if(x == Interval::zero())
    {
      if(y.lb() >= 0)
      {
        if(y.ub() == 0)
          return Interval::empty(); // atan2(0,0) is undefined
        else
          return Interval::half_pi();
      }

      else if(y.ub() <= 0)
        return -Interval::half_pi();

      else
        return Interval(-1,1)*Interval::half_pi();
    }

    else if(x.lb() >= 0)
      return atan(y/x); // now, x.ub()>0 -> atan does not give an empty set

    else if(x.ub() <= 0)
    {
      if(y.lb() >= 0)
        return atan(y/x) + Interval::pi(); // x.lb()<0
      else if(y.ub() < 0)
        return atan(y/x) - Interval::pi();
      else
        return Interval(-1,1)*Interval::pi();
    }

    else
    {
      if(y.lb() >= 0)
        return atan(y/x.ub()) | (atan(y/x.lb()) + Interval::pi());

      else if(y.ub() <= 0)
      {
        if(x.lb() != -oo)
        {
          if(x.ub() != oo)
            return (atan(y/x.lb())-Interval::pi()) | atan(y/x.ub());
          else
            return (atan(y/x.lb())-Interval::pi()) | Interval::zero();
        }

        else
        {
          if(x.ub() != oo)
            return (-Interval::pi()) | atan(y/x.ub());
          else
            return -Interval::pi() | Interval::zero();
        }
      }

      else
        return Interval(-1,1)*Interval::pi();
    }
  }

  inline Interval cosh(const Interval& x)
  {
    Interval y;
    if(x.is_unbounded()) 
      y = Interval(gaol::cosh(x).left(),oo);
    else
      y = gaol::cosh(x);
    gaol::round_upward();
    return y;
  }

  inline Interval sinh(const Interval& x)
  {
    Interval y = gaol::sinh(x);
    gaol::round_upward();
    return y;
  }

  inline Interval tanh(const Interval& x)
  {
    Interval y = gaol::tanh(x);
    gaol::round_upward();
    return y;
  }

  inline Interval acosh(const Interval& x)
  {
    Interval y = gaol::acosh(x);
    gaol::round_upward();
    return y;
  }

  inline Interval asinh(const Interval& x)
  {
    if(x.is_empty())
      return Interval::empty();

    else if(x.lb() >= 0)
      return gaol::asinh(x);

    else if(x.ub() <= 0)
      return -gaol::asinh(-x);

    else
      return {
        -gaol::asinh(gaol::interval(0,-x.lb())).right(),
        gaol::asinh(gaol::interval(0,x.ub())).right()
      };

    // no round_upward?
  }

  inline Interval atanh(const Interval& x)
  {
    Interval y = gaol::atanh(x);
    gaol::round_upward();
    return y;
  }

  inline Interval sign(const Interval& x)
  {
    return x.ub() < 0 ? -1. : x.lb() > 0 ? 1. : Interval(-1.,1.);
  }

#else

  // Without the guards, for GAOL v5 (https://github.com/Jordan08/GAOL), Codac's
  // default: GAOL is called directly. GAOL v5 leaves the rounding upward after
  // each function, handles each of the cases above itself (its nth_root is the
  // rootn of IEEE 1788, which takes the roots of odd order of negative
  // numbers), and Codac takes atan2 and sign from it (those of IEEE 1788).
  //
  // chi(), after this block, stays Codac's in both versions: the chi() of GAOL
  // is another function, the degree of symmetry of an interval of Ratschek and
  // Rokne (a double, a/b or b/a for [a,b]), where Codac's chi(x,y,z) chooses
  // between y and z after the sign of x.

  inline Interval sqrt(const Interval& x)
  {
    return gaol::sqrt(x);
  }

  inline Interval pow(const Interval& x, int p)
  {
    return gaol::pow(x,p);
  }

  inline Interval pow(const Interval& x, double p)
  {
    return gaol::pow(x,p);
  }

  // gaol::pow(x,p) of GAOL v5 is the hull of x^v over x and p, x^v being the
  // integer power x^n (pown of IEEE 1788, defined for x < 0 too, 0^0 being 1)
  // for an integer v = n of p, and the pow of IEEE 1788 for any other v:
  // pow([-2,2],[0.5,3]) is [-8,8], (-2)^3 being -8, and pow([0],[0,1]) is
  // [0,1]. GAOL 5.0.0 left the negative bases out for any p but a degenerate
  // integer, and gave [0,8] and [0] for these, which GAOL 5.0.1 corrected
  // (https://github.com/Jordan08/GAOL/issues/400): Codac asks for GAOL 5.0.2 or
  // later (CODAC_GAOL_MIN_VERSION, in scripts/CMakeModules/codac_gaol.cmake).
  // PowOp::bwd() takes the reverses of this power, pow_rel() and
  // pow_exponent_rel() of GAOL v5 (see codac2_pow.h).
  inline Interval pow(const Interval& x, const Interval& p)
  {
    return gaol::pow(x,p);
  }

  inline Interval root(const Interval& x, int p)
  {
    Interval y = gaol::nth_root(x, p >= 0 ? p : -p);

    if(p < 0)
      y = 1.0/y;

    return y;
  }

  inline Interval exp(const Interval& x)
  {
    return gaol::exp(x);
  }

  inline Interval log(const Interval& x)
  {
    return gaol::log(x);
  }

  inline Interval cos(const Interval& x)
  {
    return gaol::cos(x);
  }

  inline Interval sin(const Interval& x)
  {
    return gaol::sin(x);
  }

  inline Interval tan(const Interval& x)
  {
    return gaol::tan(x);
  }

  inline Interval acos(const Interval& x)
  {
    return gaol::acos(x);
  }

  inline Interval asin(const Interval& x)
  {
    return gaol::asin(x);
  }

  inline Interval atan(const Interval& x)
  {
    return gaol::atan(x);
  }

  inline Interval atan2(const Interval& y, const Interval& x)
  {
    return gaol::atan2(y,x);
  }

  inline Interval cosh(const Interval& x)
  {
    return gaol::cosh(x);
  }

  inline Interval sinh(const Interval& x)
  {
    return gaol::sinh(x);
  }

  inline Interval tanh(const Interval& x)
  {
    return gaol::tanh(x);
  }

  inline Interval acosh(const Interval& x)
  {
    return gaol::acosh(x);
  }

  inline Interval asinh(const Interval& x)
  {
    return gaol::asinh(x);
  }

  inline Interval atanh(const Interval& x)
  {
    return gaol::atanh(x);
  }

  inline Interval sign(const Interval& x)
  {
    // The sign of IEEE 1788 (GAOL v5): the hull of the signs of the numbers of
    // x, -1 below 0, 0 at 0 (and at -0), 1 above, and the empty set for the
    // empty set. Codac's sign, which WITH_IBEX keeps, gave [-1,1] for every x
    // containing 0, a sign of 0 being any of [-1,1]: the two differ when 0 is
    // a bound of x or x itself, sign([0,5]) being [0,1] instead of [-1,1] and
    // sign([0]) [0] instead of [-1,1], and for the empty set. SignOp::bwd()
    // (codac2_sign.h) takes the reverse of this sign, sign_rel() of GAOL v5,
    // which keeps 0 for the sign 0 only.
    return gaol::sign(x);
  }

#endif

  inline Interval abs(const Interval& x)
  {
    return gaol::abs(x);
  }

  inline Interval min(const Interval& x, const Interval& y)
  {
    return gaol::min(x,y);
  }

  inline Interval max(const Interval& x, const Interval& y)
  {
    return gaol::max(x,y);
  }

  inline Interval integer(const Interval& x)
  {
    return gaol::integer(x);
  }

  inline Interval floor(const Interval& x)
  {
    return gaol::floor(x);
  }

  inline Interval ceil(const Interval& x)
  {
    return gaol::ceil(x);
  }

  inline Interval chi(const Interval& x, const Interval& y, const Interval& z)
  {
    if(x.ub() <= 0)
      return y;
    
    else if(x.lb() > 0)
      return z;

    else
      return y | z;
  }
}