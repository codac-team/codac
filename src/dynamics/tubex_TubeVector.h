/* ============================================================================
 *  tubex-lib - TubeVector class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_TUBEVECTOR_H__
#define __TUBEX_TUBEVECTOR_H__

#include <vector>
#include "tubex_Fnc.h"
#include "tubex_TubeSlice.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_DynamicalItem.h"

#define SERIALIZATION_VERSION 2

// todo:
// - test memory leaks
// - custom propagation fixed point

namespace tubex
{
  class Fnc;
  class TubeSlice;
  class Tube;
  class Trajectory;
  
  class TubeVector : public DynamicalItem
  {
    public:

    /** Base: **/

      // Definition
      TubeVector(const ibex::Interval& domain, int dim = 1);
      TubeVector(const ibex::Interval& domain, const ibex::IntervalVector& codomain);
      TubeVector(const ibex::Interval& domain, double timestep, int dim = 1);
      TubeVector(const ibex::Interval& domain, double timestep, const ibex::IntervalVector& codomain);
      TubeVector(const ibex::Interval& domain, double timestep, const tubex::Fnc& f);
      TubeVector(const TubeVector& x);
      TubeVector(const TubeVector& x, const ibex::IntervalVector& codomain);
      TubeVector(const TrajectoryVector& traj, double timestep = 0.);
      TubeVector(const TrajectoryVector& lb, const TrajectoryVector& ub, double timestep = 0.);
      TubeVector(const std::string& binary_file_name);
      TubeVector(const std::string& binary_file_name, TrajectoryVector& traj);
      TubeVector(const std::string& binary_file_name, std::vector<TrajectoryVector>& v_trajs);
      ~TubeVector();
      const TubeVector primitive() const;
      const TubeVector& operator=(const TubeVector& x);
      const ibex::Interval domain() const;
      int dim() const;

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
      int input2index(double t) const;
      void sample(double t);
      void sample(double t, const ibex::IntervalVector& gate);

      // Access values
      const ibex::IntervalVector codomain() const;
      double volume() const;
      const ibex::IntervalVector operator[](int slice_id) const;
      const ibex::IntervalVector operator[](double t) const;
      const ibex::IntervalVector operator[](const ibex::Interval& t) const;
      const ibex::Interval invert(const ibex::IntervalVector& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::IntervalVector& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const ibex::Interval invert(const ibex::IntervalVector& y, const TubeVector& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::IntervalVector& y, std::vector<ibex::Interval> &v_t, const TubeVector& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      const ibex::IntervalVector interpol(double t, const TubeVector& v) const;
      const ibex::IntervalVector interpol(const ibex::Interval& t, const TubeVector& v) const;
      const ibex::Vector max_thickness() const;
      const ibex::Vector max_thickness(int& first_id_max_thickness) const;
      const ibex::Vector max_gate_thickness(double& t) const;

      // Tests
      bool operator==(const TubeVector& x) const;
      bool operator!=(const TubeVector& x) const;
      bool is_subset(const TubeVector& x) const;
      bool is_strict_subset(const TubeVector& x) const;
      bool is_empty() const;
      bool encloses(const TrajectoryVector& x) const;

      // Setting values
      void set(const ibex::IntervalVector& y);
      void set(const ibex::IntervalVector& y, int slice_id);
      void set(const ibex::IntervalVector& y, double t);
      void set(const ibex::IntervalVector& y, const ibex::Interval& t);
      void set_empty();
      const TubeVector& inflate(double rad);

      // Bisection
      std::pair<TubeVector,TubeVector> bisect(double t, float ratio = 0.55) const;

      // Operators
      const TubeVector& operator+=(const TrajectoryVector& x);
      const TubeVector& operator+=(const TubeVector& x);
      const TubeVector& operator+=(const ibex::IntervalVector& x);
      const TubeVector& operator-=(const TrajectoryVector& x);
      const TubeVector& operator-=(const TubeVector& x);
      const TubeVector& operator-=(const ibex::IntervalVector& x);
      const TubeVector& operator|=(const TrajectoryVector& x);
      const TubeVector& operator|=(const TubeVector& x);
      const TubeVector& operator|=(const ibex::IntervalVector& x);
      const TubeVector& operator&=(const TrajectoryVector& x);
      const TubeVector& operator&=(const TubeVector& x);
      const TubeVector& operator&=(const ibex::IntervalVector& x);
      const TubeVector& operator|(const TrajectoryVector& x);
      const TubeVector& operator|(const TubeVector& x);
      const TubeVector& operator|(const ibex::IntervalVector& x);
      const TubeVector& operator&(const TrajectoryVector& x);
      const TubeVector& operator&(const TubeVector& x);
      const TubeVector& operator&(const ibex::IntervalVector& x);

      const TubeVector& operator+=(const Trajectory& x);
      const TubeVector& operator+=(const Tube& x);
      const TubeVector& operator+=(const ibex::Interval& x);
      const TubeVector& operator-=(const Trajectory& x);
      const TubeVector& operator-=(const Tube& x);
      const TubeVector& operator-=(const ibex::Interval& x);
      const TubeVector& operator*=(const Trajectory& x);
      const TubeVector& operator*=(const Tube& x);
      const TubeVector& operator*=(const ibex::Interval& x);
      const TubeVector& operator/=(const Trajectory& x);
      const TubeVector& operator/=(const Tube& x);
      const TubeVector& operator/=(const ibex::Interval& x);
      const TubeVector& operator|=(const Trajectory& x);
      const TubeVector& operator|=(const Tube& x);
      const TubeVector& operator|=(const ibex::Interval& x);
      const TubeVector& operator&=(const Trajectory& x);
      const TubeVector& operator&=(const Tube& x);
      const TubeVector& operator&=(const ibex::Interval& x);
      const TubeVector& operator|(const Trajectory& x);
      const TubeVector& operator|(const Tube& x);
      const TubeVector& operator|(const ibex::Interval& x);
      const TubeVector& operator&(const Trajectory& x);
      const TubeVector& operator&(const Tube& x);
      const TubeVector& operator&(const ibex::Interval& x);

      // String
      friend std::ostream& operator<<(std::ostream& str, const TubeVector& x);
      const std::string class_name() const { return "TubeVector"; };

    /** Integration: **/

      const ibex::IntervalVector integral(double t) const;
      const ibex::IntervalVector integral(const ibex::Interval& t) const;
      const ibex::IntervalVector integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> partial_integral(const ibex::Interval& t) const;
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> partial_integral(const ibex::Interval& t1, const ibex::Interval& t2) const;

    /** Contractors: **/

      bool ctc_deriv(const TubeVector& v);
      bool ctc_eval(ibex::Interval& t, ibex::IntervalVector& z, TubeVector& w);

    /** Serialization: **/

      void serialize(const std::string& binary_file_name = "x.tube", int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const TrajectoryVector& traj, int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const std::vector<TrajectoryVector>& v_trajs, int version_number = SERIALIZATION_VERSION) const;

    protected:

      const ibex::IntervalVector codomain_box() const;
      void deserialize(const std::string& binary_file_name, std::vector<TrajectoryVector>& v_trajs);

    /** Class variables **/

      std::vector<TubeSlice*> m_v_slices;

      friend void deserialize_tubevector(std::ifstream& bin_file, TubeVector& tube);
  };
}

#endif