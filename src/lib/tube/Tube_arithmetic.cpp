/* ============================================================================
 *  IBEX-Robotics - Tube class
 * ============================================================================
 *  Copyright : Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the MIT license. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "Tube.h"
#include <iostream>
#include <iomanip> // for setprecision()
#include <omp.h> // for multithreading

using namespace std;
using namespace ibex;

void warningTubesSizes(const Tube& x1, const Tube& x2)
{
  if(x1.size() != x2.size())
    cout << "Warning Tube::operator(): Tubes of different dimensions: " 
         << "n1=" << x1.size() << " and n2=" << x2.size() << endl;

  if(x1.getT() != x2.getT())
    cout << "Warning Tube::unionWith(): Tubes of different domain: " 
         << "[t1]=" << x1.getT() << " and [t2]=" << x2.getT() << endl;
}

Tube operator+(const Tube& x1, const Tube& x2)
{
  warningTubesSizes(x1, x2);
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] + x2[i], i);
  return new_tube;
}

Tube operator+(const Tube& x1, double x2)
{
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] + x2, i);
  return new_tube;
}

Tube operator+(double x1, const Tube& x2)
{
  Tube new_tube(x2);
  for(int i = 0 ; i < x2.size() ; i++)
    new_tube.setY(x2[i] + x1, i);
  return new_tube;
}

Tube operator+(const Tube& x1, const Interval& x2)
{
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] + x2, i);
  return new_tube;
}

Tube operator+(const Interval& x1, const Tube& x2)
{
  Tube new_tube(x2);
  for(int i = 0 ; i < x2.size() ; i++)
    new_tube.setY(x2[i] + x1, i);
  return new_tube;
}


Tube operator-(const Tube& x)
{
  Tube new_tube(x);
  for(int i = 0 ; i < x.size() ; i++)
    new_tube.setY(-x[i], i);
  return new_tube;
}

Tube operator-(const Tube& x1, const Tube& x2)
{
  warningTubesSizes(x1, x2);
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] - x2[i], i);
  return new_tube;
}

Tube operator-(const Tube& x1, double x2)
{
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] - x2, i);
  return new_tube;
}

Tube operator-(double x1, const Tube& x2)
{
  Tube new_tube(x2);
  for(int i = 0 ; i < x2.size() ; i++)
    new_tube.setY(x2[i] - x1, i);
  return new_tube;
}

Tube operator-(const Tube& x1,  const Interval& x2)
{
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] - x2, i);
  return new_tube;
}

Tube operator-(const Interval& x1, const Tube& x2)
{
  Tube new_tube(x2);
  for(int i = 0 ; i < x2.size() ; i++)
    new_tube.setY(x2[i] - x1, i);
  return new_tube;
}


Tube operator*(const Tube& x1, const Tube& x2)
{
  warningTubesSizes(x1, x2);
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] * x2[i], i);
  return new_tube;
}

Tube operator*(const Tube& x1, double x2)
{
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] * x2, i);
  return new_tube;
}

Tube operator*(double x1, const Tube& x2)
{
  Tube new_tube(x2);
  for(int i = 0 ; i < x2.size() ; i++)
    new_tube.setY(x2[i] * x1, i);
  return new_tube;
}

Tube operator*(const Interval& x1, const Tube& x2)
{
  Tube new_tube(x2);
  for(int i = 0 ; i < x2.size() ; i++)
    new_tube.setY(x2[i] * x1, i);
  return new_tube;
}

Tube operator*(const Tube& x1, const Interval& x2)
{
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] * x2, i);
  return new_tube;
}


Tube operator/(const Tube& x1, const Tube& x2)
{
  warningTubesSizes(x1, x2);
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] / x2[i], i);
  return new_tube;
}

Tube operator/(const Tube& x1, double x2)
{
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] / x2, i);
  return new_tube;
}

Tube operator/(double x1, const Tube& x2)
{
  Tube new_tube(x2);
  for(int i = 0 ; i < x2.size() ; i++)
    new_tube.setY(x2[i] / x1, i);
  return new_tube;
}

Tube operator/(const Interval& x1, const Tube& x2)
{
  Tube new_tube(x2);
  for(int i = 0 ; i < x2.size() ; i++)
    new_tube.setY(x2[i] / x1, i);
  return new_tube;
}

Tube operator/(const Tube& x1, const Interval& x2)
{
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] / x2, i);
  return new_tube;
}


Tube operator|(const Tube& x1, const Tube& x2)
{
  warningTubesSizes(x1, x2);
  Tube new_tube(x1);
  new_tube |= x2;
  return new_tube;
}

Tube operator|(const Tube& x1, double x2)
{
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] | x2, i);
  return new_tube;
}

Tube operator|(double x1, const Tube& x2)
{
  Tube new_tube(x2);
  for(int i = 0 ; i < x2.size() ; i++)
    new_tube.setY(x2[i] | x1, i);
  return new_tube;
}

Tube operator|(const Interval& x1, const Tube& x2)
{
  Tube new_tube(x2);
  for(int i = 0 ; i < x2.size() ; i++)
    new_tube.setY(x2[i] | x1, i);
  return new_tube;
}

Tube operator|(const Tube& x1, const Interval& x2)
{
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] | x2, i);
  return new_tube;
}


Tube operator&(const Tube& x1, const Tube& x2)
{
  warningTubesSizes(x1, x2);
  Tube new_tube(x1);
  new_tube &= x2;
  return new_tube;
}

Tube operator&(const Tube& x1, double x2)
{
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] & x2, i);
  return new_tube;
}

Tube operator&(double x1, const Tube& x2)
{
  Tube new_tube(x2);
  for(int i = 0 ; i < x2.size() ; i++)
    new_tube.setY(x2[i] & x1, i);
  return new_tube;
}

Tube operator&(const Interval& x1, const Tube& x2)
{
  Tube new_tube(x2);
  for(int i = 0 ; i < x2.size() ; i++)
    new_tube.setY(x2[i] & x1, i);
  return new_tube;
}

Tube operator&(const Tube& x1, const Interval& x2)
{
  Tube new_tube(x1);
  for(int i = 0 ; i < x1.size() ; i++)
    new_tube.setY(x1[i] & x2, i);
  return new_tube;
}

#define func_unary(tube, f) \
    Tube new_tube(x); \
    for(int i = 0 ; i < x.size() ; i++) new_tube.setY(ibex::f(x[i]), i); \
    return new_tube;

#define func_binary(tube, param, f) \
    Tube new_tube(x); \
    for(int i = 0 ; i < x.size() ; i++) new_tube.setY(ibex::f(x[i], param), i); \
    return new_tube;

Tube cos(const Tube& x) { func_unary(x, cos) }
Tube sin(const Tube& x) { func_unary(x, sin) }
Tube abs(const Tube& x) { func_unary(x, abs) }
Tube sqr(const Tube& x) { func_unary(x, sqr) }
Tube sqrt(const Tube& x) { func_unary(x, sqrt) }
Tube pow(const Tube& x, int p) { func_binary(x, p, pow) }
Tube pow(const Tube& x, double p) { func_binary(x, p, pow) }
Tube pow(const Tube& x, const Interval& p) { func_binary(x, p, pow) }
Tube root(const Tube& x, int p) { func_binary(x, p, root) }
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