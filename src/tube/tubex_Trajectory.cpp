/* ============================================================================
 *  tubex-lib - Trajectory class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#include <sstream>
#include "tubex_Trajectory.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Definition

  Trajectory::Trajectory()
  {
    
  }

  Trajectory::Trajectory(const Interval& domain, const Function& f)
    : TrajectoryVector(domain, f)
  {
    DimensionException::checkScalar(f.image_dim());
  }

  Trajectory::Trajectory(const map<double,double>& map_values)
  {
    typename map<double,double>::const_iterator it_map;
    for(it_map = map_values.begin() ; it_map != map_values.end() ; it_map++)
      set(it_map->first, it_map->second);
  }

  Trajectory::~Trajectory()
  {

  }

  // Access values

  const Interval Trajectory::codomain() const
  {
    return codomainBox()[0];
  }

  double Trajectory::operator[](double t) const
  {
    return TrajectoryVector::operator[](t)[0];
  }
  
  const Interval Trajectory::operator[](const Interval& t) const
  {
    return TrajectoryVector::operator[](t)[0];
  }
  
  // Tests

  bool Trajectory::operator==(const Trajectory& x) const
  {
    return TrajectoryVector::operator==(x);
  }
  
  bool Trajectory::operator!=(const Trajectory& x) const
  {
    return TrajectoryVector::operator!=(x);
  }

  // Setting values

  double Trajectory::set(double t, double y)
  {
    return TrajectoryVector::set(t, Vector(1, y))[0];
  }
}