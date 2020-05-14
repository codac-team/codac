
/** 
 *  ile
 *  TubeVector documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 const char* DOCS_TUBEVECTOR=R"_docs(/**
* \class TubeVector
* \brief n-dimensional tube \f$[\mathbf{x}](\cdot)\f$, defined as an interval of n-dimensional trajectories
*
* \note Use Tube for the one-dimensional case
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_INTERVAL_INT=R"_docs(/// \name Definition
/// @{/**
* \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ made of one slice
*
* \param domain Interval domain \f$[t_0,t_f]\f$
* \param n dimension of this tube (default value: 1)
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_INTERVAL_INTERVALVECTOR=R"_docs(/**
* \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ made of one slice
*
* \note The dimension of the tube is specified by the codomain box
*
* \param domain Interval domain \f$[t_0,t_f]\f$
* \param codomain IntervalVector value of the slice
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_INTERVAL_DOUBLE_INT=R"_docs(/**
* \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ with some temporal discretization
*
* \param domain Interval domain \f$[t_0,t_f]\f$
* \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
* \param n dimension of this tube (default value: 1)
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_INTERVAL_DOUBLE_INTERVALVECTOR=R"_docs(/**
* \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ with some temporal discretization
*
* \param domain Interval domain \f$[t_0,t_f]\f$
* \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
* \param codomain IntervalVector value of the slices
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_INTERVAL_DOUBLE_FNC=R"_docs(/**
* \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$
*        from a tubex::Fnc object and with some temporal discretization
*
* \note Due to the slicing implementation of the tube, a wrapping
*       effect will occur to reliably enclose the tubex::Fnc object
* \note The dimension of the tube is specified by the output of \f$[\mathbf{f}]\f$
*
* \param domain Interval domain \f$[t_0,t_f]\f$
* \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
* \param f tubex::Fnc object that will be enclosed by the tube:
*          \f$\forall t\in[t_0,t_f], [\mathbf{f}](t)\subseteq[\mathbf{x}](t)\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_VECTOR_INTERVAL__VECTOR_INTERVALVECTOR_=R"_docs(/**
* \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ from a list of \f$k\f$ boxes \f$\big([t_1]\times[\mathbf{x}_1],\dots,[t_k]\times[\mathbf{x}_k]\big)\f$
*
* \note The slicing will be based on the vector of temporal domains.
* \note The \f$[t_i]\f$'s must cover continuously the domain of \f$[\mathbf{x}](\cdot)\f$.
*
* \param v_domains vector of temporal domains \f$[t_i]\f$
* \param v_codomains vector of codomains \f$[\mathbf{x}_i]\f$ related to the \f$[t_i]\f$'s
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_INITIALIZER_LIST_TUBE_=R"_docs(/**
* \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ from a list of Tube objects
*
* \param list list of \f$[x_i](\cdot)\f$ tubes
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_TUBEVECTOR=R"_docs(/**
* \brief Creates a copy of a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$, with the same time discretization
*
* \param x TubeVector to be duplicated
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_TUBEVECTOR_INTERVALVECTOR=R"_docs(/**
* \brief Creates a copy of a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$, with the same time
*        discretization but a specific constant codomain
*
* \param x TubeVector from which the sampling will be duplicated
* \param codomain IntervalVector value of the slices
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_INT_TUBE=R"_docs(/**
* \brief Creates a n-dimensional tube with all the components initialized to \f$[x](\cdot)\f$
*
* \note The TubeVector object will have the same slicing as the tube \f$[x](\cdot)\f$
*
* \param n dimension of this tube
* \param x Tube to be copied for all the components
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_TRAJECTORYVECTOR_DOUBLE=R"_docs(/**
* \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ enclosing a n-dimensional
*        trajectory \f$\mathbf{x}(\cdot)\f$, possibly with some temporal discretization
*
* \note Due to the slicing implementation of the tube, a wrapping
*       effect will occur to reliably enclose the TrajectoryVector object 
*
* \param traj TrajectoryVector \f$\mathbf{x}(\cdot)\f$ to enclose
* \param timestep sampling value \f$\delta\f$ for the temporal
*        discretization (double, no discretization by default: one slice only)
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_TRAJECTORYVECTOR_TRAJECTORYVECTOR_DOUBLE=R"_docs(/**
* \brief Creates a n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ defined as an interval
*        of two n-dimensional trajectories \f$[\mathbf{lb}(\cdot),\mathbf{ub}(\cdot)]\f$
*
* \note Due to the slicing implementation of the tube, a wrapping
*       effect will occur to reliably enclose the TrajectoryVector object 
*
* \param lb TrajectoryVector defining the lower bound \f$\mathbf{x}^{-}(\cdot)\f$ of the tube
* \param ub TrajectoryVector defining the upper bound \f$\mathbf{x}^{+}(\cdot)\f$ of the tube
* \param timestep sampling value \f$\delta\f$ for the temporal
*        discretization (double, no discretization by default: one slice only)
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_STRING=R"_docs(/**
* \brief Restore a n-dimensional tube from serialization
*
* \note The TubeVector must have been serialized beforehand by the appropriate method serialize()
*
* \param binary_file_name path to the binary file
*/
)_docs";

