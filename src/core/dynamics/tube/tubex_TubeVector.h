/** 
 *  \file
 *  TubeVector class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_TUBEVECTOR_H__
#define __TUBEX_TUBEVECTOR_H__

#include <map>
#include <list>
#include <vector>
#include <initializer_list>
#include "tubex_TFnc.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_tube_arithmetic.h"
#include "tubex_serialize_tubes.h"
#include "ibex_BoolInterval.h"

namespace tubex
{
  class TFnc;
  class Tube;
  class Trajectory;
  
  /**
   * \class TubeVector
   * \brief n-dimensional tube \f$[\mathbf{x}](\cdot)\f$, defined as an interval of n-dimensional trajectories
   *
   * \note Use Tube for the one-dimensional case
   */
  class TubeVector : public DynamicalItem
  {
    public:

      /// \name Definition
      /// @{

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ made of one slice
       *
       * \param tdomain temporal domain \f$[t_0,t_f]\f$
       * \param n dimension of this tube
       */
      explicit TubeVector(const ibex::Interval& tdomain, int n);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ made of one slice
       *
       * \note The dimension of the tube is specified by the codomain box
       *
       * \param tdomain temporal domain \f$[t_0,t_f]\f$
       * \param codomain IntervalVector value of the slice
       */
      explicit TubeVector(const ibex::Interval& tdomain, const ibex::IntervalVector& codomain);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ with some temporal discretization
       *
       * \param tdomain temporal domain \f$[t_0,t_f]\f$
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \param n dimension of this tube
       */
      explicit TubeVector(const ibex::Interval& tdomain, double timestep, int n);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ with some temporal discretization
       *
       * \param tdomain temporal domain \f$[t_0,t_f]\f$
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \param codomain IntervalVector value of the slices
       */
      explicit TubeVector(const ibex::Interval& tdomain, double timestep, const ibex::IntervalVector& codomain);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$
       *        from a TFnc object and with some temporal discretization
       *
       * \note Due to the slicing implementation of the tube, a wrapping
       *       effect will occur to reliably enclose the TFnc object
       * \note The dimension of the tube is specified by the output of \f$[\mathbf{f}]\f$
       *
       * \param tdomain temporal domain \f$[t_0,t_f]\f$
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \param f TFnc object that will be enclosed by the tube:
       *          \f$\forall t\in[t_0,t_f], [\mathbf{f}](t)\subseteq[\mathbf{x}](t)\f$
       */
      explicit TubeVector(const ibex::Interval& tdomain, double timestep, const TFnc& f);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ from a list of \f$k\f$ boxes \f$\big([t_1]\times[\mathbf{x}_1],\dots,[t_k]\times[\mathbf{x}_k]\big)\f$
       *
       * \note The slicing will be based on the vector of temporal domains.
       * \note The \f$[t_i]\f$'s must cover continuously the tdomain of \f$[\mathbf{x}](\cdot)\f$.
       *
       * \param v_tdomains vector of temporal domains \f$[t_i]\f$
       * \param v_codomains vector of codomains \f$[\mathbf{x}_i]\f$ related to the \f$[t_i]\f$'s
       */
      explicit TubeVector(const std::vector<ibex::Interval>& v_tdomains, const std::vector<ibex::IntervalVector>& v_codomains);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ from a list of Tube objects
       *
       * \param list list of \f$[x_i](\cdot)\f$ tubes
       */
      TubeVector(std::initializer_list<Tube> list);

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
      explicit TubeVector(const TubeVector& x, const ibex::IntervalVector& codomain);

      /**
       * \brief Creates a n-dimensional tube with all the components initialized to \f$[x](\cdot)\f$
       *
       * \note The TubeVector object will have the same slicing as the tube \f$[x](\cdot)\f$
       *
       * \param n dimension of this tube
       * \param x Tube to be copied for all the components
       */
      explicit TubeVector(int n, const Tube& x);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ enclosing a n-dimensional
       *        trajectory \f$\mathbf{x}(\cdot)\f$, possibly with some temporal discretization
       *
       * \note Due to the slicing implementation of the tube, a wrapping
       *       effect will occur to reliably enclose the TrajectoryVector object 
       *
       * \param traj TrajectoryVector \f$\mathbf{x}(\cdot)\f$ to enclose
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization
       */
      explicit TubeVector(const TrajectoryVector& traj, double timestep);

      /**
       * \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ defined as an interval
       *        of two n-dimensional trajectories \f$[\mathbf{lb}(\cdot),\mathbf{ub}(\cdot)]\f$
       *
       * \note Due to the slicing implementation of the tube, a wrapping
       *       effect will occur to reliably enclose the TrajectoryVector object 
       *
       * \param lb TrajectoryVector defining the lower bound \f$\mathbf{x}^{-}(\cdot)\f$ of the tube
       * \param ub TrajectoryVector defining the upper bound \f$\mathbf{x}^{+}(\cdot)\f$ of the tube
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization
       */
      explicit TubeVector(const TrajectoryVector& lb, const TrajectoryVector& ub, double timestep);

      /**
       * \brief Restore a n-dimensional tube from serialization
       *
       * \note The TubeVector must have been serialized beforehand by the appropriate method serialize()
       *
       * \param binary_file_name path to the binary file
       */
      explicit TubeVector(const std::string& binary_file_name);

      /**
       * \brief Restore a n-dimensional tube from serialization, together with a TrajectoryVector object
       *
       * \note The TubeVector and the TrajectoryVector must have been serialized
       *       beforehand by the appropriate method serialize()
       *
       * \param binary_file_name path to the binary file
       * \param traj a pointer to the TrajectoryVector object to be instantiated
       */
      explicit TubeVector(const std::string& binary_file_name, TrajectoryVector *&traj);

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
       * \brief Returns the primitive TubeVector of this tube, with a zero constant of integration
       *
       * \return a new TubeVector object with same slicing, enclosing the feasible primitives of this tube
       */
      const TubeVector primitive() const;

      /**
       * \brief Returns the primitive TubeVector of this tube
       *
       * \param c the constant of integration
       * \return a new TubeVector object with same slicing, enclosing the feasible primitives of this tube
       */
      const TubeVector primitive(const ibex::IntervalVector& c) const;

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
      const ibex::Interval tdomain() const;

      /// @}
      /// \name Slices structure
      /// @{

      /**
       * \brief Returns the number of slices of this tube
       *
       * \return an integer
       */
      int nb_slices() const;

      /**
       * \brief Returns the Slice index related to the temporal key \f$t\f$
       *
       * \param t the temporal key (double, must belong to the TubeVector's tdomain)
       * \return an integer
       */
      int time_to_index(double t) const;

      /**
       * \brief Samples this tube at \f$t\f$
       *
       * \note Without any effect on one component that has two Slice objects
       *       already defined at \f$t\f$ (if the gate \f$[\mathbf{x}](t)\f$ already exists)
       *
       * \param t the temporal key (double, must belong to the TubeVector's tdomain)
       */
      void sample(double t);

      /**
       * \brief Samples this tube at \f$t\f$ with a specific gate value
       *
       * \note Without any effect on one component that has two Slice objects
       *       already defined at \f$t\f$ (if the gate \f$[\mathbf{x}](t)\f$ already exists)
       *
       * \param t the temporal key (double, must belong to the TubeVector's tdomain)
       * \param gate the IntervalVector value of this tube at \f$t\f$
       */
      void sample(double t, const ibex::IntervalVector& gate);

      /**
       * \brief Samples this tube so that each component will share the same
       *        sampling of the scalar \f$[x](\cdot)\f$
       *
       * \note The previous sampling of this tube is preserved
       *
       * \param x the Tube from which the new sampling will come from
       */
      void sample(const Tube& x);

      /**
       * \brief Samples this tube so that its components will share the same
       *        sampling of the components of the n-dimensional \f$[\mathbf{x}](\cdot)\f$
       *
       * \note The previous sampling of this tube is preserved
       *
       * \param x the TubeVector from which the new sampling will come from
       */
      void sample(const TubeVector& x);

      /// @}
      /// \name Accessing values
      /// @{

      /**
       * \brief Returns the box of feasible values
       *
       * \return an IntervalVector object \f$[\mathbf{x}]([t_0,t_f])\f$
       */
      const ibex::IntervalVector codomain() const;

      /**
       * \brief Returns the volume of this tube
       *
       * \note returns POS_INFINITY if the codomain is unbounded
       * \note returns 0 if the tube is flat (and so without wrapping effect)
       *
       * \return volume defined as \f$w([t_0,t_f])\times w([x_1]([t_0,t_f]))\times\dots\times w([x_n]([t_0,t_f]))\f$
       */
      double volume() const;

      /**
       * \brief Returns the ith Tube of this TubeVector
       *
       * \param index the index of this ith component
       * \return a reference to the ith component
       */
      Tube& operator[](int index);

      /**
       * \brief Returns a const reference to the ith Tube of this TubeVector
       *
       * \param index the index of this ith component
       * \return a const reference to the ith component
       */
      const Tube& operator[](int index) const;

      /**
       * \brief Returns the value of the ith slice
       *
       * \param slice_id the index of the ith slice
       * \return IntervalVector value of \f$[\mathbf{x}](i)\f$
       */
      const ibex::IntervalVector operator()(int slice_id) const;

      /**
       * \brief Returns the evaluation of this tube at \f$t\f$
       *
       * \param t the temporal key (double, must belong to the TubeVector's tdomain)
       * \return IntervalVector value of \f$[\mathbf{x}](t)\f$
       */
      const ibex::IntervalVector operator()(double t) const;

      /**
       * \brief Returns the interval evaluation of this tube over \f$[t]\f$
       *
       * \param t the subtdomain (Interval, must be a subset of the TubeVector's tdomain)
       * \return IntervalVector envelope \f$[\mathbf{x}]([t])\f$
       */
      const ibex::IntervalVector operator()(const ibex::Interval& t) const;

      /**
       * \brief Returns the interval evaluations of the bounds of the
       *        tube \f$\underline{\mathbf{x}^-}(\cdot)\f$ and \f$\overline{\mathbf{x}^+}(\cdot)\f$ over \f$[t]\f$
       *
       * \param t the subtdomain (Interval, must be a subset of the TubeVector's tdomain)
       * \return the pair \f$\big([\underline{\mathbf{x}^-}]([t]),[\overline{\mathbf{x}^+}]([t])\big)\f$
       */
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      
      /**
       * \brief Returns the optimal evaluation of this tube at \f$t\f$,
       *        based on the derivative information \f$\dot{\mathbf{x}}(\cdot)\f$
       *
       * \todo Change the name of this method?
       *
       * \param t the temporal key (double, must belong to the TubeVector's tdomain)
       * \param v the derivative tube such that \f$\dot{\mathbf{x}}(\cdot)\in[\mathbf{v}](\cdot)\f$
       * \return IntervalVector value of \f$[\mathbf{x}](t)\f$
       */
      const ibex::IntervalVector interpol(double t, const TubeVector& v) const;

      /**
       * \brief Returns the optimal interval evaluation of this tube over \f$[t]\f$,
       *        based on the derivative information \f$\dot{\mathbf{x}}(\cdot)\f$
       *
       * \todo Change the name of this method?
       *
       * \param t the subtdomain (Interval, must be a subset of the TubeVector's tdomain)
       * \param v the derivative tube such that \f$\dot{\mathbf{x}}(\cdot)\in[\mathbf{v}](\cdot)\f$
       * \return IntervalVector value of \f$[\mathbf{x}]([t])\f$
       */
      const ibex::IntervalVector interpol(const ibex::Interval& t, const TubeVector& v) const;

      /**
       * \brief Returns the interval inversion \f$[\mathbf{x}]^{-1}([\mathbf{y}])\f$
       *
       * \note If the inversion results in several pre-images, their union is returned
       *
       * \param y the box codomain
       * \param search_tdomain the optional temporal domain on which the inversion will be performed
       * \return the hull of \f$[\mathbf{x}]^{-1}([\mathbf{y}])\f$
       */
      const ibex::Interval invert(const ibex::IntervalVector& y, const ibex::Interval& search_tdomain = ibex::Interval::ALL_REALS) const;

      /**
       * \brief Computes the set of continuous values of the inversion \f$[\mathbf{x}]^{-1}([\mathbf{y}])\f$
       *
       * \param y the interval codomain
       * \param v_t the vector of the sub-tdomains \f$[t_k]\f$ for which
       *            \f$\forall t\in[t_k] \mid \mathbf{x}(t)\in[\mathbf{y}], \mathbf{x}(\cdot)\in[\mathbf{x}](\cdot)\f$
       * \param search_tdomain the optional temporal domain on which the inversion will be performed
       */
      void invert(const ibex::IntervalVector& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_tdomain = ibex::Interval::ALL_REALS) const;

      /**
       * \brief Returns the optimal interval inversion \f$[\mathbf{x}]^{-1}([\mathbf{y}])\f$
       *
       * \note The knowledge of the derivative tube \f$[\mathbf{v}](\cdot)\f$ allows a finer inversion
       * \note If the inversion results in several pre-images, their union is returned
       *
       * \param y the interval codomain
       * \param v the derivative tube vector such that \f$\dot{\mathbf{x}}(\cdot)\in[\mathbf{v}](\cdot)\f$
       * \param search_tdomain the optional temporal domain on which the inversion will be performed
       * \return the hull of \f$[\mathbf{x}]^{-1}([\mathbf{y}])\f$
       */
      const ibex::Interval invert(const ibex::IntervalVector& y, const TubeVector& v, const ibex::Interval& search_tdomain = ibex::Interval::ALL_REALS) const;

      /**
       * \brief Computes the set of continuous values of the optimal inversion \f$[\mathbf{x}]^{-1}([\mathbf{y}])\f$
       *
       * \note The knowledge of the derivative tube \f$[\mathbf{v}](\cdot)\f$ allows finer inversions
       *
       * \param y the interval codomain
       * \param v_t the vector of the sub-tdomains \f$[t_k]\f$ for which
       *            \f$\exists t\in[t_k] \mid \mathbf{x}(t)\in[\mathbf{y}], \mathbf{x}(\cdot)\in[\mathbf{x}](\cdot), \dot{\mathbf{x}}(\cdot)\in[\mathbf{v}](\cdot)\f$
       * \param v the derivative tube such that \f$\dot{\mathbf{x}}(\cdot)\in[\mathbf{v}](\cdot)\f$
       * \param search_tdomain the optional temporal domain on which the inversion will be performed
       */
      void invert(const ibex::IntervalVector& y, std::vector<ibex::Interval> &v_t, const TubeVector& v, const ibex::Interval& search_tdomain = ibex::Interval::ALL_REALS) const;

      /**
       * \brief Returns the diameters of the tube as a trajectory
       *
       * \note Without derivative knowledge, and because the tube is made of boxed slices,
       *       the trajectory will be discontinuous and so the returned object will not
       *       reliably represent the diameters. It can be mainly used for display purposes.
       *
       * \param gates_thicknesses if true, the diameters of the gates will be evaluated too
       * \return the set of diameters associated to temporal inputs
       */
      const TrajectoryVector diam(bool gates_thicknesses = false) const;

      /**
       * \brief Returns the diameters of the tube as a trajectory
       *
       * \note Because the tube is made of boxed slices,
       *       the trajectory may be discontinuous and so the returned object will not
       *       reliably represent the diameters. It can be mainly used for display purposes.
       *
       * \param v the derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       * \return the set of diameters associated to temporal inputs
       */
      const TrajectoryVector diam(const TubeVector& v) const;
      
      /**
       * \brief Returns a vector of the maximum diameters of the tube for each component
       *
       * \return the maximal thicknesses of this tube
       */
      const ibex::Vector max_diam() const;

      /**
       * \brief Returns the slices diagonals of the tube as a trajectory
       *
       * \note Without derivative knowledge, and because the tube is made of boxed slices,
       *       the trajectory will be discontinuous and so the returned object will not
       *       reliably represent the diagonals. It can be mainly used for display purposes.
       *
       * \param gates_diag if true, the diagonals of the gates will be evaluated too
       * \return the set of diagonals associated to temporal inputs
       */
      const Trajectory diag(bool gates_diag = false) const;

      /**
       * \brief Returns the slices diagonals of a subvector of this tube as a trajectory
       *
       * \note Without derivative knowledge, and because the tube is made of boxed slices,
       *       the trajectory will be discontinuous and so the returned object will not
       *       reliably represent the diagonals. It can be mainly used for display purposes.
       *
       * \param start_index first component index of the subvector
       * \param end_index last component index of the subvector
       * \param gates_diag if true, the diagonals of the gates will be evaluated too
       * \return the set of diagonals associated to temporal inputs
       */
      const Trajectory diag(int start_index, int end_index, bool gates_diag = false) const;

      /// @}
      /// \name Tests
      /// @{

      /**
       * \brief Returns true if this tube is equal to \f$[\mathbf{x}](\cdot)\f$
       *
       * \note Equality is obtained if the tubes share
       *       the same bounds, tdomain and sampling
       *
       * \param x the TubeVector object
       * \return true in case of equality
       */
      bool operator==(const TubeVector& x) const;

      /**
       * \brief Returns true if this tube is different from \f$[\mathbf{x}](\cdot)\f$
       *
       * \note The two tubes are different if they do not share
       *       the same bounds, tdomain or sampling
       *
       * \param x the TubeVector object
       * \return true in case of difference
       */
      bool operator!=(const TubeVector& x) const;

      /**
       * \brief Returns true if this tube is a subset of \f$[\mathbf{x}](\cdot)\f$
       *
       * \note The two tubes may not share the same slicing,
       *       but must have the same tdomain
       *
       * \param x the TubeVector object
       * \return true in case of subset
       */
      bool is_subset(const TubeVector& x) const;

      /**
       * \brief Returns true if this tube is a subset of \f$[\mathbf{x}](\cdot)\f$,
       *        and not \f$[\mathbf{x}](\cdot)\f$ itself
       *
       * \note The two tubes may not share the same slicing,
       *       but must have the same tdomain
       *
       * \param x the TubeVector object
       * \return true in case of strict subset
       */
      bool is_strict_subset(const TubeVector& x) const;

      /**
       * \brief Returns true if this tube is a subset of the interior of \f$[\mathbf{x}](\cdot)\f$
       *
       * \note The two tubes may not share the same slicing,
       *       but must have the same tdomain
       *
       * \param x the TubeVector object
       * \return true in case of interior subset
       */
      bool is_interior_subset(const TubeVector& x) const;

      /**
       * \brief Returns true if this tube is a subset of the interior
       *        of \f$[\mathbf{x}](\cdot)\f$, and not \f$[\mathbf{x}](\cdot)\f$ itself
       *
       * \note The two tubes may not share the same slicing,
       *       but must have the same tdomain
       *
       * \param x the TubeVector object
       * \return true in case of strict interior subset
       */
      bool is_strict_interior_subset(const TubeVector& x) const;

      /**
       * \brief Returns true if this tube is a superset of \f$[\mathbf{x}](\cdot)\f$
       *
       * \note The two tubes may not share the same slicing,
       *       but must have the same tdomain
       *
       * \param x the TubeVector object
       * \return true in case of superset
       */
      bool is_superset(const TubeVector& x) const;

      /**
       * \brief Returns true if this tube is a superset of \f$[\mathbf{x}](\cdot)\f$,
       *        and not \f$[\mathbf{x}](\cdot)\f$ itself
       *
       * \note The two tubes may not share the same slicing,
       *       but must have the same tdomain
       *
       * \param x the TubeVector object
       * \return true in case of strict superset
       */
      bool is_strict_superset(const TubeVector& x) const;

      /**
       * \brief Returns true if this tube is empty
       *
       * \note If \f$[\mathbf{x}](t)=\varnothing\f$ for some \f$t\f$, then the tube
       *       \f$[\mathbf{x}](\cdot)\f$ is considered empty
       *
       * \return true in case of emptiness
       */
      bool is_empty() const;

      /**
       * \brief Returns true if this tube contains the trajectory \f$\mathbf{x}(\cdot)\f$
       *
       * \note Due to the reliable numerical representation of a trajectory, some wrapping effect
       *       may appear for its evaluations (either if it is defined by a map of values
       *       or an analytic function). Hence, this "contains" test may not be able to
       *       conclude, if the thin envelope of \f$\mathbf{x}(\cdot)\f$ overlaps a boundary of the tube.
       *
       * \param x the trajectory that might be contained by this tube
       * \return BoolInterval::YES (or BoolInterval::NO) if this tube contains \f$\mathbf{x}(\cdot)\f$
       *         (or does not contain) and BoolInterval::MAYBE in case of ambiguity
       */
      const ibex::BoolInterval contains(const TrajectoryVector& x) const;

      /**
       * \brief Returns true if this tube overlaps the tube \f$[\mathbf{x}](\cdot)\f$
       *
       * \param x the other tube
       * \param ratio an optional overlapping ratio between 0 and 1 (1 by default).
       *              For instance, if ratio=0.3 and there is an overlapping of at least 30%,
       *              then the function returns true
       * \return true in case of overlapping with respect to the defined ratio
       */
      bool overlaps(const TubeVector& x, float ratio = 1.) const;

      /// @}
      /// \name Setting values
      /// @{

      /**
       * \brief Sets a constant box value for this tube: \f$\forall t, [\mathbf{x}](t)=[\mathbf{y}]\f$
       *
       * \note The sampling of this tube is preserved
       *
       * \param y IntervalVector value of the slices
       */
      void set(const ibex::IntervalVector& y);

      /**
       * \brief Sets the box value of the ith slice of this tube
       *
       * \param y IntervalVector value of the ith slice
       * \param slice_id index of the ith Slice
       */
      void set(const ibex::IntervalVector& y, int slice_id);

      /**
       * \brief Sets the box value of this tube at \f$t\f$: \f$[\mathbf{x}](t)=[\mathbf{y}]\f$
       *
       * \note It may create a gate (and so one more slice) if the tube
       *       is not already sampled at \f$t\f$. Otherwise, it will
       *       update the value of the already existing gate.
       *
       * \param y IntervalVector value of the gate
       * \param t the temporal key (double, must belong to the TubeVector's tdomain)
       */
      void set(const ibex::IntervalVector& y, double t);

      /**
       * \brief Sets the box value of this tube over \f$[t]\f$: \f$\forall t\in[t], [\mathbf{x}](t)=[\mathbf{y}]\f$
       *
       * \note It may create two gates (and so further slices) if the tube
       *       is not already sampled at \f$t^-\f$ and \f$t^+\f$. This is
       *       done to ensure that \f$\forall t\in[t], [\mathbf{x}](t)=[\mathbf{y}]\f$.
       *
       * \param y IntervalVector value to be set
       * \param t the subtdomain (Interval, must be a subset of the TubeVector's tdomain)
       */
      void set(const ibex::IntervalVector& y, const ibex::Interval& t);

      /**
       * \brief Sets this tube to the empty set
       *
       * \note By convention, all slices will be set to the empty set
       */
      void set_empty();

      /**
       * \brief Inflates this tube by adding \f$[-\mathbf{rad},+\mathbf{rad}]\f$ to all its codomain components
       *
       * \note All slices and gates will be inflated
       *
       * \param rad half of the inflation
       * \return *this
       */
      const TubeVector& inflate(double rad);

      /**
       * \brief Inflates this tube by adding \f$[-\mathbf{rad},+\mathbf{rad}]\f$ to all its codomain components
       *
       * \note All slices and gates will be inflated
       *
       * \param rad half of the inflation for each component
       * \return *this
       */
      const TubeVector& inflate(const ibex::Vector& rad);

      /**
       * \brief Inflates this tube by adding non-constant uncertainties defined in a trajectory 
       *
       * \note From the trajectory \f$\mathbf{a}(\cdot)\f$, the function will 
       *       inflate this tube such that \f$\forall t, [\mathbf{x}](t):=[\mathbf{x}](t)+[-\mathbf{a}(t),+\mathbf{a}(t)]\f$
       * \note All slices and gates will be inflated
       *
       * \param rad the TrajectoryVector object defining the non-constant inflation
       * \return *this
       */
      const TubeVector& inflate(const TrajectoryVector& rad);

      /**
       * \brief Shifts the tdomain \f$[t_0,t_f]\f$ of \f$[\mathbf{x}](\cdot)\f$
       *
       * \param a the offset value so that \f$[t_0,t_f]:=[t_0+a,t_f+a]\f$ 
       */
      void shift_tdomain(double a);

      /// @}
      /// \name Bisection
      /// @{

      /**
       * \brief Bisects this tube
       *
       * \note The tube is bisected along the codomain and according to a defined ratio
       * \note The bisection is performed on the largest component of the gate \f$[\mathbf{x}](t)\f$
       * \note If the tube is not already sampled at \f$t\f$, then a sampling is performed
       *
       * \param t the temporal key (double, must belong to the TubeVector's domain)
       * \param ratio the bisection ratio (default value: 0.49)
       * \return a pair of two TubeVector objects resulting from the bisection
       */
      const std::pair<TubeVector,TubeVector> bisect(double t, float ratio = 0.49) const;

      /**
       * \brief Bisects this tube along a specific dimension
       *
       * \note The tube is bisected along the codomain and according to a defined ratio
       * \note The bisection is performed on the gate \f$[x_i](t)\f$
       * \note If the tube is not already sampled at \f$t\f$, then a sampling is performed
       *
       * \param t the temporal key (double, must belong to the TubeVector's domain)
       * \param dim the dimension id
       * \param ratio the bisection ratio (default value: 0.49)
       * \return a pair of two TubeVector objects resulting from the bisection
       */
      const std::pair<TubeVector,TubeVector> bisect(double t, int dim, float ratio = 0.49) const;

      /// @}
      /// \name Assignments operators
      /// @{

      /**
       * \brief Operates +=
       *
       * \param x Interval
       * \return (*this)+=x
       */
      const TubeVector& operator+=(const ibex::Interval& x);

      /**
       * \brief Operates +=
       *
       * \param x Trajectory
       * \return (*this)+=x
       */
      const TubeVector& operator+=(const Trajectory& x);

      /**
       * \brief Operates +=
       *
       * \param x Tube
       * \return (*this)+=x
       */
      const TubeVector& operator+=(const Tube& x);

      /**
       * \brief Operates +=
       *
       * \param x IntervalVector
       * \return (*this)+=x
       */
      const TubeVector& operator+=(const ibex::IntervalVector& x);

      /**
       * \brief Operates +=
       *
       * \param x TrajectoryVector
       * \return (*this)+=x
       */
      const TubeVector& operator+=(const TrajectoryVector& x);

      /**
       * \brief Operates +=
       *
       * \param x TubeVector
       * \return (*this)+=x
       */
      const TubeVector& operator+=(const TubeVector& x);

      /**
       * \brief Operates -=
       *
       * \param x Interval
       * \return (*this)-=x
       */
      const TubeVector& operator-=(const ibex::Interval& x);

      /**
       * \brief Operates -=
       *
       * \param x Trajectory
       * \return (*this)-=x
       */
      const TubeVector& operator-=(const Trajectory& x);

      /**
       * \brief Operates -=
       *
       * \param x Tube
       * \return (*this)-=x
       */
      const TubeVector& operator-=(const Tube& x);

      /**
       * \brief Operates -=
       *
       * \param x IntervalVector
       * \return (*this)-=x
       */
      const TubeVector& operator-=(const ibex::IntervalVector& x);

      /**
       * \brief Operates -=
       *
       * \param x TrajectoryVector
       * \return (*this)-=x
       */
      const TubeVector& operator-=(const TrajectoryVector& x);

      /**
       * \brief Operates -=
       *
       * \param x TubeVector
       * \return (*this)-=x
       */
      const TubeVector& operator-=(const TubeVector& x);

      /**
       * \brief Operates *=
       *
       * \param x Interval
       * \return (*this)*=x
       */
      const TubeVector& operator*=(const ibex::Interval& x);

      /**
       * \brief Operates *=
       *
       * \param x Trajectory
       * \return (*this)*=x
       */
      const TubeVector& operator*=(const Trajectory& x);

      /**
       * \brief Operates *=
       *
       * \param x Tube
       * \return (*this)*=x
       */
      const TubeVector& operator*=(const Tube& x);

      /**
       * \brief Operates /=
       *
       * \param x Interval
       * \return (*this)/=x
       */
      const TubeVector& operator/=(const ibex::Interval& x);

      /**
       * \brief Operates /=
       *
       * \param x Trajectory
       * \return (*this)/=x
       */
      const TubeVector& operator/=(const Trajectory& x);

      /**
       * \brief Operates /=
       *
       * \param x Tube
       * \return (*this)/=x
       */
      const TubeVector& operator/=(const Tube& x);

      /**
       * \brief Operates |=
       *
       * \param x IntervalVector
       * \return (*this)|=x
       */
      const TubeVector& operator|=(const ibex::IntervalVector& x);

      /**
       * \brief Operates |=
       *
       * \param x TrajectoryVector
       * \return (*this)|=x
       */
      const TubeVector& operator|=(const TrajectoryVector& x);

      /**
       * \brief Operates |=
       *
       * \param x TubeVector
       * \return (*this)|=x
       */
      const TubeVector& operator|=(const TubeVector& x);

      /**
       * \brief Operates &=
       *
       * \param x IntervalVector
       * \return (*this)&=x
       */
      const TubeVector& operator&=(const ibex::IntervalVector& x);

      /**
       * \brief Operates &=
       *
       * \param x TrajectoryVector
       * \return (*this)&=x
       */
      const TubeVector& operator&=(const TrajectoryVector& x);

      /**
       * \brief Operates &=
       *
       * \param x TubeVector
       * \return (*this)&=x
       */
      const TubeVector& operator&=(const TubeVector& x);

      /// @}
      /// \name String
      /// @{

      /**
       * \brief Returns the name of this class
       *
       * \note Only used for some generic display method
       *
       * \return the predefined name
       */
      const std::string class_name() const { return "TubeVector"; };

      /**
       * \brief Displays a synthesis of this tube
       *
       * \param str ostream
       * \param x tube to be displayed
       * \return ostream
       */
      friend std::ostream& operator<<(std::ostream& str, const TubeVector& x);

      /// @}
      /// \name Tree synthesis structure
      /// @{

      /**
       * \brief Enables the computation of a synthesis tree
       *
       * \note The synthesis tree speeds up computations such as integrals or evaluations
       *
       * \param enable boolean
       */
      void enable_synthesis(bool enable = true) const;

      /// @}
      /// \name Integration
      /// @{

      /**
       * \brief Computes the box integral \f$\int_0^t[\mathbf{x}](\tau)d\tau\f$
       *
       * \note From the monotonicity of the integral operator, 
       *       \f$\int_0^t[\mathbf{x}](\tau)d\tau=[\int_0^t\mathbf{x}^-(\tau)d\tau,\int_0^t\mathbf{x}^+(\tau)d\tau]\f$
       *
       * \param t the temporal key (double, must belong to the TubeVector's domain)
       * \return the set of feasible integral vectors
       */
      const ibex::IntervalVector integral(double t) const;

      /**
       * \brief Computes the box integral \f$\int_0^{[t]}[\mathbf{x}](\tau)d\tau\f$
       *
       * \note From the monotonicity of the integral operator, 
       *       \f$\int_0^{[t]}[\mathbf{x}](\tau)d\tau=[\int_0^{[t]}\mathbf{x}^-(\tau)d\tau,\int_0^{[t]}\mathbf{x}^+(\tau)d\tau]\f$
       *
       * \param t the subtdomain (Interval, must be a subset of the TubeVector's tdomain)
       * \return the set of feasible integral vectors
       */
      const ibex::IntervalVector integral(const ibex::Interval& t) const;

      /**
       * \brief Computes the box integral \f$\int_{[t_1]}^{[t_2]}[\mathbf{x}](\tau)d\tau\f$
       *
       * \note From the monotonicity of the integral operator, 
       *       \f$\int_{[t_1]}^{[t_2]}[\mathbf{x}](\tau)d\tau=[\int_{[t_1]}^{[t_2]}\mathbf{x}^-(\tau)d\tau,\int_{[t_1]}^{[t_2]}\mathbf{x}^+(\tau)d\tau]\f$
       *
       * \param t1 lower bound, subset of the TubeVector's tdomain
       * \param t2 upper bound, subset of the TubeVector's tdomain
       * \return the set of feasible integral vectors
       */
      const ibex::IntervalVector integral(const ibex::Interval& t1, const ibex::Interval& t2) const;

      /**
       * \brief Computes the partial box integral \f$\int_{0}^{[t]}[\mathbf{x}](\tau)d\tau\f$
       *
       * \note From the monotonicity of the integral operator, 
       *       \f$\int_{0}^{[t]}[\mathbf{x}](\tau)d\tau=[\int_{0}^{[t]}\mathbf{x}^-(\tau)d\tau,\int_{0}^{[t]}\mathbf{x}^+(\tau)d\tau]\f$
       *
       * \param t interval upper bound, subset of the TubeVector's tdomain
       * \return the pair \f$\big([i^-],[i^+]\big)\f$,
       *         where \f$[i^-]=\int_{0}^{[t]}\mathbf{x}^-(\tau)d\tau\f$ 
       *         and \f$[i^+]=\int_{0}^{[t]}\mathbf{x}^+(\tau)d\tau\f$
       */
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> partial_integral(const ibex::Interval& t) const;

      /**
       * \brief Computes the partial box integral \f$\int_{[t_1]}^{[t_2]}[\mathbf{x}](\tau)d\tau\f$
       *
       * \note From the monotonicity of the integral operator, 
       *       \f$\int_{[t_1]}^{[t_2]}[\mathbf{x}](\tau)d\tau=[\int_{[t_1]}^{[t_2]}\mathbf{x}^-(\tau)d\tau,\int_{[t_1]}^{[t_2]}\mathbf{x}^+(\tau)d\tau]\f$
       *
       * \param t1 interval lower bound, subset of the TubeVector's tdomain
       * \param t2 interval upper bound, subset of the TubeVector's tdomain
       * \return the pair \f$\big([i^-],[i^+]\big)\f$,
       *         where \f$[i^-]=\int_{[t_1]}^{[t_2]}\mathbf{x}^-(\tau)d\tau\f$ 
       *         and \f$[i^+]=\int_{[t_1]}^{[t_2]}\mathbf{x}^+(\tau)d\tau\f$
       */
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> partial_integral(const ibex::Interval& t1, const ibex::Interval& t2) const;

      /// @}
      /// \name Serialization
      /// @{

      /**
       * \brief Serializes this tube
       *
       * \note The values and sampling (slices and gates) are serialized
       *
       * \param binary_file_name name of the output file (default value: "x.tube")
       * \param version_number serialization version (used for tests purposes, default value: last version)
       */
      void serialize(const std::string& binary_file_name = "x.tube", int version_number = SERIALIZATION_VERSION) const;

      /**
       * \brief Serializes this tube together with a TrajectoryVector object
       *
       * \note The values and sampling (slices and gates) are serialized
       * \note The serialization of a TrajectoryVector defined from a Function object is not supported
       * \note The output file will appear in the executable current directory
       *
       * \param binary_file_name name of the output file (default value: "x.tube")
       * \param traj the TrajectoryVector object to serialize (for instance, actual but unknown values)
       * \param version_number serialization version (used for tests purposes, default value: last version)
       */
      void serialize(const std::string& binary_file_name, const TrajectoryVector& traj, int version_number = SERIALIZATION_VERSION) const;

      /// @}

      /**
       * \brief Tests whether a TubeVector object shares the slicing of another Tube object
       *
       * \note If true, it means the two tubes are defined with the same
       *       amount of slices and identical sampling
       *
       * \param x1 the TubeVector
       * \param x2 the Tube
       * \return true in case of same slicing
       */
      static bool same_slicing(const TubeVector& x1, const Tube& x2);

      /**
       * \brief Tests whether the two TubeVector objects are sharing the same slicing
       *
       * \note If true, it means the two tubes are defined with the same
       *       amount of slices and identical sampling
       *
       * \param x1 the first TubeVector
       * \param x2 the second TubeVector
       * \return true in case of same slicing
       */
      static bool same_slicing(const TubeVector& x1, const TubeVector& x2);
      
      /**
       * \brief Computes the hull of several tubes
       *
       * \param l_tubes list of tubes
       * \return the tube vector enveloping the other ones
       */
      static const TubeVector hull(const std::list<TubeVector>& l_tubes);

    protected:

      /**
       * \brief Creates an undefined n-dimensional tube
       *
       * \note Constructor necessary for the deserialize_TubeVector method
       */
      TubeVector();

      /**
       * \brief Returns the box \f$[\mathbf{x}]([t_0,t_f])\f$
       *
       * \note Used for genericity purposes
       *
       * \return the envelope of codomain values
       */
      const ibex::IntervalVector codomain_box() const;

      /**
       * \brief Restores a n-dimensional tube from serialization, together with a TrajectoryVector object
       *
       * \note The TubeVector and the TrajectoryVector must have been serialized
       *       beforehand by the appropriate method serialize()
       *
       * \param binary_file_name path to the binary file
       * \param traj a pointer to the TrajectoryVector object to be instantiated
       */
      void deserialize(const std::string& binary_file_name, TrajectoryVector *&traj);

      // Class variables:

        int m_n = 0; //!< dimension of this tube
        Tube *m_v_tubes = NULL; //!< array of components (scalar tubes)

      friend void deserialize_TubeVector(std::ifstream& bin_file, TubeVector *&tube);
  };
}

#endif