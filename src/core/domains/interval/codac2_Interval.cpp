/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <cassert>
#include "codac2_Interval.h"

using namespace std;

namespace codac2
{
  Interval::Interval()
    : ibex::Interval()
  {

  }

  Interval::Interval(double a)
    : ibex::Interval(a)
  {

  }

  Interval::Interval(double a, double b)
    : ibex::Interval(a,b)
  {

  }

  Interval::Interval(std::array<double,1> array)
    : ibex::Interval(array)
  {

  }

  Interval::Interval(std::array<double,2> array)
    : ibex::Interval(array)
  {

  }
  
  Interval::Interval(const Interval& x)
    : ibex::Interval(x)
  {

  }
  
  Interval::Interval(const ibex::Interval& x)
    : ibex::Interval(x)
  {

  }

  size_t Interval::size() const
  {
    return 1;
  }

  double Interval::volume() const
  {
    return ibex::Interval::diam();
  }

  std::list<Interval> Interval::complementary(bool compactness) const
  {
    if(is_empty() || (compactness && is_degenerated()))
      return { {-oo,oo} };

    std::list<Interval> l;

    if(lb() > -oo)
      l.push_back({-oo,lb()});

    if(ub() < oo)
      l.push_back({ub(),oo});

    return l;
  }

  std::list<Interval> Interval::diff(const Interval& y, bool compactness) const
  {
    Interval y_inter = *this & y;

    if(y_inter.is_empty() || (y_inter.is_degenerated() && compactness))
      return { *this };

    std::list<Interval> l;
    for(const auto& li : y_inter.complementary(compactness))
      if(li.intersects(*this))
        l.push_back(*this & li);
    return l;
  }
  
  bool Interval::is_empty() const
  {
    return ibex::Interval::is_empty();
  }
  
  Interval Interval::empty_set()
  {
    return ibex::Interval::empty_set();
  }
  
  bool Interval::is_unbounded() const
  {
    return ibex::Interval::is_unbounded();
  }
  
  bool Interval::is_degenerated() const
  {
    return ibex::Interval::is_degenerated();
  }

  bool Interval::contains(double x) const
  {
    return ibex::Interval::contains(x);
  }

  double Interval::lb() const
  {
    return ibex::Interval::lb();
  }

  double Interval::ub() const
  {
    return ibex::Interval::ub();
  }

  double Interval::diam() const
  {
    return ibex::Interval::diam();
  }

  double Interval::mid() const
  {
    return ibex::Interval::diam();
  }

  bool Interval::is_bisectable() const
  {
    return ibex::Interval::is_bisectable();
  }

  bool Interval::interior_contains(double x) const
  {
    return ibex::Interval::interior_contains(x);
  }

  std::pair<Interval,Interval> Interval::bisect(float ratio) const
  {
    return ibex::Interval::bisect(ratio);
  }

  bool Interval::operator==(const Interval& x) const
  {
    return ibex::Interval::operator==(x);
  }
  
  bool Interval::operator!=(const Interval& x) const
  {
    return ibex::Interval::operator!=(x);
  }

  Interval& Interval::operator|=(const Interval& x)
  {
    ibex::Interval::operator|=(x);
    return *this;
  }

  Interval& Interval::operator&=(const Interval& x)
  {
    ibex::Interval::operator&=(x);
    return *this;
  }

  Interval& Interval::operator+=(const Interval& x)
  {
    ibex::Interval::operator+=(x);
    return *this;
  }

  Interval& Interval::operator-=(const Interval& x)
  {
    ibex::Interval::operator-=(x);
    return *this;
  }

  Interval& Interval::operator*=(const Interval& x)
  {
    ibex::Interval::operator*=(x);
    return *this;
  }

  Interval& Interval::operator/=(const Interval& x)
  {
    ibex::Interval::operator/=(x);
    return *this;
  }

  std::ostream& operator<<(std::ostream& os, const Interval& x)
  {
    os << x; return os;
  }

  Interval operator""_i(long double x)
  {
    return Interval(x);
  }

  Interval sqr(const Interval& x)
  {
    return ibex::sqr(x);
  }

  Interval sqrt(const Interval& x)
  {
    return ibex::sqrt(x);
  }

  Interval pow(const Interval& x, int n)
  {
    return ibex::pow(x,n);
  }

  Interval pow(const Interval& x, double d)
  {
    return ibex::pow(x,d);
  }

  Interval pow(const Interval& x, const Interval& y)
  {
    return ibex::pow(x,y);
  }

  Interval root(const Interval& x, int n)
  {
    return ibex::root(x,n);
  }

  Interval exp(const Interval& x)
  {
    return ibex::exp(x);
  }

  Interval log(const Interval& x)
  {
    return ibex::log(x);
  }

  Interval cos(const Interval& x)
  {
    return ibex::cos(x);
  }

  Interval sin(const Interval& x)
  {
    return ibex::sin(x);
  }

  Interval tan(const Interval& x)
  {
    return ibex::tan(x);
  }

  Interval acos(const Interval& x)
  {
    return ibex::acos(x);
  }

  Interval asin(const Interval& x)
  {
    return ibex::asin(x);
  }

  Interval atan(const Interval& x)
  {
    return ibex::atan(x);
  }

  Interval atan2(const Interval& y, const Interval& x)
  {
    return ibex::atan2(y,x);
  }

  Interval cosh(const Interval& x)
  {
    return ibex::cosh(x);
  }

