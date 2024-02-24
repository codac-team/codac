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

#include <array>
#include <codac2_Domain.h>
#include <ibex_Interval.h>

namespace codac2
{
  const double oo = POS_INFINITY;

  class Interval : public ibex::Interval, public Domain
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
  };

  Interval operator""_i(long double x);

  /** \brief [x]^2 */
  Interval sqr(const Interval& x);

  /** \brief sqrt{[x]} */
  Interval sqrt(const Interval& x);

  /** \brief [x]^n. */
  Interval pow(const Interval& x, int n);

  /** \brief [x]^d. */
  Interval pow(const Interval& x, double d);

  /** \brief [x]^[y]. */
  Interval pow(const Interval& x, const Interval& y);

  /** \brief n^{th} root of [x]. */
  Interval root(const Interval& x, int n);

  /** \brief exp([x]). */
  Interval exp(const Interval& x);

  /** \brief log([x]). */
  Interval log(const Interval& x);

  /** \brief cos([x]). */
  Interval cos(const Interval& x);

  /** \brief sin([x]). */
  Interval sin(const Interval& x);

  /** \brief tan([x]). */
  Interval tan(const Interval& x);

  /** \brief acos([x]). */
  Interval acos(const Interval& x);

  /** \brief asin([x]). */
  Interval asin(const Interval& x);

  /** \brief atan([x]). */
  Interval atan(const Interval& x);

  /** \brief atan2([y],[x]). */
  Interval atan2(const Interval& y, const Interval& x);

  /** \brief cosh([x]). */
  Interval cosh(const Interval& x);

  /** \brief sinh([x]). */
  Interval sinh(const Interval& x);

  /** \brief tanh([x]). */
  Interval tanh(const Interval& x);

  /** \brief acosh([x]). */
  Interval acosh(const Interval& x);

  /** \brief asinh([x]). */
  Interval asinh(const Interval& x);

  /** \brief atanh([x]). */
  Interval atanh(const Interval& x);

  /** \brief \f$abs([x]) = \{|x|, x\in[x]\}.\f$. */
  Interval abs(const Interval& x);

  /** \brief Maximum of two intervals.
   *
   * Return \f$\max([x],[y]) = \{ \max\{x,y\}, x\in[x], y\in[y] \}\f$. */
  Interval max(const Interval& x, const Interval& y);

  /** \brief Minimum of two intervals.
   *
   *  Return \f$\min([x],[y]) = \{ \min\{x,y\}, x\in[x], y\in[y] \}\f$. */
  Interval min(const Interval& x, const Interval& y);

  /** \brief Sign of [x].
   *
   *  Return \f$sign([x]) = hull \{ sign{x}, x\in[x] \}\f$.
   * \remark By convention, \f$ 0\in[x] \Longrightarrow sign[x]=[-1,1]\f$. */
  Interval sign(const Interval& x);

  /** \brief Chi of [a], [b] and [c].
   *
   *  Return \f$chi([a],[b],[c]) = 0.5*(1-\sign([a]))*[b] + 0.5*(\sign([a])+1)*[c]. \f$
   * \remark  chi([a],[b],[c]) =[b] if [a]<=0, [c] if [a]>0, hull \{[b], [c]\} else.  */
  Interval chi(const Interval& a, const Interval& b, const Interval& c);

  /** \brief Return the largest integer interval included in x. */
  Interval integer(const Interval& x);

  /** \brief Floor of [x]. */
  Interval floor(const Interval& x);

  /** \brief Ceil of [x]. */
  Interval ceil(const Interval& x);

  /** \brief Projection of y=x_1+x_2.
   *
   * Set ([x]_1,[x]_2) to \f$([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1+x_2\}\f$. */
  bool bwd_add(const Interval& y, Interval& x1, Interval& x2);

  /** \brief Projection of y=x_1-x_2.
   *
   * Set ([x]_1,[x]_2) to \f$([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1-x_2\}\f$. */
  bool bwd_sub(const Interval& y, Interval& x1, Interval& x2);

  /** \brief Projection of y=x_1*x_2.
   *
   * Set ([x]_1,[x]_2) to \f$([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1\times x_2\}\f$. */
  bool bwd_mul(const Interval& y, Interval& x1, Interval& x2);

  /** \brief Projection of y=x_1/x_2.
   *
   * Set ([x]_1,[x]_2) to \f$([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1/x_2\}\f$. */
  bool bwd_div(const Interval& y, Interval& x1, Interval& x2);

  /** \brief Projection of y=x^2.
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=x^2 \}\f$. */
  bool bwd_sqr(const Interval& y, Interval& x);

  /** \brief Projection of y=sqrt{x}.
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=sqrt{x} \}\f$. */
  bool bwd_sqrt(const Interval& y, Interval& x);

  /** \brief Projection of y=x^n.
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=x^n \}\f$. */
  bool bwd_pow(const Interval& y, int n, Interval& x);

  /** \brief Projection of y=x_1^{x_2}.
   *
   * Set ([x]_1,[x]_2) to \f$([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1^{x_2}\}\f$. */
  bool bwd_pow(const Interval& y, Interval& x1, Interval& x2);

  /** \brief Projection of the \f$y=x^{\frac{1}{n}}\f$.
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=x^{\frac{1}{n}} \}\f$. */
  bool bwd_root(const Interval& y, int n, Interval& x);

  /** \brief Projection of y=exp(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=\exp(x) \}\f$. */
  bool bwd_exp(const Interval& y, Interval& x);

  /** \brief Projection of y=log(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=\log(x) \}\f$. */
  bool bwd_log(const Interval& y, Interval& x);

  /** \brief Projection of y=cos(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=\cos(x) \}\f$. */
  bool bwd_cos(const Interval& y, Interval& x);

  /** \brief Projection of y=sin(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=\sin(x) \}\f$. */
  bool bwd_sin(const Interval& y, Interval& x);

  /** \brief Projection of y=tan(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=\tan(x) \}\f$. */
  bool bwd_tan(const Interval& y, Interval& x);

  /** \brief Projection of y=acos(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=\arccos(x) \}\f$. */
  bool bwd_acos(const Interval& y, Interval& x);

  /** \brief Projection of y=asin(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=\arcsin(x) \}\f$. */
  bool bwd_asin(const Interval& y, Interval& x);

  /** \brief Projection of y=atan(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=\arctan(x) \}\f$. */
  bool bwd_atan(const Interval& y, Interval& x);

  /** \brief Projection of y=atan2(x_1,x_2).
   *
   * Set ([x]_1,[x]_2) to \f$([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=atan2(x_1,x_2)\f$. */
  bool bwd_atan2(const Interval& y, Interval& x1, Interval& x2);

  /** \brief Projection of y=cosh(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=\cosh(x) \}\f$. */
  bool bwd_cosh(const Interval& y, Interval& x);

  /** \brief Projection of y=sinh(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=\sinh(x) \}\f$. */
  bool bwd_sinh(const Interval& y, Interval& x);

  /** \brief Projection of y=tanh(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=\tanh(x) \}\f$. */
  bool bwd_tanh(const Interval& y, Interval& x);

  /** \brief Projection of y=acosh(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=arccosh(x) \}\f$. */
  bool bwd_acosh(const Interval& y, Interval& x);

  /** \brief Projection of y=asinh(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=arcsinh(x) \}\f$. */
  bool bwd_asinh(const Interval& y, Interval& x);

  /** \brief Projection of y=atanh(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=arctanh(x) \}\f$. */
  bool bwd_atanh(const Interval& y, Interval& x);

  /** \brief Projection of y=|x|.
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=|x| \}\f$. */
  bool bwd_abs(const Interval& y, Interval& x);

  /** \brief Projection of y=max(x_1,x_2).
   *
   * Set ([x]_1,[x]_2) to \f$([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=\max(x_1,x_2)\}\f$. */
  bool bwd_max(const Interval& y, Interval& x1, Interval& x2);

  /** \brief Projection of y=min(x_1,x_2).
   *
   * Set ([x]_1,[x]_2) to \f$([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=\min(x_1,x_2)\}\f$. */
  bool bwd_min(const Interval& y, Interval& x1, Interval& x2);

  /** \brief Projection of y=sign(x).
   *
   * Set [x] to \f$[x]\cap \{ x\in [x] \exists y\in [y], \quad y=sign(x) \}\f$. */
  bool bwd_sign(const Interval& y, Interval& x);

  /** \brief Projection of f=chi(a,b,c). */
  bool bwd_chi(const Interval& f, Interval& a, Interval& b, Interval& c);

  /** \brief Contract x w.r.t. y=floor(x). */
  bool bwd_floor(const Interval& y, Interval& x);

  /* \brief Contract x w.r.t. y=ceil(x). */
  bool bwd_ceil(const Interval& y, Interval& x);

  /* \brief Contract x w.r.t. y=saw(x). */
  bool bwd_saw(const Interval& y, Interval& x);

  /**
   * \brief Contract x and y w.r.t. the fact that they are equivalent modulo the period p.
   */
  bool bwd_imod(Interval& x, Interval& y, double p);

} // namespace codac

#endif