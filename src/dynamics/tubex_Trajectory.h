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

#ifndef __TUBEX_TRAJECTORY_H__
#define __TUBEX_TRAJECTORY_H__

#include "tubex_TrajectoryVector.h"

namespace tubex
{
  class Trajectory : public TrajectoryVector
  {
    public:

      // Definition
      Trajectory();
      Trajectory(const ibex::Interval& domain, const tubex::Function& f);
      Trajectory(const std::map<double,double>& m_map_values);
      Trajectory(const TrajectoryVector& traj);
      ~Trajectory();
      const Trajectory& operator=(const Trajectory& x);
      using TrajectoryVector::dim;

      // Access values
      using TrajectoryVector::getFunction;
      using TrajectoryVector::domain;
      const ibex::Interval codomain() const;
      double operator[](double t) const;
      const ibex::Interval operator[](const ibex::Interval& t) const;

      // Tests
      using TrajectoryVector::notDefined;
      bool operator==(const Trajectory& x) const;
      bool operator!=(const Trajectory& x) const;

      // Setting values
      double set(double t, double y);
      using TrajectoryVector::truncateDomain;
      using TrajectoryVector::shiftDomain;

      // String
      friend std::ostream& operator<<(std::ostream& str, const Trajectory& x);
      const std::string className() const { return "Trajectory"; };
  };
}

#endif