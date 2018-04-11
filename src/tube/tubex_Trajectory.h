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

#ifndef Trajectory_HEADER
#define Trajectory_HEADER

#include <map>
#include "ibex_Interval.h"
#include "ibex_Function.h"

namespace tubex
{
  class Trajectory
  {
    public:

      // Definition
      Trajectory();
      Trajectory(const ibex::Interval& domain, const ibex::Function& f);
      Trajectory(const std::map<double,double>& m_map_values);
      ~Trajectory();

      // Access values
      const std::map<double,double>& getMap() const;
      const ibex::Function* getFunction() const;
      const ibex::Interval& domain() const;
      const ibex::Interval& codomain() const;
      const double operator[](double t) const;
      const ibex::Interval operator[](const ibex::Interval& t) const;

      // Tests
      bool operator==(const Trajectory& x) const;
      bool operator!=(const Trajectory& x) const;

      // Setting values
      double& set(double t, double y);
      void truncateDomain(const ibex::Interval& domain);
      void shiftDomain(double shift_ref);


    protected:

      /** Class variables **/

        ibex::Interval m_domain = ibex::Interval::EMPTY_SET, m_codomain = ibex::Interval::EMPTY_SET;
        // A trajectory is defined either by a Function or a map of values
        ibex::Function *m_function = NULL;
        std::map<double,double> m_map_values;
  };
}

#endif