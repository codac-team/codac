/** 
 *  Trajectory class (operators)
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Trajectory.h"

using namespace std;
using namespace ibex;

namespace codac
{
  #define macro_assign_scal(fdef,f) \
    \
    const Trajectory& Trajectory::fdef(double x) \
    { \
      assert(definition_type() == TrajDefnType::MAP_OF_VALUES && \
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
      assert(tdomain() == x.tdomain()); \
      assert(!(definition_type() == TrajDefnType::ANALYTIC_FNC && x.definition_type() == TrajDefnType::ANALYTIC_FNC) && \
        "not supported yet for trajectories defined by a Function"); \
      \
      Trajectory x_sampled(x); \
      if(definition_type() == TrajDefnType::MAP_OF_VALUES) \
        x_sampled.sample(*this); \
      \
      map<double,double> new_map; \
      for(auto const& it : x_sampled.sampled_map()) \
        new_map[it.first] = (*this)(it.first) f it.second; \
      \
      m_map_values = new_map; \
      compute_codomain(); \
      return *this; \
    } \
    \

  macro_assign_scal(operator+=, +);
  macro_assign_scal(operator-=, -);
  macro_assign_scal(operator*=, *);
  macro_assign_scal(operator/=, /);
}