
/** 
 *  ile
 *  Trajectory documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 const char* DOCS_TRAJECTORY=R"_docs(/**
* \class Trajectory
* \brief One dimensional trajectory \f$x(\cdot)\f$, defined as a temporal map of values
*
* \note Use TrajectoryVector for the multi-dimensional case
*/
)_docs";

const char* DOCS_TRAJECTORY_TRAJECTORY=R"_docs(/// \name Definition
/// @{/**
* \brief Creates an undefined scalar trajectory \f$x(\cdot)\f$
*/
)_docs";

const char* DOCS_TRAJECTORY_TRAJECTORY_INTERVAL_FUNCTION=R"_docs(/**
* \brief Creates a scalar trajectory \f$x(\cdot)\f$ from an analytic expression
*
* \param domain Interval domain \f$[t_0,t_f]\f$
* \param f tubex::Function object defining the trajectory: \f$x(t)=f(t)\f$
*/
)_docs";

const char* DOCS_TRAJECTORY_TRAJECTORY_INTERVAL_FUNCTION_DOUBLE=R"_docs(/**
* \brief Creates a scalar trajectory \f$x(\cdot)\f$ from an analytic expression,
*        and transforms it as a map of values (sampling procedure)
*
* \param domain Interval domain \f$[t_0,t_f]\f$
* \param f tubex::Function object defining the trajectory: \f$x(t)=f(t)\f$
* \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
*/
)_docs";

const char* DOCS_TRAJECTORY_TRAJECTORY_MAP_DOUBLE_DOUBLE_=R"_docs(/**
* \brief Creates a scalar trajectory \f$x(\cdot)\f$ from a map of values
*
* \param m_map_values map<t,y> defining the trajectory: \f$x(t)=y\f$
*/
)_docs";

const char* DOCS_TRAJECTORY_TRAJECTORY_TRAJECTORY=R"_docs(/**
* \brief Creates a copy of a scalar trajectory \f$x(\cdot)\f$
*
* \param traj Trajectory to be duplicated
*/
)_docs";

const char* DOCS_TRAJECTORY_OPERATOR_TRAJECTORY=R"_docs(/**
* \brief Returns a copy of a Trajectory
*
* \param x the Trajectory object to be copied
* \return a new Trajectory object with same values/definition
*/
)_docs";

const char* DOCS_TRAJECTORY_SIZE=R"_docs(/**
* \brief Returns the dimension of the scalar trajectory (always 1)
*
* \return 1
*/
)_docs";

const char* DOCS_TRAJECTORY_TDOMAIN=R"_docs(/**
* \brief Returns the temporal definition domain of this trajectory
*
* \return an Interval object \f$[t_0,t_f]\f$
*/
)_docs";

const char* DOCS_TRAJECTORY_SAMPLED_MAP=R"_docs(/// @}
/// \name Accessing values
/// @{/**
* \brief Returns the map of values, if the object is defined as a map
*
* \return a map<t,y> of values, or an empty map
*/
)_docs";

const char* DOCS_TRAJECTORY_TFUNCTION=R"_docs(/**
* \brief Returns the temporal function, if the object is a analytic trajectory
*
* \return a pointer to a tubex::TFunction object
*/
)_docs";

const char* DOCS_TRAJECTORY_CODOMAIN=R"_docs(/**
* \brief Returns the interval of feasible values
*
* \return an Interval object \f$x([t_0,t_f])\f$
*/
)_docs";

const char* DOCS_TRAJECTORY_OPERATOR_CALL_DOUBLE=R"_docs(/**
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
)_docs";

const char* DOCS_TRAJECTORY_OPERATOR_CALL_INTERVAL=R"_docs(/**
* \brief Returns the interval evaluation of this trajectory over \f$[t]\f$
*
* \param t the subdomain (Interval, must be a subset of the trajectory domain)
* \return Interval envelope \f$x([t])\f$
*/
)_docs";

const char* DOCS_TRAJECTORY_FIRST_VALUE=R"_docs(/**
* \brief Returns the value \f$x(t_0)\f$
*
* \return real value \f$x(t_0)\f$
*/
)_docs";

const char* DOCS_TRAJECTORY_LAST_VALUE=R"_docs(/**
* \brief Returns the value \f$x(t_f)\f$
*
* \return real value \f$x(t_f)\f$
*/
)_docs";

const char* DOCS_TRAJECTORY_NOT_DEFINED=R"_docs(/// @}
/// \name Tests
/// @{/**
* \brief Tests whether this trajectory is defined or not
*
* \return false in case of non-empty map, or missing definition from a
*         tubex::Function object, true otherwise
*/
)_docs";

const char* DOCS_TRAJECTORY_EQ_TRAJECTORY=R"_docs(/**
* \brief Returns true if this trajectory is equal to \f$x(\cdot)\f$
*
* \param x the Trajectory object
* \return true in case of equality
*/
)_docs";

const char* DOCS_TRAJECTORY_NE_TRAJECTORY=R"_docs(/**
* \brief Returns true if this trajectory is different from \f$x(\cdot)\f$
*
* \param x the Trajectory object
* \return true in case of difference
*/
)_docs";

const char* DOCS_TRAJECTORY_SET_DOUBLE_DOUBLE=R"_docs(/// @}
/// \name Setting values
/// @{/**
* \brief Sets a value \f$y\f$ at \f$t\f$: \f$x(t)=y\f$
*
* \note The trajectory must not be defined from an analytic function
*
* \param y local value of the trajectory
* \param t the temporal key (double, must belong to the trajectory domain)
*/
)_docs";

