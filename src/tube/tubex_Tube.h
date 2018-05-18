/* ============================================================================
 *  tubex-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef Tube_HEADER
#define Tube_HEADER

#include "tubex_TubeTree.h"

#define SERIALIZATION_VERSION 2

namespace tubex
{
  class Tube : public TubeTree
  {
    public:

    /** Base: **/

      // Definition
      Tube(const ibex::Interval& domain, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      Tube(const ibex::Interval& domain, double timestep, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      Tube(const ibex::Interval& domain, double timestep, const ibex::Function& function);
      Tube(const Tube& x, const ibex::Interval& codomain);
      Tube(const Trajectory& traj, double thickness = 0., double timestep = 0.);
      Tube(const Trajectory& lb, const Trajectory& ub, double timestep = 0.);
      Tube(const std::string& binary_file_name);
      Tube(const std::string& binary_file_name, Trajectory& traj);
      Tube(const std::string& binary_file_name, std::vector<Trajectory>& v_trajs);
      Tube primitive(const ibex::Interval& initial_value = ibex::Interval(0.)) const;

      // Access values
      const ibex::Interval interpol(double t, const Tube& derivative) const;
      const ibex::Interval interpol(const ibex::Interval& t, const Tube& derivative) const;

      // Bisection
      //std::pair<Tube,Tube> bisect(const Tube& derivative, float ratio = 0.55) const;
      //std::pair<Tube,Tube> bisect(double t, const Tube& derivative, float ratio = 0.55) const;

      // String
      friend std::ostream& operator<<(std::ostream& str, const Tube& x);

    /** Integration: **/

      ibex::Interval integral(double t) const;
      ibex::Interval integral(const ibex::Interval& t) const;
      ibex::Interval integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
      std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t) const;
      std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t1, const ibex::Interval& t2) const;

    /** Contractors: **/

      bool ctcFwd(const Tube& derivative);
      bool ctcBwd(const Tube& derivative);
      bool ctcFwdBwd(const Tube& derivative);
      /*bool ctcEval(const Tube& derivative, ibex::Interval& t, ibex::Interval& z, bool propagate = true);
      bool ctcEval(const Tube& derivative, ibex::Interval& t, const ibex::Interval& z, bool propagate = true);
      bool ctcEval(const Tube& derivative, const ibex::Interval& t, ibex::Interval& z, bool propagate = true);
      bool ctcEval(const Tube& derivative, const ibex::Interval& t, const ibex::Interval& z, bool propagate = true);
      bool ctcOut(const ibex::Interval& t, const ibex::Interval& z);
      bool ctcIntertemporal(ibex::Interval& t1, ibex::Interval& t2) const;
      bool ctcIntertemporal(ibex::Interval& z, ibex::Interval& t1, ibex::Interval& t2) const;
      bool ctcPeriodic(const ibex::Interval& period);

    /** Serialization: **/

      void serialize(const std::string& binary_file_name = "x.tube", int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const Trajectory& traj, int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const std::vector<Trajectory>& v_trajs, int version_number = SERIALIZATION_VERSION) const;

    protected:

    /** Integration: **/
      
      void checkPartialPrimitive() const;

    /** Serialization: **/

      void deserialize(const std::string& binary_file_name, std::vector<Trajectory>& v_trajs);
  };
}

#endif