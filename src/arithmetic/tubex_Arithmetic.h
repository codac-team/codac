/* ============================================================================
 *  tubex-lib - Subtube arithmetic
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef TUBEX_ARITHMETIC
#define TUBEX_ARITHMETIC

#include "tubex_Subtube.h"
#include "ibex_Interval.h"

namespace tubex
{
  Subtube operator+(const Subtube& x);
  Subtube operator+(const Subtube& x1, const Subtube& x2);
  Subtube operator+(const Subtube& x1, double x2);
  Subtube operator+(double x1, const Subtube& x2);
  Subtube operator+(const Subtube& x1, const ibex::Interval& x2);
  Subtube operator+(const ibex::Interval& x1, const Subtube& x2);

  Subtube operator-(const Subtube& x);
  Subtube operator-(const Subtube& x1, const Subtube& x2);
  Subtube operator-(const Subtube& x1, double x2);
  Subtube operator-(double x1, const Subtube& x2);
  Subtube operator-(const Subtube& x1,  const ibex::Interval& x2);
  Subtube operator-(const ibex::Interval& x1, const Subtube& x2);

  Subtube operator*(const Subtube& x1, const Subtube& x2);
  Subtube operator*(const Subtube& x1, double x2);
  Subtube operator*(double x1, const Subtube& x2);
  Subtube operator*(const ibex::Interval& x1, const Subtube& x2);
  Subtube operator*(const Subtube& x1, const ibex::Interval& x2);

  Subtube operator/(const Subtube& x1, const Subtube& x2);
  Subtube operator/(const Subtube& x1, double x2);
  Subtube operator/(double x1, const Subtube& x2);
  Subtube operator/(const ibex::Interval& x1, const Subtube& x2);
  Subtube operator/(const Subtube& x1, const ibex::Interval& x2);

  Subtube operator|(const Subtube& x1, const Subtube& x2);
  Subtube operator|(const Subtube& x1, double x2);
  Subtube operator|(double x1, const Subtube& x2);
  Subtube operator|(const ibex::Interval& x1, const Subtube& x2);
  Subtube operator|(const Subtube& x1, const ibex::Interval& x2);

  Subtube operator&(const Subtube& x1, const Subtube& x2);
  Subtube operator&(const Subtube& x1, double x2);
  Subtube operator&(double x1, const Subtube& x2);
  Subtube operator&(const ibex::Interval& x1, const Subtube& x2);
  Subtube operator&(const Subtube& x1, const ibex::Interval& x2);

  Subtube abs(const Subtube& x);
  Subtube sqr(const Subtube& x);
  Subtube sqrt(const Subtube& x);
  Subtube pow(const Subtube& x, int p);
  Subtube pow(const Subtube& x, double p);
  Subtube pow(const Subtube &x, const ibex::Interval& p);
  Subtube root(const Subtube& x, int p);
  Subtube exp(const Subtube& x);
  Subtube log(const Subtube& x);
  Subtube cos(const Subtube& x);
  Subtube sin(const Subtube& x);
  Subtube tan(const Subtube& x);
  Subtube acos(const Subtube& x);
  Subtube asin(const Subtube& x);
  Subtube atan(const Subtube& x);
  Subtube cosh(const Subtube& x);
  Subtube sinh(const Subtube& x);
  Subtube tanh(const Subtube& x);
  Subtube acosh(const Subtube& x);
  Subtube asinh(const Subtube& x);
  Subtube atanh(const Subtube& x);
  Subtube atan2(const Subtube& y, const Subtube& x);
}

#endif