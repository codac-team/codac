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

#include <map>
#include <list>
#include <vector>
#include "tubex_Fnc.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_arithmetic.h"
#include "tubex_serializ_tubes.h"
#include "ibex_BoolInterval.h"

namespace tubex
{
  /**
   * \class TubeVector
   * \brief n-dimensional tube, defined as an interval of n-dimensional trajectories
   *
   * \note Use Tube for the one-dimensional case
   */

  class Fnc;
  class Tube;
  class Trajectory;
  
  class TubeVector : public DynamicalItem
  {
    public:

    // Base:

      // Definition

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ made of one slice
       *
       * \param domain Interval domain \f$[t_0,t_f]\f$
       * \param n dimension of this tube (default value: 1)
       */
      TubeVector(const ibex::Interval& domain, int n = 1);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ made of one slice
       *
       * \note The dimension of the tube is specified by the codomain box
       *
       * \param domain Interval domain \f$[t_0,t_f]\f$
       * \param codomain IntervalVector value of the slice
       */
      TubeVector(const ibex::Interval& domain, const ibex::IntervalVector& codomain);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ with some temporal discretization
       *
       * \param domain Interval domain \f$[t_0,t_f]\f$
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \param n dimension of this tube (default value: 1)
       */
      TubeVector(const ibex::Interval& domain, double timestep, int n = 1);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ with some temporal discretization
       *
       * \param domain Interval domain \f$[t_0,t_f]\f$
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \param codomain IntervalVector value of the slices
       */
      TubeVector(const ibex::Interval& domain, double timestep, const ibex::IntervalVector& codomain);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$
       *        from a tubex::Fnc object and with some temporal discretization
       *
       * \Note Due to the slicing implementation of the tube, a wrapping
       *       effect will occur to reliably enclose the tubex::Fnc object
       * \note The dimension of the tube is specified by the output of \f$[\mathbf{f}]\f$
       *
       * \param domain Interval domain \f$[t_0,t_f]\f$
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \param f tubex::Fnc object that will be enclosed by the tube:
       *          \f$\forall t\in[t_0,t_f], [\mathbf{f}](t)\subseteq[\mathbf{x}](t)\f$
       */
      TubeVector(const ibex::Interval& domain, double timestep, const tubex::Fnc& f);

      /**
       * \brief Creates a copy of a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$, with the same time discretization
       *
       * \param x TubeVector to be duplicated
       */
      TubeVector(const TubeVector& x);

      /**
       * \brief Creates a copy of a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$, with the same time
       *        discretization but a specific constant codomain
       *
       * \param x TubeVector from which the sampling will be duplicated
       * \param codomain IntervalVector value of the slices
       */
      TubeVector(const TubeVector& x, const ibex::IntervalVector& codomain);

      /**
       * \brief Creates a n-dimensional tube with all the components initialized to \f$[x](\cdot)\f$
       *
       * \note The TubeVector object will have the same slicing as the tube \f$[x](\cdot)\f$
       *
       * \param n dimension of this tube
       * \param x Tube to be copied for all the components
       */
      TubeVector(int n, const Tube& x);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ enclosing a n-dimensional
       *        trajectory \f$\mathbf{x}(\cdot)\f$, possibly with some temporal discretization
       *
       * \Note Due to the slicing implementation of the tube, a wrapping
       *       effect will occur to reliably enclose the TrajectoryVector object 
       *
       * \param traj TrajectoryVector \f$\mathbf{x}(\cdot)\f$ to enclose
       * \param timestep sampling value \f$\delta\f$ for the temporal
       *        discretization (double, no discretization by default: one slice only)
       */
      TubeVector(const TrajectoryVector& traj, double timestep = 0.);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ defined as an interval
       *        of two n-dimensional trajectories \f$[\mathbf{lb}(\cdot),\mathbf{ub}(\cdot)]\f$
       *
       * \Note Due to the slicing implementation of the tube, a wrapping
       *       effect will occur to reliably enclose the TrajectoryVector object 
       *
       * \param lb TrajectoryVector defining the lower bound \f$\mathbf{x}^{-}(\cdot)\f$ of the tube
       * \param ub TrajectoryVector defining the upper bound \f$\mathbf{x}^{+}(\cdot)\f$ of the tube
       * \param timestep sampling value \f$\delta\f$ for the temporal
       *        discretization (double, no discretization by default: one slice only)
       */
      TubeVector(const TrajectoryVector& lb, const TrajectoryVector& ub, double timestep = 0.);

      /**
       * \brief Restore a n-dimensional tube from serialization
       *
       * \Note The TubeVector must have been serialized beforehand by the appropriate method serialize()
       *
       * \param binary_file_name path to the binary file
       */
      TubeVector(const std::string& binary_file_name);

      /**
       * \brief Restore a n-dimensional tube from serialization, together with a TrajectoryVector object
       *
       * \Note The TubeVector and the TrajectoryVector must have been serialized
       *       beforehand by the appropriate method serialize()
       *
       * \param binary_file_name path to the binary file
       * \param traj a pointer to the TrajectoryVector object to be instantiated
       */
      TubeVector(const std::string& binary_file_name, TrajectoryVector *&traj);

      /**
       * \brief TubeVector destructor
       */
      ~TubeVector();

      /**
       * \brief Returns the dimension of the tube
       *
       * \return n
       */
      int size() const;

      /**
       * \brief Resizes this TubeVector
       *
       * \note If the size is increased, the existing components are not
       *       modified and the new ones are set to \f$[t_0,t_f]\mapsto[-\infty,\infty]\f$
       *
       * \param n the new size to be set
       */
      void resize(int n);