const char* DOCS_TRAJECTORY_TRUNCATE_TDOMAIN_INTERVAL=R"_docs(/**
* \brief Truncates the domain of \f$x(\cdot)\f$
*
* \note The new domain must be a subset of the old one
*
* \param domain new Interval domain \f$[t_0,t_f]\f$
* \return a reference to this trajectory
*/
)_docs";

const char* DOCS_TRAJECTORY_SHIFT_TDOMAIN_DOUBLE=R"_docs(/**
* \brief Shifts the domain \f$[t_0,t_f]\f$ of \f$x(\cdot)\f$
*
* \note If the trajectory is defined from a map of values, the key
*       of each value will be shifted. In case of a definition from
*       an analytic function, only the definition domain will be changed.
*
* \param a the offset value so that \f$[t_0,t_f]:=[t_0-a,t_f-a]\f$
* \return a reference to this trajectory
*/
)_docs";

const char* DOCS_TRAJECTORY_SAMPLE_DOUBLE=R"_docs(/**
* \brief Samples the trajectory by adding new points to the map of values
*
* \note If the trajectory is defined as an analytic function, then the object is
*       transformed into a map of values and the tubex::Function object is deleted.
*
* \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
* \return a reference to this trajectory
*/
)_docs";

const char* DOCS_TRAJECTORY_SAMPLE_TRAJECTORY=R"_docs(/**
* \brief Samples this trajectory so that it will share the same sampling of \f$x(\cdot)\f$
*
* \note If the trajectory is defined as an analytic function, then the object is
*       transformed into a map of values and the tubex::Function object is deleted.
*
* \note The previous sampling of this trajectory is preserved
*
* \param x the Trajectory from which the new sampling will come from
* \return a reference to this trajectory
*/
)_docs";

const char* DOCS_TRAJECTORY_PRIMITIVE_DOUBLE=R"_docs(/// @}
/// \name Integration
/// @{/**
* \brief Computes an approximative primitive of \f$x(\cdot)\f$
*
* \note The trajectory must not be defined from an analytic function.
*       Please use the other method with time discretization for such case.
*
* \param c the constant of integration (0. by default)
* \return a new Trajectory object with the same temporal keys
*/
)_docs";

const char* DOCS_TRAJECTORY_PRIMITIVE_DOUBLE_DOUBLE=R"_docs(/**
* \brief Computes an approximative primitive of \f$x(\cdot)\f$
*        whith some time discretization \f$\delta\f$
*
* \param c the constant of integration
* \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
* \return a new Trajectory object with the specified time discretization
*/
)_docs";

const char* DOCS_TRAJECTORY_DIFF=R"_docs(/**
* \brief Differentiates this trajectory
*
* \note If the trajectory is defined from an analytic function,
*       the exact differentiation is returned. Otherwise, from a map
*       of values, finite differences are computed.
* 
* \return a derivative trajectory
*/
)_docs";

const char* DOCS_TRAJECTORY_FINITE_DIFF_DOUBLE=R"_docs(/**
* \brief Computes the finite difference at \f$t\f$,
*        with an automatic order of accuracy.
*
* Central, forward or backward finite differences are computed.
*
* \note This function only works with trajectories defined as a map of values.
* \note A uniform grid spacing is assumed.
* 
* \param t the temporal key (double, must belong to the trajectory domain)
* \return a derivative trajectory
*/
)_docs";

const char* DOCS_TRAJECTORY_IADD_DOUBLE=R"_docs(/// @}
/// \name Assignments operators
/// @{/**
* \brief Operates +=
*
* \param x double
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TRAJECTORY_IADD_TRAJECTORY=R"_docs(/**
* \brief Operates +=
*
* \param x Trajectory
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TRAJECTORY_ISUB_DOUBLE=R"_docs(/**
* \brief Operates -=
*
* \param x double
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TRAJECTORY_ISUB_TRAJECTORY=R"_docs(/**
* \brief Operates -=
*
* \param x Trajectory
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TRAJECTORY_IMUL_DOUBLE=R"_docs(/**
* \brief Operates *=
*
* \param x double
* \return (*this)*=x
*/
)_docs";

const char* DOCS_TRAJECTORY_IMUL_TRAJECTORY=R"_docs(/**
* \brief Operates *=
*
* \param x Trajectory
* \return (*this)*=x
*/
)_docs";

const char* DOCS_TRAJECTORY_ITRUEDIV_DOUBLE=R"_docs(/**
* \brief Operates /=
*
* \param x double
* \return (*this)/=x
*/
)_docs";

const char* DOCS_TRAJECTORY_ITRUEDIV_TRAJECTORY=R"_docs(/**
* \brief Operates /=
*
* \param x Trajectory
* \return (*this)/=x
*/
)_docs";

const char* DOCS_TRAJECTORY_CLASS_NAME=R"_docs(/// @}
/// \name String
/// @{/**
* \brief Returns the name of this class
*
* \note Only used for some generic display method
*
* \return the predefined name
*/
)_docs";

const char* DOCS_TRAJECTORY_OPERATOR_OSTREAM_TRAJECTORY=R"_docs(/**
* \brief Displays a synthesis of this trajectory
*
* \param str ostream
* \param x trajectory to be displayed
* \return ostream
*/
)_docs";

