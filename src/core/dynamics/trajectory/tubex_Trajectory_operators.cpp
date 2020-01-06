/** 
 *  Trajectory class (operators)
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_Trajectory.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  #define macro_assign_scal(fdef,f) \
    \
    const Trajectory& Trajectory::fdef(double x) \
    { \
      assert(function() == NULL && \
        "not supported yet for trajectories defined by a Function"); \
      \
      for(auto& kv : m_map_values) \
        m_map_values[kv.first] = kv.second f x; \
      m_codomain.fdef(x); \
      return *this; \
    } \
    \
    const Trajectory& Trajectory::fdef(const Trajectory& x) \
    { \
      assert(domain() == x.domain()); \
      assert(function() == NULL && x.function() == NULL && \
        "not supported yet for trajectories defined by a Function"); \
      /* todo: x could be defined by a Function, in fact */ \
      \
      Trajectory x_sampled(x); \
      x_sampled.sample(*this); \
      \
      for(map<double,double>::iterator it = m_map_values.begin() ; \
        it != m_map_values.end() ; it++) \
        x_sampled.set(it->second f x_sampled(it->first), it->first); \
      \
      *this = x_sampled; \
      return *this; \
    } \
    \

  macro_assign_scal(operator+=, +);
  macro_assign_scal(operator-=, -);
  macro_assign_scal(operator*=, *);
  macro_assign_scal(operator/=, /);
}