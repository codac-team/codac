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

#include "tubex_Arithmetic.h"
#include "../tube/exceptions/tubex_DomainException.h"
#include "../tube/exceptions/tubex_StructureException.h"
#include <iostream>
#include <iomanip> // for setprecision()
#ifdef _OPENMP
  #include <omp.h> // for multithreading
#endif

using namespace std;
using namespace ibex;

namespace tubex
{
  Subtube operator+(const Subtube& x)
  {
    return x;
  }

  Subtube operator+(const Subtube& x1, const Subtube& x2)
  {
    StructureException::check(x1, x2);
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] + x2[i], i);
    return new_tube;
  }

  Subtube operator+(const Subtube& x1, double x2)
  {
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] + x2, i);
    return new_tube;
  }

  Subtube operator+(double x1, const Subtube& x2)
  {
    Subtube new_tube(x2);
    for(int i = 0 ; i < x2.nbSlices() ; i++)
      new_tube.set(x1 + x2[i], i);
    return new_tube;
  }

  Subtube operator+(const Subtube& x1, const Interval& x2)
  {
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] + x2, i);
    return new_tube;
  }

  Subtube operator+(const Interval& x1, const Subtube& x2)
  {
    Subtube new_tube(x2);
    for(int i = 0 ; i < x2.nbSlices() ; i++)
      new_tube.set(x1 + x2[i], i);
    return new_tube;
  }


  Subtube operator-(const Subtube& x)
  {
    Subtube new_tube(x);
    for(int i = 0 ; i < x.nbSlices() ; i++)
      new_tube.set(-x[i], i);
    return new_tube;
  }

  Subtube operator-(const Subtube& x1, const Subtube& x2)
  {
    StructureException::check(x1, x2);
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] - x2[i], i);
    return new_tube;
  }

  Subtube operator-(const Subtube& x1, double x2)
  {
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] - x2, i);
    return new_tube;
  }

  Subtube operator-(double x1, const Subtube& x2)
  {
    Subtube new_tube(x2);
    for(int i = 0 ; i < x2.nbSlices() ; i++)
      new_tube.set(x1 - x2[i], i);
    return new_tube;
  }

  Subtube operator-(const Subtube& x1, const Interval& x2)
  {
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] - x2, i);
    return new_tube;
  }

  Subtube operator-(const Interval& x1, const Subtube& x2)
  {
    Subtube new_tube(x2);
    for(int i = 0 ; i < x2.nbSlices() ; i++)
      new_tube.set(x1 - x2[i], i);
    return new_tube;
  }


  Subtube operator*(const Subtube& x1, const Subtube& x2)
  {
    StructureException::check(x1, x2);
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] * x2[i], i);
    return new_tube;
  }

  Subtube operator*(const Subtube& x1, double x2)
  {
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] * x2, i);
    return new_tube;
  }

  Subtube operator*(double x1, const Subtube& x2)
  {
    Subtube new_tube(x2);
    for(int i = 0 ; i < x2.nbSlices() ; i++)
      new_tube.set(x1 * x2[i], i);
    return new_tube;
  }

  Subtube operator*(const Interval& x1, const Subtube& x2)
  {
    Subtube new_tube(x2);
    for(int i = 0 ; i < x2.nbSlices() ; i++)
      new_tube.set(x1 * x2[i], i);
    return new_tube;
  }

  Subtube operator*(const Subtube& x1, const Interval& x2)
  {
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] * x2, i);
    return new_tube;
  }


  Subtube operator/(const Subtube& x1, const Subtube& x2)
  {
    StructureException::check(x1, x2);
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] / x2[i], i);
    return new_tube;
  }

  Subtube operator/(const Subtube& x1, double x2)
  {
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] / x2, i);
    return new_tube;
  }

  Subtube operator/(double x1, const Subtube& x2)
  {
    Subtube new_tube(x2);
    for(int i = 0 ; i < x2.nbSlices() ; i++)
      new_tube.set(x1 / x2[i], i);
    return new_tube;
  }

  Subtube operator/(const Interval& x1, const Subtube& x2)
  {
    Subtube new_tube(x2);
    for(int i = 0 ; i < x2.nbSlices() ; i++)
      new_tube.set(x1 / x2[i], i);
    return new_tube;
  }

  Subtube operator/(const Subtube& x1, const Interval& x2)
  {
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] / x2, i);
    return new_tube;
  }


  Subtube operator|(const Subtube& x1, const Subtube& x2)
  {
    StructureException::check(x1, x2);
    Subtube new_tube(x1);
    new_tube |= x2;
    return new_tube;
  }

  Subtube operator|(const Subtube& x1, double x2)
  {
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] | x2, i);
    return new_tube;
  }

  Subtube operator|(double x1, const Subtube& x2)
  {
    Subtube new_tube(x2);
    for(int i = 0 ; i < x2.nbSlices() ; i++)
      new_tube.set(x1 | x2[i], i);
    return new_tube;
  }

  Subtube operator|(const Interval& x1, const Subtube& x2)
  {
    Subtube new_tube(x2);
    for(int i = 0 ; i < x2.nbSlices() ; i++)
      new_tube.set(x1 | x2[i], i);
    return new_tube;
  }

  Subtube operator|(const Subtube& x1, const Interval& x2)
  {
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] | x2, i);
    return new_tube;
  }


  Subtube operator&(const Subtube& x1, const Subtube& x2)
  {
    StructureException::check(x1, x2);
    Subtube new_tube(x1);
    new_tube &= x2;
    return new_tube;
  }

  Subtube operator&(const Subtube& x1, double x2)
  {
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] & x2, i);
    return new_tube;
  }

  Subtube operator&(double x1, const Subtube& x2)
  {
    Subtube new_tube(x2);
    for(int i = 0 ; i < x2.nbSlices() ; i++)
      new_tube.set(x1 & x2[i], i);
    return new_tube;
  }

  Subtube operator&(const Interval& x1, const Subtube& x2)
  {
    Subtube new_tube(x2);
    for(int i = 0 ; i < x2.nbSlices() ; i++)
      new_tube.set(x1 & x2[i], i);
    return new_tube;
  }

  Subtube operator&(const Subtube& x1, const Interval& x2)
  {
    Subtube new_tube(x1);
    for(int i = 0 ; i < x1.nbSlices() ; i++)
      new_tube.set(x1[i] & x2, i);
    return new_tube;
  }


  #define func_unary(tube, f) \
      Subtube new_tube(x); \
      for(int i = 0 ; i < x.nbSlices() ; i++) new_tube.set(ibex::f(x[i]), i); \
      return new_tube;

  #define func_unary_param(tube, param, f) \
      Subtube new_tube(x); \
      for(int i = 0 ; i < x.nbSlices() ; i++) new_tube.set(ibex::f(x[i], param), i); \
      return new_tube;

  #define func_binary(tube_a, tube_b, f) \
      Subtube new_tube(x); \
      StructureException::check(tube_a, tube_b); \
      for(int i = 0 ; i < x.nbSlices() ; i++) new_tube.set(ibex::f(tube_a[i], tube_b[i]), i); \
      return new_tube;

  Subtube cos(const Subtube& x) { func_unary(x, cos) }
  Subtube sin(const Subtube& x) { func_unary(x, sin) }
  Subtube abs(const Subtube& x) { func_unary(x, abs) }
  Subtube sqr(const Subtube& x) { func_unary(x, sqr) }
  Subtube sqrt(const Subtube& x) { func_unary(x, sqrt) }
  Subtube pow(const Subtube& x, int p) { func_unary_param(x, p, pow) }
  Subtube pow(const Subtube& x, double p) { func_unary_param(x, p, pow) }
  Subtube pow(const Subtube& x, const Interval& p) { func_unary_param(x, p, pow) }
  Subtube root(const Subtube& x, int p) { func_unary_param(x, p, root) }
  Subtube exp(const Subtube& x) { func_unary(x, exp) }
  Subtube log(const Subtube& x) { func_unary(x, log) }
  Subtube tan(const Subtube& x) { func_unary(x, tan) }
  Subtube acos(const Subtube& x) { func_unary(x, acos) }
  Subtube asin(const Subtube& x) { func_unary(x, asin) }
  Subtube atan(const Subtube& x) { func_unary(x, atan) }
  Subtube cosh(const Subtube& x) { func_unary(x, cosh) }
  Subtube sinh(const Subtube& x) { func_unary(x, sinh) }
  Subtube tanh(const Subtube& x) { func_unary(x, tanh) }
  Subtube acosh(const Subtube& x) { func_unary(x, acosh) }
  Subtube asinh(const Subtube& x) { func_unary(x, asinh) }
  Subtube atanh(const Subtube& x) { func_unary(x, atanh) }
  Subtube atan2(const Subtube& y, const Subtube& x) { func_binary(y, x, atan2) }
}