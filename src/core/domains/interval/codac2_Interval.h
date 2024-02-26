/** 
 *  \file
 *  
 *  This class reuses many of the functions developed for ibex::Interval. 
 *  The original IBEX code is encapsulated in Codac for allowing inheritance 
 *  to Codac classes and also for documentation and independency purposes.
 *  See ibex::Interval (IBEX lib, author: G. Chabert)
 *  
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_INTERVAL_H__
#define __CODAC2_INTERVAL_H__

#include <list>
#include <array>
#include <ibex_Interval.h>
#include "codac2_Domain.h"

namespace codac2
{
  const double oo = POS_INFINITY;

  class Interval : protected ibex::Interval, public Domain
  {
    public:

      using DegeneratedType = double;

      Interval();
      Interval(double a);
      Interval(double a, double b);
      Interval(std::array<double,1> array);
      Interval(std::array<double,2> array);
      Interval(const Interval& x);
      Interval(const ibex::Interval& x);
      double volume() const;
      size_t size() const;

      std::list<Interval> complementary(bool compactness = true) const;
      std::list<Interval> diff(const Interval& y, bool compactness = true) const;

      bool is_empty() const;
      bool is_unbounded() const;
      bool is_degenerated() const;

      double lb() const;
      double ub() const;
      double diam() const;
      double mid() const;
      bool contains(double x) const;

      bool is_bisectable() const;
      bool interior_contains(double x) const;
      std::pair<Interval,Interval> bisect(float ratio = 0.49) const;

      bool operator==(const Interval& x) const;
      bool operator!=(const Interval& x) const;

      Interval& operator|=(const Interval& x);
      Interval& operator&=(const Interval& x);
      Interval& operator+=(const Interval& x);
      Interval& operator-=(const Interval& x);
      Interval& operator*=(const Interval& x);
      Interval& operator/=(const Interval& x);

      static Interval empty_set();
      
      friend std::ostream& operator<<(std::ostream& os, const Interval& x);
      friend Interval operator&(const Interval& x, const Interval& y);
      friend Interval operator+(const Interval& x);
      friend Interval operator+(const Interval& x, const Interval& y);
      friend Interval operator-(const Interval& x);
      friend Interval operator-(const Interval& x, const Interval& y);
      friend Interval operator*(const Interval& x, const Interval& y);
      friend Interval operator/(const Interval& x, const Interval& y);

      friend Interval sqr(const Interval& x);
      friend Interval sqrt(const Interval& x);
      friend Interval pow(const Interval& x, int n);
      friend Interval pow(const Interval& x, double d);
      friend Interval pow(const Interval& x, const Interval& y);
      friend Interval root(const Interval& x, int n);
      friend Interval exp(const Interval& x);
      friend Interval log(const Interval& x);
      friend Interval cos(const Interval& x);
      friend Interval sin(const Interval& x);
      friend Interval tan(const Interval& x);
      friend Interval acos(const Interval& x);
      friend Interval asin(const Interval& x);
      friend Interval atan(const Interval& x);
      friend Interval atan2(const Interval& y, const Interval& x);
      friend Interval cosh(const Interval& x);
      friend Interval sinh(const Interval& x);
      friend Interval tanh(const Interval& x);
      friend Interval acosh(const Interval& x);
      friend Interval asinh(const Interval& x);
      friend Interval atanh(const Interval& x);
      friend Interval abs(const Interval& x);
      friend Interval max(const Interval& x, const Interval& y);
      friend Interval min(const Interval& x, const Interval& y);
      friend Interval sign(const Interval& x);
      friend Interval chi(const Interval& a, const Interval& b, const Interval& c);
      friend Interval integer(const Interval& x);
      friend Interval floor(const Interval& x);
      friend Interval ceil(const Interval& x);
      friend bool bwd_add(const Interval& y, Interval& x1, Interval& x2);
      friend bool bwd_sub(const Interval& y, Interval& x1, Interval& x2);
      friend bool bwd_mul(const Interval& y, Interval& x1, Interval& x2);
      friend bool bwd_div(const Interval& y, Interval& x1, Interval& x2);
      friend bool bwd_sqr(const Interval& y, Interval& x);
      friend bool bwd_sqrt(const Interval& y, Interval& x);
      friend bool bwd_pow(const Interval& y, int n, Interval& x);
      friend bool bwd_pow(const Interval& y, Interval& x1, Interval& x2);
      friend bool bwd_root(const Interval& y, int n, Interval& x);
      friend bool bwd_exp(const Interval& y, Interval& x);
      friend bool bwd_log(const Interval& y, Interval& x);
      friend bool bwd_cos(const Interval& y, Interval& x);
      friend bool bwd_sin(const Interval& y, Interval& x);
      friend bool bwd_tan(const Interval& y, Interval& x);
      friend bool bwd_acos(const Interval& y, Interval& x);
      friend bool bwd_asin(const Interval& y, Interval& x);
      friend bool bwd_atan(const Interval& y, Interval& x);
      friend bool bwd_atan2(const Interval& y, Interval& x1, Interval& x2);
      friend bool bwd_cosh(const Interval& y, Interval& x);
      friend bool bwd_sinh(const Interval& y, Interval& x);
      friend bool bwd_tanh(const Interval& y, Interval& x);
      friend bool bwd_acosh(const Interval& y, Interval& x);
      friend bool bwd_asinh(const Interval& y, Interval& x);
      friend bool bwd_atanh(const Interval& y, Interval& x);
      friend bool bwd_abs(const Interval& y, Interval& x);
      friend bool bwd_max(const Interval& y, Interval& x1, Interval& x2);
      friend bool bwd_min(const Interval& y, Interval& x1, Interval& x2);
      friend bool bwd_sign(const Interval& y, Interval& x);
      friend bool bwd_chi(const Interval& f, Interval& a, Interval& b, Interval& c);
      friend bool bwd_floor(const Interval& y, Interval& x);
      friend bool bwd_ceil(const Interval& y, Interval& x);
      friend bool bwd_saw(const Interval& y, Interval& x);
      friend bool bwd_imod(Interval& x, Interval& y, double p);


      friend const ibex::Interval& to_ibex(const Interval& x);
      friend const Interval& to_codac(const ibex::Interval& x);
  };

  inline Interval operator&(const Interval& x, const Interval& y)
  {
    return ibex::operator&(x,y);
  }

  inline Interval operator+(const Interval& x)
  {
    return x;
  }

  inline Interval operator+(const Interval& x, const Interval& y)
  {
    return ibex::operator+(x,y);
  }

  inline Interval operator-(const Interval& x)
  {
    return -x;
  }

  inline Interval operator-(const Interval& x, const Interval& y)
  {
    return ibex::operator-(x,y);
  }

  inline Interval operator*(const Interval& x, const Interval& y)
  {
    return ibex::operator*(x,y);
  }

  inline Interval operator/(const Interval& x, const Interval& y)
  {
    return ibex::operator/(x,y);
  }

  Interval operator""_i(long double x);

  Interval sqr(const Interval& x);
  Interval sqrt(const Interval& x);
  Interval pow(const Interval& x, int n);
  Interval pow(const Interval& x, double d);
  Interval pow(const Interval& x, const Interval& y);
  Interval root(const Interval& x, int n);
  Interval exp(const Interval& x);
  Interval log(const Interval& x);
  Interval cos(const Interval& x);
  Interval sin(const Interval& x);
  Interval tan(const Interval& x);
  Interval acos(const Interval& x);
  Interval asin(const Interval& x);
  Interval atan(const Interval& x);
  Interval atan2(const Interval& y, const Interval& x);
  Interval cosh(const Interval& x);
  Interval sinh(const Interval& x);
  Interval tanh(const Interval& x);
  Interval acosh(const Interval& x);
  Interval asinh(const Interval& x);
  Interval atanh(const Interval& x);
  Interval abs(const Interval& x);
  Interval max(const Interval& x, const Interval& y);
  Interval min(const Interval& x, const Interval& y);
  Interval sign(const Interval& x);
  Interval chi(const Interval& a, const Interval& b, const Interval& c);
  Interval integer(const Interval& x);
  Interval floor(const Interval& x);
  Interval ceil(const Interval& x);
  bool bwd_add(const Interval& y, Interval& x1, Interval& x2);
  bool bwd_sub(const Interval& y, Interval& x1, Interval& x2);
  bool bwd_mul(const Interval& y, Interval& x1, Interval& x2);
  bool bwd_div(const Interval& y, Interval& x1, Interval& x2);
  bool bwd_sqr(const Interval& y, Interval& x);
  bool bwd_sqrt(const Interval& y, Interval& x);
  bool bwd_pow(const Interval& y, int n, Interval& x);
  bool bwd_pow(const Interval& y, Interval& x1, Interval& x2);
  bool bwd_root(const Interval& y, int n, Interval& x);
  bool bwd_exp(const Interval& y, Interval& x);
  bool bwd_log(const Interval& y, Interval& x);
  bool bwd_cos(const Interval& y, Interval& x);
  bool bwd_sin(const Interval& y, Interval& x);
  bool bwd_tan(const Interval& y, Interval& x);
  bool bwd_acos(const Interval& y, Interval& x);
  bool bwd_asin(const Interval& y, Interval& x);
  bool bwd_atan(const Interval& y, Interval& x);
  bool bwd_atan2(const Interval& y, Interval& x1, Interval& x2);
  bool bwd_cosh(const Interval& y, Interval& x);
  bool bwd_sinh(const Interval& y, Interval& x);
  bool bwd_tanh(const Interval& y, Interval& x);
  bool bwd_acosh(const Interval& y, Interval& x);
  bool bwd_asinh(const Interval& y, Interval& x);
  bool bwd_atanh(const Interval& y, Interval& x);
  bool bwd_abs(const Interval& y, Interval& x);
  bool bwd_max(const Interval& y, Interval& x1, Interval& x2);
  bool bwd_min(const Interval& y, Interval& x1, Interval& x2);
  bool bwd_sign(const Interval& y, Interval& x);
  bool bwd_chi(const Interval& f, Interval& a, Interval& b, Interval& c);
  bool bwd_floor(const Interval& y, Interval& x);
  bool bwd_ceil(const Interval& y, Interval& x);
  bool bwd_saw(const Interval& y, Interval& x);
  bool bwd_imod(Interval& x, Interval& y, double p);

} // namespace codac

#endif