/** 
 *  TrajectoryVector class (operators)
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "ibex_Vector.h"
#include "tubex_Trajectory.h"
#include "tubex_TrajectoryVector.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  #define macro_assign_vect_vect(f) \
    \
    const TrajectoryVector& TrajectoryVector::f(const Vector& x) \
    { \
      assert(size() == x.size()); \
      \
      for(int i = 0 ; i < size() ; i++) \
        (*this)[i].f(x[i]); \
      return *this; \
    } \
    \
    const TrajectoryVector& TrajectoryVector::f(const TrajectoryVector& x) \
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
    const TrajectoryVector& TrajectoryVector::f(double x) \
    { \
      for(int i = 0 ; i < size() ; i++) \
        (*this)[i].f(x); \
      return *this; \
    } \
    \
    const TrajectoryVector& TrajectoryVector::f(const Trajectory& x) \
    { \
      assert(tdomain() == x.tdomain()); \
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
}