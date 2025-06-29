/** 
 *  \file codac2_RobotSimulator.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <list>
#include "codac2_Vector.h"
#include "codac2_SampledTraj.h"

namespace codac2
{
  class RobotSimulator
  {
    public:

      double v_min = 0.0; // min linear speed (m/s)
      double v_max = 1.0; // max linear speed (m/s)
      double w_max = 1.0; // max rotational speed (rad/s)
      double a_max = 2.0; // max linear acceleration (m/s²)
      double dist_threshold = 0.2; // waypoint change threshold

      RobotSimulator();
      Vector controller(const Vector& x, const Vector& wpt) const;
      SampledTraj<Vector> simulate(const Vector& x0, double dt, std::list<Vector> wpts, SampledTraj<Vector>& u) const;
  };
}