/** 
 *  \file codac2_Interval_impl.h
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
  inline Interval::Interval()
    : gaol::interval(-oo,oo)
  { }

  inline Interval::Interval(double a)
    : gaol::interval(a)
  {
    if(a == -oo || a == oo)
      set_empty();
  }

  inline Interval::Interval(double a, double b)
    : gaol::interval(a,b)
  { }

  inline Interval::Interval(const Interval& x)
    : gaol::interval(x)
  { }

  inline Interval::Interval(const std::array<double,1>& array)
    : Interval(array[0])
  { }

  inline Interval::Interval(const std::array<double,2>& array)
    : Interval(array[0],array[1])
  { }

  inline Interval::Interval(std::initializer_list<double> l)
    : Interval()
  {
    init_from_list(l);
  }

  inline Interval& Interval::init()
  {
    *this = Interval(-oo,oo);
    return *this;
  }

  inline Interval& Interval::init(const Interval& x)
  {
    *this = x;
    return *this;
  }

  inline Interval& Interval::init_from_list(const std::list<double>& l)
  {
    if(l.size() == 1)
      *this = Interval(*l.begin());

    else if(l.size() == 2)
      *this = Interval(*l.begin(),*std::prev(l.end()));

    else
    {
      assert_release("'Interval' can only be defined by one or two 'double' values.");
    }

    return *this;
  }

  inline Interval& Interval::operator=(double x)
  {
    if(x == -oo || x == oo)
      set_empty();
    else
      gaol::interval::operator=(x);

    return *this;
  }

  inline Interval& Interval::operator=(const Interval& x)
  {
    gaol::interval::operator=(x);
    return *this;
  }

  inline bool Interval::operator==(const Interval& x) const
  {
    return (is_empty() && x.is_empty()) || (lb() == x.lb() && ub() == x.ub());
  }
  
  inline bool Interval::operator!=(const Interval& x) const
  {
    return !(*this == x);
  }

  inline double Interval::lb() const
  {
    return gaol::interval::left();
  }

  inline double Interval::ub() const
  {
    return gaol::interval::right();
  }

  inline double Interval::mid() const
  {
    double m = gaol::interval::midpoint();
    gaol::round_upward();
    return m;
  }

  inline double Interval::mag() const
  {
    return gaol::interval::mag();
  }

  inline double Interval::mig() const
  {
    return gaol::interval::mig();
  }

  inline double Interval::rand() const
  {
    if(is_empty())
      return std::numeric_limits<double>::quiet_NaN();

    double a = std::max<double>(next_float(-oo),lb());
    double b = std::min<double>(previous_float(oo),ub());
    double r = a + (((double)std::rand())/(double)RAND_MAX)*(b-a);
    // The above operation may result in a floating point outside the bounds,
    // due to floating-point errors. Such possible error is corrected below:
    return std::max<double>(lb(),std::min<double>(r,ub()));
  }

  inline double Interval::rad() const
  {
    if(is_empty())
      return std::numeric_limits<double>::quiet_NaN();

    else if(is_unbounded())
      return oo;

    else
    {
      double t = mid();
      double t1 = (t-*this).ub();
      double t2 = (*this-t).ub();
      return (t1>t2) ? t1 : t2;
    }
  }

  inline double Interval::diam() const
  {
    if(is_empty())
      return std::numeric_limits<double>::quiet_NaN();

    else
    {
      double d = gaol::interval::width();
      gaol::round_upward();
      return d;
    }
  }

  inline double Interval::volume() const
  {
    return diam();
  }

  inline Index Interval::size() const
  {
    return 1;
  }

  inline void Interval::set_empty()
  {
    *this = Interval::empty();
  }
  
  inline bool Interval::is_empty() const
  {
    return gaol::interval::is_empty();
  }

  inline bool Interval::contains(const double& x) const
  {
    return gaol::interval::set_contains(x);
  }

  inline bool Interval::interior_contains(const double& x) const
  {
    return !is_empty() && x > lb() && x < ub();
  }

  inline bool Interval::is_unbounded() const
  {
    return !gaol::interval::is_finite();
  }
  
  inline bool Interval::is_degenerated() const
  {
    return is_empty() || gaol::interval::is_a_double();
  }

  inline bool Interval::is_integer() const
  {
    return gaol::interval::is_an_int();
  }

  inline bool Interval::intersects(const Interval &x) const
  {
    return !is_empty() && !x.is_empty() && lb() <= x.ub() && ub() >= x.lb();
  }
  
  inline bool Interval::is_disjoint(const Interval& x) const
  {
    return is_empty() || x.is_empty() || lb() > x.ub() || ub() < x.lb();
  }
  
  inline bool Interval::overlaps(const Interval& x) const
  {
    return !is_empty() && !x.is_empty() && ub() > x.lb() && x.ub() > lb();
  }
  
  inline bool Interval::is_subset(const Interval& x) const
  {
    return is_empty() || (!x.is_empty() && x.lb() <= lb() && x.ub() >= ub());
  }
  
  inline bool Interval::is_strict_subset(const Interval& x) const
  {
    return !x.is_empty() && (is_empty() || (x.lb() < lb() && x.ub() >= ub()) || (x.ub() > ub() && x.lb() <= lb()));
  }
  
  inline bool Interval::is_interior_subset(const Interval& x) const
  {
    return is_empty() || (!x.is_empty() && (x.lb() == -oo || x.lb() < lb()) && (x.ub() == oo || x.ub() > ub()));
  }
  
  inline bool Interval::is_strict_interior_subset(const Interval& x) const
  {
    return !x.is_empty() && (is_empty() || (
         (x.lb() < lb() && (x.ub() == oo  || x.ub() > ub()))
      || (x.ub() > ub() && (x.lb() == -oo || x.lb() < lb()))
    ));
  }

  inline bool Interval::is_superset(const Interval& x) const
  {
    return x.is_subset(*this);
  }
  
  inline bool Interval::is_strict_superset(const Interval& x) const
  {
    return x.is_strict_subset(*this);
  }
  
  inline Interval& Interval::inflate(const double& rad)
  {
    (*this) += Interval(-rad,rad);
    return *this;
  }

  inline bool Interval::is_bisectable() const
  {
    if(is_empty())
      return false;
    double m = mid();
    return lb() < m && m < ub();
  }

  inline std::pair<Interval,Interval> Interval::bisect(float ratio) const
  {
    assert_release(is_bisectable());
    assert_release(Interval(0,1).interior_contains(ratio));

    if(lb() == -oo)
    {
      if(ub() == oo)
        return { Interval(-oo,0), Interval(0,oo) };
      else
        return { Interval(-oo,-std::numeric_limits<double>::max()), Interval(-std::numeric_limits<double>::max(),ub()) };
    }

    else if(ub() == oo)
      return { Interval(lb(),std::numeric_limits<double>::max()), Interval(std::numeric_limits<double>::max(),oo) };

    else
    {
      double m;

      if(ratio == 0.5)
        m = mid();

      else
      {
        m = lb() + ratio*diam();
        if(m >= ub())
          m = next_float(lb());

        assert(m < ub());
      }

      return { Interval(lb(),m), Interval(m,ub()) };
    }
  }

  inline std::vector<Interval> Interval::complementary(bool compactness) const
  {
    if(is_empty() || (compactness && is_degenerated()))
      return { {-oo,oo} };

    std::vector<Interval> l;

    if(lb() > -oo)
      l.push_back({-oo,lb()});

    if(ub() < oo)
      l.push_back({ub(),oo});

    return l;
  }

  inline std::vector<Interval> Interval::diff(const Interval& y, bool compactness) const
  {
    if(compactness && is_degenerated())
    {
      if(is_empty() || y.contains(lb()))
        return {};
      else
        return { *this };
    }

    std::vector<Interval> l;
    for(const auto& li : y.complementary(compactness))
    {
      Interval inter = li & *this;
      if(!inter.is_degenerated())
        l.push_back(inter);
    }

    return l;
  }

  inline Interval operator&(const Interval& x, const Interval& y)
  {
    if(x.is_empty() || y.is_empty() || x.ub() < y.lb())
      return Interval::empty();
    
    else
      return gaol::operator&(x,y);
  }

  inline Interval operator|(const Interval& x, double y)
  {
    return gaol::operator|(x,Interval(y));
  }

  inline Interval operator|(const Interval& x, const Interval& y)
  {
    return gaol::operator|(x,y);
  }

  inline const Interval& operator+(const Interval& x)
  {
    return x;
  }

  inline Interval operator+(const Interval& x, double y)
  {
    if(y == -oo || y == oo)
      return Interval::empty();

    else
      return gaol::operator+(x,y);
  }

  inline Interval operator+(double x, const Interval& y)
  {
    if(x == -oo || x == oo)
      return Interval::empty();

    else
      return gaol::operator+(x,y);
  }

  inline Interval operator+(const Interval& x, const Interval& y)
  {
    return gaol::operator+(x,y);
  }

  inline Interval operator-(const Interval& x, double y)
  {
    if(y == -oo || y == oo)
      return Interval::empty();

    else
      return gaol::operator-(x, y);
  }

  inline Interval operator-(double x, const Interval& y)
  {
    if(x == -oo || x == oo)
      return Interval::empty();

    else
      return gaol::operator-(x, y);
  }

  inline Interval operator-(const Interval& x, const Interval& y)
  {
    return gaol::operator-(x, y);
  }

  inline Interval operator*(const Interval& x, double y)
  {
    if(y == -oo || y == oo)
      return Interval::empty();

    else
      return gaol::operator*(x,y);
  }

  inline Interval operator*(double x, const Interval& y)
  {
    if(x == -oo || x == oo)
      return Interval::empty();

    else
      return gaol::operator*(x,y);
  }

  inline Interval operator*(const Interval& x, const Interval& y)
  {
    return gaol::operator*(x,y);
  }

  inline Interval operator/(const Interval& x, double y)
  {
    if(y == -oo || y == oo)
      return Interval::empty();

    else
      return gaol::operator/(x,y);
  }

  inline Interval operator/(double x, const Interval& y)
  {
    if(x == -oo || x == oo)
      return Interval::empty();

    else
      return gaol::operator/(x,y);
  }

  inline Interval operator/(const Interval& x, const Interval& y)
  {
    return gaol::operator/(x,y);
  }

  inline Interval& Interval::operator|=(const Interval& x)
  {
    gaol::interval::operator|=(x);
    return *this;
  }

  inline Interval& Interval::operator&=(const Interval& x)
  {
    gaol::interval::operator&=(x);
    return *this;
  }

  inline Interval& Interval::operator+=(double x)
  {
    if(x == -oo || x == oo)
      set_empty();
    else
      gaol::interval::operator+=(x);
    return *this;
  }

  inline Interval& Interval::operator+=(const Interval& x)
  {
    gaol::interval::operator+=(x);
    return *this;
  }

  inline Interval Interval::operator-() const
  {
    return 0.-*this;
  }

  inline Interval& Interval::operator-=(double x)
  {
    if(x == -oo || x == oo)
      set_empty();
    else
      gaol::interval::operator-=(x);
    return *this;
  }

  inline Interval& Interval::operator-=(const Interval& x)
  {
    gaol::interval::operator-=(x);
    return *this;
  }

  inline Interval& Interval::operator*=(double x)
  {
    if(x == -oo || x == oo)
      set_empty();
    else
      gaol::interval::operator*=(x);
    return *this;
  }

  inline Interval& Interval::operator*=(const Interval& x)
  {
    gaol::interval::operator*=(x);
    return *this;
  }

  inline Interval& Interval::operator/=(double x)
  {
    if(x == -oo || x == oo)
      set_empty();
    else
      gaol::interval::operator/=(x);
    return *this;
  }

  inline Interval& Interval::operator/=(const Interval& x)
  {
    gaol::interval::operator/=(x);
    return *this;
  }

  inline Interval Interval::empty()
  {
    return std::numeric_limits<double>::quiet_NaN();
  }
  
  inline Interval Interval::zero()
  {
    return gaol::interval::zero();
  }
  
  inline Interval Interval::one()
  {
    return gaol::interval::one();
  }

  inline Interval Interval::half_pi()
  {
    return gaol::interval::half_pi();
  }

  inline Interval Interval::pi()
  {
    return gaol::interval::pi();
  }

  inline Interval Interval::two_pi()
  {
    return gaol::interval::two_pi();
  }

  inline std::ostream& operator<<(std::ostream& os, const Interval& x)
  {
    gaol::interval::precision(os.precision());
    gaol::operator<<(os,x);
    return os;
  }

  inline Interval::Interval(const gaol::interval& x)
    : gaol::interval(x)
  { }

  inline Interval operator""_i(long double x)
  {
    return Interval(x);
  }

  inline double previous_float(double x)
  {
    return gaol::previous_float(x);
  }

  inline double next_float(double x)
  {
    return gaol::next_float(x);
  }
}
