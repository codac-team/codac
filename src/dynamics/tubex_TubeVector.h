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

#ifndef TubeVector_HEADER
#define TubeVector_HEADER

#include <vector>
#include "tubex_Fnc.h"
#include "tubex_TubeSlice.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_DynamicalItem.h"

#define SERIALIZATION_VERSION 2

// todo:
// - test memory leaks
// - integral computations with gates
// - custom propagation fixed point

namespace tubex
{
  class Fnc;
  class TubeSlice;
  
  class TubeVector : public DynamicalItem
  {
    public:

    /** Base: **/

      // Definition
      TubeVector(const ibex::Interval& domain, int dim = 1);
      TubeVector(const ibex::Interval& domain, const ibex::IntervalVector& codomain);
      TubeVector(const ibex::Interval& domain, double timestep, int dim = 1);
      TubeVector(const ibex::Interval& domain, double timestep, const ibex::IntervalVector& codomain);
      TubeVector(const ibex::Interval& domain, double timestep, const Fnc& f);
      TubeVector(const TubeVector& x);
      TubeVector(const TubeVector& x, const ibex::IntervalVector& codomain);
      TubeVector(const TubeVector& x, const Fnc& f);
      TubeVector(const TrajectoryVector& traj, double timestep = 0.);
      TubeVector(const TrajectoryVector& lb, const TrajectoryVector& ub, double timestep = 0.);
      TubeVector(const std::string& binary_file_name);
      TubeVector(const std::string& binary_file_name, TrajectoryVector& traj);
      TubeVector(const std::string& binary_file_name, std::vector<TrajectoryVector>& v_trajs);
      ~TubeVector();
      TubeVector primitive() const;
      TubeVector& operator=(const TubeVector& x);
      const ibex::Interval domain() const;
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
      const ibex::Interval invert(const ibex::IntervalVector& y, const TubeVector& derivative, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::IntervalVector& y, std::vector<ibex::Interval> &v_t, const TubeVector& derivative, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      const ibex::IntervalVector interpol(double t, const TubeVector& derivative) const;
      const ibex::IntervalVector interpol(const ibex::Interval& t, const TubeVector& derivative) const;
      double maxThickness() const;
      double maxThickness(int& first_id_max_thickness) const;
      double maxGateThickness(double& t) const;

      // Tests
      bool operator==(const TubeVector& x) const;
      bool operator!=(const TubeVector& x) const;
      bool isSubset(const TubeVector& x) const;
      bool isStrictSubset(const TubeVector& x) const;
      bool isEmpty() const;
      bool encloses(const TrajectoryVector& x) const;

      // Setting values
      void set(const ibex::IntervalVector& y);
      void set(const ibex::IntervalVector& y, int slice_id);
      void set(const ibex::IntervalVector& y, double t);
      void set(const ibex::IntervalVector& y, const ibex::Interval& t);
      void setEmpty();
      TubeVector& inflate(double rad);

      // Bisection
      std::pair<TubeVector,TubeVector> bisect(double t, float ratio = 0.55) const;

      // Operators
      TubeVector& operator+=(const TrajectoryVector& x);
      TubeVector& operator+=(const TubeVector& x);
      TubeVector& operator+=(const ibex::IntervalVector& x);
      TubeVector& operator-=(const TrajectoryVector& x);
      TubeVector& operator-=(const TubeVector& x);
      TubeVector& operator-=(const ibex::IntervalVector& x);
      TubeVector& operator|=(const TrajectoryVector& x);
      TubeVector& operator|=(const TubeVector& x);
      TubeVector& operator|=(const ibex::IntervalVector& x);
      TubeVector& operator&=(const TrajectoryVector& x);
      TubeVector& operator&=(const TubeVector& x);
      TubeVector& operator&=(const ibex::IntervalVector& x);

      // String
      friend std::ostream& operator<<(std::ostream& str, const TubeVector& x);
      const std::string className() const { return "TubeVector"; };

    /** Integration: **/

      ibex::IntervalVector integral(double t) const;
      ibex::IntervalVector integral(const ibex::Interval& t) const;
      ibex::IntervalVector integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
      std::pair<ibex::IntervalVector,ibex::IntervalVector> partialIntegral(const ibex::Interval& t) const;
      std::pair<ibex::IntervalVector,ibex::IntervalVector> partialIntegral(const ibex::Interval& t1, const ibex::Interval& t2) const;

    /** Contractors: **/

      bool ctcDeriv(const TubeVector& v);
      bool ctcEval(ibex::Interval& t, ibex::IntervalVector& z, TubeVector& w);

    /** Serialization: **/

      void serialize(const std::string& binary_file_name = "x.tube", int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const TrajectoryVector& traj, int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const std::vector<TrajectoryVector>& v_trajs, int version_number = SERIALIZATION_VERSION) const;

    protected:

    /** Base: **/

      // Access values
      const ibex::IntervalVector codomainBox() const;

    /** Integration: **/
      
      void checkPartialPrimitive() const;

    /** Serialization: **/

      void deserialize(const std::string& binary_file_name, std::vector<TrajectoryVector>& v_trajs);

    /** Class variables **/

      std::vector<TubeSlice*> m_v_slices;

      friend class CtcDeriv; // todo: remove this
      friend class CtcEval; // todo: remove this? not sure
      friend class TubeSlice; // todo: remove this? not sure
      friend void serializeTubeVector(std::ofstream& bin_file, const TubeVector& tube, int version_number); // todo: remove this? not sure
      friend void deserializeTubeVector(std::ifstream& bin_file, TubeVector& tube); // todo: remove this? not sure
  };
}

#endif