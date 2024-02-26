/** 
 *  TrajectoryVector class (operators)
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Vector.h"
#include "codac_Trajectory.h"
#include "codac_TrajectoryVector.h"

using namespace std;
using namespace ibex;

namespace codac
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