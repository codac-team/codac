/** 
 *  \file
 *  TrajectoryVector class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_TRAJECTORYVECTOR_H__
#define __TUBEX_TRAJECTORYVECTOR_H__

#include <map>
#include <initializer_list>
#include "ibex_Vector.h"
#include "ibex_Interval.h"
#include "tubex_TFunction.h"
#include "tubex_Trajectory.h"
#include "tubex_DynamicalItem.h"
#include "tubex_traj_arithmetic.h"

namespace tubex
{
  class TFunction;
  class TubeVector;
  class Trajectory;

  /**
   * \class TrajectoryVector
   * \brief n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$, defined as a temporal map of vector values
   *
   * \note Use Trajectory for the one-dimensional case
   */
  class TrajectoryVector : public DynamicalItem
  {
    public:

      /// \name Definition
      /// @{

      /**
       * \brief Creates a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$
       *
       * \param n dimension of this trajectory
       */
      explicit TrajectoryVector(int n);

      /**
       * \brief Creates a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$ from an analytic expression
       *
       * \param tdomain temporal domain \f$[t_0,t_f]\f$
       * \param f TFunction object defining the trajectory: \f$\mathbf{x}(t)=\mathbf{f}(t)\f$
       */
      TrajectoryVector(const ibex::Interval& tdomain, const TFunction& f);

      /**
       * \brief Creates a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$ from an analytic expression,
       *        and transforms it as a map of values (sampling procedure)
       *
       * \param tdomain temporal domain \f$[t_0,t_f]\f$
       * \param f TFunction object defining the trajectory: \f$\mathbf{x}(t)=\mathbf{f}(t)\f$
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       */
      TrajectoryVector(const ibex::Interval& tdomain, const TFunction& f, double timestep);

      /**
       * \brief Creates a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$ from a map of vector values
       *
       * \param m_map_values map<t,y> defining the trajectory: \f$\mathbf{x}(t)=\mathbf{y}\f$
       */
      explicit TrajectoryVector(const std::map<double,ibex::Vector>& m_map_values);

      /**
       * \brief Creates a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$ from a vector of maps of scalar values
       *
       * \param v_map_values set of map<t,y> defining the trajectory: \f$\mathbf{x}(t)=\mathbf{y}\f$
       */
      explicit TrajectoryVector(const std::vector<std::map<double,double> >& v_map_values);

      /**
       * \brief Creates a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$ from a list of Trajectory objects
       *
       * \param list list of \f$x_i(\cdot)\f$ trajectories
       */
      TrajectoryVector(std::initializer_list<Trajectory> list);

      /**
       * \brief Creates a copy of a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$
       *
       * \param traj TrajectoryVector to be duplicated
       */
      TrajectoryVector(const TrajectoryVector& traj);

      /**
       * \brief Creates a n-dimensional trajectory with all the components initialized to \f$x(\cdot)\f$
       *
       * \param n dimension of this trajectory
       * \param x Trajectory to be copied for all the components
       */
      TrajectoryVector(int n, const Trajectory& x);

      /**
       * \brief TrajectoryVector destructor
       */
      ~TrajectoryVector();

      /**
       * \brief Returns a copy of a TrajectoryVector
       *
       * \param x the TrajectoryVector object to be copied
       * \return a new TrajectoryVector object with same values/definition
       */
      const TrajectoryVector& operator=(const TrajectoryVector& x);

      /**
       * \brief Returns the dimension of the trajectory
       *
       * \return n
       */
      int size() const;

      /**
       * \brief Returns the temporal definition domain of this trajectory
       *
       * \return an Interval object \f$[t_0,t_f]\f$
       */
      const ibex::Interval tdomain() const;

      /**
       * \brief Resizes this TrajectoryVector
       *
       * \note If the size is increased, the existing components are not
       *       modified and the new ones are set to empty trajectories
       *
       * \param n the new size to be set
       */
      void resize(int n);

      /**
       * \brief Returns a subvector of this TrajectoryVector
       *
       * \param start_index first component index of the subvector to be returned
       * \param end_index last component index of the subvector to be returned
       * \return a TrajectoryVector extracted from this TrajectoryVector
       */
      const TrajectoryVector subvector(int start_index, int end_index) const;

      /**
       * \brief Puts a subvector into this TrajectoryVector at a given position
       *
       * \param start_index position where the subvector will be put
       * \param subvec the TrajectoryVector to put from start_index
       */
      void put(int start_index, const TrajectoryVector& subvec);

      /// @}
      /// \name Accessing values
      /// @{

      /**
       * \brief Returns the box of feasible values
       *
       * \return an IntervalVector object \f$\mathbf{x}([t_0,t_f])\f$
       */
      const ibex::IntervalVector codomain() const;

      /**
       * \brief Returns the ith Trajectory of this TrajectoryVector
       *
       * \param index the index of this ith component
       * \return a reference to the ith component
       */
      Trajectory& operator[](int index);

      /**
       * \brief Returns a const reference to the ith Trajectory of this TrajectoryVector
       *
       * \param index the index of this ith component
       * \return a const reference to the ith component
       */
      const Trajectory& operator[](int index) const;

      /**
       * \brief Returns the evaluation of this trajectory at \f$t\f$
       *
       * \note Be careful, if the trajectory is defined from an analytic function,
       *       then an approximation will be made (since the TFunction returns
       *       a boxed evaluation, while the expected returned value is a real here).
       *       Please use the operator(Interval(double)) for a reliable evaluation.
       *
       * \param t the temporal key (double, must belong to the trajectory's tdomain)
       * \return real vector value \f$\mathbf{x}(t)\f$
       */
      const ibex::Vector operator()(double t) const;

      /**
       * \brief Returns the interval evaluation of this trajectory over \f$[t]\f$
       *
       * \param t the subtdomain (Interval, must be a subset of the trajectory's tdomain)
       * \return IntervalVector envelope \f$\mathbf{x}([t])\f$
       */
      const ibex::IntervalVector operator()(const ibex::Interval& t) const;

      /**
       * \brief Returns the value \f$\mathbf{x}(t_0)\f$
       *
       * \return real value \f$\mathbf{x}(t_0)\f$
       */
      const ibex::Vector first_value() const;

      /**
       * \brief Returns the value \f$\mathbf{x}(t_f)\f$
       *
       * \return real value \f$\mathbf{x}(t_f)\f$
       */
      const ibex::Vector last_value() const;

      /// @}
      /// \name Tests
      /// @{

      /**
       * \brief Tests whether this trajectory is defined or not
       *
       * \return false in case of non-empty map, or definition from a
       *         TFunction object, true otherwise
       */
      bool not_defined() const;

      /**
       * \brief Returns true if this trajectory is equal to \f$\mathbf{x}(\cdot)\f$
       *
       * \param x the TrajectoryVector object
       * \return true in case of equality
       */
      bool operator==(const TrajectoryVector& x) const;

      /**
       * \brief Returns true if this trajectory is different from \f$\mathbf{x}(\cdot)\f$
       *
       * \param x the TrajectoryVector object
       * \return true in case of difference
       */
      bool operator!=(const TrajectoryVector& x) const;

      /// @}
      /// \name Setting values
      /// @{

      /**
       * \brief Sets a value \f$\mathbf{y}\f$ at \f$t\f$: \f$\mathbf{x}(t)=\mathbf{y}\f$
       *
       * \param y local vector value of the trajectory
       * \param t the temporal key (double, must belong to the trajectory's tdomain)
       */
      void set(const ibex::Vector& y, double t);

      /**
       * \brief Truncates the tdomain of \f$\mathbf{x}(\cdot)\f$
       *
       * \note The new tdomain must be a subset of the old one
       *
       * \param tdomain new temporal domain \f$[t_0,t_f]\f$
       * \return a reference to this trajectory
       */
      TrajectoryVector& truncate_tdomain(const ibex::Interval& tdomain);

      /**
       * \brief Shifts the tdomain \f$[t_0,t_f]\f$ of \f$\mathbf{x}(\cdot)\f$
       *
       * \note If the trajectory is defined from a map of values, the key
       *       of each value will be shifted. In case of a definition from
       *       an analytic function, only the tdomain will be changed.
       *
       * \param a the offset value so that \f$[t_0,t_f]:=[t_0-a,t_f-a]\f$
       * \return a reference to this trajectory
       */
      TrajectoryVector& shift_tdomain(double a);

      /**
       * \brief Transforms an analytic trajectory as a map of values
       *
       * \note Sampling only available for trajectories firstly defined as analytic functions
       *
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \return a reference to this trajectory
       */
      TrajectoryVector& sample(double timestep);

      /**
       * \brief Samples this trajectory so that it will share the same sampling of \f$x(\cdot)\f$
       *
       * \note If the trajectory is defined as an analytic function, then the object is
       *       transformed into a map of values and the TFunction object is deleted.
       *
       * \note The previous sampling of this trajectory is preserved
       *
       * \param x the Trajectory from which the new sampling will come from
       * \return a reference to this trajectory
       */
      TrajectoryVector& sample(const Trajectory& x);

      /**
       * \brief Samples this trajectory so that it will share the same sampling of \f$\mathbf{x}(\cdot)\f$
       *
       * \note If the trajectory is defined as an analytic function, then the object is
       *       transformed into a map of values and the TFunction object is deleted.
       *
       * \note The previous sampling of this trajectory is preserved
       *
       * \param x the TrajectoryVector from which the new sampling will come from
       * \return a reference to this trajectory
       */
      TrajectoryVector& sample(const TrajectoryVector& x);

      /// @}
      /// \name Integration
      /// @{

      /**
       * \brief Computes an approximative primitive of \f$\mathbf{x}(\cdot)\f$
       *
       * \note The trajectory must not be defined from an analytic function.
       *       Please use the other method with time discretization for such case.
       *
       * \param c the constant of integration
       * \return a new TrajectoryVector object with the same temporal keys
       */
      const TrajectoryVector primitive(const ibex::Vector& c) const;

      /**
       * \brief Computes an approximative primitive of \f$\mathbf{x}(\cdot)\f$
       *        with some time discretization \f$\delta\f$
       *
       * \param c the constant of integration
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \return a new TrajectoryVector object with the specified time discretization
       */
      const TrajectoryVector primitive(const ibex::Vector& c, double timestep) const;

      /**
       * \brief Differentiates this trajectory vector
       *
       * \note If the trajectory is defined from an analytic function,
       *       the exact differentiation is returned. Otherwise, from a map
       *       of values, finite differences are computed.
       * 
       * \return a derivative trajectory vector
       */
      const TrajectoryVector diff() const;

      /// @}
      /// \name Assignments operators
      /// @{

      /**
       * \brief Operates +=
       *
       * \param x double
       * \return (*this)+=x
       */
      const TrajectoryVector& operator+=(double x);

      /**
       * \brief Operates +=
       *
       * \param x Trajectory
       * \return (*this)+=x
       */
      const TrajectoryVector& operator+=(const Trajectory& x);

      /**
       * \brief Operates +=
       *
       * \param x Vector
       * \return (*this)+=x
       */
      const TrajectoryVector& operator+=(const ibex::Vector& x);

      /**
       * \brief Operates +=
       *
       * \param x TrajectoryVector
       * \return (*this)+=x
       */
      const TrajectoryVector& operator+=(const TrajectoryVector& x);

      /**
       * \brief Operates -=
       *
       * \param x double
       * \return (*this)-=x
       */
      const TrajectoryVector& operator-=(double x);

      /**
       * \brief Operates -=
       *
       * \param x Trajectory
       * \return (*this)-=x
       */
      const TrajectoryVector& operator-=(const Trajectory& x);

      /**
       * \brief Operates -=
       *
       * \param x Vector
       * \return (*this)-=x
       */
      const TrajectoryVector& operator-=(const ibex::Vector& x);

      /**
       * \brief Operates -=
       *
       * \param x TrajectoryVector
       * \return (*this)-=x
       */
      const TrajectoryVector& operator-=(const TrajectoryVector& x);

      /**
       * \brief Operates *=
       *
       * \param x double
       * \return (*this)*=x
       */
      const TrajectoryVector& operator*=(double x);

      /**
       * \brief Operates *=
       *
       * \param x Trajectory
       * \return (*this)*=x
       */
      const TrajectoryVector& operator*=(const Trajectory& x);

      /**
       * \brief Operates /=
       *
       * \param x double
       * \return (*this)/=x
       */
      const TrajectoryVector& operator/=(double x);

      /**
       * \brief Operates /=
       *
       * \param x Trajectory
       * \return (*this)/=x
       */
      const TrajectoryVector& operator/=(const Trajectory& x);

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
      const std::string class_name() const { return "TrajectoryVector"; };

      /**
       * \brief Displays a synthesis of this trajectory
       *
       * \param str ostream
       * \param x trajectory to be displayed
       * \return ostream
       */
      friend std::ostream& operator<<(std::ostream& str, const TrajectoryVector& x);

      /// @}
      
    protected:

      /**
       * \brief Creates an undefined n-dimensional trajectory
       *
       * \note Constructor necessary for the deserialize_TrajectoryVector method
       */
      TrajectoryVector();

      /**
       * \brief Returns the box \f$\mathbf{x}([t_0,t_f])\f$
       *
       * \note Used for genericity purposes
       *
       * \return the envelope of codomain values
       */
      const ibex::IntervalVector codomain_box() const;

      // Class variables:

        int m_n = 0; //!< dimension of this trajectory
        Trajectory *m_v_trajs = NULL; //!< array of components (scalar trajectories)

      friend void deserialize_TrajectoryVector(std::ifstream& bin_file, TrajectoryVector *&traj);
      friend class TubeVector; // for TubeVector::deserialize method
  };
}

#endif