      /**
       * \brief Returns a subvector of this TubeVector
       *
       * \param start_index first component index of the subvector to be returned
       * \param end_index last component index of the subvector to be returned
       * \return a TubeVector extracted from this TubeVector
       */
      const TubeVector subvector(int start_index, int end_index) const;

      /**
       * \brief Puts a subvector into this TubeVector at a given position
       *
       * \param start_index position where the subvector will be put
       * \param subvec the TubeVector to put from start_index
       */
      void put(int start_index, const TubeVector& subvec);

      /**
       * \brief Returns the primitive TubeVector of this tube
       *
       * \return a new TubeVector object with same slicing, enclosing the feasible primitives of this tube
       */
      const TubeVector primitive() const;

      /**
       * \brief Returns a copy of a TubeVector
       *
       * \param x the TubeVector object to be copied
       * \return a new TubeVector object with same slicing and values
       */
      const TubeVector& operator=(const TubeVector& x);

      /**
       * \brief Returns the temporal definition domain of this tube
       *
       * \return an Interval object \f$[t_0,t_f]\f$
       */
      const ibex::Interval domain() const;

      // Slices structure
      int nb_slices() const;
      int input2index(double t) const;
      void sample(double t);
      void sample(double t, const ibex::IntervalVector& gate);
      static bool same_slicing(const TubeVector& x1, const Tube& x2);
      static bool same_slicing(const TubeVector& x1, const TubeVector& x2);

      // Accessing values
      const ibex::IntervalVector codomain() const;
      double volume() const;
      Tube& operator[](int index);
      const Tube& operator[](int index) const;
      const ibex::IntervalVector operator()(int slice_id) const;
      const ibex::IntervalVector operator()(double t) const;
      const ibex::IntervalVector operator()(const ibex::Interval& t) const;
      const ibex::Interval invert(const ibex::IntervalVector& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::IntervalVector& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const ibex::Interval invert(const ibex::IntervalVector& y, const TubeVector& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::IntervalVector& y, std::vector<ibex::Interval> &v_t, const TubeVector& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      const ibex::IntervalVector interpol(double t, const TubeVector& v) const;
      const ibex::IntervalVector interpol(const ibex::Interval& t, const TubeVector& v) const;
      const ibex::Vector max_thickness() const;
      const ibex::Vector max_gate_thickness() const;

      // Tests
      bool operator==(const TubeVector& x) const;
      bool operator!=(const TubeVector& x) const;
      bool is_subset(const TubeVector& x) const;
      bool is_strict_subset(const TubeVector& x) const;
      bool is_interior_subset(const TubeVector& x) const;
      bool is_strict_interior_subset(const TubeVector& x) const;
      bool is_superset(const TubeVector& x) const;
      bool is_strict_superset(const TubeVector& x) const;
      bool is_empty() const;
      const ibex::BoolInterval contains(const TrajectoryVector& x) const;
      bool overlaps(const TubeVector& x, float ratio = 1.) const;

      // Setting values
      void set(const ibex::IntervalVector& y);
      void set(const ibex::IntervalVector& y, int slice_id);
      void set(const ibex::IntervalVector& y, double t);
      void set(const ibex::IntervalVector& y, const ibex::Interval& t);
      void set_empty();
      const TubeVector& inflate(const ibex::Vector& rad);
      const TubeVector& inflate(const TrajectoryVector& rad);

      // Bisection
      const std::pair<TubeVector,TubeVector> bisect(double t, float ratio = 0.55) const;

      // Assignments operators
      const TubeVector& operator+=(const TubeVector& x);
      const TubeVector& operator+=(const TrajectoryVector& x);
      const TubeVector& operator+=(const ibex::IntervalVector& x);
      const TubeVector& operator-=(const TubeVector& x);
      const TubeVector& operator-=(const TrajectoryVector& x);
      const TubeVector& operator-=(const ibex::IntervalVector& x);
      const TubeVector& operator|=(const TubeVector& x);
      const TubeVector& operator|=(const TrajectoryVector& x);
      const TubeVector& operator|=(const ibex::IntervalVector& x);
      const TubeVector& operator&=(const TubeVector& x);
      const TubeVector& operator&=(const TrajectoryVector& x);
      const TubeVector& operator&=(const ibex::IntervalVector& x);
      const TubeVector& operator*=(const Tube& x);
      const TubeVector& operator*=(const Trajectory& x);
      const TubeVector& operator*=(const ibex::Interval& x);

      // String
      const std::string class_name() const { return "TubeVector"; };
      friend std::ostream& operator<<(std::ostream& str, const TubeVector& x);

      // Tree synthesis structure
      void enable_synthesis(bool enable = true) const;

      // Static methods
      static const TubeVector hull(const std::list<TubeVector>& l_tubes);

    // Integration:

      const ibex::IntervalVector integral(double t) const;
      const ibex::IntervalVector integral(const ibex::Interval& t) const;
      const ibex::IntervalVector integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> partial_integral(const ibex::Interval& t) const;
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> partial_integral(const ibex::Interval& t1, const ibex::Interval& t2) const;

    // Serialization:

      void serialize(const std::string& binary_file_name = "x.tube", int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const TrajectoryVector& traj, int version_number = SERIALIZATION_VERSION) const;

    protected:

      TubeVector();
      const ibex::IntervalVector codomain_box() const;
      void deserialize(const std::string& binary_file_name, TrajectoryVector *&traj);

      // Class variables:

        int m_n = 0;
        Tube *m_v_tubes = NULL;

      friend void deserialize_TubeVector(std::ifstream& bin_file, TubeVector *&tube);
  };
}

#endif