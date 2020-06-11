/** 
 *  RandTrajectory class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <time.h>
#include "tubex_RandTrajectory.h"
#include "tubex_Tools.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    RandTrajectory::RandTrajectory(const Interval& tdomain, double timestep, const Interval& bounds)
      : Trajectory()
    {
      assert(valid_tdomain(tdomain));
      assert(timestep > 0.);
      assert(!bounds.is_empty() && !bounds.is_unbounded());

      srand(time(NULL));

      double t;
      for(t = tdomain.lb() ; t <= tdomain.ub() ; t+=timestep)
        m_map_values[t] = Tools::rand_in_bounds(bounds);
      m_tdomain = Interval(tdomain.lb(),t);

      truncate_tdomain(tdomain);
      compute_codomain();

      if(!(m_codomain.is_subset(bounds)))
        cout << m_codomain << bounds << endl;
      assert(m_codomain.is_subset(bounds));
    }
}