const char* DOCS_TUBEVECTOR_TUBEVECTOR_STRING_TRAJECTORYVECTOR=R"_docs(/**
* \brief Restore a n-dimensional tube from serialization, together with a TrajectoryVector object
*
* \note The TubeVector and the TrajectoryVector must have been serialized
*       beforehand by the appropriate method serialize()
*
* \param binary_file_name path to the binary file
* \param traj a pointer to the TrajectoryVector object to be instantiated
*/
)_docs";

const char* DOCS_TUBEVECTOR_SIZE=R"_docs(/**
* \brief Returns the dimension of the tube
*
* \return n
*/
)_docs";

const char* DOCS_TUBEVECTOR_RESIZE_INT=R"_docs(/**
* \brief Resizes this TubeVector
*
* \note If the size is increased, the existing components are not
*       modified and the new ones are set to \f$[t_0,t_f]\mapsto[-\infty,\infty]\f$
*
* \param n the new size to be set
*/
)_docs";

const char* DOCS_TUBEVECTOR_SUBVECTOR_INT_INT=R"_docs(/**
* \brief Returns a subvector of this TubeVector
*
* \param start_index first component index of the subvector to be returned
* \param end_index last component index of the subvector to be returned
* \return a TubeVector extracted from this TubeVector
*/
)_docs";

const char* DOCS_TUBEVECTOR_PUT_INT_TUBEVECTOR=R"_docs(/**
* \brief Puts a subvector into this TubeVector at a given position
*
* \param start_index position where the subvector will be put
* \param subvec the TubeVector to put from start_index
*/
)_docs";

const char* DOCS_TUBEVECTOR_PRIMITIVE=R"_docs(/**
* \brief Returns the primitive TubeVector of this tube, with a zero constant of integration
*
* \return a new TubeVector object with same slicing, enclosing the feasible primitives of this tube
*/
)_docs";

const char* DOCS_TUBEVECTOR_PRIMITIVE_INTERVALVECTOR=R"_docs(/**
* \brief Returns the primitive TubeVector of this tube
*
* \param c the constant of integration
* \return a new TubeVector object with same slicing, enclosing the feasible primitives of this tube
*/
)_docs";

const char* DOCS_TUBEVECTOR_OPERATOR_TUBEVECTOR=R"_docs(/**
* \brief Returns a copy of a TubeVector
*
* \param x the TubeVector object to be copied
* \return a new TubeVector object with same slicing and values
*/
)_docs";

const char* DOCS_TUBEVECTOR_TDOMAIN=R"_docs(/**
* \brief Returns the temporal definition domain of this tube
*
* \return an Interval object \f$[t_0,t_f]\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_NB_SLICES=R"_docs(/// @}
/// \name Slices structure
/// @{/**
* \brief Returns the number of slices of this tube
*
* \return an integer
*/
)_docs";

const char* DOCS_TUBEVECTOR_INPUT2INDEX_DOUBLE=R"_docs(/**
* \brief Returns the Slice index related to the temporal key \f$t\f$
*
* \param t the temporal key (double, must belong to the TubeVector domain)
* \return an integer
*/
)_docs";

const char* DOCS_TUBEVECTOR_SAMPLE_DOUBLE=R"_docs(/**
* \brief Samples this tube at \f$t\f$
*
* \note Without any effect on one component that has two Slice objects
*       already defined at \f$t\f$ (if the gate \f$[\mathbf{x}](t)\f$ already exists)
*
* \param t the temporal key (double, must belong to the TubeVector domain)
*/
)_docs";

const char* DOCS_TUBEVECTOR_SAMPLE_DOUBLE_INTERVALVECTOR=R"_docs(/**
* \brief Samples this tube at \f$t\f$ with a specific gate value
*
* \note Without any effect on one component that has two Slice objects
*       already defined at \f$t\f$ (if the gate \f$[\mathbf{x}](t)\f$ already exists)
*
* \param t the temporal key (double, must belong to the TubeVector domain)
* \param gate the IntervalVector value of this tube at \f$t\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_SAMPLE_TUBE=R"_docs(/**
* \brief Samples this tube so that each component will share the same
*        sampling of the scalar \f$[x](\cdot)\f$
*
* \note The previous sampling of this tube is preserved
*
* \param x the Tube from which the new sampling will come from
*/
)_docs";

