/** 
 *  TubeVector class (operators)
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Tube.h"
#include "codac_TubeVector.h"
#include "codac_TrajectoryVector.h"

using namespace std;
using namespace ibex;

namespace codac
{
  #define macro_assign_vect_vect(f) \
    \
    const TubeVector& TubeVector::f(const IntervalVector& x) \
    { \
      assert(size() == x.size()); \
      \
      for(int i = 0 ; i < size() ; i++) \
        (*this)[i].f(x[i]); \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const TrajectoryVector& x) \
    { \
      assert(size() == x.size()); \
      assert(tdomain() == x.tdomain()); \
      \
      for(int i = 0 ; i < size() ; i++) \
        (*this)[i].f(x[i]); \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const TubeVector& x) \
    { \
      assert(size() == x.size()); \
      assert(tdomain() == x.tdomain()); \
      \
      for(int i = 0 ; i < size() ; i++) \
        (*this)[i].f(x[i]); \
      return *this; \
    } \
    \

  #define macro_assign_vect_scal(f) \
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
      assert(tdomain() == x.tdomain()); \
      \
      for(int i = 0 ; i < size() ; i++) \
        (*this)[i].f(x); \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const Tube& x) \
    { \
      assert(tdomain() == x.tdomain()); \
      assert(same_slicing(*this, x)); \
      \
      for(int i = 0 ; i < size() ; i++) \
        (*this)[i].f(x); \
      return *this; \
    } \
    \

  macro_assign_vect_vect(operator+=);
  macro_assign_vect_vect(operator-=);
  macro_assign_vect_scal(operator+=);
  macro_assign_vect_scal(operator-=);
  macro_assign_vect_scal(operator*=);
  macro_assign_vect_scal(operator/=);
  macro_assign_vect_vect(operator|=);
  macro_assign_vect_vect(operator&=);
}