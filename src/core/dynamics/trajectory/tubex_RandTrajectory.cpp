/** 
 *  RandTrajectory class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_RandTrajectory.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    RandTrajectory::RandTrajectory(const Interval& domain, double timestep, const Interval& bounds)
      : Trajectory()
    {
      assert(valid_domain(domain));
      assert(timestep > 0.);
      assert(!bounds.is_empty() && !bounds.is_unbounded());

      srand(time(NULL));

      double t;
      for(t = domain.lb() ; t <= domain.ub() ; t+=timestep)
        m_map_values[t] = (rand()/double(RAND_MAX))*bounds.diam()+bounds.lb();
      m_domain = Interval(domain.lb(),t);

      truncate_domain(domain);
      compute_codomain();
      assert(m_codomain.is_subset(bounds));
    }
}