const char* DOCS_TUBEVECTOR_SAMPLE_TUBEVECTOR=R"_docs(/**
* \brief Samples this tube so that its components will share the same
*        sampling of the components of the n-dimensional \f$[\mathbf{x}](\cdot)\f$
*
* \note The previous sampling of this tube is preserved
*
* \param x the TubeVector from which the new sampling will come from
*/
)_docs";

const char* DOCS_TUBEVECTOR_CODOMAIN=R"_docs(/// @}
/// \name Accessing values
/// @{/**
* \brief Returns the box of feasible values
*
* \return an IntervalVector object \f$[\mathbf{x}]([t_0,t_f])\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_VOLUME=R"_docs(/**
* \brief Returns the volume of this tube
*
* \note returns POS_INFINITY if the codomain is unbounded
* \note returns 0 if the tube is flat (and so without wrapping effect)
*
* \return volume defined as \f$w([t_0,t_f])\times w([x_1]([t_0,t_f]))\times\dots\times w([x_n]([t_0,t_f]))\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_OPERATOR_INDEX_INT=R"_docs(/**
* \brief Returns the ith Tube of this TubeVector
*
* \param index the index of this ith component
* \return a reference to the ith component
*/
)_docs";

const char* DOCS_TUBEVECTOR_OPERATOR_INDEX_INT1=R"_docs(/**
* \brief Returns a const reference to the ith Tube of this TubeVector
*
* \param index the index of this ith component
* \return a const reference to the ith component
*/
)_docs";

const char* DOCS_TUBEVECTOR_OPERATOR_CALL_INT=R"_docs(/**
* \brief Returns the value of the ith slice
*
* \param slice_id the index of the ith slice
* \return IntervalVector value of \f$[\mathbf{x}](i)\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_OPERATOR_CALL_DOUBLE=R"_docs(/**
* \brief Returns the evaluation of this tube at \f$t\f$
*
* \param t the temporal key (double, must belong to the TubeVector domain)
* \return IntervalVector value of \f$[\mathbf{x}](t)\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_OPERATOR_CALL_INTERVAL=R"_docs(/**
* \brief Returns the interval evaluation of this tube over \f$[t]\f$
*
* \param t the subdomain (Interval, must be a subset of the TubeVector domain)
* \return IntervalVector envelope \f$[\mathbf{x}]([t])\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_EVAL_INTERVAL=R"_docs(/**
* \brief Returns the interval evaluations of the bounds of the
*        tube \f$\underline{\mathbf{x}^-}(\cdot)\f$ and \f$\overline{\mathbf{x}^+}(\cdot)\f$ over \f$[t]\f$
*
* \param t the subdomain (Interval, must be a subset of the TubeVector domain)
* \return the pair \f$\big([\underline{\mathbf{x}^-}]([t]),[\overline{\mathbf{x}^+}]([t])\big)\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_INTERPOL_DOUBLE_TUBEVECTOR=R"_docs(/**
* \brief Returns the optimal evaluation of this tube at \f$t\f$,
*        based on the derivative information \f$\dot{\mathbf{x}}(\cdot)\f$
*
* \todo Change the name of this method?
*
* \param t the temporal key (double, must belong to the TubeVector domain)
* \param v the derivative tube such that \f$\dot{\mathbf{x}}(\cdot)\in[\mathbf{v}](\cdot)\f$
* \return IntervalVector value of \f$[\mathbf{x}](t)\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_INTERPOL_INTERVAL_TUBEVECTOR=R"_docs(/**
* \brief Returns the optimal interval evaluation of this tube over \f$[t]\f$,
*        based on the derivative information \f$\dot{\mathbf{x}}(\cdot)\f$
*
* \todo Change the name of this method?
*
* \param t the subdomain (Interval, must be a subset of the TubeVector domain)
* \param v the derivative tube such that \f$\dot{\mathbf{x}}(\cdot)\in[\mathbf{v}](\cdot)\f$
* \return IntervalVector value of \f$[\mathbf{x}]([t])\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_INVERT_INTERVALVECTOR_INTERVAL=R"_docs(/**
* \brief Returns the interval inversion \f$[\mathbf{x}]^{-1}([\mathbf{y}])\f$
*
* \note If the inversion results in several pre-images, their union is returned
*
* \param y the box codomain
* \param search_domain the optional interval domain on which the inversion will be performed
* \return the hull of \f$[\mathbf{x}]^{-1}([\mathbf{y}])\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_INVERT_INTERVALVECTOR_VECTOR_INTERVAL__INTERVAL=R"_docs(/**
* \brief Computes the set of continuous values of the inversion \f$[\mathbf{x}]^{-1}([\mathbf{y}])\f$
*
* \param y the interval codomain
* \param v_t the vector of the sub-domains \f$[t_k]\f$ for which
*            \f$\forall t\in[t_k] \mid \mathbf{x}(t)\in[\mathbf{y}], \mathbf{x}(\cdot)\in[\mathbf{x}](\cdot)\f$
* \param search_domain the optional interval domain on which the inversion will be performed
*/
)_docs";

