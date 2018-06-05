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

#include "tubex_TubeComponent.h"
#include "tubex_TubeSlice.h"

#define SERIALIZATION_VERSION 2

namespace tubex
{
  class Tube
  {
    public:

    /** Base: **/

      // Definition
      Tube(const ibex::Interval& domain, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      Tube(const ibex::Interval& domain, double timestep, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      Tube(const ibex::Interval& domain, double timestep, const ibex::Function& function);
      Tube(const Tube& x);
      Tube(const Tube& x, const ibex::Interval& codomain);
      Tube(const Trajectory& traj, double thickness = 0., double timestep = 0.);
      Tube(const Trajectory& lb, const Trajectory& ub, double timestep = 0.);
      Tube(const std::string& binary_file_name);
      Tube(const std::string& binary_file_name, Trajectory& traj);
      Tube(const std::string& binary_file_name, std::vector<Trajectory>& v_trajs);
      ~Tube();
      Tube primitive(const ibex::Interval& initial_value = ibex::Interval(0.)) const;
      Tube& operator=(const Tube& x);
      const ibex::Interval& domain() const;

      // Slices structure
      int nbSlices() const;
      TubeSlice* getSlice(int slice_id);
      const TubeSlice* getSlice(int slice_id) const;
      TubeSlice* getSlice(double t);
      const TubeSlice* getSlice(double t) const;
      TubeSlice* getFirstSlice() const; // todo: check constness of these
      TubeSlice* getLastSlice() const;
      void getSlices(std::vector<const TubeSlice*>& v_slices) const;
      int input2index(double t) const;
      double index2input(int slice_id) const;
      void sample(double t, const ibex::Interval& gate = ibex::Interval::ALL_REALS);
      void sample(const std::vector<double>& v_bounds);

      // Access values
      const ibex::Interval& codomain() const;
      double volume() const;
      const ibex::Interval operator[](int slice_id) const;
      const ibex::Interval operator[](double t) const;
      const ibex::Interval operator[](const ibex::Interval& t) const;
      ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      const ibex::Interval interpol(double t, const Tube& derivative) const;
      const ibex::Interval interpol(const ibex::Interval& t, const Tube& derivative) const;
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
      void set(const ibex::Interval& y);
      void set(const ibex::Interval& y, int slice_id);
      void set(const ibex::Interval& y, double t);
      void set(const ibex::Interval& y, const ibex::Interval& t);
      void setEmpty();
      Tube& inflate(double rad);

      // Bisection
      //std::pair<Tube,Tube> bisect(const Tube& derivative, float ratio = 0.55) const;
      //std::pair<Tube,Tube> bisect(double t, const Tube& derivative, float ratio = 0.55) const;

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

      ibex::Interval integral(double t) const;
      ibex::Interval integral(const ibex::Interval& t) const;
      ibex::Interval integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
      std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t) const;
      std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t1, const ibex::Interval& t2) const;

    /** Contractors: **/

      bool ctcFwd(const Tube& derivative);
      bool ctcBwd(const Tube& derivative);
      bool ctcFwdBwd(const Tube& derivative);
      bool ctcEval(ibex::Interval& t, ibex::Interval& z, const Tube& derivative, bool propagate = true);
      /*bool ctcOut(const ibex::Interval& t, const ibex::Interval& z);
      bool ctcIntertemporal(ibex::Interval& t1, ibex::Interval& t2) const;
      bool ctcIntertemporal(ibex::Interval& z, ibex::Interval& t1, ibex::Interval& t2) const;
      bool ctcPeriodic(const ibex::Interval& period);

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

      TubeComponent *m_component = NULL;

      friend class CtcDeriv; // todo: remove this
      friend class CtcEval; // todo: remove this? not sure
      friend class TubeSlice; // todo: remove this? not sure
      friend class TubeComponent; // todo: remove this? not sure
      friend void serializeTube(std::ofstream& bin_file, const Tube& tube, int version_number); // todo: remove this? not sure
      friend void deserializeTube(std::ifstream& bin_file, Tube& tube); // todo: remove this? not sure
  };
}

#endif