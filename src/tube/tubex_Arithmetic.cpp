/* ============================================================================
 *  tubex-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Tube.h"
#include "exceptions/tubex_DomainException.h"
#include <iostream>
#include <iomanip> // for setprecision()
#ifdef _OPENMP
  #include <omp.h> // for multithreading
#endif

using namespace std;
using namespace ibex;

namespace tubex
{
  Tube operator+(const Tube& x)
  {
    return x;
  }

  Tube operator+(const Tube& x1, const Tube& x2)
  {
    checkStructures(x1, x2);
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] + x2[i], i);
    return new_tube;
  }

  Tube operator+(const Tube& x1, double x2)
  {
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] + x2, i);
    return new_tube;
  }

  Tube operator+(double x1, const Tube& x2)
  {
    Tube new_tube(x2);
    for(int i = 0 ; i < x2.size() ; i++)
      new_tube.set(x1 + x2[i], i);
    return new_tube;
  }

  Tube operator+(const Tube& x1, const Interval& x2)
  {
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] + x2, i);
    return new_tube;
  }

  Tube operator+(const Interval& x1, const Tube& x2)
  {
    Tube new_tube(x2);
    for(int i = 0 ; i < x2.size() ; i++)
      new_tube.set(x1 + x2[i], i);
    return new_tube;
  }


  Tube operator-(const Tube& x)
  {
    Tube new_tube(x);
    for(int i = 0 ; i < x.size() ; i++)
      new_tube.set(-x[i], i);
    return new_tube;
  }

  Tube operator-(const Tube& x1, const Tube& x2)
  {
    checkStructures(x1, x2);
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] - x2[i], i);
    return new_tube;
  }

  Tube operator-(const Tube& x1, double x2)
  {
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] - x2, i);
    return new_tube;
  }

  Tube operator-(double x1, const Tube& x2)
  {
    Tube new_tube(x2);
    for(int i = 0 ; i < x2.size() ; i++)
      new_tube.set(x1 - x2[i], i);
    return new_tube;
  }

  Tube operator-(const Tube& x1, const Interval& x2)
  {
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] - x2, i);
    return new_tube;
  }

  Tube operator-(const Interval& x1, const Tube& x2)
  {
    Tube new_tube(x2);
    for(int i = 0 ; i < x2.size() ; i++)
      new_tube.set(x1 - x2[i], i);
    return new_tube;
  }


  Tube operator*(const Tube& x1, const Tube& x2)
  {
    checkStructures(x1, x2);
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] * x2[i], i);
    return new_tube;
  }

  Tube operator*(const Tube& x1, double x2)
  {
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] * x2, i);
    return new_tube;
  }

  Tube operator*(double x1, const Tube& x2)
  {
    Tube new_tube(x2);
    for(int i = 0 ; i < x2.size() ; i++)
      new_tube.set(x1 * x2[i], i);
    return new_tube;
  }

  Tube operator*(const Interval& x1, const Tube& x2)
  {
    Tube new_tube(x2);
    for(int i = 0 ; i < x2.size() ; i++)
      new_tube.set(x1 * x2[i], i);
    return new_tube;
  }

  Tube operator*(const Tube& x1, const Interval& x2)
  {
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] * x2, i);
    return new_tube;
  }


  Tube operator/(const Tube& x1, const Tube& x2)
  {
    checkStructures(x1, x2);
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] / x2[i], i);
    return new_tube;
  }

  Tube operator/(const Tube& x1, double x2)
  {
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] / x2, i);
    return new_tube;
  }

  Tube operator/(double x1, const Tube& x2)
  {
    Tube new_tube(x2);
    for(int i = 0 ; i < x2.size() ; i++)
      new_tube.set(x1 / x2[i], i);
    return new_tube;
  }

  Tube operator/(const Interval& x1, const Tube& x2)
  {
    Tube new_tube(x2);
    for(int i = 0 ; i < x2.size() ; i++)
      new_tube.set(x1 / x2[i], i);
    return new_tube;
  }

  Tube operator/(const Tube& x1, const Interval& x2)
  {
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] / x2, i);
    return new_tube;
  }


  Tube operator|(const Tube& x1, const Tube& x2)
  {
    checkStructures(x1, x2);
    Tube new_tube(x1);
    new_tube |= x2;
    return new_tube;
  }

  Tube operator|(const Tube& x1, double x2)
  {
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] | x2, i);
    return new_tube;
  }

  Tube operator|(double x1, const Tube& x2)
  {
    Tube new_tube(x2);
    for(int i = 0 ; i < x2.size() ; i++)
      new_tube.set(x1 | x2[i], i);
    return new_tube;
  }

  Tube operator|(const Interval& x1, const Tube& x2)
  {
    Tube new_tube(x2);
    for(int i = 0 ; i < x2.size() ; i++)
      new_tube.set(x1 | x2[i], i);
    return new_tube;
  }

  Tube operator|(const Tube& x1, const Interval& x2)
  {
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] | x2, i);
    return new_tube;
  }


  Tube operator&(const Tube& x1, const Tube& x2)
  {
    checkStructures(x1, x2);
    Tube new_tube(x1);
    new_tube &= x2;
    return new_tube;
  }

  Tube operator&(const Tube& x1, double x2)
  {
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] & x2, i);
    return new_tube;
  }

  Tube operator&(double x1, const Tube& x2)
  {
    Tube new_tube(x2);
    for(int i = 0 ; i < x2.size() ; i++)
      new_tube.set(x1 & x2[i], i);
    return new_tube;
  }

  Tube operator&(const Interval& x1, const Tube& x2)
  {
    Tube new_tube(x2);
    for(int i = 0 ; i < x2.size() ; i++)
      new_tube.set(x1 & x2[i], i);
    return new_tube;
  }

  Tube operator&(const Tube& x1, const Interval& x2)
  {
    Tube new_tube(x1);
    for(int i = 0 ; i < x1.size() ; i++)
      new_tube.set(x1[i] & x2, i);
    return new_tube;
  }


  #define func_unary(tube, f) \
      Tube new_tube(x); \
      for(int i = 0 ; i < x.size() ; i++) new_tube.set(ibex::f(x[i]), i); \
      return new_tube;

  #define func_unary_param(tube, param, f) \
      Tube new_tube(x); \
      for(int i = 0 ; i < x.size() ; i++) new_tube.set(ibex::f(x[i], param), i); \
      return new_tube;

  #define func_binary(tube_a, tube_b, f) \
      Tube new_tube(x); \
      checkStructures(tube_a, tube_b); \
      for(int i = 0 ; i < x.size() ; i++) new_tube.set(ibex::f(tube_a[i], tube_b[i]), i); \
      return new_tube;

  Tube cos(const Tube& x) { func_unary(x, cos) }
  Tube sin(const Tube& x) { func_unary(x, sin) }
  Tube abs(const Tube& x) { func_unary(x, abs) }
  Tube sqr(const Tube& x) { func_unary(x, sqr) }
  Tube sqrt(const Tube& x) { func_unary(x, sqrt) }
  Tube pow(const Tube& x, int p) { func_unary_param(x, p, pow) }
  Tube pow(const Tube& x, double p) { func_unary_param(x, p, pow) }
  Tube pow(const Tube& x, const Interval& p) { func_unary_param(x, p, pow) }
  Tube root(const Tube& x, int p) { func_unary_param(x, p, root) }
  Tube exp(const Tube& x) { func_unary(x, exp) }
  Tube log(const Tube& x) { func_unary(x, log) }
  Tube tan(const Tube& x) { func_unary(x, tan) }
  Tube acos(const Tube& x) { func_unary(x, acos) }
  Tube asin(const Tube& x) { func_unary(x, asin) }
  Tube atan(const Tube& x) { func_unary(x, atan) }
  Tube cosh(const Tube& x) { func_unary(x, cosh) }
  Tube sinh(const Tube& x) { func_unary(x, sinh) }
  Tube tanh(const Tube& x) { func_unary(x, tanh) }
  Tube acosh(const Tube& x) { func_unary(x, acosh) }
  Tube asinh(const Tube& x) { func_unary(x, asinh) }
  Tube atanh(const Tube& x) { func_unary(x, atanh) }
  Tube atan2(const Tube& y, const Tube& x) { func_binary(y, x, atan2) }
}