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

#ifndef TRAJECTORY_HEADER
#define TRAJECTORY_HEADER

#include <map>
#include "ibex_Interval.h"
#include "ibex_Function.h"

namespace tubex
{
  class Trajectory
  {
    public:

      /**
       * \brief Default constructor
       */
      Trajectory(const std::string& name = "");
      Trajectory(const ibex::Function& f, const ibex::Interval& domain, const std::string& name = "");
      Trajectory(const std::map<double,double>& m_map_values, const std::string& name = "");
      ~Trajectory();
      const std::string& name() const;
      void setName(const std::string& name);
      const std::map<double,double> getMap() const;
      const ibex::Function* getFunction() const;
      const ibex::Interval domain() const;
      double& set(double t, double y);
      const double operator[](double t) const;
      void truncateDomain(const ibex::Interval& domain);
      void shiftDomain(double shift_ref);


    protected:

      /** Class variables **/

        std::map<double,double> m_map_values;
        ibex::Interval m_domain;
        ibex::Function *m_function;

        // Name attributes
        std::string m_name;
        static int nb_traj;
        static std::vector<std::string> v_traj_names;
  };
}

#endif