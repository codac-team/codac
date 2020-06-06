/** 
 *  \file
 *  Trajectory class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_TRAJECTORY_H__
#define __TUBEX_TRAJECTORY_H__

#include <map>
#include "tubex_DynamicalItem.h"
#include "tubex_TFunction.h"
#include "tubex_traj_arithmetic.h"

namespace tubex
{
  class TFunction;
  class TrajectoryVector;

  enum class TrajDefnType { ANALYTIC_FNC, MAP_OF_VALUES };
  
  /**
   * \class Trajectory
   * \brief One dimensional trajectory \f$x(\cdot)\f$, defined as a temporal map of values
   *
   * \note Use TrajectoryVector for the multi-dimensional case
   */
  class Trajectory : public DynamicalItem
  {
    public:

      /// \name Definition
      /// @{

      /**
       * \brief Creates an empty scalar trajectory \f$x(\cdot)\f$
       *
       * The trajectory is made of an empty map of values.
       */
      Trajectory();

      /**
       * \brief Creates a scalar trajectory \f$x(\cdot)\f$ from an analytic expression
       *
       * \param tdomain temporal domain \f$[t_0,t_f]\f$
       * \param f TFunction object defining the trajectory: \f$x(t)=f(t)\f$
       */
      Trajectory(const ibex::Interval& tdomain, const TFunction& f);

      /**
       * \brief Creates a scalar trajectory \f$x(\cdot)\f$ from an analytic expression,
       *        and transforms it as a map of values (sampling procedure)
       *
       * \param tdomain temporal domain \f$[t_0,t_f]\f$
       * \param f TFunction object defining the trajectory: \f$x(t)=f(t)\f$
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       */
      Trajectory(const ibex::Interval& tdomain, const TFunction& f, double timestep);

      /**
       * \brief Creates a scalar trajectory \f$x(\cdot)\f$ from a map of values
       *
       * \param m_map_values map<t,y> defining the trajectory: \f$x(t)=y\f$
       */
      explicit Trajectory(const std::map<double,double>& m_map_values);

      /**
       * \brief Creates a copy of a scalar trajectory \f$x(\cdot)\f$
       *
       * \param traj Trajectory to be duplicated
       */
      Trajectory(const Trajectory& traj);

      /**
       * \brief Trajectory destructor
       */
      ~Trajectory();

      /**
       * \brief Returns a copy of a Trajectory
       *
       * \param x the Trajectory object to be copied
       * \return a new Trajectory object with same values/definition
       */
      const Trajectory& operator=(const Trajectory& x);

      /**
       * \brief Returns the dimension of the scalar trajectory (always 1)
       *
       * \return 1
       */
      int size() const;

      /**
       * \brief Returns the temporal definition domain of this trajectory
       *
       * \return an Interval object \f$[t_0,t_f]\f$
       */
      const ibex::Interval tdomain() const;

      /**
       * \brief Returns the definition type of this trajectory
       *
       * \return TrajDefnType::ANALYTIC_FNC or TrajDefnType::MAP_OF_VALUES
       */
      TrajDefnType definition_type() const;

      /// @}
      /// \name Accessing values
      /// @{

      /**
       * \brief Returns the map of values, if the object is defined as a map
       *
       * \return a map<t,y> of values, or an empty map
       */
      const std::map<double,double>& sampled_map() const;

      /**
       * \brief Returns the temporal function, if the object is an analytic trajectory
       *
       * \return a pointer to a TFunction object
       */
      const TFunction* tfunction() const;

      /**
       * \brief Returns the interval of feasible values
       *
       * \return an Interval object \f$x([t_0,t_f])\f$
       */
      const ibex::Interval codomain() const;

      /**
       * \brief Returns the evaluation of this trajectory at \f$t\f$
       *
       * \note Be careful, if the trajectory is defined from an analytic function,
       *       then an approximation will be made (since the TFunction returns
       *       a boxed evaluation, while the expected returned value is a real here).
       *       Please use the operator(Interval(double)) for a reliable evaluation.
       *
       * \param t the temporal key (double, must belong to the trajectory's tdomain)
       * \return real value \f$x(t)\f$
       */
      double operator()(double t) const;

      /**
       * \brief Returns the interval evaluation of this trajectory over \f$[t]\f$
       *
       * \param t the subtdomain (Interval, must be a subset of the trajectory's domain)
       * \return Interval envelope \f$x([t])\f$
       */
      const ibex::Interval operator()(const ibex::Interval& t) const;

      /**
       * \brief Returns the value \f$x(t_0)\f$
       *
       * \return real value \f$x(t_0)\f$
       */
      double first_value() const;

      /**
       * \brief Returns the value \f$x(t_f)\f$
       *
       * \return real value \f$x(t_f)\f$
       */
      double last_value() const;

      /// @}
      /// \name Tests
      /// @{

      /**
       * \brief Tests whether this trajectory is defined or not
       *
       * \return false in case of non-empty map, or missing definition from a
       *         TFunction object, true otherwise
       */
      bool not_defined() const;

      /**
       * \brief Returns true if this trajectory is equal to \f$x(\cdot)\f$
       *
       * \param x the Trajectory object
       * \return true in case of equality
       */
      bool operator==(const Trajectory& x) const;

      /**
       * \brief Returns true if this trajectory is different from \f$x(\cdot)\f$
       *
       * \param x the Trajectory object
       * \return true in case of difference
       */
      bool operator!=(const Trajectory& x) const;

      /// @}
      /// \name Setting values
      /// @{

      /**
       * \brief Sets a value \f$y\f$ at \f$t\f$: \f$x(t)=y\f$
       *
       * \note The trajectory must not be defined from an analytic function
       *
       * \param y local value of the trajectory
       * \param t the temporal key (double, must belong to the trajectory's tdomain)
       */
      void set(double y, double t);

      /**
       * \brief Truncates the tdomain of \f$x(\cdot)\f$
       *
       * \note The new tdomain must be a subset of the old one
       *
       * \param tdomain new temporal domain \f$[t_0,t_f]\f$
       * \return a reference to this trajectory
       */
      Trajectory& truncate_tdomain(const ibex::Interval& tdomain);

      /**
       * \brief Shifts the tdomain \f$[t_0,t_f]\f$ of \f$x(\cdot)\f$
       *
       * \note If the trajectory is defined from a map of values, the key
       *       of each value will be shifted. In case of a definition from
       *       an analytic function, only the tdomain will be changed.
       *
       * \param a the offset value so that \f$[t_0,t_f]:=[t_0-a,t_f-a]\f$
       * \return a reference to this trajectory
       */
      Trajectory& shift_tdomain(double a);

      /**
       * \brief Samples the trajectory by adding new points to the map of values
       *
       * \note If the trajectory is defined as an analytic function, then the object is
       *       transformed into a map of values and the TFunction object is deleted.
       *
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \return a reference to this trajectory
       */
      Trajectory& sample(double timestep);

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
      Trajectory& sample(const Trajectory& x);

      /**
       * \brief Makes a trajectory continuous by avoiding infinite slopes
       *
       * \note This is mainly used when angles are expressed between \f$[-\pi,\pi]\f$,
       *       which produces troublesome discontinuities. For instance, a tube directly
       *       built from such discontinuous trajectory will be made of very large slices,
       *       inducing unwanted pessimism.
       *
       * \note The previous sampling of this trajectory is preserved
       *
       * \return a reference to this trajectory
       */
      Trajectory& make_continuous();

      /// @}
      /// \name Integration
      /// @{

      /**
       * \brief Computes an approximative primitive of \f$x(\cdot)\f$
       *
       * \note The trajectory must not be defined from an analytic function.
       *       Please use the other method with time discretization for such case.
       *
       * \param c the constant of integration (0. by default)
       * \return a new Trajectory object with the same temporal keys
       */
      const Trajectory primitive(double c = 0.) const;

      /**
       * \brief Computes an approximative primitive of \f$x(\cdot)\f$
       *        with some time discretization \f$\delta\f$
       *
       * \param c the constant of integration
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \return a new Trajectory object with the specified time discretization
       */
      const Trajectory primitive(double c, double timestep) const;

      /**
       * \brief Differentiates this trajectory
       *
       * \note If the trajectory is defined from an analytic function,
       *       the exact differentiation is returned. Otherwise, from a map
       *       of values, finite differences are computed.
       * 
       * \return a derivative trajectory
       */
      const Trajectory diff() const;

      /**
       * \brief Computes the finite difference at \f$t\f$,
       *        with an automatic order of accuracy.
       *
       * Central, forward or backward finite differences are computed.
       *
       * \note This function only works with trajectories defined as a map of values.
       * \note A uniform grid spacing is assumed.
       * 
       * \param t the temporal key (double, must belong to the trajectory's tdomain)
       * \return a derivative trajectory
       */
      double finite_diff(double t) const;

      /// @}
      /// \name Assignments operators
      /// @{

      /**
       * \brief Operates +=
       *
       * \param x double
       * \return (*this)+=x
       */
      const Trajectory& operator+=(double x);

      /**
       * \brief Operates +=
       *
       * \param x Trajectory
       * \return (*this)+=x
       */
      const Trajectory& operator+=(const Trajectory& x);

      /**
       * \brief Operates -=
       *
       * \param x double
       * \return (*this)-=x
       */
      const Trajectory& operator-=(double x);

      /**
       * \brief Operates -=
       *
       * \param x Trajectory
       * \return (*this)-=x
       */
      const Trajectory& operator-=(const Trajectory& x);

      /**
       * \brief Operates *=
       *
       * \param x double
       * \return (*this)*=x
       */
      const Trajectory& operator*=(double x);

      /**
       * \brief Operates *=
       *
       * \param x Trajectory
       * \return (*this)*=x
       */
      const Trajectory& operator*=(const Trajectory& x);

      /**
       * \brief Operates /=
       *
       * \param x double
       * \return (*this)/=x
       */
      const Trajectory& operator/=(double x);

      /**
       * \brief Operates /=
       *
       * \param x Trajectory
       * \return (*this)/=x
       */
      const Trajectory& operator/=(const Trajectory& x);

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
      const std::string class_name() const { return "Trajectory"; };

      /**
       * \brief Displays a synthesis of this trajectory
       *
       * \param str ostream
       * \param x trajectory to be displayed
       * \return ostream
       */
      friend std::ostream& operator<<(std::ostream& str, const Trajectory& x);

      /// @}

    protected:

      /**
       * \brief Returns the box \f$x([t_0,t_f])\f$
       *
       * \note Used for genericity purposes
       *
       * \return the envelope of codomain values
       */
      const ibex::IntervalVector codomain_box() const;

      /**
       * \brief Computes the envelope of trajectory values
       */
      void compute_codomain();

      // Class variables:

        ibex::Interval m_tdomain = ibex::Interval::EMPTY_SET; //!< temporal domain \f$[t_0,t_f]\f$ of the trajectory
        ibex::Interval m_codomain = ibex::Interval::EMPTY_SET; //!< envelope of the values of the trajectory
        
        // A trajectory is defined either by a TFunction or a map of values
        TrajDefnType m_traj_def_type = TrajDefnType::MAP_OF_VALUES; //!< definition type

        //union
        //{
          TFunction *m_function = NULL; //!< optional pointer to the analytic expression of this trajectory
          std::map<double,double> m_map_values; //!< optional map of values <t,y>: \f$x(t)=y\f$
        //};

      friend void deserialize_Trajectory(std::ifstream& bin_file, Trajectory *&traj);
      friend void deserialize_TrajectoryVector(std::ifstream& bin_file, TrajectoryVector *&traj);
  };
}

#endif