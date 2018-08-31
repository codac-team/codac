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
#include <list>
#include <vector>
#include "tubex_Fnc.h"
#include "tubex_Slice.h"
#include "tubex_Trajectory.h"
#include "tubex_serializ_tubes.h"
#include "tubex_arithmetic.h"
#include "tubex_TubeTreeSynthesis.h"

namespace tubex
{
  class Fnc;
  class Tube;
  class Slice;
  class Trajectory;
  
  class Tube : public DynamicalItem
  {
    public:

    /** Base: **/

      // Definition
      Tube(const ibex::Interval& domain, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      Tube(const ibex::Interval& domain, double timestep, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      Tube(const ibex::Interval& domain, double timestep, const tubex::Fnc& f, int f_image_id = 0);
      Tube(const Tube& x);
      Tube(const Tube& x, const ibex::Interval& codomain);
      Tube(const Tube& x, const tubex::Fnc& f, int f_image_id = 0);
      Tube(const Trajectory& traj, double timestep = 0.);
      Tube(const Trajectory& lb, const Trajectory& ub, double timestep = 0.);
      Tube(const std::string& binary_file_name);
      Tube(const std::string& binary_file_name, Trajectory *&traj);
      ~Tube();
      int size() const;
      const Tube primitive() const;
      const Tube& operator=(const Tube& x);
      const ibex::Interval domain() const;

      // Slices structure
      int nb_slices() const;
      Slice* get_slice(int slice_id);
      const Slice* get_slice(int slice_id) const;
      Slice* get_slice(double t);
      const Slice* get_slice(double t) const;
      Slice* get_first_slice();
      const Slice* get_first_slice() const;
      Slice* get_last_slice();
      const Slice* get_last_slice() const;
      Slice* get_wider_slice();
      const Slice* get_wider_slice() const;
      Slice* get_largest_slice();
      const Slice* get_largest_slice() const;
      const ibex::Interval slice_domain(int slice_id) const;
      int input2index(double t) const;
      int index(const Slice* slice) const;
      void sample(double t);
      void sample(double t, const ibex::Interval& gate);
      static bool share_same_slicing(const Tube& x1, const Tube& x2);

      // Accessing values
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
      bool contains(const Trajectory& x) const;

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

      // Assignments operators
      const Tube& operator+=(const ibex::Interval& x);
      const Tube& operator+=(const Trajectory& x);
      const Tube& operator+=(const Tube& x);
      const Tube& operator-=(const ibex::Interval& x);
      const Tube& operator-=(const Trajectory& x);
      const Tube& operator-=(const Tube& x);
      const Tube& operator*=(const ibex::Interval& x);
      const Tube& operator*=(const Trajectory& x);
      const Tube& operator*=(const Tube& x);
      const Tube& operator/=(const ibex::Interval& x);
      const Tube& operator/=(const Trajectory& x);
      const Tube& operator/=(const Tube& x);
      const Tube& operator|=(const ibex::Interval& x);
      const Tube& operator|=(const Trajectory& x);
      const Tube& operator|=(const Tube& x);
      const Tube& operator&=(const ibex::Interval& x);
      const Tube& operator&=(const Trajectory& x);
      const Tube& operator&=(const Tube& x);

      // String
      const std::string class_name() const { return "Tube"; };
      friend std::ostream& operator<<(std::ostream& str, const Tube& x);

      // Static methods
      static const Tube hull(const std::list<Tube>& l_tubes);

    /** Integration: **/

      const ibex::Interval integral(double t) const;
      const ibex::Interval integral(const ibex::Interval& t) const;
      const ibex::Interval integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
      const std::pair<ibex::Interval,ibex::Interval> partial_integral(const ibex::Interval& t) const;
      const std::pair<ibex::Interval,ibex::Interval> partial_integral(const ibex::Interval& t1, const ibex::Interval& t2) const;

    /** Serialization: **/

      void serialize(const std::string& binary_file_name = "x.tube", int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const Trajectory& traj, int version_number = SERIALIZATION_VERSION) const;

    protected:

      Tube();
      const ibex::IntervalVector codomain_box() const;
      void deserialize(const std::string& binary_file_name, Trajectory *&traj);

      /** Class variables **/

        Slice *m_first_slice = NULL;
        TubeTreeSynthesis *m_data_tree = NULL;
        
      friend void deserialize_Tube(std::ifstream& bin_file, Tube *&tube);
      friend void deserialize_TubeVector(std::ifstream& bin_file, TubeVector *&tube);
      friend class TubeVector;
  };
}

#endif