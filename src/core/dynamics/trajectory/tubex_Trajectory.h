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

#include <map>
#include "tubex_DynamicalItem.h"
#include "tubex_Function.h"

namespace tubex
{
  class TrajectoryVector;
  
  class Trajectory : public DynamicalItem
  {
    public:

      // Definition
      Trajectory();
      Trajectory(const ibex::Interval& domain, const tubex::Function& f);
      Trajectory(const std::map<double,double>& m_map_values);
      Trajectory(const Trajectory& traj);
      ~Trajectory();
      const Trajectory& operator=(const Trajectory& x);
      int size() const;

      // Accessing values
      const std::map<double,double>& sampled_map() const;
      const tubex::Function* function() const;
      const ibex::Interval domain() const;
      const ibex::Interval codomain() const;
      double operator()(double t) const; // /!\ approx here, if traj defined by Function, use operator(Interval(double)) for reliable approx
      const ibex::Interval operator()(const ibex::Interval& t) const;

      // Tests
      bool not_defined() const;
      bool operator==(const Trajectory& x) const;
      bool operator!=(const Trajectory& x) const;

      // Setting values
      void set(double t, double y);
      void truncate_domain(const ibex::Interval& domain);
      void shift_domain(double shift_ref);

      // String
      friend std::ostream& operator<<(std::ostream& str, const Trajectory& x);
      const std::string class_name() const { return "Trajectory"; };

    protected:

      const ibex::IntervalVector codomain_box() const;

      /** Class variables **/

        ibex::Interval m_domain = ibex::Interval::EMPTY_SET;
        ibex::Interval m_codomain = ibex::Interval::EMPTY_SET;
        // A trajectory is defined either by a Function or a map of values
        tubex::Function *m_function = NULL;
        std::map<double,double> m_map_values;

      friend void deserialize_Trajectory(std::ifstream& bin_file, Trajectory *&traj);
      friend void deserialize_TrajectoryVector(std::ifstream& bin_file, TrajectoryVector *&traj);
  };
}

#endif