  Interval sinh(const Interval& x)
  {
    return ibex::sinh(x);
  }

  Interval tanh(const Interval& x)
  {
    return ibex::tanh(x);
  }

  Interval acosh(const Interval& x)
  {
    return ibex::acosh(x);
  }

  Interval asinh(const Interval& x)
  {
    return ibex::asinh(x);
  }

  Interval atanh(const Interval& x)
  {
    return ibex::atanh(x);
  }

  Interval abs(const Interval& x)
  {
    return static_cast<Interval>(ibex::abs(static_cast<ibex::Interval>(x)));
  }

  Interval max(const Interval& x, const Interval& y)
  {
    return ibex::max(x,y);
  }

  Interval min(const Interval& x, const Interval& y)
  {
    return ibex::min(x,y);
  }

  Interval sign(const Interval& x)
  {
    return ibex::sign(x);
  }

  Interval integer(const Interval& x)
  {
    return ibex::integer(x);
  }

  Interval floor(const Interval& x)
  {
    return ibex::floor(x);
  }

  Interval ceil(const Interval& x)
  {
    return ibex::ceil(x);
  }

  bool bwd_add(const Interval& y, Interval& x1, Interval& x2)
  {
    return ibex::bwd_add(y,x1,x2);
  }

  bool bwd_sub(const Interval& y, Interval& x1, Interval& x2)
  {
    return ibex::bwd_sub(y,x1,x2);
  }

  bool bwd_mul(const Interval& y, Interval& x1, Interval& x2)
  {
    return ibex::bwd_mul(y,x1,x2);
  }

  bool bwd_div(const Interval& y, Interval& x1, Interval& x2)
  {
    return ibex::bwd_div(y,x1,x2);
  }

  bool bwd_sqr(const Interval& y, Interval& x)
  {
    return ibex::bwd_sqr(y,x);
  }

  bool bwd_sqrt(const Interval& y, Interval& x)
  {
    return ibex::bwd_sqrt(y,x);
  }

  bool bwd_pow(const Interval& y, int n, Interval& x)
  {
    return ibex::bwd_pow(y,n,x);
  }

  bool bwd_pow(const Interval& y, Interval& x1, Interval& x2)
  {
    return ibex::bwd_pow(y,x1,x2);
  }

  bool bwd_root(const Interval& y, int n, Interval& x)
  {
    return ibex::bwd_root(y,n,x);
  }

  bool bwd_exp(const Interval& y, Interval& x)
  {
    return ibex::bwd_exp(y,x);
  }

  bool bwd_log(const Interval& y, Interval& x)
  {
    return ibex::bwd_log(y,x);
  }

  bool bwd_cos(const Interval& y, Interval& x)
  {
    return ibex::bwd_cos(y,x);
  }

  bool bwd_sin(const Interval& y, Interval& x)
  {
    return ibex::bwd_sin(y,x);
  }

  bool bwd_tan(const Interval& y, Interval& x)
  {
    return ibex::bwd_tan(y,x);
  }

  bool bwd_acos(const Interval& y, Interval& x)
  {
    return ibex::bwd_acos(y,x);
  }

  bool bwd_asin(const Interval& y, Interval& x)
  {
    return ibex::bwd_asin(y,x);
  }

  bool bwd_atan(const Interval& y, Interval& x)
  {
    return ibex::bwd_atan(y,x);
  }

  bool bwd_atan2(const Interval& y, Interval& x1, Interval& x2)
  {
    return ibex::bwd_atan2(y,x1,x2);
  }

  bool bwd_cosh(const Interval& y, Interval& x)
  {
    return ibex::bwd_cosh(y,x);
  }

  bool bwd_sinh(const Interval& y, Interval& x)
  {
    return ibex::bwd_sinh(y,x);
  }

  bool bwd_tanh(const Interval& y, Interval& x)
  {
    return ibex::bwd_tanh(y,x);
  }

  bool bwd_acosh(const Interval& y, Interval& x)
  {
    return ibex::bwd_acosh(y,x);
  }

  bool bwd_asinh(const Interval& y, Interval& x)
  {
    return ibex::bwd_asinh(y,x);
  }

  bool bwd_atanh(const Interval& y, Interval& x)
  {
    return ibex::bwd_atanh(y,x);
  }

  bool bwd_abs(const Interval& y, Interval& x)
  {
    return ibex::bwd_abs(y,x);
  }

  bool bwd_max(const Interval& y, Interval& x1, Interval& x2)
  {
    return ibex::bwd_max(y,x1,x2);
  }

  bool bwd_min(const Interval& y, Interval& x1, Interval& x2)
  {
    return ibex::bwd_min(y,x1,x2);
  }

  bool bwd_sign(const Interval& y, Interval& x)
  {
    return ibex::bwd_sign(y,x);
  }

  bool bwd_chi(const Interval& f, Interval& a, Interval& b, Interval& c)
  {
    return ibex::bwd_chi(f,a,b,c);
  }

  bool bwd_floor(const Interval& y, Interval& x)
  {
    return ibex::bwd_floor(y,x);
  }

  bool bwd_ceil(const Interval& y, Interval& x)
  {
    return ibex::bwd_ceil(y,x);
  }

  bool bwd_saw(const Interval& y, Interval& x)
  {
    return ibex::bwd_saw(y,x);
  }

  bool bwd_imod(Interval& x, Interval& y, double p)
  {
    return ibex::bwd_imod(x,y,p);
  }
} // namespace codac