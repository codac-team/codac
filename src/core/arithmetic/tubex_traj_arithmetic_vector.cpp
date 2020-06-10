/** 
 *  Arithmetic operations on trajectories
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
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
      assert(x1.tdomain() == x2.tdomain()); \
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

  const TrajectoryVector operator*(const Trajectory& x1, const Vector& x2)
  {
    TrajectoryVector y(x2.size(), x1);
    for(int i = 0 ; i < y.size() ; i++)
      y[i] *= x2[i];
    return y;
  }

  const TrajectoryVector operator*(const Matrix& x1, const TrajectoryVector& x2)
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

  const TrajectoryVector operator/(const Vector& x1, const Trajectory& x2)
  {
    TrajectoryVector y(x1.size());
    for(int i = 0 ; i < y.size() ; i++)
      y[i] = x1[i] / x2;
    return y;
  }

  const TrajectoryVector vecto_product(const TrajectoryVector& x1, const Vector& x2)
  {
    assert(x1.size() == 3 && x2.size() == 3);

    TrajectoryVector result(x1.size());
    for(auto const& it : x1[0].sampled_map())
    {
      double t = it.first;
      Vector v(3);
      v[0] = x1[1](t)*x2[2] - x1[2](t)*x2[1];
      v[1] = x1[2](t)*x2[0] - x1[0](t)*x2[2];
      v[2] = x1[0](t)*x2[1] - x1[1](t)*x2[0];
      result.set(v, t);
    }

    return result;
  }

  const TrajectoryVector vecto_product(const Vector& x1, const TrajectoryVector& x2)
  {
    assert(x1.size() == 3 && x2.size() == 3);
    return -vecto_product(x2, x1);
  }

  const TrajectoryVector abs(const TrajectoryVector& x)
  {
    TrajectoryVector y(x.size());
    for(int i = 0 ; i < x.size() ; i++)
      y[i] = abs(x[i]);
    return y;
  }
}