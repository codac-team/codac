/** 
 *  @file
 *  Trajectory class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_TRAJECTORY_H__
#define __TUBEX_TRAJECTORY_H__

#include <map>
#include "tubex_DynamicalItem.h"
#include "tubex_Function.h"

namespace tubex
{
  /**
   * \class Trajectory
   * \brief One dimensional trajectory \f$x(\cdot)\f$, defined as a temporal map of values
   *
   * \todo Implement arithmetic operators for trajectories?
   *
   * \note Use TrajectoryVector for the multi-dimensional case
   */

  class Function;
  class TrajectoryVector;
  
  class Trajectory : public DynamicalItem
  {
    public:

      /// \name Definition
      /// @{

      /**
       * \brief Creates an undefined scalar trajectory \f$x(\cdot)\f$
       */
      Trajectory();

      /**
       * \brief Creates a scalar trajectory \f$x(\cdot)\f$ from an analytic expression
       *
       * \param domain Interval domain \f$[t_0,t_f]\f$
       * \param f tubex::Function object defining the trajectory: \f$x(t)=f(t)\f$
       */
      Trajectory(const ibex::Interval& domain, const tubex::Function& f);

      /**
       * \brief Creates a scalar trajectory \f$x(\cdot)\f$ from a map of values
       *
       * \param m_map_values map<t,y> defining the trajectory: \f$x(t)=y\f$
       */
      Trajectory(const std::map<double,double>& m_map_values);

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
      const ibex::Interval domain() const;

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
       * \brief Returns the temporal function, if the object is a analytic trajectory
       *
       * \return a pointer to a tubex::Function object
       */
      const tubex::Function* function() const;

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
       *       then an approximation will be made (since the tubex::Function returns
       *       a boxed evaluation, while the expected returned value is a real here).
       *       Please use the operator(Interval(double)) for a reliable evaluation.
       *
       * \param t the temporal key (double, must belong to the trajectory domain)
       * \return real value \f$x(t)\f$
       */
      double operator()(double t) const;

      /**
       * \brief Returns the interval evaluation of this trajectory over \f$[t]\f$
       *
       * \param t the subdomain (Interval, must be a subset of the trajectory domain)
       * \return Interval envelope \f$x([t])\f$
       */
      const ibex::Interval operator()(const ibex::Interval& t) const;

      /// @}
      /// \name Tests
      /// @{

      /**
       * \brief Tests whether this trajectory is defined or not
       *
       * \return false in case of non-empty map, or definition from a
       *         tubex::Function object, true otherwise
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
       * \param t the temporal key (double, must belong to the trajectory domain)
       */
      void set(double y, double t);

      /**
       * \brief Truncates the domain of \f$x(\cdot)\f$
       *
       * \note The new domain must be a subset of the old one
       *
       * \param domain new Interval domain \f$[t_0,t_f]\f$
       */
      void truncate_domain(const ibex::Interval& domain);

      /**
       * \brief Shifts the domain \f$[t_0,t_f]\f$ of \f$x(\cdot)\f$
       *
       * \note If the trajectory is defined from a map of values, the key
       *       of each value will be shifted. In case of a definition from
       *       an analytic function, only the definition domain will be changed.
       *
       * \param a the offset value so that \f$[t_0,t_f]:=[t_0-a,t_f-a]\f$
       */
      void shift_domain(double a);

      /// @}
      /// \name Integration
      /// @{

      /**
       * \brief Computes an approximative primitive of \f$x(\cdot)\f$
       *
       * \note The trajectory must not be defined from an analytic function.
       *       Please use the other method with time discretization for such case.
       *
       * \return a new Trajectory object with the same temporal keys
       */
      const Trajectory primitive() const;

      /**
       * \brief Computes an approximative primitive of \f$x(\cdot)\f$
       *        whith some time discretization \f$\delta\f$
       *
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \return a new Trajectory object with the specified time discretization
       */
      const Trajectory primitive(double timestep) const;

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

      // Class variables:

        ibex::Interval m_domain = ibex::Interval::EMPTY_SET; //!< temporal domain \f$[t_0,t_f]\f$ of the trajectory
        ibex::Interval m_codomain = ibex::Interval::EMPTY_SET; //!< envelope of the values of the trajectory
        // A trajectory is defined either by a Function or a map of values
        tubex::Function *m_function = NULL; //!< optional pointer to the analytic expression of this trajectory
        std::map<double,double> m_map_values; //!< optional map of values <t,y>: \f$x(t)=y\f$

      friend void deserialize_Trajectory(std::ifstream& bin_file, Trajectory *&traj);
      friend void deserialize_TrajectoryVector(std::ifstream& bin_file, TrajectoryVector *&traj);
  };
}

#endif