const char* DOCS_TUBEVECTOR_INVERT_INTERVALVECTOR_TUBEVECTOR_INTERVAL=R"_docs(/**
* \brief Returns the optimal interval inversion \f$[\mathbf{x}]^{-1}([\mathbf{y}])\f$
*
* \note The knowledge of the derivative tube \f$[\mathbf{v}](\cdot)\f$ allows a finer inversion
* \note If the inversion results in several pre-images, their union is returned
*
* \param y the interval codomain
* \param v the derivative tube vector such that \f$\dot{\mathbf{x}}(\cdot)\in[\mathbf{v}](\cdot)\f$
* \param search_domain the optional interval domain on which the inversion will be performed
* \return the hull of \f$[\mathbf{x}]^{-1}([\mathbf{y}])\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_INVERT_INTERVALVECTOR_VECTOR_INTERVAL__TUBEVECTOR_INTERVAL=R"_docs(/**
* \brief Computes the set of continuous values of the optimal inversion \f$[\mathbf{x}]^{-1}([\mathbf{y}])\f$
*
* \note The knowledge of the derivative tube \f$[\mathbf{v}](\cdot)\f$ allows finer inversions
*
* \param y the interval codomain
* \param v_t the vector of the sub-domains \f$[t_k]\f$ for which
*            \f$\exists t\in[t_k] \mid \mathbf{x}(t)\in[\mathbf{y}], \mathbf{x}(\cdot)\in[\mathbf{x}](\cdot), \dot{\mathbf{x}}(\cdot)\in[\mathbf{v}](\cdot)\f$
* \param v the derivative tube such that \f$\dot{\mathbf{x}}(\cdot)\in[\mathbf{v}](\cdot)\f$
* \param search_domain the optional interval domain on which the inversion will be performed
*/
)_docs";

const char* DOCS_TUBEVECTOR_MAX_DIAM=R"_docs(/**
* \brief Returns a vector of the maximum diameters of the tube for each component
*
* \return the maximal thicknesses of this tube
*/
)_docs";

const char* DOCS_TUBEVECTOR_DIAM_BOOL=R"_docs(/**
* \brief Returns the diameters of the tube as a trajectory
*
* \note Without derivative knowledge, and because the tube is made of boxed slices,
*       the trajectory will be discontinuous and so the returned object will not
*       reliably represent the diameters. It can be mainly used for display purposes.
*
* \param gates_thicknesses if true, the diameters of the gates will be evaluated too
* \return the set of diameters associated to temporal inputs
*/
)_docs";

const char* DOCS_TUBEVECTOR_DIAM_TUBEVECTOR=R"_docs(/**
* \brief Returns the diameters of the tube as a trajectory
*
* \note Because the tube is made of boxed slices,
*       the trajectory may be discontinuous and so the returned object will not
*       reliably represent the diameters. It can be mainly used for display purposes.
*
* \param v the derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
* \return the set of diameters associated to temporal inputs
*/
)_docs";

const char* DOCS_TUBEVECTOR_DIAG_BOOL=R"_docs(/**
* \brief Returns the slices diagonals of the tube as a trajectory
*
* \note Without derivative knowledge, and because the tube is made of boxed slices,
*       the trajectory will be discontinuous and so the returned object will not
*       reliably represent the diagonals. It can be mainly used for display purposes.
*
* \param gates_diag if true, the diagonals of the gates will be evaluated too
* \return the set of diagonals associated to temporal inputs
*/
)_docs";

const char* DOCS_TUBEVECTOR_DIAG_INT_INT_BOOL=R"_docs(/**
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
)_docs";

const char* DOCS_TUBEVECTOR_EQ_TUBEVECTOR=R"_docs(/// @}
/// \name Tests
/// @{/**
* \brief Returns true if this tube is equal to \f$[\mathbf{x}](\cdot)\f$
*
* \note Equality is obtained if the tubes share
*       the same bounds, domain and sampling
*
* \param x the TubeVector object
* \return true in case of equality
*/
)_docs";

const char* DOCS_TUBEVECTOR_NE_TUBEVECTOR=R"_docs(/**
* \brief Returns true if this tube is different from \f$[\mathbf{x}](\cdot)\f$
*
* \note The two tubes are different if they do not share
*       the same bounds, domain or sampling
*
* \param x the TubeVector object
* \return true in case of difference
*/
)_docs";

