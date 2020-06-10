
/** 
 *  ile
 *  TrajectoryVector documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 const char* DOCS_TRAJECTORYVECTOR=R"_docs(/**
* \class TrajectoryVector
* \brief n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$, defined as a temporal map of vector values
*
* \note Use Trajectory for the one-dimensional case
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_TRAJECTORYVECTOR_INT=R"_docs(/// \name Definition
/// @{/**
* \brief Creates a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$
*
* \param n dimension of this trajectory
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_TRAJECTORYVECTOR_INTERVAL_FUNCTION=R"_docs(/**
* \brief Creates a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$ from an analytic expression
*
* \param domain Interval domain \f$[t_0,t_f]\f$
* \param f tubex::Function object defining the trajectory: \f$\mathbf{x}(t)=\mathbf{f}(t)\f$
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_TRAJECTORYVECTOR_INTERVAL_FUNCTION_DOUBLE=R"_docs(/**
* \brief Creates a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$ from an analytic expression,
*        and transforms it as a map of values (sampling procedure)
*
* \param tdomain temporal domain \f$[t_0,t_f]\f$
* \param f TFunction object defining the trajectory: \f$\mathbf{x}(t)=\mathbf{f}(t)\f$
* \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_TRAJECTORYVECTOR_MAP_DOUBLE_VECTOR_=R"_docs(/**
* \brief Creates a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$ from a map of vector values
*
* \param m_map_values map<t,y> defining the trajectory: \f$\mathbf{x}(t)=\mathbf{y}\f$
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_TRAJECTORYVECTOR_VECTOR_MAP_DOUBLE_DOUBLE=R"_docs(/**
* \brief Creates a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$ from a vector of maps of scalar values
*
* \param v_map_values set of map<t,y> defining the trajectory: \f$\mathbf{x}(t)=\mathbf{y}\f$
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_TRAJECTORYVECTOR_INITIALIZER_LIST_TRAJECTORY_=R"_docs(/**
* \brief Creates a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$ from a list of Trajectory objects
*
* \param list list of \f$x_i(\cdot)\f$ trajectories
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_TRAJECTORYVECTOR_TRAJECTORYVECTOR=R"_docs(/**
* \brief Creates a copy of a n-dimensional trajectory \f$\mathbf{x}(\cdot)\f$
*
* \param traj TrajectoryVector to be duplicated
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_TRAJECTORYVECTOR_INT_TRAJECTORY=R"_docs(/**
* \brief Creates a n-dimensional trajectory with all the components initialized to \f$x(\cdot)\f$
*
* \param n dimension of this trajectory
* \param x Trajectory to be copied for all the components
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_OPERATOR_TRAJECTORYVECTOR=R"_docs(/**
* \brief Returns a copy of a TrajectoryVector
*
* \param x the TrajectoryVector object to be copied
* \return a new TrajectoryVector object with same values/definition
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_SIZE=R"_docs(/**
* \brief Returns the dimension of the trajectory
*
* \return n
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_TDOMAIN=R"_docs(/**
* \brief Returns the temporal definition domain of this trajectory
*
* \return an Interval object \f$[t_0,t_f]\f$
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_RESIZE_INT=R"_docs(/**
* \brief Resizes this TrajectoryVector
*
* \note If the size is increased, the existing components are not
*       modified and the new ones are set to empty trajectories
*
* \param n the new size to be set
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_SUBVECTOR_INT_INT=R"_docs(/**
* \brief Returns a subvector of this TrajectoryVector
*
* \param start_index first component index of the subvector to be returned
* \param end_index last component index of the subvector to be returned
* \return a TrajectoryVector extracted from this TrajectoryVector
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_PUT_INT_TRAJECTORYVECTOR=R"_docs(/**
* \brief Puts a subvector into this TrajectoryVector at a given position
*
* \param start_index position where the subvector will be put
* \param subvec the TrajectoryVector to put from start_index
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_CODOMAIN=R"_docs(/// @}
/// \name Accessing values
/// @{/**
* \brief Returns the box of feasible values
*
* \return an IntervalVector object \f$\mathbf{x}([t_0,t_f])\f$
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_OPERATOR_INDEX_INT=R"_docs(/**
* \brief Returns the ith Trajectory of this TrajectoryVector
*
* \param index the index of this ith component
* \return a reference to the ith component
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_OPERATOR_INDEX_INT1=R"_docs(/**
* \brief Returns a const reference to the ith Trajectory of this TrajectoryVector
*
* \param index the index of this ith component
* \return a const reference to the ith component
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_OPERATOR_CALL_DOUBLE=R"_docs(/**
* \brief Returns the evaluation of this trajectory at \f$t\f$
*
* \note Be careful, if the trajectory is defined from an analytic function,
*       then an approximation will be made (since the tubex::Function returns
*       a boxed evaluation, while the expected returned value is a real here).
*       Please use the operator(Interval(double)) for a reliable evaluation.
*
* \param t the temporal key (double, must belong to the trajectory domain)
* \return real vector value \f$\mathbf{x}(t)\f$
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_OPERATOR_CALL_INTERVAL=R"_docs(/**
* \brief Returns the interval evaluation of this trajectory over \f$[t]\f$
*
* \param t the subdomain (Interval, must be a subset of the trajectory domain)
* \return IntervalVector envelope \f$\mathbf{x}([t])\f$
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_FIRST_VALUE=R"_docs(/**
* \brief Returns the value \f$\mathbf{x}(t_0)\f$
*
* \return real value \f$\mathbf{x}(t_0)\f$
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_LAST_VALUE=R"_docs(/**
* \brief Returns the value \f$\mathbf{x}(t_f)\f$
*
* \return real value \f$\mathbf{x}(t_f)\f$
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_NOT_DEFINED=R"_docs(/// @}
/// \name Tests
/// @{/**
* \brief Tests whether this trajectory is defined or not
*
* \return false in case of non-empty map, or definition from a
*         tubex::Function object, true otherwise
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_EQ_TRAJECTORYVECTOR=R"_docs(/**
* \brief Returns true if this trajectory is equal to \f$\mathbf{x}(\cdot)\f$
*
* \param x the TrajectoryVector object
* \return true in case of equality
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_NE_TRAJECTORYVECTOR=R"_docs(/**
* \brief Returns true if this trajectory is different from \f$\mathbf{x}(\cdot)\f$
*
* \param x the TrajectoryVector object
* \return true in case of difference
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_SET_VECTOR_DOUBLE=R"_docs(/// @}
/// \name Setting values
/// @{/**
* \brief Sets a value \f$\mathbf{y}\f$ at \f$t\f$: \f$\mathbf{x}(t)=\mathbf{y}\f$
*
* \param y local vector value of the trajectory
* \param t the temporal key (double, must belong to the trajectory domain)
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_TRUNCATE_TDOMAIN_INTERVAL=R"_docs(/**
* \brief Truncates the domain of \f$\mathbf{x}(\cdot)\f$
*
* \note The new domain must be a subset of the old one
*
* \param domain new Interval domain \f$[t_0,t_f]\f$
* \return a reference to this trajectory
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_SHIFT_TDOMAIN_DOUBLE=R"_docs(/**
* \brief Shifts the domain \f$[t_0,t_f]\f$ of \f$\mathbf{x}(\cdot)\f$
*
* \note If the trajectory is defined from a map of values, the key
*       of each value will be shifted. In case of a definition from
*       an analytic function, only the definition domain will be changed.
*
* \param a the offset value so that \f$[t_0,t_f]:=[t_0-a,t_f-a]\f$
* \return a reference to this trajectory
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_SAMPLE_DOUBLE=R"_docs(/**
* \brief Transforms an analytic trajectory as a map of values
*
* \note Sampling only available for trajectories firstly defined as analytic functions
*
* \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
* \return a reference to this trajectory
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_SAMPLE_TRAJECTORY=R"_docs(/**
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

const char* DOCS_TRAJECTORYVECTOR_SAMPLE_TRAJECTORYVECTOR=R"_docs(/**
* \brief Samples this trajectory so that it will share the same sampling of \f$\mathbf{x}(\cdot)\f$
*
* \note If the trajectory is defined as an analytic function, then the object is
*       transformed into a map of values and the tubex::Function object is deleted.
*
* \note The previous sampling of this trajectory is preserved
*
* \param x the TrajectoryVector from which the new sampling will come from
* \return a reference to this trajectory
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_PRIMITIVE_VECTOR=R"_docs(/// @}
/// \name Integration
/// @{/**
* \brief Computes an approximative primitive of \f$\mathbf{x}(\cdot)\f$
*
* \note The trajectory must not be defined from an analytic function.
*       Please use the other method with time discretization for such case.
*
* \param c the constant of integration
* \return a new TrajectoryVector object with the same temporal keys
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_PRIMITIVE_VECTOR_DOUBLE=R"_docs(/**
* \brief Computes an approximative primitive of \f$\mathbf{x}(\cdot)\f$
*        whith some time discretization \f$\delta\f$
*
* \param c the constant of integration
* \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
* \return a new TrajectoryVector object with the specified time discretization
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_DIFF=R"_docs(/**
* \brief Differentiates this trajectory vector
*
* \note If the trajectory is defined from an analytic function,
*       the exact differentiation is returned. Otherwise, from a map
*       of values, finite differences are computed.
* 
* \return a derivative trajectory vector
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_IADD_DOUBLE=R"_docs(/// @}
/// \name Assignments operators
/// @{/**
* \brief Operates +=
*
* \param x double
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_IADD_TRAJECTORY=R"_docs(/**
* \brief Operates +=
*
* \param x Trajectory
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_IADD_VECTOR=R"_docs(/**
* \brief Operates +=
*
* \param x Vector
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_IADD_TRAJECTORYVECTOR=R"_docs(/**
* \brief Operates +=
*
* \param x TrajectoryVector
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_ISUB_DOUBLE=R"_docs(/**
* \brief Operates -=
*
* \param x double
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_ISUB_TRAJECTORY=R"_docs(/**
* \brief Operates -=
*
* \param x Trajectory
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_ISUB_VECTOR=R"_docs(/**
* \brief Operates -=
*
* \param x Vector
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_ISUB_TRAJECTORYVECTOR=R"_docs(/**
* \brief Operates -=
*
* \param x TrajectoryVector
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_IMUL_DOUBLE=R"_docs(/**
* \brief Operates *=
*
* \param x double
* \return (*this)*=x
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_IMUL_TRAJECTORY=R"_docs(/**
* \brief Operates *=
*
* \param x Trajectory
* \return (*this)*=x
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_ITRUEDIV_DOUBLE=R"_docs(/**
* \brief Operates /=
*
* \param x double
* \return (*this)/=x
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_ITRUEDIV_TRAJECTORY=R"_docs(/**
* \brief Operates /=
*
* \param x Trajectory
* \return (*this)/=x
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_CLASS_NAME=R"_docs(/// @}
/// \name String
/// @{/**
* \brief Returns the name of this class
*
* \note Only used for some generic display method
*
* \return the predefined name
*/
)_docs";

const char* DOCS_TRAJECTORYVECTOR_OPERATOR_OSTREAM_TRAJECTORYVECTOR=R"_docs(/**
* \brief Displays a synthesis of this trajectory
*
* \param str ostream
* \param x trajectory to be displayed
* \return ostream
*/
)_docs";

