/** 
 *  \file codac2_Interval.h
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

#pragma once

#include <list>
#include <array>
#include <ibex_Interval.h>
#include "codac2_Domain.h"

namespace codac2
{
  const double oo = POS_INFINITY;

  /**
   * \class Interval
   * \brief Interval class, for representing closed and connected subsets of \f$\mathbb{R}\f$.
   *
   * The class also encapsulates the interval representation provided by the Gaol library.
   * 
   * \note This class currently wraps the Interval class of the IBEX library.
   *       See more: https://ibex-lib.readthedocs.io
   */
  class Interval : protected ibex::Interval, public DomainInterface<Interval,double>
  {
    public:

      using DegeneratedType = double;

      /**
       * \brief Creates an interval \f$[-\infty,\infty]\f$
       */
      Interval();

      /**
       * \brief Creates a degenerate interval \f$[a,a]\f$, \f$a\in\mathbb{R}\f$
       * 
       * \param a single value contained in the resulting interval
       */
      Interval(double a);

      /**
       * \brief Creates an interval \f$[a,b]\f$, \f$a\in\mathbb{R}, b\in\mathbb{R}\f$
       * 
       * \param a lower bound of the interval
       * \param b upper bound of the interval
       */
      Interval(double a, double b);

      /**
       * \brief Creates an interval from another one
       * 
       * \param x the interval to be copied
       */
      Interval(const Interval& x);

      /**
       * \brief Create an interval \f$[a,a]\f$ from a fixed-sized array of size 1
       * 
       * \param array the array of doubles
       */
      Interval(std::array<double,1> array);

      /**
       * \brief Create an interval \f$[a,b]\f$ from a fixed-sized array of size 2
       * 
       * \param array the array of doubles
       */
      Interval(std::array<double,2> array);


      Interval(std::initializer_list<double> l);

      /**
       * \brief Sets the value of this interval to x
       * 
       * \note This function is used for template purposes.
       * 
       * \param x the value for re-initialization
       * \return a reference to this
       */
      Interval& init(const Interval& x);

      /**
       * \brief Sets this to x
       * 
       * \param x interval to be copied
       * \return a reference to this
       */
      Interval& operator=(const Interval& x);

      /**
       * \brief Comparison (equality) between two intervals
       * 
       * \param x interval to be compared with
       * \return true iff this and x are exactly the same intervals
       */
      bool operator==(const Interval& x) const;

      /**
       * \brief Comparison (non equality) between two intervals
       * 
       * \param x interval to be compared with
       * \return true iff this and x are not exactly the same intervals
       */
      bool operator!=(const Interval& x) const;

      /**
       * \brief Returns the lower bound of this
       * 
       * \return lower bound
       */
      double lb() const;

      /**
       * \brief Returns the upper bound of this
       * 
       * \return upper bound
       */
      double ub() const;
      
      /**
       * \brief Returns the midpoint of this
       * 
       * \note The return point is guaranteed to be included
       *       in this but not necessarily to be the closest 
       *       floating point from the real midpoint.
       *       In particular, cases are:
       *       - [empty]    -> Quiet NaN
       *       - [-oo,+oo]  -> mid = 0.0
       *       - [-oo,b]    -> mid = -MAXREAL
       *       - [a,+oo]    -> mid = MAXREAL
       *       - [a,b]      -> mid ~ a + .5*(b-a)
       *
       * \return midpoint
       */
      double mid() const;

      /**
       * \brief Returns the radius of this
       * 
       * \return the radius, or 0 if this is empty
       */
      double rad() const;

      /**
       * \brief Returns the diameter of this
       * 
       * \return the diameter, or 0 if this is empty
       */
      double diam() const;

      /**
       * \brief Returns the diameter of this
       * 
       * \note This function is used for template purposes. See diam()
       * 
       * \return the diameter, or 0 if this is empty
       */
      double volume() const;

      /**
       * \brief Returns the dimension of this (which is always )
       * 
       * \note This function is used for template purposes.
       * 
       * \return 1
       */
      size_t size() const;
    
      /**
       * \brief Sets this interval to the empty set
       */
      void set_empty();

      /**
       * \brief Tests if this is empty
       * 
       * \return true in case of empty set
       */
      bool is_empty() const;

      /**
       * \brief Tests if this contains x
       * 
       * \note x can also be an "open bound", i.e., infinity.
       * So this function is not restricted to a set-membership
       * interpretation.
       * 
       * \param x real value
       * \return true if this contains x
       */
      bool contains(const double& x) const;

      /**
       * \brief Tests if the interior of this contains x
       * 
       * \param x real value
       * \return true if the interior of this contains x
       */
      bool interior_contains(const double& x) const;

      /**
       * \brief Tests if one of the bounds of this is infinite
       *
       * \note An empty interval is always bounded
       * 
       * \return true if of the bounds of this is infinite
       */
      bool is_unbounded() const;

      /**
       * \brief Tests if this is degenerated, that is, in the
       * form of \f$[a,a]\f$
       *
       * \note An empty interval is considered here as degenerated
       * 
       * \return true if this is degenerated
       */
      bool is_degenerated() const;

      /**
       * \brief Tests if this and x intersect
       * 
       * \param x the other interval
       * \return true if the intervals intersect
       */
      bool intersects(const Interval &x) const;

      /**
       * \brief Tests if this and x do not intersect
       * 
       * \param x the other interval
       * \return true if the intervals are disjoint
       */
      bool is_disjoint(const Interval& x) const;

      /**
       * \brief Tests if this and x intersect and their intersection
       * has a non-null volume
       * 
       * \param x the other interval
       * \return true if some interior points (of this or x) belong to their intersection
       */
      bool overlaps(const Interval &x) const;

      /**
       * \brief Tests if this is a subset of x
       *
       * \note If this is empty, always returns true
       * 
       * \param x the other interval
       * \return true iff this interval is a subset of x
       */
      bool is_subset(const Interval& x) const;

      /**
       * \brief Tests if this is a subset of x and not x itself
       *
       * \note In particular, \f$[-\infty,\infty]\f$ is not a strict
       * subset of \f$[-\infty,\infty]\f$ and \f$\varnothing\f$ is not 
       * a strict subset of \f$\varnothing\f$, although in both cases,
       * the first is inside the interior of the second.
       * 
       * \param x the other interval
       * \return true iff this interval is a strict subset of x
       */
      bool is_strict_subset(const Interval& x) const;

      /**
       * \brief Tests if this is in the interior of x
       * 
       * \note In particular, \f$[-\infty,\infty]\f$ is in the interior
       * of \f$[-\infty,\infty]\f$ and \f$\varnothing\f$ is in the 
       * interior of \f$\varnothing\f$.
       * 
       * \param x the other interval
       * \return true iff this interval is in the interior of x
       */
      bool is_interior_subset(const Interval& x) const;

      /**
       * \brief Tests if this is in the interior of x and different from x
       * 
       * \note In particular, \f$[-\infty,\infty]\f$ is not strictly in the 
       * interior of \f$[-\infty,\infty]\f$ and \f$\varnothing\f$ is not strictly
       * in the interior of \f$\varnothing\f$.
       * 
       * \param x the other interval
       * \return true iff this interval is a strict interior subset of x
       */
      bool is_strict_interior_subset(const Interval& x) const;

      /**
       * \brief Tests if this is a superset of x
       *
       * \note If this is empty, always returns true
       * 
       * \param x the other interval
       * \return true iff this interval is a superset of x
       */
      bool is_superset(const Interval& x) const;

      /**
       * \brief Tests if this is a superset of x and different from x
       *
       * \see is_strict_subset()
       * 
       * \param x the other interval
       * \return true iff this interval is a strict superset of x
       */
      bool is_strict_superset(const Interval& x) const;

      /**
       * \brief Adds [-rad,+rad] to this
       * 
       * \param rad the radius of the inflation
       * \return a reference to this
       */
      Interval& inflate(const double& rad);

      /**
       * \brief Tests if this can be bisected into two non-degenerated intervals
       * 
       * \note Examples of non bisectable intervals are [0,next_float(0)] or [DBL_MAX,+oo).
       * 
       * \return true iff this can be bisected
       */
      bool is_bisectable() const;

      /**
       * \brief Bisects this into two subintervals
       * 
       * \pre is_bisectable() must be true
       * \pre 0 < ratio < 1
       * \param ratio optional proportion of split (0.5 corresponds to middle)
       * \return a pair of resulting subintervals
       */
      std::pair<Interval,Interval> bisect(float ratio = 0.49) const;

      /**
       * \brief Computes the complementary of this
       * 
       * \param compactness optional boolean to obtain or not disjoint intervals
       * \return a vector of complementary intervals
       */
      std::vector<Interval> complementary(bool compactness = true) const;

      /**
       * \brief Computes the result of \f$[x]\[y]\f$
       * 
       * \param y interval to remove from this
       * \param compactness optional boolean to obtain or not disjoint intervals
       * \return a vector of resulting diff intervals
       */
      std::vector<Interval> diff(const Interval& y, bool compactness = true) const;

      /**
       * \brief Self union of this and x
       * 
       * \param x the other interval
       * \return a reference to this
       */
      Interval& operator|=(const Interval& x);

      /**
       * \brief Self intersection of this and x
       * 
       * \param x the other interval
       * \return a reference to this
       */
      Interval& operator&=(const Interval& x);

      /**
       * \brief Self addition of this and a real x
       * 
       * \param x the real to add
       * \return a reference to this
       */
      Interval& operator+=(double x);

      /**
       * \brief Self addition of this and x
       * 
       * \param x the other interval
       * \return a reference to this
       */
      Interval& operator+=(const Interval& x);

      /**
       * \brief Substraction of this
       * 
       * \return (- this)
       */
      Interval operator-() const;

      /**
       * \brief Self substraction of this and a real x
       * 
       * \param x the real to substract
       * \return a reference to this
       */
      Interval& operator-=(double x);

      /**
       * \brief Self substraction of this and x
       * 
       * \param x the other interval
       * \return a reference to this
       */
      Interval& operator-=(const Interval& x);

      /**
       * \brief Self multiplication of this and a real x
       * 
       * \param x the real to multiply
       * \return a reference to this
       */
      Interval& operator*=(double x);

      /**
       * \brief Self multiplication of this and x
       * 
       * \param x the other interval
       * \return a reference to this
       */
      Interval& operator*=(const Interval& x);

      /**
       * \brief Self division of this and a real x
       * 
       * \param x the real to divide
       * \return a reference to this
       */
      Interval& operator/=(double x);

      /**
       * \brief Self division of this and x
       * 
       * \note This computation is more efficient than
       * the non-self div operator, because it calculates
       * the union of this/x as intermediate result.
       * 
       * \param x the other interval
       * \return a reference to this
       */
      Interval& operator/=(const Interval& x);

      /**
       * \brief Provides an empty interval
       * 
       * \return an empty set
       */
      static Interval empty();

      /**
       * \brief Provides an interval for \f$[0]\f$
       * 
       * \return an interval containing \f$0\f$
       */
      static Interval zero();
      static Interval zeros();

      /**
       * \brief Provides an interval for \f$[1]\f$
       * 
       * \return an interval containing \f$1\f$
       */
      static Interval one();
      static Interval ones();

      /**
       * \brief Provides an interval for \f$[\frac{\pi}{2}]\f$
       * 
       * \return an interval containing \f$\frac{\pi}{2}\f$
       */
      static Interval half_pi();

      /**
       * \brief Provides an interval for \f$[\pi]\f$
       * 
       * \return an interval containing \f$\pi\f$
       */
      static Interval pi();

      /**
       * \brief Provides an interval for \f$[2\pi]\f$
       * 
       * \return an interval containing \f$2\pi\f$
       */
      static Interval two_pi();
      
      friend std::ostream& operator<<(std::ostream& os, const Interval& x);

    protected:
      
      Interval(const ibex::Interval& x);

      friend ibex::Interval& to_ibex(codac2::Interval& x);
      friend const ibex::Interval& to_ibex(const Interval& x);
      friend Interval to_codac(const ibex::Interval& x);

      #define _dec_friend_interval_arithm_op(f) \
        friend Interval f(double, const Interval&); \
        friend Interval f(const Interval&, double); \
        friend Interval f(const Interval&, const Interval&); \

      _dec_friend_interval_arithm_op(operator&);
      _dec_friend_interval_arithm_op(operator|);
      _dec_friend_interval_arithm_op(operator+);
      _dec_friend_interval_arithm_op(operator-);
      _dec_friend_interval_arithm_op(operator*);
      _dec_friend_interval_arithm_op(operator/);

      #define _dec_friend_interval_unary_op(f) \
        friend Interval f(const Interval&); \

      _dec_friend_interval_unary_op(sqr);
      _dec_friend_interval_unary_op(sqrt);
      _dec_friend_interval_unary_op(exp);
      _dec_friend_interval_unary_op(log);
      _dec_friend_interval_unary_op(cos);
      _dec_friend_interval_unary_op(sin);
      _dec_friend_interval_unary_op(tan);
      _dec_friend_interval_unary_op(acos);
      _dec_friend_interval_unary_op(asin);
      _dec_friend_interval_unary_op(atan);
      _dec_friend_interval_unary_op(cosh);
      _dec_friend_interval_unary_op(sinh);
      _dec_friend_interval_unary_op(tanh);
      _dec_friend_interval_unary_op(acosh);
      _dec_friend_interval_unary_op(asinh);
      _dec_friend_interval_unary_op(atanh);
      _dec_friend_interval_unary_op(abs);
      _dec_friend_interval_unary_op(sign);
      _dec_friend_interval_unary_op(integer);
      _dec_friend_interval_unary_op(floor);
      _dec_friend_interval_unary_op(ceil);

      #define _dec_friend_interval_binary_op(f) \
        friend Interval f(const Interval&, const Interval&); \

      _dec_friend_interval_binary_op(max);
      _dec_friend_interval_binary_op(min);
      _dec_friend_interval_binary_op(atan2);

      friend Interval pow(const Interval&, int);
      friend Interval pow(const Interval&, double);

      _dec_friend_interval_binary_op(pow);

      friend Interval root(const Interval&, int);

      #define _dec_friend_interval_unary_bwd(f) \
        friend void f(const Interval&, Interval&); \

      _dec_friend_interval_unary_bwd(bwd_sqr);
      _dec_friend_interval_unary_bwd(bwd_sqrt);
      _dec_friend_interval_unary_bwd(bwd_exp);
      _dec_friend_interval_unary_bwd(bwd_log);
      _dec_friend_interval_unary_bwd(bwd_cos);
      _dec_friend_interval_unary_bwd(bwd_sin);
      _dec_friend_interval_unary_bwd(bwd_tan);
      _dec_friend_interval_unary_bwd(bwd_acos);
      _dec_friend_interval_unary_bwd(bwd_asin);
      _dec_friend_interval_unary_bwd(bwd_atan);
      _dec_friend_interval_unary_bwd(bwd_cosh);
      _dec_friend_interval_unary_bwd(bwd_sinh);
      _dec_friend_interval_unary_bwd(bwd_tanh);
      _dec_friend_interval_unary_bwd(bwd_acosh);
      _dec_friend_interval_unary_bwd(bwd_asinh);
      _dec_friend_interval_unary_bwd(bwd_atanh);
      _dec_friend_interval_unary_bwd(bwd_abs);
      _dec_friend_interval_unary_bwd(bwd_sign);
      _dec_friend_interval_unary_bwd(bwd_floor);
      _dec_friend_interval_unary_bwd(bwd_ceil);
      _dec_friend_interval_unary_bwd(bwd_saw);

      #define _dec_friend_interval_binary_bwd(f) \
        friend void f(const Interval&, Interval&, Interval&); \
      
      _dec_friend_interval_binary_bwd(bwd_add);
      _dec_friend_interval_binary_bwd(bwd_sub);
      _dec_friend_interval_binary_bwd(bwd_mul);
      _dec_friend_interval_binary_bwd(bwd_div);
      _dec_friend_interval_binary_bwd(bwd_pow);
      _dec_friend_interval_binary_bwd(bwd_min);
      _dec_friend_interval_binary_bwd(bwd_max);
      _dec_friend_interval_binary_bwd(bwd_atan2);

      friend void bwd_pow(const Interval&, Interval&, int);
      friend void bwd_root(const Interval&, Interval&, int);
      friend void bwd_imod(Interval&, Interval&, double);
  };
      
  /**
   * \brief Streams out this
   * 
   * \param os the stream to be updated
   * \param x the interval to stream out
   * \return a reference to the updated stream
   */
  std::ostream& operator<<(std::ostream& os, const Interval& x);

  /**
   * \brief Codac defined literals allowing to produce an interval from a double
   * 
   * \param x value to convert into an interval object
   * \return the interval \f$[x]\f$
   */
  Interval operator""_i(long double x);

  inline double previous_float(double x)
  {
    return ibex::previous_float(x);
  }

  inline double next_float(double x)
  {
    return ibex::next_float(x);
  }
}