const char* DOCS_TUBEVECTOR_IS_SUBSET_TUBEVECTOR=R"_docs(/**
* \brief Returns true if this tube is a subset of \f$[\mathbf{x}](\cdot)\f$
*
* \note The two tubes may not share the same slicing,
*       but must have the same definition domain
*
* \param x the TubeVector object
* \return true in case of subset
*/
)_docs";

const char* DOCS_TUBEVECTOR_IS_STRICT_SUBSET_TUBEVECTOR=R"_docs(/**
* \brief Returns true if this tube is a subset of \f$[\mathbf{x}](\cdot)\f$,
*        and not \f$[\mathbf{x}](\cdot)\f$ itself
*
* \note The two tubes may not share the same slicing,
*       but must have the same definition domain
*
* \param x the TubeVector object
* \return true in case of strict subset
*/
)_docs";

const char* DOCS_TUBEVECTOR_IS_INTERIOR_SUBSET_TUBEVECTOR=R"_docs(/**
* \brief Returns true if this tube is a subset of the interior of \f$[\mathbf{x}](\cdot)\f$
*
* \note The two tubes may not share the same slicing,
*       but must have the same definition domain
*
* \param x the TubeVector object
* \return true in case of interior subset
*/
)_docs";

const char* DOCS_TUBEVECTOR_IS_STRICT_INTERIOR_SUBSET_TUBEVECTOR=R"_docs(/**
* \brief Returns true if this tube is a subset of the interior
*        of \f$[\mathbf{x}](\cdot)\f$, and not \f$[\mathbf{x}](\cdot)\f$ itself
*
* \note The two tubes may not share the same slicing,
*       but must have the same definition domain
*
* \param x the TubeVector object
* \return true in case of strict interior subset
*/
)_docs";

const char* DOCS_TUBEVECTOR_IS_SUPERSET_TUBEVECTOR=R"_docs(/**
* \brief Returns true if this tube is a superset of \f$[\mathbf{x}](\cdot)\f$
*
* \note The two tubes may not share the same slicing,
*       but must have the same definition domain
*
* \param x the TubeVector object
* \return true in case of superset
*/
)_docs";

const char* DOCS_TUBEVECTOR_IS_STRICT_SUPERSET_TUBEVECTOR=R"_docs(/**
* \brief Returns true if this tube is a superset of \f$[\mathbf{x}](\cdot)\f$,
*        and not \f$[\mathbf{x}](\cdot)\f$ itself
*
* \note The two tubes may not share the same slicing,
*       but must have the same definition domain
*
* \param x the TubeVector object
* \return true in case of strict superset
*/
)_docs";

const char* DOCS_TUBEVECTOR_IS_EMPTY=R"_docs(/**
* \brief Returns true if this tube is empty
*
* \note If \f$[\mathbf{x}](t)=\varnothing\f$ for some \f$t\f$, then the tube
*       \f$[\mathbf{x}](\cdot)\f$ is considered empty
*
* \return true in case of emptiness
*/
)_docs";

const char* DOCS_TUBEVECTOR_CONTAINS_TRAJECTORYVECTOR=R"_docs(/**
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
)_docs";

const char* DOCS_TUBEVECTOR_OVERLAPS_TUBEVECTOR_FLOAT=R"_docs(/**
* \brief Returns true if this tube overlaps the tube \f$[\mathbf{x}](\cdot)\f$
*
* \param x the other tube
* \param ratio an optional overlapping ratio between 0 and 1 (1 by default).
*              For instance, if ratio=0.3 and there is an overlapping of at least 30%,
*              then the function returns true
* \return true in case of overlapping with respect to the defined ratio
*/
)_docs";

const char* DOCS_TUBEVECTOR_SET_INTERVALVECTOR=R"_docs(/// @}
/// \name Setting values
/// @{/**
* \brief Sets a constant box value for this tube: \f$\forall t, [\mathbf{x}](t)=[\mathbf{y}]\f$
*
* \note The sampling of this tube is preserved
*
* \param y IntervalVector value of the slices
*/
)_docs";

const char* DOCS_TUBEVECTOR_SET_INTERVALVECTOR_INT=R"_docs(/**
* \brief Sets the box value of the ith slice of this tube
*
* \param y IntervalVector value of the ith slice
* \param slice_id index of the ith Slice
*/
)_docs";

const char* DOCS_TUBEVECTOR_SET_INTERVALVECTOR_DOUBLE=R"_docs(/**
* \brief Sets the box value of this tube at \f$t\f$: \f$[\mathbf{x}](t)=[\mathbf{y}]\f$
*
* \note It may create a gate (and so one more slice) if the tube
*       is not already sampled at \f$t\f$. Otherwise, it will
*       update the value of the already existing gate.
*
* \param y IntervalVector value of the gate
* \param t the temporal key (double, must belong to the TubeVector domain)
*/
)_docs";

