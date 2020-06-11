/** 
 *  Arithmetic operations on tubes
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_tube_arithmetic.h"
#include "tubex_Slice.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  const TubeVector operator+(const TubeVector& x)
  {
    return x;
  }

  const TubeVector operator-(const TubeVector& x)
  {
    TubeVector y(x);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] = -y[i];
    return y;
  }

  #define macro_vect_binary(f, feq) \
    \
    const TubeVector f(const TubeVector& x1, const TubeVector& x2) \
    { \
      assert(x1.size() == x2.size()); \
      assert(x1.tdomain() == x2.tdomain()); \
      \
      TubeVector y(x1); \
      for(int i = 0 ; i < y.size() ; i++) \
        y[i] = tubex::f(x1[i], x2[i]); \
      return y; \
    } \
    \
    const TubeVector f(const TubeVector& x1, const IntervalVector& x2) \
    { \
      assert(x1.size() == x2.size()); \
      \
      TubeVector y(x1); \
      for(int i = 0 ; i < y.size() ; i++) \
        y[i] = tubex::f(x1[i], x2[i]); \
      return y; \
    } \
    \
    const TubeVector f(const IntervalVector& x1, const TubeVector& x2) \
    { \
      assert(x1.size() == x2.size()); \
      \
      TubeVector y(x2); \
      for(int i = 0 ; i < y.size() ; i++) \
        y[i] = tubex::f(x1[i], x2[i]); \
      return y; \
    } \
    \
    const TubeVector f(const TubeVector& x1, const TrajectoryVector& x2) \
    { \
      assert(x1.size() == x2.size()); \
      assert(x1.tdomain() == x2.tdomain()); \
      \
      TubeVector y(x1); \
      for(int i = 0 ; i < y.size() ; i++) \
        y[i] = tubex::f(x1[i], x2[i]); \
      return y; \
    } \
    \
    const TubeVector f(const TrajectoryVector& x1, const TubeVector& x2) \
    { \
      assert(x1.size() == x2.size()); \
      assert(x1.tdomain() == x2.tdomain()); \
      \
      TubeVector y(x2); \
      for(int i = 0 ; i < y.size() ; i++) \
        y[i] = tubex::f(x1[i], x2[i]); \
      return y; \
    } \
    \

  macro_vect_binary(operator+, operator+=);
  macro_vect_binary(operator-, operator-=);
  macro_vect_binary(operator|, operator|=);
  macro_vect_binary(operator&, operator&=);

  const TubeVector operator*(const Interval& x1, const TubeVector& x2)
  {
    TubeVector y(x2);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] = operator*(x1, y[i]);
    return y;
  }

  const TubeVector operator*(const Tube& x1, const IntervalVector& x2)
  {
    TubeVector y(x2.size(), x1);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] = operator*(y[i], x2[i]);
    return y;
  }

  const TubeVector operator*(const Tube& x1, const TubeVector& x2)
  {
    assert(x1.tdomain() == x2.tdomain()); \
    TubeVector y(x2);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] = operator*(x1, y[i]);
    return y;
  }

  const TubeVector operator*(const Trajectory& x1, const TubeVector& x2)
  {
    assert(x1.tdomain() == x2.tdomain()); \
    TubeVector y(x2);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] = operator*(x1, y[i]);
    return y;
  }

  const TubeVector operator/(const TubeVector& x1, const Interval& x2)
  {
    TubeVector y(x1);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] = operator/(y[i], x2);
    return y;
  }

  const TubeVector operator/(const IntervalVector& x1, const Tube& x2)
  {
    TubeVector y(x1.size(), x2);
    y.set(x1);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] = operator/(y[i], x2);
    return y;
  }

  const TubeVector operator/(const TubeVector& x1, const Tube& x2)
  {
    assert(x1.tdomain() == x2.tdomain()); \
    TubeVector y(x1);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] = operator/(y[i], x2);
    return y;
  }

  const TubeVector operator/(const TubeVector& x1, const Trajectory& x2)
  {
    assert(x1.tdomain() == x2.tdomain()); \
    TubeVector y(x1);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] = operator/(y[i], x2);
    return y;
  }

  const TubeVector abs(const TubeVector& x)
  {
    TubeVector y(x.tdomain(), x.size());
    for(int i = 0 ; i < x.size() ; i++)
      y[i] = abs(x[i]);
    return y;
  }
}