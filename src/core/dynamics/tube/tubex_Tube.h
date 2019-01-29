/* ================================================================
 *  tubex-lib - Tube class
 * ================================================================
 *  Copyright : ENSTA Bretagne (France)
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ------------------------------------------------------------- */

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
#include "ibex_BoolInterval.h"

#define AUTO_SYNTHESIS_BY_DEFAULT false

namespace tubex
{
  /**
   * \class Tube
   * \brief Interval of one-dimensional trajectories
   *
   * One dimensional tube, defined as an interval of scalar trajectories.
   *
   * \note Use TubeVector for the multi-dimensional case
   */

  class Fnc;
  class Tube;
  class Slice;
  class Trajectory;
  class TubeTreeSynthesis;
  
  class Tube : public DynamicalItem
  {
    public:

    /** Base: **/

      /**
       * \brief Creates a scalar tube made of one slice
       * \param domain Interval domain \f$[t_0,t_f]\f$
       * \param codomain Interval value of the slice (all reals \f$[-\infty,\infty]\f$ by default)
       */
      Tube(const ibex::Interval& domain, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);

      /**
       * \brief Creates a scalar tube with some temporal discretization
       * \param domain Interval domain \f$[t_0,t_f]\f$
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \param codomain Interval value of the slices (all reals \f$[-\infty,\infty]\f$ by default)
       */
      Tube(const ibex::Interval& domain, double timestep, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);

      /**
       * \brief Creates a scalar tube from a tubex::Fnc object and with some temporal discretization
       * \param domain Interval domain \f$[t_0,t_f]\f$
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \param f tubex::Fnc object that will be enclosed by the tube: \f$\forall t\in[t_0,t_f], f(t)\in[x](t)\f$
       * \param f_image_id component index of the evaluated function \f$f\f$ (int, first component by default)
       */
      Tube(const ibex::Interval& domain, double timestep, const tubex::Fnc& f, int f_image_id = 0);

      /**
       * \brief Creates a copy of a scalar tube, with the same time discretization
       *
       * \param x Tube to be duplicated
       */
      Tube(const Tube& x);

      /**
       * \brief Creates a copy of a scalar tube, with the same time discretization but a specific constant codomain
       *
       * \param x Tube from which the sampling will be duplicated
       * \param codomain Interval value of the slices
       */
      Tube(const Tube& x, const ibex::Interval& codomain);

      /**
       * \brief Creates a copy of a scalar tube, with the same time discretization but a specific codomain defined by a tubex::Fnc object
       *
       * \Note Due to the slicing implementation of the tube, a wrapping effect will occur to reliably enclose the tubex::Fnc object 
       *
       * \param x Tube from which the sampling will be duplicated
       * \param f tubex::Fnc object that will be enclosed by the tube: \f$\forall t\in[t_0,t_f], f(t)\in[x](t)\f$
       * \param f_image_id component index of the evaluated function \f$f\f$ (int, first component by default)
       */
      Tube(const Tube& x, const tubex::Fnc& f, int f_image_id = 0);

      /**
       * \brief Creates a scalar tube enclosing a Trajectory, possibly with some temporal discretization
       *
       * \Note Due to the slicing implementation of the tube, a wrapping effect will occur to reliably enclose the Trajectory object 
       *
       * \param traj Trajectory \f$a(\cdot)\f$ to enclose
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double, no discretization by default: one slice only)
       */
      Tube(const Trajectory& traj, double timestep = 0.);

      /**
       * \brief Creates a scalar tube defined as an interval of two Trajectory objects
       *
       * \Note Due to the slicing implementation of the tube, a wrapping effect will occur to reliably enclose the Trajectory object 
       *
       * \param lb Trajectory defining the lower bound \f$x^{-}(\cdot)\f$ of the tube
       * \param ub Trajectory defining the upper bound \f$x^{+}(\cdot)\f$of the tube
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double, no discretization by default: one slice only)
       */
      Tube(const Trajectory& lb, const Trajectory& ub, double timestep = 0.);