const char* DOCS_TUBEVECTOR_SET_INTERVALVECTOR_INTERVAL=R"_docs(/**
* \brief Sets the box value of this tube over \f$[t]\f$: \f$\forall t\in[t], [\mathbf{x}](t)=[\mathbf{y}]\f$
*
* \note It may create two gates (and so further slices) if the tube
*       is not already sampled at \f$t^-\f$ and \f$t^+\f$. This is
*       done to ensure that \f$\forall t\in[t], [\mathbf{x}](t)=[\mathbf{y}]\f$.
*
* \param y IntervalVector value to be set
* \param t the subdomain (Interval, must be a subset of the TubeVector domain)
*/
)_docs";

const char* DOCS_TUBEVECTOR_SET_EMPTY=R"_docs(/**
* \brief Sets this tube to the empty set
*
* \note By convention, all slices will be set to the empty set
*/
)_docs";

const char* DOCS_TUBEVECTOR_INFLATE_DOUBLE=R"_docs(/**
* \brief Inflates this tube by adding \f$[-\mathbf{rad},+\mathbf{rad}]\f$ to all its codomain components
*
* \note All slices and gates will be inflated
*
* \param rad half of the inflation
* \return *this
*/
)_docs";

const char* DOCS_TUBEVECTOR_INFLATE_VECTOR=R"_docs(/**
* \brief Inflates this tube by adding \f$[-\mathbf{rad},+\mathbf{rad}]\f$ to all its codomain components
*
* \note All slices and gates will be inflated
*
* \param rad half of the inflation for each component
* \return *this
*/
)_docs";

const char* DOCS_TUBEVECTOR_INFLATE_TRAJECTORYVECTOR=R"_docs(/**
* \brief Inflates this tube by adding non-constant uncertainties defined in a trajectory 
*
* \note From the trajectory \f$\mathbf{a}(\cdot)\f$, the function will 
*       inflate this tube such that \f$\forall t, [\mathbf{x}](t):=[\mathbf{x}](t)+[-\mathbf{a}(t),+\mathbf{a}(t)]\f$
* \note All slices and gates will be inflated
*
* \param rad the TrajectoryVector object defining the non-constant inflation
* \return *this
*/
)_docs";

const char* DOCS_TUBEVECTOR_SHIFT_TDOMAIN_DOUBLE=R"_docs(/**
* \brief Shifts the domain \f$[t_0,t_f]\f$ of \f$[\mathbf{x}](\cdot)\f$
*
* \param a the offset value so that \f$[t_0,t_f]:=[t_0-a,t_f-a]\f$ 
*/
)_docs";

const char* DOCS_TUBEVECTOR_BISECT_DOUBLE_FLOAT=R"_docs(/// @}
/// \name Bisection
/// @{/**
* \brief Bisects this tube
*
* \note The tube is bisected along the codomain and according to a defined ratio
* \note The bisection is performed on the largest component of the gate \f$[\mathbf{x}](t)\f$
* \note If the tube is not already sampled at \f$t\f$, then a sampling is performed
*
* \param t the temporal key (double, must belong to the TubeVector domain)
* \param ratio the bisection ratio (default value: 0.49)
* \return a pair of two TubeVector objects resulting from the bisection
*/
)_docs";

const char* DOCS_TUBEVECTOR_BISECT_DOUBLE_INT_FLOAT=R"_docs(/**
* \brief Bisects this tube along a specific dimension
*
* \note The tube is bisected along the codomain and according to a defined ratio
* \note The bisection is performed on the gate \f$[x_i](t)\f$
* \note If the tube is not already sampled at \f$t\f$, then a sampling is performed
*
* \param t the temporal key (double, must belong to the TubeVector domain)
* \param dim the dimension id
* \param ratio the bisection ratio (default value: 0.49)
* \return a pair of two TubeVector objects resulting from the bisection
*/
)_docs";

