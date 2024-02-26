/** 
 *  \file
 *  Tube class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_TUBE_H__
#define __CODAC_TUBE_H__

#include <map>
#include <list>
#include <vector>
#include "codac_TFnc.h"
#include "codac_Slice.h"
#include "codac_Trajectory.h"
#include "codac_serialize_tubes.h"
#include "codac_tube_arithmetic.h"
#include "codac_TubeTreeSynthesis.h"
#include "codac_TubePolynomialSynthesis.h"
#include "codac_TubeSynthesis.h"
#include "codac_Polygon.h"
#include "codac_BoolInterval.h"

// The following define constant is now set in CMake configuration file:
//#define AUTO_SYNTHESIS_BY_DEFAULT false

namespace codac
{
  class TFnc;
  class Tube;
  class Slice;
  class Trajectory;
  class TubeTreeSynthesis;
  class TubePolynomialSynthesis;

  /**
   * \class Tube
   * \brief One dimensional tube \f$[x](\cdot)\f$, defined as an interval of scalar trajectories
   *
   * \note Use TubeVector for the multi-dimensional case
   */
  class Tube : public DynamicalItem
  {
    public:

      /// \name Definition
      /// @{

      /**
       * \brief Creates a scalar tube \f$[x](\cdot)\f$ made of one slice
       * 
       * \param tdomain temporal domain \f$[t_0,t_f]\f$
       * \param codomain Interval value of the slice (all reals \f$[-\infty,\infty]\f$ by default)
       */
      explicit Tube(const Interval& tdomain, const Interval& codomain = Interval::ALL_REALS);

      /**
       * \brief Creates a scalar tube \f$[x](\cdot)\f$ with some temporal discretization
       *
       * \param tdomain temporal domain \f$[t_0,t_f]\f$
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \param codomain Interval value of the slices (all reals \f$[-\infty,\infty]\f$ by default)
       */
      explicit Tube(const Interval& tdomain, double timestep, const Interval& codomain = Interval::ALL_REALS);

      /**
       * \brief Creates a scalar tube \f$[x](\cdot)\f$ from a TFnc object and with some temporal discretization
       *
       * \note Due to the slicing implementation of the tube, a wrapping
       *       effect will occur to reliably enclose the TFnc object 

       * \param tdomain temporal domain \f$[t_0,t_f]\f$
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \param f TFnc object that will be enclosed by the tube:
       *          \f$\forall t\in[t_0,t_f], [f](t)\subseteq[x](t)\f$
       * \param f_image_id component index of the interval function
       *                   \f$[f]\f$ (that is possibly multidimensional, first component by default)
       */
      explicit Tube(const Interval& tdomain, double timestep, const TFnc& f, int f_image_id = 0);

      /**
       * \brief Creates a tube \f$[x](\cdot)\f$ from a list of \f$k\f$ boxes \f$\big([t_1]\times[x_1],\dots,[t_k]\times[x_k]\big)\f$
       *
       * \note The slicing will be based on the vector of temporal domains.
       * \note The \f$[t_i]\f$'s must cover continuously the tdomain of \f$[x](\cdot)\f$.
       *
       * \param v_tdomains vector of temporal domains \f$[t_i]\f$
       * \param v_codomains vector of codomains \f$[x_i]\f$ related to the \f$[t_i]\f$'s
       */
      explicit Tube(const std::vector<Interval>& v_tdomains, const std::vector<Interval>& v_codomains);

      /**
       * \brief Creates a copy of a scalar tube \f$[x](\cdot)\f$, with the same time discretization
       *
       * \param x Tube to be duplicated
       */
      Tube(const Tube& x);

      /**
       * \brief Creates a copy of a scalar tube \f$[x](\cdot)\f$, with the same time
       *        discretization but a specific codomain defined by a TFnc object
       *
       * \note Due to the slicing implementation of the tube, a wrapping
       *       effect will occur to reliably enclose the TFnc object 
       *
       * \param x Tube from which the sampling will be duplicated
       * \param f TFnc object that will be enclosed by the tube:
       *                     \f$\forall t\in[t_0,t_f], [f](t)\subseteq[x](t)\f$
       * \param f_image_id component index of the interval function \f$[f]\f$
       *                   (that is possibly multidimensional, first component by default)
       */
      explicit Tube(const Tube& x, const TFnc& f, int f_image_id = 0);

      /**
       * \brief Creates a scalar tube \f$[x](\cdot)\f$ enclosing a trajectory \f$x(\cdot)\f$,
       *        possibly with some temporal discretization
       *
       * \note Due to the slicing implementation of the tube, a wrapping
       *       effect will occur to reliably enclose the Trajectory object 
       *
       * \param traj Trajectory \f$x(\cdot)\f$ to enclose
       * \param timestep sampling value \f$\delta\f$ for the temporal
       *        discretization (double, no discretization by default: one slice only)
       */
      explicit Tube(const Trajectory& traj, double timestep);

      /**
       * \brief Creates a scalar tube \f$[x](\cdot)\f$ defined as an interval
       *        of two trajectories \f$[lb(\cdot),ub(\cdot)]\f$
       *
       * \note Due to the slicing implementation of the tube, a wrapping
       *       effect will occur to reliably enclose the Trajectory object 
       *
       * \param lb Trajectory defining the lower bound \f$x^{-}(\cdot)\f$ of the tube
       * \param ub Trajectory defining the upper bound \f$x^{+}(\cdot)\f$ of the tube
       * \param timestep sampling value \f$\delta\f$ for the temporal
       *        discretization (double, no discretization by default: one slice only)
       */
      explicit Tube(const Trajectory& lb, const Trajectory& ub, double timestep);

      /**
       * \brief Restore a scalar tube from serialization
       *
       * \note The Tube must have been serialized beforehand by the appropriate method serialize()
       *
       * \param binary_file_name path to the binary file
       */
      explicit Tube(const std::string& binary_file_name);

      /**
       * \brief Restore a scalar tube from serialization, together with a Trajectory object
       *
       * \note The Tube and the Trajectory must have been serialized
       *       beforehand by the appropriate method serialize()
       *
       * \param binary_file_name path to the binary file
       * \param traj a pointer to the Trajectory object to be instantiated
       */
      explicit Tube(const std::string& binary_file_name, Trajectory *&traj);

      /**
       * \brief Tube destructor
       */
      ~Tube();

      /**
       * \brief Returns the dimension of the scalar tube (always 1)
       *
       * \return 1
       */
      int size() const;

      /**
       * \brief Returns the primitive Tube of this tube
       *
       * \param c the constant of integration (0. by default)
       * \return a new Tube object with same slicing, enclosing the feasible primitives of this tube
       */
      const Tube primitive(const Interval& c = Interval(0.)) const;

      /**
       * \brief Returns a copy of a Tube
       *
       * \param x the Tube object to be copied
       * \return a new Tube object with same slicing and values
       */
      const Tube& operator=(const Tube& x);

      /**
       * \brief Returns the temporal definition domain of this tube
       *
       * \return an Interval object \f$[t_0,t_f]\f$
       */
      const Interval tdomain() const;

      /**
       * \brief Returns the polygon envelope of this tube
       *
       * \return a Polygon object enclosing the slices
       */
      const Polygon polygon_envelope() const;

      /**
       * \brief Returns a possible lower bound \f$x^{-}(\cdot)\f$ of the tube
       *
       * \note The exact lower bound cannot be known. However, the returned trajectory \f$x^{-}(\cdot)\f$
       * is guaranteed to be enclosed in the tube \f$[x](\cdot)\f$.
       *
       * \return a candidate for \f$x^{-}(\cdot)\f$
       */
      const Trajectory lb() const;

      /**
       * \brief Returns a possible upper bound \f$x^{+}(\cdot)\f$ of the tube
       *
       * \note The exact upper bound cannot be known. However, the returned trajectory \f$x^{+}(\cdot)\f$
       * is guaranteed to be enclosed in the tube \f$[x](\cdot)\f$.
       *
       * \return a candidate for \f$x^{+}(\cdot)\f$
       */
      const Trajectory ub() const;

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
       * \brief Returns a pointer to the ith Slice object of this tube
       *
       * \param slice_id the index of the ith Slice
       * \return a pointer to the corresponding Slice
       */
      Slice* slice(int slice_id);

      /**
       * \brief Returns a constant pointer to the ith Slice object of this tube
       *
       * \param slice_id the index of the ith Slice
       * \return a const pointer to the corresponding Slice
       */
      const Slice* slice(int slice_id) const;

      /**
       * \brief Returns a pointer to the Slice object of this tube that is defined at \f$t\f$
       *
       * \note If two Slices are defined at \f$t\f$ (common tdomain),
       *       then the first Slice is considered
       *
       * \param t the temporal key (double, must belong to the Tube's tdomain)
       * \return a pointer to the corresponding Slice
       */
      Slice* slice(double t);

      /**
       * \brief Returns a constant pointer to the Slice object of this tube that is defined at \f$t\f$
       *
       * \note If two Slices are defined at \f$t\f$ (common tdomain),
       *       then the first Slice is considered
       *
       * \param t the temporal key (double, must belong to the Tube's tdomain)
       * \return a const pointer to the corresponding Slice
       */
      const Slice* slice(double t) const;

      /**
       * \brief Returns a pointer to the first Slice object of this tube
       *
       * \return a pointer to the corresponding Slice
       */
      Slice* first_slice();

      /**
       * \brief Returns a constant pointer to the first Slice object of this tube
       *
       * \return a const pointer to the corresponding Slice
       */
      const Slice* first_slice() const;

      /**
       * \brief Returns a pointer to the last Slice object of this tube
       *
       * \return a pointer to the corresponding Slice
       */
      Slice* last_slice();

      /**
       * \brief Returns a constant pointer to the last Slice object of this tube
       *
       * \return a const pointer to the corresponding Slice
       */
      const Slice* last_slice() const;

      /**
       * \brief Returns a pointer to the Slice defined over the wider temporal domain
       *
       * \note If two Slice objects have the same tdomain width, then the first one is considered
       *
       * \return a pointer to the corresponding Slice
       */
      Slice* wider_slice();

      /**
       * \brief Returns a constant pointer to the Slice defined over the wider temporal domain
       *
       * \note If two Slice objects have the same tdomain width, then the first one is considered
       *
       * \return a const pointer to the corresponding Slice
       */
      const Slice* wider_slice() const;

      /**
       * \brief Returns a pointer to the Slice object of this tube for which
       *        the interval value is the most uncertain
       *
       * \return a pointer to the corresponding Slice
       */
      Slice* largest_slice();

      /**
       * \brief Returns a constant pointer to the Slice object of this tube for
       *        which the interval value is the most uncertain
       *
       * \return a const pointer to the corresponding Slice
       */
      const Slice* largest_slice() const;

      /**
       * \brief Returns a pointer to the Slice object of this tube for which
       *        the difference between the mid values of the input and output 
       *        gates is the largest
       *
       * \return a pointer to the corresponding Slice
       */
      Slice* steepest_slice();

       /**
       * \brief Returns a const pointer to the Slice object of this tube for which
       *        the difference between the mid values of the input and output 
       *        gates is the largest
       *
       * \return a const pointer to the corresponding Slice
       */
      const Slice* steepest_slice() const;

       /**
       * \brief Returns the temporal definition domain of the ith Slice of this tube
       *
       * \param slice_id the index of the ith Slice
       * \return an Interval object \f$[t_0^i,t_f^i]\f$
       */
      const Interval slice_tdomain(int slice_id) const;

      /**
       * \brief Returns the Slice index related to the temporal key \f$t\f$
       *
       * \param t the temporal key (double, must belong to the Tube's tdomain)
       * \return an integer
       */
      int time_to_index(double t) const;

      /**
       * \brief Returns the Slice index related to the Slice pointer
       *
       * \param slice a const pointer to a Slice object of this tube
       * \return an integer
       */
      int index(const Slice* slice) const;

      /**
       * \brief Samples this tube at \f$t\f$
       *
       * \note Without any effect if two Slice objects are already defined at \f$t\f$
       *       (if the gate \f$[x](t)\f$ already exists)
       *
       * \param t the temporal key (double, must belong to the Tube's tdomain)
       */

      void sample(double t);

      /**
       * \brief Samples this tube at \f$t\f$ from a pointer to the corresponding slice
       *
       * Reduces the complexity of related methods by providing a direct access
       * to the Slice object covering the input \f$t\f$.
       *
       * \note Without any effect if two Slice objects are already defined at \f$t\f$
       *       (if the gate \f$[x](t)\f$ already exists)
       *
       * \param t the temporal key (double, must belong to the Tube's tdomain)
       * \param slice_to_be_sampled a pointer to the Slice whose tdomain contains \f$t\f$
       */
      void sample(double t, Slice *slice_to_be_sampled);

      /**
       * \brief Samples this tube at \f$t\f$ with a specific gate value
       *
       * \note Without any sampling effect if two Slice objects are already defined at \f$t\f$
       *       (if the gate \f$[x](t)\f$ already exists)
       *
       * \param t the temporal key (double, must belong to the Tube's tdomain)
       * \param gate the Interval value of this tube at \f$t\f$
       */
      void sample(double t, const Interval& gate);

      /**
       * \brief Samples this tube so that it will share the same sampling of \f$[x](\cdot)\f$
       *
       * \note The previous sampling of this tube is preserved
       *
       * \param x the Tube from which the new sampling will come from
       */
      void sample(const Tube& x);

      /**
       * \brief Tests if a gate exists at time \f$t\f$
       *
       * \param t time input to test
       * \return `true` if a gate exists
       */
      bool gate_exists(double t) const;

      /**
       * \brief Removes the gate at \f$t\f$ and merges the two related slices
       *
       * \param t time input where the gate to remove is
       */
      void remove_gate(double t);

      /**
       * \brief Merges all adjacent slices whose Hausdorff distance is less than the given threshold
       *
       * \note Adjacent slices whose Hausdorff distance is larger (or equal) than the given threshold are not merged
       *
       * \param distance_threshold the threshold for the maximum Haussdorf distance between adjacent slices
       */
      void merge_similar_slices(double distance_threshold);

      /// @}
      /// \name Accessing values
      /// @{

      /**
       * \brief Returns the interval of feasible values
       *
       * \return an Interval object \f$[x]([t_0,t_f])\f$
       */
      const Interval codomain() const;

      /**
       * \brief Returns the volume of this tube
       *
       * \note returns POS_INFINITY if the codomain is unbounded
       * \note returns 0 if the tube is flat (and so without wrapping effect)
       *
       * \return volume defined as \f$w([t_0,t_f])\times w([x]([t_0,t_f]))\f$
       */
      double volume() const;

      /**
       * \brief Returns the value of the ith slice
       *
       * \param slice_id the index of the ith slice
       * \return Interval value of \f$[x](i)\f$
       */
      const Interval operator()(int slice_id) const;

      /**
       * \brief Returns the evaluation of this tube at \f$t\f$
       *
       * \param t the temporal key (double, must belong to the Tube's tdomain)
       * \return Interval value of \f$[x](t)\f$
       */
      const Interval operator()(double t) const;

      /**
       * \brief Returns the interval evaluation of this tube over \f$[t]\f$
       *
       * \param t the subtdomain (Interval, must be a subset of the Tube's tdomain)
       * \return Interval envelope \f$[x]([t])\f$
       */
      const Interval operator()(const Interval& t) const;

      /**
       * \brief Returns the interval evaluations of the bounds of the
       *        tube \f$\underline{x^-}(\cdot)\f$ and \f$\overline{x^+}(\cdot)\f$ over \f$[t]\f$
       *
       * \param t the subtdomain (Interval, must be a subset of the Tube's tdomain)
       * \return the pair \f$\big([\underline{x^-}]([t]),[\overline{x^+}]([t])\big)\f$
       */
      const std::pair<Interval,Interval> eval(const Interval& t = Interval::ALL_REALS) const;
      
      /**
       * \brief Returns the optimal evaluation of this tube at \f$t\f$,
       *        based on the derivative information \f$\dot{x}(\cdot)\f$
       *
       * \todo Change the name of this method?
       *
       * \param t the temporal key (double, must belong to the Tube's tdomain)
       * \param v the derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       * \return Interval value of \f$[x](t)\f$
       */
      const Interval interpol(double t, const Tube& v) const;

      /**
       * \brief Returns the optimal interval evaluation of this tube over \f$[t]\f$,
       *        based on the derivative information \f$\dot{x}(\cdot)\f$
       *
       * \todo Change the name of this method?
       *
       * \param t the subtdomain (Interval, must be a subset of the Tube's tdomain)
       * \param v the derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       * \return Interval value of \f$[x]([t])\f$
       */
      const Interval interpol(const Interval& t, const Tube& v) const;

      /**
       * \brief Returns the interval inversion \f$[x]^{-1}([y])\f$
       *
       * \note If the inversion results in several pre-images, their union is returned
       *
       * \param y the interval codomain
       * \param search_tdomain the optional temporal domain on which the inversion will be performed
       * \return the hull of \f$[x]^{-1}([y])\f$
       */
      const Interval invert(const Interval& y, const Interval& search_tdomain = Interval::ALL_REALS) const;

      /**
       * \brief Computes the set of continuous values of the inversion \f$[x]^{-1}([y])\f$
       *
       * \param y the interval codomain
       * \param v_t the vector of the sub-tdomains \f$[t_k]\f$ for which
       *            \f$\forall t\in[t_k] \mid x(t)\in[y], x(\cdot)\in[x](\cdot)\f$
       * \param search_tdomain the optional temporal domain on which the inversion will be performed
       */
      void invert(const Interval& y, std::vector<Interval> &v_t, const Interval& search_tdomain = Interval::ALL_REALS) const;

      /**
       * \brief Returns the optimal interval inversion \f$[x]^{-1}([y])\f$
       *
       * \note The knowledge of the derivative tube \f$[v](\cdot)\f$ allows a finer inversion
       * \note If the inversion results in several pre-images, their union is returned
       *
       * \param y the interval codomain
       * \param v the derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       * \param search_tdomain the optional temporal domain on which the inversion will be performed
       * \return the hull of \f$[x]^{-1}([y])\f$
       */
      const Interval invert(const Interval& y, const Tube& v, const Interval& search_tdomain = Interval::ALL_REALS) const;

      /**
       * \brief Computes the set of continuous values of the optimal inversion \f$[x]^{-1}([y])\f$
       *
       * \note The knowledge of the derivative tube \f$[v](\cdot)\f$ allows finer inversions
       *
       * \param y the interval codomain
       * \param v_t the vector of the sub-tdomains \f$[t_k]\f$ for which
       *            \f$\exists t\in[t_k] \mid x(t)\in[y], x(\cdot)\in[x](\cdot), \dot{x}(\cdot)\in[v](\cdot)\f$
       * \param v the derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       * \param search_tdomain the optional temporal domain on which the inversion will be performed
       */
      void invert(const Interval& y, std::vector<Interval> &v_t, const Tube& v, const Interval& search_tdomain = Interval::ALL_REALS) const;
      
      /**
       * \brief Returns the diameter of the interval value \f$[x](t)\f$ that is the more uncertain
       *
       * \return the maximal thickness of this tube
       */
      double max_diam() const;

      /**
       * \brief Returns the diameter of the gate of this tube that is the more uncertain
       *
       * \param t the temporal key of the corresponding uncertain gate
       * \return the maximal thickness of the gate
       */
      double max_gate_diam(double& t) const;

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
      const Trajectory diam(bool gates_thicknesses = false) const;

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
      const Trajectory diam(const Tube& v) const;

      /// @}
      /// \name Tests
      /// @{

      /**
       * \brief Returns true if this tube is equal to \f$[x](\cdot)\f$
       *
       * \note Equality is obtained if the tubes share
       *       the same bounds, tdomain and sampling
       *
       * \param x the Tube object
       * \return true in case of equality
       */
      bool operator==(const Tube& x) const;

      /**
       * \brief Returns true if this tube is different from \f$[x](\cdot)\f$
       *
       * \note The two tubes are different if they do not share
       *       the same bounds, tdomain or sampling
       *
       * \param x the Tube object
       * \return true in case of difference
       */
      bool operator!=(const Tube& x) const;

      /**
       * \brief Returns true if this tube is a subset of \f$[x](\cdot)\f$
       *
       * \note The two tubes may not share the same slicing,
       *       but must have the same tdomain
       *
       * \param x the Tube object
       * \return true in case of subset
       */
      bool is_subset(const Tube& x) const;

      /**
       * \brief Returns true if this tube is a subset of \f$[x](\cdot)\f$,
       *        and not \f$[x](\cdot)\f$ itself
       *
       * \note The two tubes may not share the same slicing,
       *       but must have the same tdomain
       *
       * \param x the Tube object
       * \return true in case of strict subset
       */
      bool is_strict_subset(const Tube& x) const;

      /**
       * \brief Returns true if this tube is a subset of the interior of \f$[x](\cdot)\f$
       *
       * \note The two tubes may not share the same slicing,
       *       but must have the same tdomain
       *
       * \param x the Tube object
       * \return true in case of interior subset
       */
      bool is_interior_subset(const Tube& x) const;

      /**
       * \brief Returns true if this tube is a subset of the interior
       *        of \f$[x](\cdot)\f$, and not \f$[x](\cdot)\f$ itself
       *
       * \note The two tubes may not share the same slicing,
       *       but must have the same tdomain
       *
       * \param x the Tube object
       * \return true in case of strict interior subset
       */
      bool is_strict_interior_subset(const Tube& x) const;

      /**
       * \brief Returns true if this tube is a superset of \f$[x](\cdot)\f$
       *
       * \note The two tubes may not share the same slicing,
       *       but must have the same tdomain
       *
       * \param x the Tube object
       * \return true in case of superset
       */
      bool is_superset(const Tube& x) const;

      /**
       * \brief Returns true if this tube is a superset of \f$[x](\cdot)\f$,
       *        and not \f$[x](\cdot)\f$ itself
       *
       * \note The two tubes may not share the same slicing,
       *       but must have the same tdomain
       *
       * \param x the Tube object
       * \return true in case of strict superset
       */
      bool is_strict_superset(const Tube& x) const;

      /**
       * \brief Returns true if this tube is empty
       *
       * \note If \f$[x](t)=\varnothing\f$ for some \f$t\f$, then the tube
       *       \f$[x](\cdot)\f$ is considered empty
       *
       * \return true in case of emptiness
       */
      bool is_empty() const;

      /**
       * \brief Returns true if this tube contains the trajectory \f$x(\cdot)\f$
       *
       * \note Due to the reliable numerical representation of a trajectory, some wrapping effect
       *       may appear for its evaluations (either if it is defined by a map of values
       *       or an analytic function). Hence, this "contains" test may not be able to
       *       conclude, if the thin envelope of \f$x(\cdot)\f$ overlaps a boundary of the tube.
       *
       * \param x the trajectory that might be contained by this tube
       * \return BoolInterval::YES (or BoolInterval::NO) if this tube contains \f$x(\cdot)\f$
       *         (or does not contain) and BoolInterval::MAYBE in case of ambiguity
       */
      const BoolInterval contains(const Trajectory& x) const;

      /**
       * \brief Returns true if this tube overlaps the tube \f$[x](\cdot)\f$
       *
       * \param x the other tube
       * \param ratio an optional overlapping ratio between 0 and 1 (1 by default).
       *              For instance, if ratio=0.3 and there is an overlapping of at least 30%,
       *              then the function returns true
       * \return true in case of overlapping with respect to the defined ratio
       */
      bool overlaps(const Tube& x, float ratio = 1.) const;

      /// @}
      /// \name Setting values
      /// @{

      /**
       * \brief Sets a constant interval value for this tube: \f$\forall t, [x](t)=[y]\f$
       *
       * \note The sampling of this tube is preserved
       *
       * \param y Interval value of the slices
       * \return *this
       */
      const Tube& set(const Interval& y);

      /**
       * \brief Sets the interval value of the ith slice of this tube
       *
       * \param y Interval value of the ith slice
       * \param slice_id index of the ith Slice
       * \return *this
       */
      const Tube& set(const Interval& y, int slice_id);

      /**
       * \brief Sets the interval value of this tube at \f$t\f$: \f$[x](t)=[y]\f$
       *
       * \note It may create a gate (and so one more slice) if the tube
       *       is not already sampled at \f$t\f$. Otherwise, it will
       *       update the value of the already existing gate.
       *
       * \param y Interval value of the gate
       * \param t the temporal key (double, must belong to the Tube's tdomain)
       * \return *this
       */
      const Tube& set(const Interval& y, double t);

      /**
       * \brief Sets the interval value of this tube over \f$[t]\f$: \f$\forall t\in[t], [x](t)=[y]\f$
       *
       * \note It may create two gates (and so further slices) if the tube
       *       is not already sampled at \f$t^-\f$ and \f$t^+\f$. This is
       *       done to ensure that \f$\forall t\in[t], [x](t)=[y]\f$.
       *
       * \param y Interval value to be set
       * \param t the subtdomain (Interval, must be a subset of the Tube's tdomain)
       * \return *this
       */
      const Tube& set(const Interval& y, const Interval& t);

      /**
       * \brief Sets this tube to the empty set
       *
       * \note By convention, all slices will be set to the empty set
       * \return *this
       */
      const Tube& set_empty();

      /**
       * \brief Inflates this tube by adding \f$[-rad,+rad]\f$ to all its codomain components
       *
       * \note All slices and gates will be inflated
       *
       * \param rad half of the inflation
       * \return *this
       */
      const Tube& inflate(double rad);

      /**
       * \brief Inflates this tube by adding non-constant uncertainties defined in a trajectory 
       *
       * \note From the trajectory \f$a(\cdot)\f$, the function will 
       *       inflate this tube such that \f$\forall t, [x](t):=[x](t)+[-a(t),+a(t)]\f$
       * \note All slices and gates will be inflated
       *
       * \param rad the Trajectory object defining the non-constant inflation
       * \return *this
       */
      const Tube& inflate(const Trajectory& rad);

      /**
       * \brief Truncates the tdomain of \f$[x](\cdot)\f$
       *
       * \note The new tdomain must be a subset of the old one
       *
       * \param tdomain new temporal domain \f$[t_0,t_f]\f$
       * \return a reference to this tube
       */
      Tube& truncate_tdomain(const Interval& tdomain);

      /**
       * \brief Shifts the tdomain \f$[t_0,t_f]\f$ of \f$[x](\cdot)\f$
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
       * \note The bisection is performed on the interval value of the gate \f$[x](t)\f$
       * \note If the tube is not already sampled at \f$t\f$, then a sampling is performed
       *
       * \param t the temporal key (double, must belong to the Tube's tdomain)
       * \param ratio the bisection ratio (default value: 0.55)
       * \return a pair of two Tube objects resulting from the bisection
       */
      const std::pair<Tube,Tube> bisect(double t, float ratio = 0.49) const;

      /// @}
      /// \name Assignments operators
      /// @{

      /**
       * \brief Operates +=
       *
       * \param x Interval
       * \return (*this)+=x
       */
      const Tube& operator+=(const Interval& x);

      /**
       * \brief Operates +=
       *
       * \param x Trajectory
       * \return (*this)+=x
       */
      const Tube& operator+=(const Trajectory& x);

      /**
       * \brief Operates +=
       *
       * \param x Tube
       * \return (*this)+=x
       */
      const Tube& operator+=(const Tube& x);

      /**
       * \brief Operates -=
       *
       * \param x Interval
       * \return (*this)-=x
       */
      const Tube& operator-=(const Interval& x);

      /**
       * \brief Operates -=
       *
       * \param x Trajectory
       * \return (*this)-=x
       */
      const Tube& operator-=(const Trajectory& x);

      /**
       * \brief Operates -=
       *
       * \param x Tube
       * \return (*this)-=x
       */
      const Tube& operator-=(const Tube& x);

      /**
       * \brief Operates *=
       *
       * \param x Interval
       * \return (*this)*=x
       */
      const Tube& operator*=(const Interval& x);

      /**
       * \brief Operates *=
       *
       * \param x Trajectory
       * \return (*this)*=x
       */
      const Tube& operator*=(const Trajectory& x);

      /**
       * \brief Operates *=
       *
       * \param x Tube
       * \return (*this)*=x
       */
      const Tube& operator*=(const Tube& x);

      /**
       * \brief Operates /=
       *
       * \param x Interval
       * \return (*this)/=x
       */
      const Tube& operator/=(const Interval& x);

      /**
       * \brief Operates /=
       *
       * \param x Trajectory
       * \return (*this)/=x
       */
      const Tube& operator/=(const Trajectory& x);

      /**
       * \brief Operates /=
       *
       * \param x Tube
       * \return (*this)/=x
       */
      const Tube& operator/=(const Tube& x);

      /**
       * \brief Operates |=
       *
       * \param x Interval
       * \return (*this)|=x
       */
      const Tube& operator|=(const Interval& x);

      /**
       * \brief Operates |=
       *
       * \param x Trajectory
       * \return (*this)|=x
       */
      const Tube& operator|=(const Trajectory& x);

      /**
       * \brief Operates |=
       *
       * \param x Tube
       * \return (*this)|=x
       */
      const Tube& operator|=(const Tube& x);

      /**
       * \brief Operates &=
       *
       * \param x Interval
       * \return (*this)&=x
       */
      const Tube& operator&=(const Interval& x);

      /**
       * \brief Operates &=
       *
       * \param x Trajectory
       * \return (*this)&=x
       */
      const Tube& operator&=(const Trajectory& x);

      /**
       * \brief Operates &=
       *
       * \param x Tube
       * \return (*this)&=x
       */
      const Tube& operator&=(const Tube& x);

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
      const std::string class_name() const { return "Tube"; };

      /**
       * \brief Displays a synthesis of this tube
       *
       * \param str ostream
       * \param x tube to be displayed
       * \return ostream
       */
      friend std::ostream& operator<<(std::ostream& str, const Tube& x);

      // Tree synthesis structure

      /**
       * \brief Enables the computation of a synthesis tree
       *
       * \note The synthesis tree speeds up computations such as integrals or evaluations
       *
       * \param mode mode of synthesis
       * \param eps precision of the polynomial approximation, if selected
       */
      void enable_synthesis(SynthesisMode mode = SynthesisMode::BINARY_TREE, double eps = 1.e-3) const;

      /// @}
      /// \name Integration
      /// @{

      /**
       * \brief Computes the interval integral \f$\int_0^t[x](\tau)d\tau\f$
       *
       * \note From the monotonicity of the integral operator, 
       *       \f$\int_0^t[x](\tau)d\tau=[\int_0^t x^-(\tau)d\tau,\int_0^t x^+(\tau)d\tau]\f$
       *
       * \param t the temporal key (double, must belong to the Tube's tdomain)
       * \return the set of feasible integral values
       */
      const Interval integral(double t) const;

      /**
       * \brief Computes the interval integral \f$\int_0^{[t]}[x](\tau)d\tau\f$
       *
       * \note From the monotonicity of the integral operator, 
       *       \f$\int_0^{[t]}[x](\tau)d\tau=[\int_0^{[t]}x^-(\tau)d\tau,\int_0^{[t]}x^+(\tau)d\tau]\f$
       *
       * \param t the subtdomain (Interval, must be a subset of the Tube's tdomain)
       * \return the set of feasible integral values
       */
      const Interval integral(const Interval& t) const;

      /**
       * \brief Computes the interval integral \f$\int_{[t_1]}^{[t_2]}[x](\tau)d\tau\f$
       *
       * \note From the monotonicity of the integral operator, 
       *       \f$\int_{[t_1]}^{[t_2]}[x](\tau)d\tau=[\int_{[t_1]}^{[t_2]}x^-(\tau)d\tau,\int_{[t_1]}^{[t_2]}x^+(\tau)d\tau]\f$
       *
       * \param t1 lower bound, subset of the Tube's tdomain
       * \param t2 upper bound, subset of the Tube's tdomain
       * \return the set of feasible integral values
       */
      const Interval integral(const Interval& t1, const Interval& t2) const;

      /**
       * \brief Computes the partial interval integral \f$\int_{0}^{[t]}[x](\tau)d\tau\f$
       *
       * \note From the monotonicity of the integral operator, 
       *       \f$\int_{0}^{[t]}[x](\tau)d\tau=[\int_{0}^{[t]}x^-(\tau)d\tau,\int_{0}^{[t]}x^+(\tau)d\tau]\f$
       *
       * \param t interval upper bound, subset of the Tube's tdomain
       * \return the pair \f$\big([i^-],[i^+]\big)\f$,
       *         where \f$[i^-]=\int_{0}^{[t]}x^-(\tau)d\tau\f$ 
       *         and \f$[i^+]=\int_{0}^{[t]}x^+(\tau)d\tau\f$
       */
      const std::pair<Interval,Interval> partial_integral(const Interval& t) const;

      /**
       * \brief Computes the partial interval integral \f$\int_{[t_1]}^{[t_2]}[x](\tau)d\tau\f$
       *
       * \note From the monotonicity of the integral operator, 
       *       \f$\int_{[t_1]}^{[t_2]}[x](\tau)d\tau=[\int_{[t_1]}^{[t_2]}x^-(\tau)d\tau,\int_{[t_1]}^{[t_2]}x^+(\tau)d\tau]\f$
       *
       * \param t1 interval lower bound, subset of the Tube's tdomain
       * \param t2 interval upper bound, subset of the Tube's tdomain
       * \return the pair \f$\big([i^-],[i^+]\big)\f$,
       *         where \f$[i^-]=\int_{[t_1]}^{[t_2]}x^-(\tau)d\tau\f$ 
       *         and \f$[i^+]=\int_{[t_1]}^{[t_2]}x^+(\tau)d\tau\f$
       */
      const std::pair<Interval,Interval> partial_integral(const Interval& t1, const Interval& t2) const;

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
       * \brief Serializes this tube together with a Trajectory object
       *
       * \note The values and sampling (slices and gates) are serialized
       * \note The serialization of a Trajectory defined from a TFunction object is not supported
       * \note The output file will appear in the executable current directory
       *
       * \param binary_file_name name of the output file (default value: "x.tube")
       * \param traj the Trajectory object to serialize (for instance, actual but unknown values)
       * \param version_number serialization version (used for tests purposes, default value: last version)
       */
      void serialize(const std::string& binary_file_name, const Trajectory& traj, int version_number = SERIALIZATION_VERSION) const;

      /// @}

      /**
       * \brief Tests whether the two Tube objects are sharing the same slicing
       *
       * \note If true, it means the two tubes are defined with the same
       *       amount of slices and identical sampling
       *
       * \param x1 the first Tube
       * \param x2 the second Tube
       * \return true in case of same slicing
       */
      static bool same_slicing(const Tube& x1, const Tube& x2);

      /**
       * \brief Enables the computation of a synthesis tree for any Tube object
       *
       * \note A synthesis tree speeds up computations such as integrals or evaluations
       *
       * \param enable boolean
       * \return void
       */
      static void enable_syntheses(bool enable = true);

      /**
       * \brief Computes the hull of several tubes
       *
       * \param l_tubes list of tubes
       * \return the tube enveloping the other ones
       */
      static const Tube hull(const std::list<Tube>& l_tubes);

    protected:

      /**
       * \brief Creates a not-defined scalar tube
       *
       * \note Constructor necessary for the TubeVector class
       */
      Tube();

      /**
       * \brief Returns the box \f$[x]([t_0,t_f])\f$
       *
       * \note Used for genericity purposes
       *
       * \return the envelope of codomain values
       */
      const IntervalVector codomain_box() const;

      /**
       * \brief Restores a scalar tube from serialization, together with a Trajectory object
       *
       * \note The Tube and the Trajectory must have been serialized
       *       beforehand by the appropriate method serialize()
       *
       * \param binary_file_name path to the binary file
       * \param traj a pointer to the Trajectory object to be instantiated
       */
      void deserialize(const std::string& binary_file_name, Trajectory *&traj);

      /**
       * \brief Creates the synthesis tree associated to the values of this tube
       *
       * \note The synthesis tree speeds up computations such as integrals or evaluations
       */
      void create_synthesis_tree() const;

      /**
       * \brief Deletes the synthesis tree of this tube
       */
      void delete_synthesis_tree() const;

      /**
       * \brief Creates the synthesis tree associated to the values of this tube
       *
       * \note The synthesis tree speeds up computations such as integrals or evaluations
       * 
       * \param eps precision of the polynomial approximation
       */
      void create_polynomial_synthesis(double eps) const;

      /**
       * \brief Deletes the synthesis tree of this tube
       */
      void delete_polynomial_synthesis() const;

      // Class variables:

        Slice *m_first_slice = nullptr; //!< pointer to the first Slice object of this tube
        mutable TubeTreeSynthesis *m_synthesis_tree = nullptr; //!< pointer to the optional synthesis tree
        mutable TubePolynomialSynthesis *m_polynomial_synthesis = nullptr; //!< pointer to the optional synthesis tree
        mutable SynthesisMode m_synthesis_mode = SynthesisMode::NONE; //!< enables of the use of a synthesis tree
        Interval m_tdomain; //!< redundant information for fast evaluations

      friend void deserialize_Tube(std::ifstream& bin_file, Tube *&tube);
      friend void deserialize_TubeVector(std::ifstream& bin_file, TubeVector *&tube);
      friend class TubeVector;
      friend class CtcEval;

      static bool s_enable_syntheses;
  };
}

#endif
