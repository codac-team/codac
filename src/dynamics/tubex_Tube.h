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

#include "tubex_TubeVector.h"
#include "tubex_Trajectory.h"

namespace tubex
{
  class Tube : public TubeVector // todo: make this private?
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
      const Tube primitive() const;
      using TubeVector::operator=;
      using TubeVector::domain;
      using TubeVector::dim;

      // Slices structure
      using TubeVector::nbSlices;
      using TubeVector::getSlice;
      using TubeVector::getFirstSlice;
      using TubeVector::getLastSlice;
      using TubeVector::getWiderSlice;
      using TubeVector::input2index;
      using TubeVector::sample;
      void sample(double t, const ibex::Interval& gate);

      // Access values
      const ibex::Interval codomain() const;
      using TubeVector::volume;
      const ibex::Interval operator[](int slice_id) const;
      const ibex::Interval operator[](double t) const;
      const ibex::Interval operator[](const ibex::Interval& t) const;
      const ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const ibex::Interval invert(const ibex::Interval& y, const Tube& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const Tube& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      const ibex::Interval interpol(double t, const Tube& derivative) const;
      const ibex::Interval interpol(const ibex::Interval& t, const Tube& derivative) const;
      double maxThickness() const;
      double maxThickness(int& first_id_max_thickness) const;
      double maxGateThickness(double& t) const;

      // Tests
      bool operator==(const Tube& x) const;
      bool operator!=(const Tube& x) const;
      bool isSubset(const Tube& x) const;
      bool isStrictSubset(const Tube& x) const;
      using TubeVector::isEmpty;
      using TubeVector::encloses;

      // Setting values
      void set(const ibex::Interval& y);
      void set(const ibex::Interval& y, int slice_id);
      void set(const ibex::Interval& y, double t);
      void set(const ibex::Interval& y, const ibex::Interval& t);
      using TubeVector::setEmpty;
      using TubeVector::inflate;

      // Bisection
      using TubeVector::bisect;

      // Operators
      using TubeVector::operator+=;
      using TubeVector::operator-=;
      const Tube& operator*=(const Trajectory& x);
      const Tube& operator*=(const Tube& x);
      const Tube& operator*=(const ibex::Interval& x);
      const Tube& operator/=(const Trajectory& x);
      const Tube& operator/=(const Tube& x);
      const Tube& operator/=(const ibex::Interval& x);
      const Tube& operator&=(const Trajectory& x);
      const Tube& operator&=(const Tube& x);
      const Tube& operator&=(const ibex::Interval& x);
      const Tube& operator|=(const Trajectory& x);
      const Tube& operator|=(const Tube& x);
      const Tube& operator|=(const ibex::Interval& x);
      using TubeVector::operator|;
      using TubeVector::operator&;

      // String
      friend std::ostream& operator<<(std::ostream& str, const TubeVector& x);
      const std::string className() const { return "Tube"; };

    /** Integration: **/

      const ibex::Interval integral(double t) const;
      const ibex::Interval integral(const ibex::Interval& t) const;
      const ibex::Interval integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
      const std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t) const;
      const std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t1, const ibex::Interval& t2) const;

    /** Contractors: **/

      using TubeVector::ctcDeriv;
      bool ctcEval(ibex::Interval& t, ibex::Interval& z, Tube& w);

    /** Serialization: **/

      void serialize(const std::string& binary_file_name = "x.tube", int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const Trajectory& traj, int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const std::vector<Trajectory>& v_trajs, int version_number = SERIALIZATION_VERSION) const;

    protected:

      using TubeVector::codomainBox;
  };
}

#endif