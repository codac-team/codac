/** 
 *  \file codac2_Interval_operations.h
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

#include "codac2_Interval.h"

namespace codac2
{
  /**
   * \brief Returns the intersection of two intervals: \f$[x]\cap[y]\f$
   * 
   * \note Returns an empty interval if there is no intersection.
   * 
   * \param x interval value
   * \param y interval value
   * \return intersection result
   */
  Interval operator&(const Interval& x, const Interval& y);

  /**
   * \brief Returns the squared-union of two intervals: \f$[x]\sqcup[y]\f$
   * 
   * \note The squared-union is defined as: \f$[x]\sqcup[y]=\left[[x]\cup[y]\right]\f$
   * 
   * \param x interval value
   * \param y interval value
   * \return squared-union result
   */
  Interval operator|(const Interval& x, const Interval& y);

  /**
   * \brief Returns this
   * 
   * \note This operator is only provided for consistency purposes.
   * 
   * \param x interval value
   * \return the same interval
   */
  const Interval& operator+(const Interval& x);

  /**
   * \brief Returns \f$[x]+y\f$ with \f$y\in\mathbb{R}\f$
   * 
   * \param x interval value
   * \param y real value
   * \return the addition result
   */
  Interval operator+(const Interval& x, double y);

  /**
   * \brief Returns \f$x+[y]\f$ with \f$x\in\mathbb{R}\f$
   * 
   * \param x real value
   * \param y interval value
   * \return the addition result
   */
  Interval operator+(double x, const Interval& y);

  /**
   * \brief Returns \f$[x]+[y]\f$
   * 
   * \param x interval value
   * \param y interval value
   * \return the addition result
   */
  Interval operator+(const Interval& x, const Interval& y);

  /**
   * \brief Returns \f$[x]-y\f$ with \f$y\in\mathbb{R}\f$
   * 
   * \param x interval value
   * \param y real value
   * \return the substraction result
   */
  Interval operator-(const Interval& x, double y);

  /**
   * \brief Returns \f$x-[y]\f$ with \f$x\in\mathbb{R}\f$
   * 
   * \param x real value
   * \param y interval value
   * \return the substraction result
   */
  Interval operator-(double x, const Interval& y);

  /**
   * \brief Returns \f$[x]-[y]\f$
   * 
   * \param x interval value
   * \param y interval value
   * \return the substraction result
   */
  Interval operator-(const Interval& x, const Interval& y);

  /**
   * \brief Returns \f$[x]*y\f$ with \f$y\in\mathbb{R}\f$
   * 
   * \param x interval value
   * \param y real value
   * \return the multiplication result
   */
  Interval operator*(const Interval& x, double y);

  /**
   * \brief Returns \f$x*[y]\f$ with \f$x\in\mathbb{R}\f$
   * 
   * \param x real value
   * \param y interval value
   * \return the multiplication result
   */
  Interval operator*(double x, const Interval& y);

  /**
   * \brief Returns \f$[x]*[y]\f$
   * 
   * \param x interval value
   * \param y interval value
   * \return the multiplication result
   */
  Interval operator*(const Interval& x, const Interval& y);

  /**
   * \brief Returns \f$[x]/y\f$ with \f$y\in\mathbb{R}\f$
   * 
   * \param x interval value
   * \param y real value
   * \return the division result
   */
  Interval operator/(const Interval& x, double y);

  /**
   * \brief Returns \f$x/[y]\f$ with \f$x\in\mathbb{R}\f$
   * 
   * \param x real value
   * \param y interval value
   * \return the division result
   */
  Interval operator/(double x, const Interval& y);

  /**
   * \brief Returns \f$[x]/[y]\f$
   * 
   * \param x interval value
   * \param y interval value
   * \return the division result
   */
  Interval operator/(const Interval& x, const Interval& y);

  /**
   * \brief Returns \f$[x]^2\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval sqr(const Interval& x);

  /**
   * \brief Returns \f$\sqrt{[x]}\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval sqrt(const Interval& x);

  /**
   * \brief Returns \f$[x]^n\f$, \f$n\in\mathbb{Z}\f$
   * 
   * \param x interval value
   * \param n integer power value
   * \return the operation result
   */
  Interval pow(const Interval& x, int n);

  /**
   * \brief Returns \f$[x]^d\f$, \f$d\in\mathbb{R}\f$
   * 
   * \param x interval value
   * \param d real power value
   * \return the operation result
   */
  Interval pow(const Interval& x, double d);

  /**
   * \brief Returns \f$[x]^{[y]}\f$, \f$y\in\mathbb{IR}\f$
   * 
   * \param x interval value
   * \param y interval power value
   * \return the operation result
   */
  Interval pow(const Interval& x, const Interval& y);

  /**
   * \brief Returns the n-th root: \f$\sqrt[n]{[x]}\f$
   * 
   * \param x interval value
   * \param n integer root
   * \return the operation result
   */
  Interval root(const Interval& x, int n);

  /**
   * \brief Returns \f$\exp([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval exp(const Interval& x);

  /**
   * \brief Returns \f$\log([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval log(const Interval& x);

  /**
   * \brief Returns \f$\cos([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval cos(const Interval& x);

  /**
   * \brief Returns \f$\sin([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval sin(const Interval& x);

  /**
   * \brief Returns \f$\tan([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval tan(const Interval& x);

  /**
   * \brief Returns \f$\acos([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval acos(const Interval& x);

  /**
   * \brief Returns \f$\asin([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval asin(const Interval& x);

  /**
   * \brief Returns \f$\atan([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval atan(const Interval& x);

  /**
   * \brief Returns \f$\mathrm{arctan2}([y],[x])\f$
   * 
   * \param y interval value
   * \param x interval value
   * \return the operation result
   */
  Interval atan2(const Interval& y, const Interval& x);

  /**
   * \brief Returns \f$\cosh([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval cosh(const Interval& x);

  /**
   * \brief Returns \f$\sinh([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval sinh(const Interval& x);

  /**
   * \brief Returns \f$\tanh([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval tanh(const Interval& x);

  /**
   * \brief Returns \f$\acosh([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval acosh(const Interval& x);

  /**
   * \brief Returns \f$\asinh([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval asinh(const Interval& x);

  /**
   * \brief Returns \f$\atanh([x])\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval atanh(const Interval& x);

  /**
   * \brief Returns \f$\mid[x]\mid = \left\{\mid x \mid, x\in[x]\right\}\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval abs(const Interval& x);

  /**
   * \brief Returns \f$\min([x],[y])=\left\{\min(x,y), x\in[x], y\in[y]\right\}\f$
   * 
   * \param x interval value
   * \param y interval value
   * \return the operation result
   */
  Interval min(const Interval& x, const Interval& y);

  /**
   * \brief Returns \f$\max([x],[y])=\left\{\max(x,y), x\in[x], y\in[y]\right\}\f$
   * 
   * \param x interval value
   * \param y interval value
   * \return the operation result
   */
  Interval max(const Interval& x, const Interval& y);

  /**
   * \brief Returns \f$\sign([x])=\left[\left\{\sign(x), x\in[x]\right\}\right]\f$
   * 
   * \note By convention, \f$ 0\in[x] \Longrightarrow \sign([x])=[-1,1]\f$.
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval sign(const Interval& x);

  /**
   * \brief Returns the largest integer interval included in \f$[x]\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval integer(const Interval& x);

  /**
   * \brief Returns floor of \f$[x]\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval floor(const Interval& x);

  /**
   * \brief Returns ceil of \f$[x]\f$
   * 
   * \param x interval value
   * \return the operation result
   */
  Interval ceil(const Interval& x);

  /**
   * \brief Computes the backward (reverse) addition
   * 
   * \note Contracts \f$[x_1]\f$, \f$[x_2]\f$ as: 
   * \f$([x_1],[x_2])\cap\{(x_1,x_2)\in[x_1]\times[x_2]\ \mid\ \exists y\in[y],\ y=x_1+x_2\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x1 prior value for \f$[x_1]\f$, may be contracted
   * \param x2 prior value for \f$[x_2]\f$, may be contracted
   */
  void bwd_add(const Interval& y, Interval& x1, Interval& x2);

  /**
   * \brief Computes the backward (reverse) substraction
   * 
   * \note Contracts \f$[x_1]\f$, \f$[x_2]\f$ as: 
   * \f$([x_1],[x_2])\cap\{(x_1,x_2)\in[x_1]\times[x_2]\ \mid\ \exists y\in[y],\ y=x_1-x_2\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x1 prior value for \f$[x_1]\f$, may be contracted
   * \param x2 prior value for \f$[x_2]\f$, may be contracted
   */
  void bwd_sub(const Interval& y, Interval& x1, Interval& x2);

  /**
   * \brief Computes the backward (reverse) multiplication
   * 
   * \note Contracts \f$[x_1]\f$, \f$[x_2]\f$ as: 
   * \f$([x_1],[x_2])\cap\{(x_1,x_2)\in[x_1]\times[x_2]\ \mid\ \exists y\in[y],\ y=x_1*x_2\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x1 prior value for \f$[x_1]\f$, may be contracted
   * \param x2 prior value for \f$[x_2]\f$, may be contracted
   */
  void bwd_mul(const Interval& y, Interval& x1, Interval& x2);

  /**
   * \brief Computes the backward (reverse) division
   * 
   * \note Contracts \f$[x_1]\f$, \f$[x_2]\f$ as: 
   * \f$([x_1],[x_2])\cap\{(x_1,x_2)\in[x_1]\times[x_2]\ \mid\ \exists y\in[y],\ y=x_1/x_2\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x1 prior value for \f$[x_1]\f$, may be contracted
   * \param x2 prior value for \f$[x_2]\f$, may be contracted
   */
  void bwd_div(const Interval& y, Interval& x1, Interval& x2);

  /**
   * \brief Computes the backward (reverse) squared operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=x^2\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_sqr(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) squared-root operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\sqrt{x}\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_sqrt(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) power operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=x^p\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   * \param p power integer value
   */
  void bwd_pow(const Interval& y, Interval& x, int p);

  /**
   * \brief Computes the backward (reverse) power operation
   * 
   * \note Contracts \f$[x]\f$, \f$[p]\f$ as: 
   * \f$([x],[p])\cap\{(x,p)\in[x]\times[p]\ \mid\ \exists y\in[y],\ y=x^{p}\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   * \param p prior value for \f$[p]\f$, may be contracted
   */
  void bwd_pow(const Interval& y, Interval& x, Interval& p);

  /**
   * \brief Computes the backward (reverse) root operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\sqrt[p]{x}\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   * \param p root integer value
   */
  void bwd_root(const Interval& y, Interval& x, int p);

  /**
   * \brief Computes the backward (reverse) exponential operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\exp(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_exp(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) logarithmic operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\log(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_log(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) cosine operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\cos(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_cos(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) sine operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\sin(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_sin(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) tangent operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\tan(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_tan(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) arccos operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\acos(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_acos(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) arcsin operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\asin(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_asin(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) arctan operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\atan(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_atan(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) arctan2 operation
   * 
   * \note Contracts \f$[x_1]\f$, \f$[x_2]\f$ as: 
   * \f$([x_1],[x_2])\cap\{(x_1,x_2)\in[x_1]\times[x_2]\ \mid\ \exists y\in[y],\ y=\atan2(x_1,x_2)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x1 prior value for \f$[x_1]\f$, may be contracted
   * \param x2 prior value for \f$[x_2]\f$, may be contracted
   */
  void bwd_atan2(const Interval& y, Interval& x1, Interval& x2);

  /**
   * \brief Computes the backward (reverse) hyperbolic cosine operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\cosh(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_cosh(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) hyperbolic sine operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\sinh(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_sinh(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) hyperbolic tangent operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\tanh(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_tanh(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) hyperbolic arccos operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\acosh(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_acosh(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) hyperbolic arcsin operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\asinh(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_asinh(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) hyperbolic arctan operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\atanh(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_atanh(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) absolute-value operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\mid x\mid\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_abs(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) of the max operation
   * 
   * \note Contracts \f$[x_1]\f$, \f$[x_2]\f$ as: 
   * \f$([x_1],[x_2])\cap\{(x_1,x_2)\in[x_1]\times[x_2]\ \mid\ \exists y\in[y],\ y=\max(x_1,x_2)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x1 prior value for \f$[x_1]\f$, may be contracted
   * \param x2 prior value for \f$[x_2]\f$, may be contracted
   */
  void bwd_min(const Interval& y, Interval& x1, Interval& x2);

  /**
   * \brief Computes the backward (reverse) of the min operation
   * 
   * \note Contracts \f$[x_1]\f$, \f$[x_2]\f$ as: 
   * \f$([x_1],[x_2])\cap\{(x_1,x_2)\in[x_1]\times[x_2]\ \mid\ \exists y\in[y],\ y=\min(x_1,x_2)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x1 prior value for \f$[x_1]\f$, may be contracted
   * \param x2 prior value for \f$[x_2]\f$, may be contracted
   */
  void bwd_max(const Interval& y, Interval& x1, Interval& x2);

  /**
   * \brief Computes the backward (reverse) sign operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\sign(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_sign(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) floor operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\floor(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_floor(const Interval& y, Interval& x);

  /**
   * \brief Computes the backward (reverse) ceil operation
   * 
   * \note Contracts \f$[x]\f$ as: 
   * \f$[x]\cap\{x\in[x]\ \mid\ \exists y\in[y],\ y=\ceil(x)\}\f$.
   * 
   * \param y interval value (result of the forward operation)
   * \param x prior value for \f$[x]\f$, may be contracted
   */
  void bwd_ceil(const Interval& y, Interval& x);

  /**
   * \brief Contract \f$[x_1]\f$ and \f$[x_2]\f$ w.r.t. the fact that they are equivalent modulo the period \f$p\f$
   * 
   * \param x1 prior value for \f$[x_1]\f$, may be contracted
   * \param x2 prior value for \f$[x_2]\f$, may be contracted
   * \param p period value
   */
  void bwd_imod(Interval& x1, Interval& x2, double p);

}