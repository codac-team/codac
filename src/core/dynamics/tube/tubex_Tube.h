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

#ifndef __TUBEX_TUBE_H__
#define __TUBEX_TUBE_H__

#include <map>
#include <vector>
#include "tubex_Fnc.h"
#include "tubex_TubeSlice.h"
#include "tubex_Trajectory.h"
#include "tubex_AbstractTube.h"
#include "tubex_TubeSerialization.h"

namespace tubex
{
  class Fnc;
  class TubeSlice;
  class Tube;
  class Trajectory;
  
  class Tube : public AbstractTube
  {
    public:

    /** Base: **/

      // Definition
      Tube(const ibex::Interval& domain, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      Tube(const ibex::Interval& domain, double timestep, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      Tube(const ibex::Interval& domain, double timestep, const tubex::Fnc& f);
      Tube(const Tube& x);
      Tube(const Tube& x, const ibex::Interval& codomain);
      Tube(const Trajectory& traj, double timestep = 0.);
      Tube(const Trajectory& lb, const Trajectory& ub, double timestep = 0.);
      Tube(const std::string& binary_file_name);
      Tube(const std::string& binary_file_name, Trajectory& traj);
      Tube(const std::string& binary_file_name, std::vector<Trajectory>& v_trajs);
      ~Tube();
      int dim() const;
      const Tube primitive() const;
      const Tube& operator=(const Tube& x);
      const ibex::Interval domain() const;

      // Slices structure
      int nb_slices() const;
      TubeSlice* get_slice(int slice_id);
      const TubeSlice* get_slice(int slice_id) const;
      TubeSlice* get_slice(double t);
      const TubeSlice* get_slice(double t) const;
      TubeSlice* get_first_slice();
      const TubeSlice* get_first_slice() const;
      TubeSlice* get_last_slice();
      const TubeSlice* get_last_slice() const;
      TubeSlice* get_wider_slice();
      const TubeSlice* get_wider_slice() const;
      TubeSlice* get_largest_slice();
      const TubeSlice* get_largest_slice() const;
      int input2index(double t) const;
      int index(const TubeSlice* slice) const;
      void sample(double t);
      void sample(double t, const ibex::Interval& gate);

      // Access values
      const ibex::Interval codomain() const;
      double volume() const;
      const ibex::Interval operator()(int slice_id) const;
      const ibex::Interval operator()(double t) const;
      const ibex::Interval operator()(const ibex::Interval& t) const;
      const ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const ibex::Interval invert(const ibex::Interval& y, const Tube& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const Tube& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      const ibex::Interval interpol(double t, const Tube& v) const;
      const ibex::Interval interpol(const ibex::Interval& t, const Tube& v) const;
      double max_thickness() const;
      double max_gate_thickness(double& t) const;

      // Tests
      bool operator==(const Tube& x) const;
      bool operator!=(const Tube& x) const;
      bool is_subset(const Tube& x) const;
      bool is_strict_subset(const Tube& x) const;
      bool is_empty() const;
      bool encloses(const Trajectory& x) const;

      // Setting values
      void set(const ibex::Interval& y);
      void set(const ibex::Interval& y, int slice_id);
      void set(const ibex::Interval& y, double t);
      void set(const ibex::Interval& y, const ibex::Interval& t);
      void set_empty();
      const Tube& inflate(double rad);
      const Tube& inflate(const Trajectory& rad);

      // Bisection
      const std::pair<Tube,Tube> bisect(double t, float ratio = 0.55) const;

      // Operators

        // Assignments

        const Tube& operator+=(const Tube& x);
        const Tube& operator+=(const Trajectory& x);
        const Tube& operator+=(const ibex::Interval& x);

        const Tube& operator-=(const Tube& x);
        const Tube& operator-=(const Trajectory& x);
        const Tube& operator-=(const ibex::Interval& x);

        const Tube& operator*=(const Tube& x);
        const Tube& operator*=(const Trajectory& x);
        const Tube& operator*=(const ibex::Interval& x);

        const Tube& operator/=(const Tube& x);
        const Tube& operator/=(const Trajectory& x);
        const Tube& operator/=(const ibex::Interval& x);

        const Tube& operator|=(const Tube& x);
        const Tube& operator|=(const Trajectory& x);
        const Tube& operator|=(const ibex::Interval& x);

        const Tube& operator&=(const Tube& x);
        const Tube& operator&=(const Trajectory& x);
        const Tube& operator&=(const ibex::Interval& x);

      // Other mathematical operators

        const Tube& operator|(const Tube& x);
        const Tube& operator|(const Trajectory& x);
        const Tube& operator|(const ibex::Interval& x);

        const Tube& operator&(const Tube& x);
        const Tube& operator&(const Trajectory& x);
        const Tube& operator&(const ibex::Interval& x);

      // String
      friend std::ostream& operator<<(std::ostream& str, const Tube& x);
      const std::string class_name() const { return "Tube"; };

    /** Integration: **/

      const ibex::Interval integral(double t) const;
      const ibex::Interval integral(const ibex::Interval& t) const;
      const ibex::Interval integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
      const std::pair<ibex::Interval,ibex::Interval> partial_integral(const ibex::Interval& t) const;
      const std::pair<ibex::Interval,ibex::Interval> partial_integral(const ibex::Interval& t1, const ibex::Interval& t2) const;

    /** Contractors: **/

      bool ctc_deriv(const Tube& v);
      bool ctc_eval(ibex::Interval& t, ibex::Interval& z, Tube& w);

    /** Serialization: **/

      void serialize(const std::string& binary_file_name = "x.tube", int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const Trajectory& traj, int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const std::vector<Trajectory>& v_trajs, int version_number = SERIALIZATION_VERSION) const;

    protected:

      Tube();
      const ibex::IntervalVector codomain_box() const;
      void deserialize(const std::string& binary_file_name, std::vector<Trajectory>& v_trajs);

    /** Class variables **/

      TubeSlice *m_first_slice = NULL;
      friend void deserialize_Tube(std::ifstream& bin_file, Tube& tube);
  };
}

#endif