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
#include "ibex_Vector.h"
#include "ibex_Interval.h"
#include "ibex_Function.h"
#include "tubex_DynamicalItem.h"

namespace tubex
{
  class Trajectory : DynamicalItem
  {
    public:

      // Definition
      Trajectory();
      Trajectory(const ibex::Interval& domain, const ibex::Function& f);
      Trajectory(const std::map<double,ibex::Vector>& m_map_values);
      ~Trajectory();
      int dim() const;

      // Access values
      const std::map<double,ibex::Vector>& getMap() const;
      const ibex::Function* getFunction() const;
      const ibex::Interval domain() const; // todo: output const Interval& (reference)
      const ibex::IntervalVector codomain() const; // todo: output const Interval& (reference)
      const ibex::Vector operator[](double t) const;
      const ibex::IntervalVector operator[](const ibex::Interval& t) const;

      // Tests
      bool operator==(const Trajectory& x) const;
      bool operator!=(const Trajectory& x) const;

      // Setting values
      ibex::Vector& set(double t, const ibex::Vector& y);
      void truncateDomain(const ibex::Interval& domain);
      void shiftDomain(double shift_ref);


    protected:

      /** Class variables **/

        ibex::Interval m_domain = ibex::Interval::EMPTY_SET;
        ibex::IntervalVector m_codomain = ibex::IntervalVector(1);
        // A trajectory is defined either by a Function or a map of values
        ibex::Function *m_function = NULL;
        std::map<double,ibex::Vector> m_map_values;
  };
}

#endif