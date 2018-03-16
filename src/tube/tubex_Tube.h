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

#ifndef TUBE_HEADER
#define TUBE_HEADER

#include <utility>
#include "tubex_Subtube.h"

namespace tubex
{
  class Tube : public Subtube
  {
    public:

    /** Base: see file tubex_Tube_base.cpp **/

      // Definition
      Tube(const ibex::Interval& domain, const ibex::Interval& value);
      Tube(const ibex::Interval& domain, double timestep, const ibex::Interval& value = ibex::Interval::ALL_REALS);
      Tube(const Tube& x, const ibex::Interval& value);
      Tube(const std::string& binary_file_name);
      Tube(const std::string& binary_file_name, Trajectory& traj);
      Tube(const std::string& binary_file_name, std::vector<Trajectory>& v_trajs);
      Tube primitive(const ibex::Interval& initial_value = ibex::Interval(0.)) const;
      std::pair<Tube,Tube> bisect(const Tube& derivative, double t, float ratio = 0.55) const;

      // Slices structure
      void sample(int slice_id);
      const ibex::Interval& inputGate(int slice_id) const;
      const ibex::Interval& outputGate(int slice_id) const;
      void setInputGate(int slice_id);
      void setOutputGate(int slice_id);

      // Access values
      ibex::Interval operator[](double t) const;
      ibex::Interval operator[](const ibex::Interval& t) const;
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      ibex::Interval interpol(double t, const Tube& derivative) const;
      ibex::Interval interpol(const ibex::Interval& t, const Tube& derivative) const;
      std::pair<ibex::Interval,ibex::Interval> partialInterpol(const ibex::Interval& t, const Tube& derivative) const;
      ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& t = ibex::Interval::ALL_REALS) const;

      // Tests
      bool operator==(const Tube& x) const;
      bool operator!=(const Tube& x) const;
      bool isEmpty() const;
      bool isInteriorSubset(const Tube& outer_tube) const;
      bool encloses(const Trajectory& x) const;

      // Setting values
      void set(const ibex::Interval& y);
      void set(const ibex::Interval& y, int slice_id);
      void setEmpty();
      bool contract(const ibex::Interval& y, int slice_id);
      const ibex::Interval inflate(const ibex::Interval& y, int slice_id);
      const ibex::Interval inflate(const ibex::Interval& y, double t);
      Tube& inflate(double rad);

      // Operators
      Tube& operator=(const Tube& x);
      Tube& operator|=(const Tube& x);
      Tube& operator|=(const Trajectory& x);
      Tube& operator&=(const Tube& x);

      // String
      const std::string& name() const;
      void setName(const std::string& name);
      friend std::ostream& operator<<(std::ostream& str, const Tube& x);

    /** Contractors: see file tubex_Tube_contractors.cpp **/

      bool ctcFwd(const Tube& derivative, const ibex::Interval& initial_value = ibex::Interval::ALL_REALS);
      bool ctcBwd(const Tube& derivative);
      bool ctcFwdBwd(const Tube& derivative, const ibex::Interval& initial_value = ibex::Interval::ALL_REALS);
      bool ctcEval(const Tube& derivative, ibex::Interval& t, ibex::Interval& z, bool propagate = true);
      bool ctcEval(const Tube& derivative, ibex::Interval& t, const ibex::Interval& z, bool propagate = true);
      bool ctcEval(const Tube& derivative, const ibex::Interval& t, ibex::Interval& z, bool propagate = true);
      bool ctcEval(const Tube& derivative, const ibex::Interval& t, const ibex::Interval& z, bool propagate = true);
      bool ctcOut(const ibex::Interval& t, const ibex::Interval& z);
      bool ctcIntertemporal(ibex::Interval& t1, ibex::Interval& t2) const;
      bool ctcIntertemporal(ibex::Interval& z, ibex::Interval& t1, ibex::Interval& t2) const;
      bool ctcPeriodic(const ibex::Interval& period);

    /** Serialization: see file tubex_Tube_serialization.cpp **/

      bool serialize(const std::string& binary_file_name = "x.tube", int version_number = SERIALIZATION_VERSION) const;
      bool serialize(const std::string& binary_file_name, const Trajectory& traj, int version_number = SERIALIZATION_VERSION) const;
      bool serialize(const std::string& binary_file_name, const std::vector<const Trajectory&>& v_trajs, int version_number = SERIALIZATION_VERSION) const;
      void deserialize(const std::string& binary_file_name, Trajectory& traj);
      void deserialize(const std::string& binary_file_name, std::vector<Trajectory>& v_trajs);

    protected:

    /** Basics: see file tubex_Tube_base.cpp **/

      void createTube(const std::vector<ibex::Interval>& v_slices_domains, const ibex::Interval& value);
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain, bool concatenate_results) const;

    /** Contractors: see file tubex_Tube_contractors.cpp **/

      void ctcEval_computeIndex(const ibex::Interval& z, const ibex::Interval& t, int& index_lb, int& index_ub);

    /** Class variables **/

      std::vector<Subtube*> m_v_slices;
      std::map<int,ibex::Interval> m_m_output_gates; // map<slice_id,gate_value> ; slice_id=-1 <=> init value of the tube
      std::string m_name = "";

      static int nb_tubes;
      static std::vector<std::string> v_tubes_names;
  };
}

#endif