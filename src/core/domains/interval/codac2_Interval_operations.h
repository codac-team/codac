/** 
 *  \file codac2_Interval_operations.h
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

#include "codac2_Interval.h"

namespace codac2
{
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
   * \brief Returns \f$[x]^p\f$, \f$p\in\mathbb{R}\f$
   * 
   * \param x interval value
   * \param p real power value
   * \return the operation result
   */
  Interval pow(const Interval& x, double p);

  /**
   * \brief Returns \f$[x]^{[p]}\f$, \f$p\in\mathbb{IR}\f$
   * 
   * \param x interval value
   * \param p interval power value
   * \return the operation result
   */
  Interval pow(const Interval& x, const Interval& p);

  /**
   * \brief Returns the p-th root: \f$\sqrt[p]{[x]}\f$
   * 
   * \param x interval value
   * \param p integer root
   * \return the operation result
   */
  Interval root(const Interval& x, int p);

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
}

#include "codac2_Interval_operations_impl.h"