const char* DOCS_TUBEVECTOR_IADD_INTERVAL=R"_docs(/// @}
/// \name Assignments operators
/// @{/**
* \brief Operates +=
*
* \param x Interval
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IADD_TRAJECTORY=R"_docs(/**
* \brief Operates +=
*
* \param x Trajectory
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IADD_TUBE=R"_docs(/**
* \brief Operates +=
*
* \param x Tube
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IADD_INTERVALVECTOR=R"_docs(/**
* \brief Operates +=
*
* \param x IntervalVector
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IADD_TRAJECTORYVECTOR=R"_docs(/**
* \brief Operates +=
*
* \param x TrajectoryVector
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IADD_TUBEVECTOR=R"_docs(/**
* \brief Operates +=
*
* \param x TubeVector
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_ISUB_INTERVAL=R"_docs(/**
* \brief Operates -=
*
* \param x Interval
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_ISUB_TRAJECTORY=R"_docs(/**
* \brief Operates -=
*
* \param x Trajectory
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_ISUB_TUBE=R"_docs(/**
* \brief Operates -=
*
* \param x Tube
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_ISUB_INTERVALVECTOR=R"_docs(/**
* \brief Operates -=
*
* \param x IntervalVector
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_ISUB_TRAJECTORYVECTOR=R"_docs(/**
* \brief Operates -=
*
* \param x TrajectoryVector
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_ISUB_TUBEVECTOR=R"_docs(/**
* \brief Operates -=
*
* \param x TubeVector
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IMUL_INTERVAL=R"_docs(/**
* \brief Operates *=
*
* \param x Interval
* \return (*this)*=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IMUL_TRAJECTORY=R"_docs(/**
* \brief Operates *=
*
* \param x Trajectory
* \return (*this)*=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IMUL_TUBE=R"_docs(/**
* \brief Operates *=
*
* \param x Tube
* \return (*this)*=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_ITRUEDIV_INTERVAL=R"_docs(/**
* \brief Operates /=
*
* \param x Interval
* \return (*this)/=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_ITRUEDIV_TRAJECTORY=R"_docs(/**
* \brief Operates /=
*
* \param x Trajectory
* \return (*this)/=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_ITRUEDIV_TUBE=R"_docs(/**
* \brief Operates /=
*
* \param x Tube
* \return (*this)/=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IOR_INTERVALVECTOR=R"_docs(/**
* \brief Operates |=
*
* \param x IntervalVector
* \return (*this)|=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IOR_TRAJECTORYVECTOR=R"_docs(/**
* \brief Operates |=
*
* \param x TrajectoryVector
* \return (*this)|=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IOR_TUBEVECTOR=R"_docs(/**
* \brief Operates |=
*
* \param x TubeVector
* \return (*this)|=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IAND_INTERVALVECTOR=R"_docs(/**
* \brief Operates &=
*
* \param x IntervalVector
* \return (*this)&=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IAND_TRAJECTORYVECTOR=R"_docs(/**
* \brief Operates &=
*
* \param x TrajectoryVector
* \return (*this)&=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_IAND_TUBEVECTOR=R"_docs(/**
* \brief Operates &=
*
* \param x TubeVector
* \return (*this)&=x
*/
)_docs";

const char* DOCS_TUBEVECTOR_CLASS_NAME=R"_docs(/// @}
/// \name String
/// @{/**
* \brief Returns the name of this class
*
* \note Only used for some generic display method
*
* \return the predefined name
*/
)_docs";

const char* DOCS_TUBEVECTOR_OPERATOR_OSTREAM_TUBEVECTOR=R"_docs(/**
* \brief Displays a synthesis of this tube
*
* \param str ostream
* \param x tube to be displayed
* \return ostream
*/
)_docs";

const char* DOCS_TUBEVECTOR_ENABLE_SYNTHESIS_BOOL=R"_docs(/// @}
/// \name Tree synthesis structure
/// @{/**
* \brief Enables the computation of a synthesis tree
*
* \note The synthesis tree speeds up computations such as integrals or evaluations
*
* \param enable boolean
*/
)_docs";

const char* DOCS_TUBEVECTOR_INTEGRAL_DOUBLE=R"_docs(/// @}
/// \name Integration
/// @{/**
* \brief Computes the box integral \f$\int_0^t[\mathbf{x}](\tau)d\tau\f$
*
* \note From the monotonicity of the integral operator, 
*       \f$\int_0^t[\mathbf{x}](\tau)d\tau=[\int_0^t\mathbf{x}^-(\tau)d\tau,\int_0^t\mathbf{x}^+(\tau)d\tau]\f$
*
* \param t the temporal key (double, must belong to the TubeVector domain)
* \return the set of feasible integral vectors
*/
)_docs";

const char* DOCS_TUBEVECTOR_INTEGRAL_INTERVAL=R"_docs(/**
* \brief Computes the box integral \f$\int_0^{[t]}[\mathbf{x}](\tau)d\tau\f$
*
* \note From the monotonicity of the integral operator, 
*       \f$\int_0^{[t]}[\mathbf{x}](\tau)d\tau=[\int_0^{[t]}\mathbf{x}^-(\tau)d\tau,\int_0^{[t]}\mathbf{x}^+(\tau)d\tau]\f$
*
* \param t the subdomain (Interval, must be a subset of the TubeVector domain)
* \return the set of feasible integral vectors
*/
)_docs";

