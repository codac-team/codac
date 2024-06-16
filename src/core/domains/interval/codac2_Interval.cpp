/** 
 *  codac2_Interval.cpp
 *  
 *  This class reuses several functions developed for ibex::Interval. 
 *  The original IBEX code is encapsulated in Codac for allowing inheritance 
 *  to Codac classes and also for documentation, binding, and independency purposes.
 *  See ibex::Interval (IBEX lib, main author: Gilles Chabert)
 *    https://ibex-lib.readthedocs.io
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Gilles Chabert, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <cassert>
#include "codac2_Interval.h"

using namespace std;

namespace codac2
{
  Interval::Interval()
    : ibex::Interval()
  { }

  Interval::Interval(double a)
    : ibex::Interval(a)
  { }

  Interval::Interval(double a, double b)
    : ibex::Interval(a,b)
  { }
  
  Interval::Interval(const Interval& x)
    : ibex::Interval(x)
  { }

  Interval::Interval(array<double,1> array)
    : ibex::Interval(array)
  { }

  Interval::Interval(array<double,2> array)
    : ibex::Interval(array)
  { }

  Interval::Interval(std::initializer_list<double> l)
    : ibex::Interval()
  {
    assert(((int)l.size() == 2)||((int)l.size() == 1));
    (int)l.size() == 2? *this = ibex::Interval(*(l.begin()+0),*(l.begin()+1)): *this = ibex::Interval(*l.begin());
  }

  Interval& Interval::init(const Interval& x)
  {
    *this = x;
    return *this;
  }

  Interval& Interval::operator=(const Interval& x)
  {
    ibex::Interval::operator=(x);
    return *this;
  }

  bool Interval::operator==(const Interval& x) const
  {
    return ibex::Interval::operator==(x);
  }
  
  bool Interval::operator!=(const Interval& x) const
  {
    return ibex::Interval::operator!=(x);
  }

  double Interval::lb() const
  {
    return ibex::Interval::lb();
  }

  double Interval::ub() const
  {
    return ibex::Interval::ub();
  }

  double Interval::mid() const
  {
    return ibex::Interval::mid();
  }

  double Interval::rad() const
  {
    return ibex::Interval::rad();
  }

  double Interval::diam() const
  {
    return ibex::Interval::diam();
  }

  double Interval::volume() const
  {
    return ibex::Interval::diam();
  }

  size_t Interval::size() const
  {
    return 1;
  }

  void Interval::set_empty()
  {
    ibex::Interval::set_empty();
  }
  
  bool Interval::is_empty() const
  {
    return ibex::Interval::is_empty();
  }

  bool Interval::contains(const double& x) const
  {
    return ibex::Interval::contains(x);
  }

  bool Interval::interior_contains(const double& x) const
  {
    return ibex::Interval::interior_contains(x);
  }

  bool Interval::is_unbounded() const
  {
    return ibex::Interval::is_unbounded();
  }
  
  bool Interval::is_degenerated() const
  {
    return ibex::Interval::is_degenerated();
  }

  bool Interval::intersects(const Interval &x) const
  {
    return ibex::Interval::intersects(x);
  }
  
  bool Interval::is_disjoint(const Interval& x) const
  {
    return ibex::Interval::is_disjoint(x);
  }
  
  bool Interval::overlaps(const Interval& x) const
  {
    return ibex::Interval::overlaps(x);
  }
  
  bool Interval::is_subset(const Interval& x) const
  {
    return ibex::Interval::is_subset(x);
  }
  
  bool Interval::is_strict_subset(const Interval& x) const
  {
    return ibex::Interval::is_strict_subset(x);
  }
  
  bool Interval::is_interior_subset(const Interval& x) const
  {
    return ibex::Interval::is_interior_subset(x);
  }
  
  bool Interval::is_strict_interior_subset(const Interval& x) const
  {
    return ibex::Interval::is_strict_interior_subset(x);
  }
  
  bool Interval::is_superset(const Interval& x) const
  {
    return ibex::Interval::is_superset(x);
  }
  
  bool Interval::is_strict_superset(const Interval& x) const
  {
    return ibex::Interval::is_strict_superset(x);
  }

  Interval& Interval::inflate(const double& rad)
  {
    ibex::Interval::inflate(rad);
    return *this;
  }

  bool Interval::is_bisectable() const
  {
    return ibex::Interval::is_bisectable();
  }

  pair<Interval,Interval> Interval::bisect(float ratio) const
  {
    auto p = ibex::Interval::bisect(ratio);
    return { p.first, p.second };
  }

  vector<Interval> Interval::complementary(bool compactness) const
  {
    if(is_empty() || (compactness && is_degenerated()))
      return { {-oo,oo} };

    vector<Interval> l;

    if(lb() > -oo)
      l.push_back({-oo,lb()});

    if(ub() < oo)
      l.push_back({ub(),oo});

    return l;
  }

  vector<Interval> Interval::diff(const Interval& y, bool compactness) const
  {
    if(compactness && is_degenerated())
    {
      if(is_empty() || y.contains(lb()))
        return {};
      else
        return { *this };
    }

    vector<Interval> l;
    for(const auto& li : y.complementary(compactness))
    {
      Interval inter = li & *this;
      if(!inter.is_degenerated())
        l.push_back(inter);
    }

    return l;
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

  Interval& Interval::operator+=(double x)
  {
    ibex::Interval::operator+=(x);
    return *this;
  }

  Interval& Interval::operator+=(const Interval& x)
  {
    ibex::Interval::operator+=(x);
    return *this;
  }

  Interval Interval::operator-() const
  {
    return 0.-*this;
  }

  Interval& Interval::operator-=(double x)
  {
    ibex::Interval::operator-=(x);
    return *this;
  }

  Interval& Interval::operator-=(const Interval& x)
  {
    ibex::Interval::operator-=(x);
    return *this;
  }

  Interval& Interval::operator*=(double x)
  {
    ibex::Interval::operator*=(x);
    return *this;
  }

  Interval& Interval::operator*=(const Interval& x)
  {
    ibex::Interval::operator*=(x);
    return *this;
  }

  Interval& Interval::operator/=(double x)
  {
    ibex::Interval::operator/=(x);
    return *this;
  }

  Interval& Interval::operator/=(const Interval& x)
  {
    ibex::Interval::operator/=(x);
    return *this;
  }
  
  Interval Interval::empty()
  {
    return ibex::Interval::empty_set();
  }
  
  Interval Interval::zero()
  {
    return ibex::Interval::zero();
  }
  
  Interval Interval::zeros()
  {
    return ibex::Interval::zero();
  }
  
  Interval Interval::one()
  {
    return ibex::Interval::one();
  }
  
  Interval Interval::ones()
  {
    return ibex::Interval::one();
  }

  Interval Interval::half_pi()
  {
    return ibex::Interval::half_pi();
  }

  Interval Interval::pi()
  {
    return ibex::Interval::pi();
  }

  Interval Interval::two_pi()
  {
    return ibex::Interval::two_pi();
  }

  ostream& operator<<(ostream& os, const Interval& x)
  {
    ibex::operator<<(os,x);
    return os;
  }

  Interval::Interval(const ibex::Interval& x)
    : ibex::Interval(x)
  { }

  Interval operator""_i(long double x)
  {
    return Interval(x);
  }

} // namespace codac