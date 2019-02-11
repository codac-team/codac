/* ============================================================================
 *  tubex-lib - TrajectoryVector class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_TRAJECTORYVECTOR_H__
#define __TUBEX_TRAJECTORYVECTOR_H__

#include <map>
#include "ibex_Vector.h"
#include "ibex_Interval.h"
#include "tubex_Function.h"
#include "tubex_Trajectory.h"
#include "tubex_DynamicalItem.h"

// todo: arithmetic operators for trajectories?

namespace tubex
{
  class Function;
  class TubeVector;
  class Trajectory;

  class TrajectoryVector : public DynamicalItem
  {
    public:

      // Definition
      TrajectoryVector(int n);
      TrajectoryVector(const ibex::Interval& domain, const tubex::Function& f);
      TrajectoryVector(const std::map<double,ibex::Vector>& m_map_values);
      TrajectoryVector(const TrajectoryVector& traj);
      ~TrajectoryVector();
      const TrajectoryVector& operator=(const TrajectoryVector& x);
      int size() const;
      void resize(int n);
      const TrajectoryVector subvector(int start_index, int end_index) const;
      void put(int start_index, const TrajectoryVector& subvec);

      // Accessing values
      const ibex::Interval domain() const;
      const ibex::IntervalVector codomain() const;
      Trajectory& operator[](int index);
      const Trajectory& operator[](int index) const;
      const ibex::Vector operator()(double t) const;
      const ibex::IntervalVector operator()(const ibex::Interval& t) const;

      // Tests
      bool not_defined() const;
      bool operator==(const TrajectoryVector& x) const;
      bool operator!=(const TrajectoryVector& x) const;

      // Setting values
      void set(const ibex::Vector& y, double t);
      void truncate_domain(const ibex::Interval& domain);
      void shift_domain(double shift_ref);

      // Integration
      const TrajectoryVector primitive() const;
      const TrajectoryVector primitive(double dt) const;

      // String
      friend std::ostream& operator<<(std::ostream& str, const TrajectoryVector& x);
      const std::string class_name() const { return "TrajectoryVector"; };

    protected:

      TrajectoryVector();
      const ibex::IntervalVector codomain_box() const;

      /** Class variables **/

        int m_n = 0;
        Trajectory *m_v_trajs = NULL;

      friend void deserialize_TrajectoryVector(std::ifstream& bin_file, TrajectoryVector *&traj);
      friend class TubeVector; // for TubeVector::deserialize method
  };
}

#endif