/* ============================================================================
 *  tubex-lib - Arithmetic
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_arithmetic.h"
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

  #define macro_vect_binary(f) \
    \
    const TubeVector f(const TubeVector& x1, const TubeVector& x2) \
    { \
      assert(x1.size() == x2.size()); \
      assert(x1.domain() == x2.domain()); \
      assert(TubeVector::same_slicing(x1, x2)); \
      TubeVector y(x1); \
      for(int i = 0 ; i < y.size() ; i++) \
        y[i] = tubex::f(x1[i], x2[i]); \
      return y; \
    } \
    \
    const TubeVector f(const TubeVector& x1, const IntervalVector& x2) \
    { \
      assert(x1.size() == x2.size()); \
      TubeVector y(x1); \
      for(int i = 0 ; i < y.size() ; i++) \
        y[i] = tubex::f(x1[i], x2[i]); \
      return y; \
    } \
    \
    const TubeVector f(const IntervalVector& x1, const TubeVector& x2) \
    { \
      assert(x1.size() == x2.size()); \
      TubeVector y(x2); \
      for(int i = 0 ; i < y.size() ; i++) \
        y[i] = tubex::f(x1[i], x2[i]); \
      return y; \
    } \
    \

  macro_vect_binary(operator+);
  macro_vect_binary(operator-);
  macro_vect_binary(operator|);
  macro_vect_binary(operator&);

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
    assert(x1.domain() == x2.domain()); \
    assert(TubeVector::same_slicing(x2, x1)); \
    TubeVector y(x2);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] = operator*(x1, y[i]);
    return y;
  }
}