      /**
       * \brief Restore a scalar tube from serialization
       *
       * \Note The Tube must have been serialized beforehand by the appropriate method serialize()
       *
       * \param binary_file_name path to the binary file
       */
      Tube(const std::string& binary_file_name);

      /**
       * \brief Restore a scalar tube from serialization, together with a Trajectory object
       *
       * \Note The Tube and the Trajectory must have been serialized beforehand by the appropriate method serialize()
       *
       * \param binary_file_name path to the binary file
       * \param traj a pointer to the Trajectory object to be instantiated
       */
      Tube(const std::string& binary_file_name, Trajectory *&traj);

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
       * \brief Returns the primitive Tube of this
       *
       * \return a new Tube object with same slicing, enclosing the feasible primitives of this
       */
      const Tube primitive() const;

      /**
       * \brief Returns a copy of a Tube
       *
       * \param x the Tube object to be copied
       * \return a new Tube object with same slicing and values
       */
      const Tube& operator=(const Tube& x);

      /**
       * \brief Returns the temporal definition domain of this
       *
       * \return an Interval object \f$[t_0,t_f]\f$
       */
      const ibex::Interval domain() const;

      // Slices structure

      /**
       * \brief Returns the number of slices of this
       *
       * \return an integer
       */
      int nb_slices() const;

      /**
       * \brief Returns a pointer to the ith Slice object of this
       *
       * \param slice_id the index of the ith Slice
       * \return a pointer to the corresponding Slice
       */
      Slice* slice(int slice_id);

      /**
       * \brief Returns a constant pointer to the ith Slice object of this
       *
       * \param slice_id the index of the ith Slice
       * \return a const pointer to the corresponding Slice
       */
      const Slice* slice(int slice_id) const;

      /**
       * \brief Returns a pointer to the Slice object of this that is defined at \f$t\f$
       *
       * \note if two Slices are defined at \f$t\f$ (common domain), then the first Slice is considered
       *
       * \param t the temporal key (double, must belong to the Tube domain)
       * \return a pointer to the corresponding Slice
       */
      Slice* slice(double t);

      /**
       * \brief Returns a constant pointer to the Slice object of this that is defined at \f$t\f$
       *
       * \note if two Slices are defined at \f$t\f$ (common domain), then the first Slice is considered
       *
       * \param t the temporal key (double, must belong to the Tube domain)
       * \return a const pointer to the corresponding Slice
       */
      const Slice* slice(double t) const;

      /**
       * \brief Returns a pointer to the first Slice object of this
       *
       * \return a pointer to the corresponding Slice
       */
      Slice* first_slice();

      /**
       * \brief Returns a constant pointer to the first Slice object of this
       *
       * \return a const pointer to the corresponding Slice
       */
      const Slice* first_slice() const;

      /**
       * \brief Returns a pointer to the last Slice object of this
       *
       * \return a pointer to the corresponding Slice
       */
      Slice* last_slice();

      /**
       * \brief Returns a constant pointer to the last Slice object of this
       *
       * \return a const pointer to the corresponding Slice
       */
      const Slice* last_slice() const;

      /**
       * \brief Returns a pointer to the Slice defined over the wider temporal domain
       *
       * \note If two Slice objects have the same domain's width, then the first one is considered
       *
       * \return a pointer to the corresponding Slice
       */
      Slice* wider_slice();

      /**
       * \brief Returns a constant pointer to the Slice defined over the wider temporal domain
       *
       * \note If two Slice objects have the same domain's width, then the first one is considered
       *
       * \return a const pointer to the corresponding Slice
       */
      const Slice* wider_slice() const;

      /**
       * \brief Returns a pointer to the Slice object of this for which the interval value is the more uncertain
       *
       * \return a pointer to the corresponding Slice
       */
      Slice* largest_slice();

      /**
       * \brief Returns a constant pointer to the Slice object of this for which the interval value is the more uncertain
       *
       * \return a const pointer to the corresponding Slice
       */
      const Slice* largest_slice() const;

      /**
       * \brief Returns the temporal definition domain of the ith Slice of this
       *
       * \param slice_id the index of the ith Slice
       * \return an Interval object \f$[t_0^i,t_f^i]\f$
       */
      const ibex::Interval slice_domain(int slice_id) const;

