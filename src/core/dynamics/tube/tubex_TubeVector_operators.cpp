/* ============================================================================
 *  tubex-lib - TubeVector class (operators)
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
#include "tubex_TubeVector.h"
#include "tubex_TrajectoryVector.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  #define assign_vect_vect(f) \
    \
    const TubeVector& TubeVector::f(const IntervalVector& x) \
    { \
      assert(size() == x.size()); \
      for(int i = 0 ; i < size() ; i++) \
        (*this)[i].f(x[i]); \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const TrajectoryVector& x) \
    { \
      assert(size() == x.size()); \
      assert(domain() == x.domain()); \
      for(int i = 0 ; i < size() ; i++) \
        (*this)[i].f(x[i]); \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const TubeVector& x) \
    { \
      assert(size() == x.size()); \
      assert(domain() == x.domain()); \
      for(int i = 0 ; i < size() ; i++) \
        (*this)[i].f(x[i]); \
      return *this; \
    } \
    \

  assign_vect_vect(operator+=);
  assign_vect_vect(operator-=);
  assign_vect_vect(operator|=);
  assign_vect_vect(operator&=);

  #define assign_vect_scal(f) \
    \
    const TubeVector& TubeVector::f(const Interval& x) \
    { \
      for(int i = 0 ; i < size() ; i++) \
        (*this)[i].f(x); \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const Trajectory& x) \
    { \
      assert(domain() == x.domain()); \
      for(int i = 0 ; i < size() ; i++) \
        (*this)[i].f(x); \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const Tube& x) \
    { \
      assert(domain() == x.domain()); \
      assert(same_slicing(*this, x)); \
      for(int i = 0 ; i < size() ; i++) \
        (*this)[i].f(x); \
      return *this; \
    } \
    \

  assign_vect_scal(operator*=);
}