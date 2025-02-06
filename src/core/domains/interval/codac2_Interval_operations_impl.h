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

  inline Interval sign(const Interval& x)
  {
    return x.ub() < 0 ? -1. : x.lb() > 0 ? 1. : Interval(-1.,1.);
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
}