      /**
       * \brief Returns the Slice index related to the temporal key \f$t\f$
       *
       * \param t the temporal key (double, must belong to the Tube domain)
       * \return an integer
       */
      int input2index(double t) const;

      /**
       * \brief Returns the Slice index related to Slice pointer
       *
       * \param slice a const pointer to a Slice object of this
       * \return an integer
       */
      int index(const Slice* slice) const;

      /**
       * \brief Samples this at \f$t\f$
       *
       * \note without any effect if two Slice objects are already defined at \f$t\f$
       *
       * \param t the temporal key (double, must belong to the Tube domain)
       */
      void sample(double t);

      /**
       * \brief Samples this at \f$t\f$ with a specific gate value
       *
       * \note Without any sampling effect if two Slice objects are already defined at \f$t\f$
       *
       * \param t the temporal key (double, must belong to the Tube domain)
       * \param gate the Interval value of this at \f$t\f$
       */
      void sample(double t, const ibex::Interval& gate);

      /**
       * \brief Samples this so that it will share the same sampling of Tube x
       *
       * \note The previous sampling of this is preserved
       *
       * \param x the Tube from which the new sampling will come from
       */
      void sample(const Tube& x);

      /**
       * \brief Tests whether the two Tube objects are sharing the same slicing
       *
       * \note If true, it means the two tubes are defined with the same amount of slices and identical sampling
       *
       * \param x1 the first Tube
       * \param x2 the first Tube
       * \return true in case of same slicing
       */
      static bool same_slicing(const Tube& x1, const Tube& x2);

      // Accessing values

      /**
       * \brief Returns the interval of feasible values
       *
       * \return an Interval object \f$[x]([t_0,t_f])\f$
       */
      const ibex::Interval codomain() const;

      /**
       * \brief Returns the volume of this
       *
       * \note returns POS_INFINITY if the codomain is unbounded
       * \note returns 0 if the tube is flat (and so without wrapping effect)
       *
       * \return volume (domain.diam()*codomain.diam())
       */
      double volume() const;

      /**
       * \brief Returns the value of the ith slice
       *
       * \param slice_id the index of the ith slice
       * \return Interval value of \f$[x](i)\f$
       */
      const ibex::Interval operator()(int slice_id) const;

      /**
       * \brief Returns the evaluation of this at \f$t\f$
       *
       * \param t the temporal key (double, must belong to the Tube domain)
       * \return Interval value of \f$[x](t)\f$
       */
      const ibex::Interval operator()(double t) const;

      /**
       * \brief Returns the interval evaluation of this over \f$[t]\f$
       *
       * \param t the subdomain (Interval, must be a subset of the Tube domain)
       * \return Interval envelope \f$[x]([t])\f$
       */
      const ibex::Interval operator()(const ibex::Interval& t) const;

      /**
       * \brief Returns the interval inversion \f$[x]^{-1}([y])\f$
       *
       * \note If the inversion results in several pre-images, their union is returned
       *
       * \param y the interval codomain
       * \param search_domain the optional interval domain on which the inversion will be performed
       * \return the hull of \f$[x]^{-1}([y])\f$
       */
      const ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;

      /**
       * \brief Computes the set of continuous values of the inversion \f$[x]^{-1}([y])\f$
       *
       * \param y the interval codomain
       * \param v_t the vector of the sub-domains \f$[t_k]\f$ for which \f$\exists t\in[t_k] \mid x(t)\in[y], x(\cdot)\in[x](\cdot)\f$
       * \param search_domain the optional interval domain on which the inversion will be performed
       */
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;

