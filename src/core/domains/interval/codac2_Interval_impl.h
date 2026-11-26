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

#include <cmath> // for trunc

// Inline functions

namespace codac2
{
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
      assert_release_unreachable("'Interval' can only be defined by one or two 'double' values.");
    }

    return *this;
  }

  inline Interval& Interval::operator=(const Interval& x)
  {
    gaol::interval::operator=(x);
    return *this;
  }

  inline BoolInterval Interval::operator>(const Interval& x) const
  {
    return x < *this;
  }

  inline double Interval::lb() const
  {
    return gaol::interval::left();
  }

  inline double Interval::ub() const
  {
    return gaol::interval::right();
  }

  inline double Interval::mag() const
  {
    return gaol::interval::mag();
  }

  inline double Interval::mig() const
  {
    return gaol::interval::mig();
  }

  inline double Interval::smag() const
  {
    return (std::abs(lb()) > std::abs(ub())) ? lb() : ub();
  }

  inline double Interval::smig() const
  {
    return gaol::interval::smig();
  }

  inline double Interval::rand() const
  {
    if(is_empty())
      return std::numeric_limits<double>::quiet_NaN();

    double a = std::max<double>(next_float(-oo),lb());
    double b = std::min<double>(prev_float(oo),ub());
    double r = a + (((double)std::rand())/(double)RAND_MAX)*(b-a);
    // The above operation may result in a floating point outside the bounds,
    // due to floating-point errors. Such possible error is corrected below:
    return std::max<double>(lb(),std::min<double>(r,ub()));
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

  inline bool Interval::has_integer_bounds() const
  {
    return trunc(lb()) == lb() && trunc(ub()) == ub();
  }

  inline bool Interval::overlaps(const Interval& x) const
  {
    return !is_empty() && !x.is_empty() && ub() > x.lb() && x.ub() > lb();
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

  inline Interval operator|(const Interval& x, const Interval& y)
  {
    return gaol::operator|(x,y);
  }

  inline const Interval& operator+(const Interval& x)
  {
    return x;
  }

  inline Interval operator+(const Interval& x, const Interval& y)
  {
    return gaol::operator+(x,y);
  }

  inline Interval operator-(const Interval& x, const Interval& y)
  {
    return gaol::operator-(x, y);
  }

  inline Interval operator*(const Interval& x, const Interval& y)
  {
    return gaol::operator*(x,y);
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

  inline Interval& Interval::operator+=(const Interval& x)
  {
    gaol::interval::operator+=(x);
    return *this;
  }

  inline Interval& Interval::operator-=(const Interval& x)
  {
    gaol::interval::operator-=(x);
    return *this;
  }

  inline Interval& Interval::operator*=(const Interval& x)
  {
    gaol::interval::operator*=(x);
    return *this;
  }

  inline Interval& Interval::operator/=(const Interval& x)
  {
    gaol::interval::operator/=(x);
    return *this;
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
    return Interval(static_cast<double>(x));
  }

  inline double prev_float(double x)
  {
    return gaol::previous_float(x);
  }

  inline double next_float(double x)
  {
    return gaol::next_float(x);
  }

  // ---------------------------------------------------------------------------
  // The functions whose code depends on the GAOL Codac is built on
  // ---------------------------------------------------------------------------
  //
  // CODAC_GAOL_GUARDS (see codac2_Interval.h) chooses between two versions of
  // them: the code Codac had before GAOL v5, with its guards around GAOL 4, and
  // the code that calls GAOL v5 directly, taking from it every function it
  // provides. This block, the one of codac2_Interval_operations_impl.h and the
  // bwd() of the operators listed in codac2_Interval.h are the only places
  // where the two versions differ. The functions of this block used to be defined among the
  // others above; their version with the guards is moved here unchanged, so
  // that going back to it (WITH_IBEX) gives exactly the Codac of before.
#if CODAC_GAOL_GUARDS

  // With the guards, for GAOL 4, the GAOL that IBEX bundles (WITH_IBEX):
  //
  //  - Interval(double), operator=(double), and the arithmetic operators and
  //    compound assignments with a double give the empty set for an infinite
  //    double, of which GAOL 4 makes the interval [+oo,+oo] (or [-oo,-oo]),
  //    which contains no real number;
  //  - mid() and diam() set the rounding upward again after midpoint() and
  //    width() of GAOL 4, which may leave it in another direction, whereas
  //    Codac's own computations on doubles assume it upward (see the
  //    initialization of oo in codac2_math.h); diam() of the empty set is NaN;
  //  - rad() and bisect() compute the radius and the cuts themselves;
  //  - intersects(), is_disjoint(), the subset and superset relations, strict
  //    and interior, compare the bounds themselves: set_le() of GAOL 4, its
  //    interior relation, is false for the empty set in the empty set and for
  //    two intervals with the same infinite bound ([-oo,+oo] in [-oo,+oo]),
  //    where is_interior_subset() is true;
  //  - operator& returns Interval::empty() for an empty operand or when
  //    x.ub() < y.lb(): GAOL 4 gives reversed bounds for disjoint intervals,
  //    [3,2] for [1,2] & [3,4], which is_empty() takes for empty but the
  //    operations computing on the bounds do not ([3,2] + [0,1] gives [3,3]);
  //  - the others (the default constructor, init(), ==, !=, <,
  //    interior_contains(), inflate(), is_bisectable(), operator-() of an
  //    interval, the union with a double and empty()) are Codac's code of
  //    before GAOL v5, which the version without the guards replaces by the
  //    functions of GAOL v5.

  inline Interval::Interval()
    : gaol::interval(-oo,oo)
  { }

  inline Interval::Interval(double a)
    : gaol::interval(a)
  {
    if(a == -oo || a == oo)
      set_empty();
  }

  inline Interval& Interval::init()
  {
    *this = Interval(-oo,oo);
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

  inline bool Interval::operator==(const Interval& x) const
  {
    return (is_empty() && x.is_empty()) || (lb() == x.lb() && ub() == x.ub());
  }

  inline bool Interval::operator!=(const Interval& x) const
  {
    return !(*this == x);
  }

  inline BoolInterval Interval::operator<(const Interval& x) const
  {
    if(is_empty() || x.is_empty())
      return BoolInterval::EMPTY;
    if(this->ub() < x.lb())
      return BoolInterval::TRUE;
    if(x.ub() <= this->lb())
      return BoolInterval::FALSE;
    return BoolInterval::UNKNOWN;
  }

  inline double Interval::mid() const
  {
    double m = gaol::interval::midpoint();
    gaol::round_upward();
    return m;
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

  inline bool Interval::interior_contains(const double& x) const
  {
    return !is_empty() && x > lb() && x < ub();
  }

  inline bool Interval::intersects(const Interval &x) const
  {
    return !is_empty() && !x.is_empty() && lb() <= x.ub() && ub() >= x.lb();
  }

  inline bool Interval::is_disjoint(const Interval& x) const
  {
    return is_empty() || x.is_empty() || lb() > x.ub() || ub() < x.lb();
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

  inline Interval operator&(const Interval& x, const Interval& y)
  {
    if(x.is_empty() || y.is_empty() || x.ub() < y.lb())
      return Interval::empty();
    
    else
      return gaol::operator&(x,y);
  }

  inline Interval Interval::operator-() const
  {
    return 0.-*this;
  }

  inline Interval operator|(const Interval& x, double y)
  {
    return gaol::operator|(x,Interval(y));
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

  inline Interval& Interval::operator+=(double x)
  {
    if(x == -oo || x == oo)
      set_empty();
    else
      gaol::interval::operator+=(x);
    return *this;
  }

  inline Interval& Interval::operator-=(double x)
  {
    if(x == -oo || x == oo)
      set_empty();
    else
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

  inline Interval& Interval::operator/=(double x)
  {
    if(x == -oo || x == oo)
      set_empty();
    else
      gaol::interval::operator/=(x);
    return *this;
  }


  inline Interval Interval::empty()
  {
    return std::numeric_limits<double>::quiet_NaN();
  }

  // The operators with an integer (declared at the end of codac2_Interval.h),
  // which GAOL 4 does not have: n converted to a double, as the implicit
  // conversion did before they were declared, and the operator with a double
  // above.

  template<IsIntervalIntegerType T>
  inline Interval operator+(const Interval& x, T n)
  {
    return x + static_cast<double>(n);
  }

  template<IsIntervalIntegerType T>
  inline Interval operator+(T n, const Interval& y)
  {
    return static_cast<double>(n) + y;
  }

  template<IsIntervalIntegerType T>
  inline Interval operator-(const Interval& x, T n)
  {
    return x - static_cast<double>(n);
  }

  template<IsIntervalIntegerType T>
  inline Interval operator-(T n, const Interval& y)
  {
    return static_cast<double>(n) - y;
  }

  template<IsIntervalIntegerType T>
  inline Interval operator*(const Interval& x, T n)
  {
    return x * static_cast<double>(n);
  }

  template<IsIntervalIntegerType T>
  inline Interval operator*(T n, const Interval& y)
  {
    return static_cast<double>(n) * y;
  }

  template<IsIntervalIntegerType T>
  inline Interval operator/(const Interval& x, T n)
  {
    return x / static_cast<double>(n);
  }

  template<IsIntervalIntegerType T>
  inline Interval operator/(T n, const Interval& y)
  {
    return static_cast<double>(n) / y;
  }

#else

  // Without the guards, for GAOL v5 (https://github.com/Jordan08/GAOL), Codac's
  // default: GAOL is called directly, GAOL v5 handling each of the cases above
  // itself. An infinite double gives the empty set, every operation leaves the
  // rounding upward, width() of the empty set is NaN, the empty intersection is
  // the canonical empty set [NaN,NaN] (in operator& and operator&= alike, for
  // disjoint intervals in either order), and Codac takes rad() and split() from
  // GAOL rather than computing them again.
  //
  // The relations between two intervals are GAOL's too, those of IEEE
  // 1788-2015 (Table 10.3): set_disjoint() (disjoint), set_leq() (subset),
  // set_le() (interior), set_geq() (its converse) and set_eq() (equal), the
  // strict relations adding !set_eq(). They give the results of the comparisons
  // of bounds of the version with the guards, which were compared to them on
  // every pair of 36 intervals (the empty set, points, infinite bounds, -0 and
  // +0).
  //
  // So are, giving the same results as Codac's code of before (compared on 78
  // intervals and 15 doubles, NaN among them): the default constructor and
  // init() (gaol::interval(), [-oo,+oo], and universe()), == and != (set_eq()
  // and set_neq()), < (certainly_le() and certainly_leq()),
  // interior_contains() (set_strictly_contains()), inflate() (GAOL's
  // inflate(), the sum with [-rad,rad]), is_bisectable(), the union with a
  // double (|= of GAOL) and empty() (emptyset()). bisect() is GAOL's as well,
  // whose cut points may differ from Codac's in their last bits (see it), and
  // so is operator-() of an interval, GAOL's -x, whose zero bounds are -0 where
  // Codac's 0.-x gave +0 (see it).
  //
  // Codac keeps its own functions where GAOL v5 has none: smag(), rand(),
  // has_integer_bounds(), overlaps() (no relation of GAOL tells that the
  // intersection of two intervals is wider than a point; the state "overlaps"
  // of overlap(), al < bl < au < bu, is another relation), complementary()
  // and diff().

  inline Interval::Interval()
    : gaol::interval()
  { }

  inline Interval::Interval(double a)
    : gaol::interval(a)
  { }

  inline Interval& Interval::init()
  {
    gaol::interval::operator=(gaol::interval::universe());
    return *this;
  }

  inline Interval& Interval::operator=(double x)
  {
    gaol::interval::operator=(x);
    return *this;
  }

  inline bool Interval::operator==(const Interval& x) const
  {
    return gaol::interval::set_eq(x);
  }

  inline bool Interval::operator!=(const Interval& x) const
  {
    return gaol::interval::set_neq(x);
  }

  inline BoolInterval Interval::operator<(const Interval& x) const
  {
    // certainly_le() and certainly_leq() of GAOL, strictPrecedes and precedes
    // of IEEE 1788, are true when an operand is empty: the empty set is told
    // first
    if(is_empty() || x.is_empty())
      return BoolInterval::EMPTY;
    if(gaol::interval::certainly_le(x)) // ub() < x.lb()
      return BoolInterval::TRUE;
    if(static_cast<const gaol::interval&>(x).certainly_leq(*this)) // x.ub() <= lb()
      return BoolInterval::FALSE;
    return BoolInterval::UNKNOWN;
  }

  inline double Interval::mid() const
  {
    return gaol::interval::midpoint();
  }

  inline double Interval::rad() const
  {
    // GAOL's rad(), rad of IEEE 1788-2015 (12.12.8), in the fork of GAOL since its
    // version 4.3.1: the smallest double r such that this interval is included in
    // [m-r,m+r], m being mid(), computed as the greater of m-lb() and ub()-m rounded
    // upward; NaN for the empty set, +oo for an unbounded interval. Codac used to
    // compute the same bound itself, with two interval subtractions,
    // (mid()-*this).ub() and (*this-mid()).ub(), and the same special cases.
    return gaol::interval::rad();
  }

  inline double Interval::diam() const
  {
    return gaol::interval::width();
  }

  inline bool Interval::interior_contains(const double& x) const
  {
    return gaol::interval::set_strictly_contains(x);
  }

  inline bool Interval::intersects(const Interval &x) const
  {
    return !gaol::interval::set_disjoint(x);
  }

  inline bool Interval::is_disjoint(const Interval& x) const
  {
    return gaol::interval::set_disjoint(x);
  }

  inline bool Interval::is_subset(const Interval& x) const
  {
    return gaol::interval::set_leq(x);
  }

  inline bool Interval::is_strict_subset(const Interval& x) const
  {
    return gaol::interval::set_leq(x) && !gaol::interval::set_eq(x);
  }

  inline bool Interval::is_interior_subset(const Interval& x) const
  {
    return gaol::interval::set_le(x);
  }

  inline bool Interval::is_strict_interior_subset(const Interval& x) const
  {
    return gaol::interval::set_le(x) && !gaol::interval::set_eq(x);
  }

  inline bool Interval::is_superset(const Interval& x) const
  {
    return gaol::interval::set_geq(x);
  }

  inline bool Interval::is_strict_superset(const Interval& x) const
  {
    return gaol::interval::set_geq(x) && !gaol::interval::set_eq(x);
  }

  inline Interval& Interval::inflate(const double& rad)
  {
    gaol::interval::operator=(gaol::interval::inflate(rad));
    return *this;
  }

  inline bool Interval::is_bisectable() const
  {
    return gaol::interval::is_bisectable();
  }

  inline std::pair<Interval,Interval> Interval::bisect(float ratio) const
  {
    assert_release(is_bisectable());
    assert_release(Interval(0,1).interior_contains(ratio));

    // GAOL's bisect() (GAOL v5): the two parts of [l,r] on either side of the
    // cut point c, which they share, so that they cover this interval exactly.
    // In halves, and an unbounded interval whatever the ratio, c is midpoint()
    // (mid()), where Codac used to cut them itself: 0 for [-oo,+oo], -MAX for
    // [-oo,b] and MAX for [a,+oo], MAX being the largest double. For another ratio, c is l(1-ratio) + r ratio, in GAOL's
    // rounding, where Codac computed lb()+ratio*diam(): it does not overflow,
    // as diam() of [-MAX,MAX] did (+oo), which made Codac cut that interval at
    // -MAX, and when c falls on a bound of a bisectable interval, it is moved
    // to the double next to that bound, on the side of the other one, where
    // Codac always took the double next to lb(). Elsewhere, the cut points
    // differ in their last bits only, in about half of the cases (the parts
    // still covering the interval exactly). Codac's assertions are kept: for an
    // interval that is not bisectable, GAOL would return two empty sets or the
    // point twice, and it throws std::domain_error for a ratio outside (0,1).
    const std::pair<gaol::interval,gaol::interval> parts = gaol::interval::bisect(ratio);
    return { parts.first, parts.second };
  }

  inline Interval operator&(const Interval& x, const Interval& y)
  {
    // GAOL is called without a guard. Codac used to return Interval::empty() itself
    // when an operand was empty or when x.ub() < y.lb(), because GAOL did not give
    // the canonical empty set for disjoint intervals: it kept the largest lower bound
    // and the smallest upper bound, [3, 2] for [1, 2] & [3, 4]. is_empty() took such
    // an interval for empty, but the operations computing on its bounds did not:
    // [3, 2] + [0, 1] gave [3, 3]. That guard also missed disjoint intervals in the
    // other order (y.ub() < x.lb()), and did not protect operator&= at all. Since its
    // version 4.3.2, the fork of GAOL returns the empty set [NaN, NaN] for every empty
    // intersection, empty operands included, in operator& and operator&= alike.
    return gaol::operator&(x,y);
  }

  inline Interval Interval::operator-() const
  {
    // GAOL's -x (GAOL v5): the opposites of the bounds, exactly, where Codac
    // computed 0.-x. Both give the same set, but a zero bound changes its sign
    // with GAOL: -[0,1] is [-1,-0], written [-1, -0], where 0.-[0,1] gives
    // [-1,0] (0-0 is +0). -0 and +0 are the same number for the operations and
    // the relations of GAOL and Codac (-0 == 0, Interval(-1,-0) ==
    // Interval(-1,0)); only the writing and std::signbit() tell them apart.
    return gaol::interval::operator-();
  }

  inline Interval operator|(const Interval& x, double y)
  {
    gaol::interval z(x);
    z |= y;
    return z;
  }

  inline Interval operator+(const Interval& x, double y)
  {
    return gaol::operator+(x,y);
  }

  inline Interval operator+(double x, const Interval& y)
  {
    return gaol::operator+(x,y);
  }

  inline Interval operator-(const Interval& x, double y)
  {
    return gaol::operator-(x, y);
  }

  inline Interval operator-(double x, const Interval& y)
  {
    return gaol::operator-(x, y);
  }

  inline Interval operator*(const Interval& x, double y)
  {
    return gaol::operator*(x,y);
  }

  inline Interval operator*(double x, const Interval& y)
  {
    return gaol::operator*(x,y);
  }

  inline Interval operator/(const Interval& x, double y)
  {
    return gaol::operator/(x,y);
  }

  inline Interval operator/(double x, const Interval& y)
  {
    return gaol::operator/(x,y);
  }

  inline Interval& Interval::operator+=(double x)
  {
    gaol::interval::operator+=(x);
    return *this;
  }

  inline Interval& Interval::operator-=(double x)
  {
    gaol::interval::operator-=(x);
    return *this;
  }

  inline Interval& Interval::operator*=(double x)
  {
    gaol::interval::operator*=(x);
    return *this;
  }

  inline Interval& Interval::operator/=(double x)
  {
    gaol::interval::operator/=(x);
    return *this;
  }


  inline Interval Interval::empty()
  {
    return gaol::interval::emptyset();
  }

  // The operators with an integer (declared at the end of codac2_Interval.h):
  // GAOL v5's, which compute with the double n where n is one for certain, and
  // with the tightest interval containing n otherwise.

  template<IsIntervalIntegerType T>
  inline Interval operator+(const Interval& x, T n)
  {
    return gaol::operator+(static_cast<const gaol::interval&>(x), n);
  }

  template<IsIntervalIntegerType T>
  inline Interval operator+(T n, const Interval& y)
  {
    return gaol::operator+(n, static_cast<const gaol::interval&>(y));
  }

  template<IsIntervalIntegerType T>
  inline Interval operator-(const Interval& x, T n)
  {
    return gaol::operator-(static_cast<const gaol::interval&>(x), n);
  }

  template<IsIntervalIntegerType T>
  inline Interval operator-(T n, const Interval& y)
  {
    return gaol::operator-(n, static_cast<const gaol::interval&>(y));
  }

  template<IsIntervalIntegerType T>
  inline Interval operator*(const Interval& x, T n)
  {
    return gaol::operator*(static_cast<const gaol::interval&>(x), n);
  }

  template<IsIntervalIntegerType T>
  inline Interval operator*(T n, const Interval& y)
  {
    return gaol::operator*(n, static_cast<const gaol::interval&>(y));
  }

  template<IsIntervalIntegerType T>
  inline Interval operator/(const Interval& x, T n)
  {
    return gaol::operator/(static_cast<const gaol::interval&>(x), n);
  }

  template<IsIntervalIntegerType T>
  inline Interval operator/(T n, const Interval& y)
  {
    return gaol::operator/(n, static_cast<const gaol::interval&>(y));
  }

#endif
}