const char* DOCS_TUBEVECTOR_INTEGRAL_INTERVAL_INTERVAL=R"_docs(/**
* \brief Computes the box integral \f$\int_{[t_1]}^{[t_2]}[\mathbf{x}](\tau)d\tau\f$
*
* \note From the monotonicity of the integral operator, 
*       \f$\int_{[t_1]}^{[t_2]}[\mathbf{x}](\tau)d\tau=[\int_{[t_1]}^{[t_2]}\mathbf{x}^-(\tau)d\tau,\int_{[t_1]}^{[t_2]}\mathbf{x}^+(\tau)d\tau]\f$
*
* \param t1 lower bound, subset of the TubeVector domain
* \param t2 upper bound, subset of the TubeVector domain
* \return the set of feasible integral vectors
*/
)_docs";

const char* DOCS_TUBEVECTOR_PARTIAL_INTEGRAL_INTERVAL=R"_docs(/**
* \brief Computes the partial box integral \f$\int_{0}^{[t]}[\mathbf{x}](\tau)d\tau\f$
*
* \note From the monotonicity of the integral operator, 
*       \f$\int_{0}^{[t]}[\mathbf{x}](\tau)d\tau=[\int_{0}^{[t]}\mathbf{x}^-(\tau)d\tau,\int_{0}^{[t]}\mathbf{x}^+(\tau)d\tau]\f$
*
* \param t interval upper bound, subset of the TubeVector domain
* \return the pair \f$\big([i^-],[i^+]\big)\f$,
*         where \f$[i^-]=\int_{0}^{[t]}\mathbf{x}^-(\tau)d\tau\f$ 
*         and \f$[i^+]=\int_{0}^{[t]}\mathbf{x}^+(\tau)d\tau\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_PARTIAL_INTEGRAL_INTERVAL_INTERVAL=R"_docs(/**
* \brief Computes the partial box integral \f$\int_{[t_1]}^{[t_2]}[\mathbf{x}](\tau)d\tau\f$
*
* \note From the monotonicity of the integral operator, 
*       \f$\int_{[t_1]}^{[t_2]}[\mathbf{x}](\tau)d\tau=[\int_{[t_1]}^{[t_2]}\mathbf{x}^-(\tau)d\tau,\int_{[t_1]}^{[t_2]}\mathbf{x}^+(\tau)d\tau]\f$
*
* \param t1 interval lower bound, subset of the TubeVector domain
* \param t2 interval upper bound, subset of the TubeVector domain
* \return the pair \f$\big([i^-],[i^+]\big)\f$,
*         where \f$[i^-]=\int_{[t_1]}^{[t_2]}\mathbf{x}^-(\tau)d\tau\f$ 
*         and \f$[i^+]=\int_{[t_1]}^{[t_2]}\mathbf{x}^+(\tau)d\tau\f$
*/
)_docs";

const char* DOCS_TUBEVECTOR_SERIALIZE_STRING_INT=R"_docs(/// @}
/// \name Serialization
/// @{/**
* \brief Serializes this tube
*
* \note The values and sampling (slices and gates) are serialized
*
* \param binary_file_name name of the output file (default value: "x.tube")
* \param version_number serialization version (used for tests purposes, default value: last version)
*/
)_docs";

const char* DOCS_TUBEVECTOR_SERIALIZE_STRING_TRAJECTORYVECTOR_INT=R"_docs(/**
* \brief Serializes this tube together with a TrajectoryVector object
*
* \note The values and sampling (slices and gates) are serialized
* \note The serialization of a TrajectoryVector defined from a tubex::Function object is not supported
* \note The output file will appear in the executable current directory
*
* \param binary_file_name name of the output file (default value: "x.tube")
* \param traj the TrajectoryVector object to serialize (for instance, actual but unknown values)
* \param version_number serialization version (used for tests purposes, default value: last version)
*/
)_docs";

const char* DOCS_TUBEVECTOR_SAME_SLICING_TUBEVECTOR_TUBE=R"_docs(/// @}/**
* \brief Tests whether a TubeVector object shares the slicing of another Tube object
*
* \note If true, it means the two tubes are defined with the same
*       amount of slices and identical sampling
*
* \param x1 the TubeVector
* \param x2 the Tube
* \return true in case of same slicing
*/
)_docs";

const char* DOCS_TUBEVECTOR_SAME_SLICING_TUBEVECTOR_TUBEVECTOR=R"_docs(/**
* \brief Tests whether the two TubeVector objects are sharing the same slicing
*
* \note If true, it means the two tubes are defined with the same
*       amount of slices and identical sampling
*
* \param x1 the first TubeVector
* \param x2 the second TubeVector
* \return true in case of same slicing
*/
)_docs";

const char* DOCS_TUBEVECTOR_HULL_LIST_TUBEVECTOR_=R"_docs(/**
* \brief Computes the hull of several tubes
*
* \param l_tubes list of tubes
* \return the tube vector enveloping the other ones
*/
)_docs";