      /**
       * \brief Returns the optimal interval inversion \f$[x]^{-1}([y])\f$
       *
       * \note The knowledge of the derivative tube \f$[v](\cdot)\f$ allows a finer inversion
       * \note If the inversion results in several pre-images, their union is returned
       *
       * \param y the interval codomain
       * \param v the derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       * \param search_domain the optional interval domain on which the inversion will be performed
       * \return the hull of \f$[x]^{-1}([y])\f$
       */
      const ibex::Interval invert(const ibex::Interval& y, const Tube& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;

      /**
       * \brief Computes the set of continuous values of the optimal inversion \f$[x]^{-1}([y])\f$
       *
       * \note The knowledge of the derivative tube \f$[v](\cdot)\f$ allows finer inversions
       * \note If the inversion results in several pre-images, their union is returned
       *
       * \param y the interval codomain
       * \param v_t the vector of the sub-domains \f$[t_k]\f$ for which \f$\exists t\in[t_k] \mid x(t)\in[y], x(\cdot)\in[x](\cdot), \dot{x}(\cdot)\in[v](\cdot)\f$
       * \param v the derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       * \param search_domain the optional interval domain on which the inversion will be performed
       * \return the hull of \f$[x]^{-1}([y])\f$
       */
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const Tube& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      
      /**
       * \brief Returns the interval evaluations of the bounds of the tube \f$x^-(\cdot)\f$ and \f$x^+(\cdot)\f$ over \f$[t]\f$
       *
       * \param t the subdomain (Interval, must be a subset of the Tube domain)
       * \return the pair \f$\big([x^-]([t]),[x^+]([t])\big)\f$
       */
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      
      /**
       * \brief Returns the optimal evaluation of this at \f$t\f$, based on the derivative information \f$\dot{x}(\cdot)\f$
       *
       * \todo Change the name of this method?
       *
       * \param t the temporal key (double, must belong to the Tube domain)
       * \param v the derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       * \return Interval value of \f$[x](t)\f$
       */
      const ibex::Interval interpol(double t, const Tube& v) const;

      /**
       * \brief Returns the optimal interval evaluation of this over \f$[t]\f$, based on the derivative information \f$\dot{x}(\cdot)\f$
       *
       * \todo Change the name of this method?
       *
       * \param t the subdomain (Interval, must be a subset of the Tube domain)
       * \param v the derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       * \return Interval value of \f$[x]([t])\f$
       */
      const ibex::Interval interpol(const ibex::Interval& t, const Tube& v) const;

      /**
       * \brief Returns the diameter of the interval value \f$[x](t)\f$ that is the more uncertain
       *
       * \return the maximal thickness of this
       */
      double max_thickness() const;

      /**
       * \brief Returns the diameter of the gate of this that is the more uncertain
       *
       * \param t the temporal key of the corresponding uncertain gate
       * \return the maximal thickness of the gate
       */
      double max_gate_thickness(double& t) const;

      // Tests
      bool operator==(const Tube& x) const;
      bool operator!=(const Tube& x) const;
      bool is_subset(const Tube& x) const;
      bool is_strict_subset(const Tube& x) const;
      bool is_interior_subset(const Tube& x) const;
      bool is_strict_interior_subset(const Tube& x) const;
      bool is_superset(const Tube& x) const;
      bool is_strict_superset(const Tube& x) const;
      bool is_empty() const;
      const ibex::BoolInterval contains(const Trajectory& x) const;
      bool overlaps(const Tube& x, float ratio = 1.) const;

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

      // Tree synthesis structure
      void enable_synthesis(bool enable = true) const;
      static void enable_syntheses(bool enable = true);

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

      // Definition
      Tube();

      // Accessing values
      const ibex::IntervalVector codomain_box() const;

      // Serialization
      void deserialize(const std::string& binary_file_name, Trajectory *&traj);

      // Integration

      // Tree synthesis structure
      void create_synthesis_tree() const;
      void delete_synthesis_tree() const;

      /** Class variables **/

        Slice *m_first_slice = NULL; //!< 
        mutable TubeTreeSynthesis *m_synthesis_tree = NULL; //!< 
        mutable bool m_enable_synthesis = Tube::s_enable_syntheses;
        
      friend void deserialize_Tube(std::ifstream& bin_file, Tube *&tube);
      friend void deserialize_TubeVector(std::ifstream& bin_file, TubeVector *&tube);
      friend class TubeVector;

      static bool s_enable_syntheses;
  };
}

#endif