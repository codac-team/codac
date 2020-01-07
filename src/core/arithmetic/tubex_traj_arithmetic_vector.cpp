/** 
 *  Arithmetic operations on trajectories
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_traj_arithmetic.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  const TrajectoryVector operator+(const TrajectoryVector& x)
  {
    return x;
  }

  const TrajectoryVector operator-(const TrajectoryVector& x)
  {
    TrajectoryVector y(x);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] = -y[i];
    return y;
  }

  #define macro_vect_binary(f) \
    \
    const TrajectoryVector f(const TrajectoryVector& x1, const TrajectoryVector& x2) \
    { \
      assert(x1.size() == x2.size()); \
      assert(x1.domain() == x2.domain()); \
      TrajectoryVector y(x1); \
      for(int i = 0 ; i < y.size() ; i++) \
        y[i] = tubex::f(x1[i], x2[i]); \
      return y; \
    } \
    \
    const TrajectoryVector f(const TrajectoryVector& x1, const Vector& x2) \
    { \
      assert(x1.size() == x2.size()); \
      TrajectoryVector y(x1); \
      for(int i = 0 ; i < y.size() ; i++) \
        y[i] = tubex::f(x1[i], x2[i]); \
      return y; \
    } \
    \
    const TrajectoryVector f(const Vector& x1, const TrajectoryVector& x2) \
    { \
      assert(x1.size() == x2.size()); \
      TrajectoryVector y(x2); \
      for(int i = 0 ; i < y.size() ; i++) \
        y[i] = tubex::f(x1[i], x2[i]); \
      return y; \
    } \

  macro_vect_binary(operator+);
  macro_vect_binary(operator-);

  const TrajectoryVector operator*(double x1, const TrajectoryVector& x2)
  {
    TrajectoryVector y(x2);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] *= x1;
    return y;
  }

  const TrajectoryVector operator*(const Trajectory& x1, const TrajectoryVector& x2)
  {
    TrajectoryVector y(x2);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] *= x1;
    return y;
  }

  const TrajectoryVector operator*(const Trajectory& x1, const ibex::Vector& x2)
  {
    TrajectoryVector y(x2.size(), x1);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] *= x2[i];
    return y;
  }

  const TrajectoryVector operator*(const ibex::Matrix& x1, const TrajectoryVector& x2)
  {
    assert(x1.nb_cols() == x2.size());

    TrajectoryVector x2_(x2);
    // Same sampling for all components of this trajectory
    for(int i = 0 ; i < x2_.size() ; i++)
      for(int j = 0 ; j < x2_.size() ; j++)
        if(i != j)
          x2_[i].sample(x2_[j]);

    TrajectoryVector result(x2.size());
    for(auto const& it : x2_[0].sampled_map())
      result.set(x1*x2_(it.first), it.first);
    
    return result;
  }

  const TrajectoryVector operator/(const TrajectoryVector& x1, double x2)
  {
    TrajectoryVector y(x1);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] /= x2;
    return y;
  }

  const TrajectoryVector operator/(const TrajectoryVector& x1, const Trajectory& x2)
  {
    TrajectoryVector y(x1);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] /= x2;
    return y;
  }

  const TrajectoryVector operator/(const ibex::Vector& x1, const Trajectory& x2)
  {
    TrajectoryVector y(x1.size());
    for(int i = 0 ; i < y.size() ; i++)
      y[i] = x1[i] / x2;
    return y;
  }
}