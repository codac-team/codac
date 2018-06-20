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

#include <vector>
#include "ibex.h"
#include "tubex_TubeSlice.h"
#include "tubex_Trajectory.h"
#include "tubex_DynamicalItem.h"

#define SERIALIZATION_VERSION 2

// todo:
// - test memory leaks
// - integral computations with gates
// - custom propagation fixed point

namespace tubex
{
  class TubeSlice;
  
  class Tube : DynamicalItem
  {
    public:

    /** Base: **/

      // Definition
      Tube(const ibex::Interval& domain, int dim = 1);
      Tube(const ibex::Interval& domain, const ibex::IntervalVector& codomain);
      Tube(const ibex::Interval& domain, double timestep, int dim = 1);
      Tube(const ibex::Interval& domain, double timestep, const ibex::IntervalVector& codomain);
      Tube(const ibex::Interval& domain, double timestep, const ibex::Function& function);
      Tube(const Tube& x);
      Tube(const Tube& x, const ibex::IntervalVector& codomain);
      Tube(const Tube& x, const ibex::Function& function);
      Tube(const Trajectory& traj, double timestep);
      Tube(const Trajectory& lb, const Trajectory& ub, double timestep);
      Tube(const std::string& binary_file_name);
      Tube(const std::string& binary_file_name, Trajectory& traj);
      Tube(const std::string& binary_file_name, std::vector<Trajectory>& v_trajs);
      ~Tube();
      Tube primitive() const;
      Tube& operator=(const Tube& x);
      const ibex::Interval domain() const; // todo: output const Interval& (reference)
      int dim() const;

      // Slices structure
      int nbSlices() const;
      TubeSlice* getSlice(int slice_id);
      const TubeSlice* getSlice(int slice_id) const;
      TubeSlice* getSlice(double t);
      const TubeSlice* getSlice(double t) const;
      TubeSlice* getFirstSlice() const; // todo: check constness of these
      TubeSlice* getLastSlice() const;
      TubeSlice* getWiderSlice() const;
      int input2index(double t) const;
      void sample(double t, const ibex::IntervalVector& gate);

      // Access values
      const ibex::IntervalVector codomain() const; // todo: output const Interval& (reference)
      double volume() const;
      const ibex::IntervalVector operator[](int slice_id) const;
      const ibex::IntervalVector operator[](double t) const;
      const ibex::IntervalVector operator[](const ibex::Interval& t) const;
      ibex::Interval invert(const ibex::IntervalVector& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::IntervalVector& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      const ibex::IntervalVector interpol(double t, const Tube& derivative) const;
      const ibex::IntervalVector interpol(const ibex::Interval& t, const Tube& derivative) const;
      double maxThickness();
      double maxThickness(int& first_id_max_thickness);

      // Tests
      bool operator==(const Tube& x) const;
      bool operator!=(const Tube& x) const;
      bool isSubset(const Tube& x) const;
      bool isStrictSubset(const Tube& x) const;
      bool isEmpty() const;
      bool encloses(const Trajectory& x) const;

      // Setting values
      void set(const ibex::IntervalVector& y);
      void set(const ibex::IntervalVector& y, int slice_id);
      void set(const ibex::IntervalVector& y, double t);
      void set(const ibex::IntervalVector& y, const ibex::Interval& t);
      void set(const ibex::Function& function);
      void setEmpty();
      Tube& inflate(double rad);

      // Bisection
      std::pair<Tube,Tube> bisect(double t, float ratio = 0.55) const;

      // Operators
      Tube& operator+=(const Trajectory& x);
      Tube& operator+=(const Tube& x);
      Tube& operator-=(const Trajectory& x);
      Tube& operator-=(const Tube& x);
      Tube& operator*=(const Trajectory& x);
      Tube& operator*=(const Tube& x);
      Tube& operator/=(const Trajectory& x);
      Tube& operator/=(const Tube& x);
      Tube& operator|=(const Trajectory& x);
      Tube& operator|=(const Tube& x);
      Tube& operator&=(const Trajectory& x);
      Tube& operator&=(const Tube& x);

      // String
      friend std::ostream& operator<<(std::ostream& str, const Tube& x);

    /** Integration: **/

      ibex::IntervalVector integral(double t) const;
      ibex::IntervalVector integral(const ibex::Interval& t) const;
      ibex::IntervalVector integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
      std::pair<ibex::IntervalVector,ibex::IntervalVector> partialIntegral(const ibex::Interval& t) const;
      std::pair<ibex::IntervalVector,ibex::IntervalVector> partialIntegral(const ibex::Interval& t1, const ibex::Interval& t2) const;

    /** Contractors: **/

      bool ctcFwd(const Tube& derivative);
      bool ctcBwd(const Tube& derivative);
      bool ctcFwdBwd(const Tube& derivative);
      bool ctcEval(ibex::Interval& t, ibex::IntervalVector& z, const Tube& derivative, bool propagate = true);

    /** Serialization: **/

      void serialize(const std::string& binary_file_name = "x.tube", int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const Trajectory& traj, int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const std::vector<Trajectory>& v_trajs, int version_number = SERIALIZATION_VERSION) const;

    protected:

    /** Base: **/

    /** Integration: **/
      
      void checkPartialPrimitive() const;

    /** Serialization: **/

      void deserialize(const std::string& binary_file_name, std::vector<Trajectory>& v_trajs);

    /** Class variables **/

      //TubeComponent *m_component = NULL;
      std::vector<TubeSlice*> m_v_slices;

      friend class CtcDeriv; // todo: remove this
      friend class CtcEval; // todo: remove this? not sure
      friend class TubeSlice; // todo: remove this? not sure
      friend void serializeTube(std::ofstream& bin_file, const Tube& tube, int version_number); // todo: remove this? not sure
      friend void deserializeTube(std::ifstream& bin_file, Tube& tube); // todo: remove this? not sure
  };
}

#endif