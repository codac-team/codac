
/** 
 *  ile
 *  Tube documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 const char* DOCS_TUBE=R"_docs(/**
* \class Tube
* \brief One dimensional tube \f$[x](\cdot)\f$, defined as an interval of scalar trajectories
*
* \note Use TubeVector for the multi-dimensional case
*/
)_docs";

const char* DOCS_TUBE_TUBE_INTERVAL_INTERVAL=R"_docs(/// \name Definition
/// @{/**
* \brief Creates a scalar tube \f$[x](\cdot)\f$ made of one slice
*
* \param domain Interval domain \f$[t_0,t_f]\f$
* \param codomain Interval value of the slice (all reals \f$[-\infty,\infty]\f$ by default)
*/
)_docs";

const char* DOCS_TUBE_TUBE_INTERVAL_DOUBLE_INTERVAL=R"_docs(/**
* \brief Creates a scalar tube \f$[x](\cdot)\f$ with some temporal discretization
*
* \param domain Interval domain \f$[t_0,t_f]\f$
* \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
* \param codomain Interval value of the slices (all reals \f$[-\infty,\infty]\f$ by default)
*/
)_docs";

const char* DOCS_TUBE_TUBE_INTERVAL_DOUBLE_FNC_INT=R"_docs(/**
* \brief Creates a scalar tube \f$[x](\cdot)\f$ from a tubex::Fnc object and with some temporal discretization
*
* \note Due to the slicing implementation of the tube, a wrapping
*       effect will occur to reliably enclose the tubex::Fnc object 
* \param domain Interval domain \f$[t_0,t_f]\f$
* \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
* \param f tubex::Fnc object that will be enclosed by the tube:
*          \f$\forall t\in[t_0,t_f], [f](t)\subseteq[x](t)\f$
* \param f_image_id component index of the interval function
*                   \f$[f]\f$ (that is possibly multidimensional, first component by default)
*/
)_docs";

const char* DOCS_TUBE_TUBE_VECTOR_INTERVAL__VECTOR_INTERVAL_=R"_docs(/**
* \brief Creates a tube \f$[x](\cdot)\f$ from a list of \f$k\f$ boxes \f$\big([t_1]\times[x_1],\dots,[t_k]\times[x_k]\big)\f$
*
* \note The slicing will be based on the vector of temporal domains.
* \note The \f$[t_i]\f$'s must cover continuously the domain of \f$[x](\cdot)\f$.
*
* \param v_domains vector of temporal domains \f$[t_i]\f$
* \param v_codomains vector of codomains \f$[x_i]\f$ related to the \f$[t_i]\f$'s
*/
)_docs";

const char* DOCS_TUBE_TUBE_TUBE=R"_docs(/**
* \brief Creates a copy of a scalar tube \f$[x](\cdot)\f$, with the same time discretization
*
* \param x Tube to be duplicated
*/
)_docs";

const char* DOCS_TUBE_TUBE_TUBE_INTERVAL=R"_docs(/**
* \brief Creates a copy of a scalar tube \f$[x](\cdot)\f$, with the same time
*        discretization but a specific constant codomain
*
* \param x Tube from which the sampling will be duplicated
* \param codomain Interval value of the slices
*/
)_docs";

const char* DOCS_TUBE_TUBE_TUBE_FNC_INT=R"_docs(/**
* \brief Creates a copy of a scalar tube \f$[x](\cdot)\f$, with the same time
*        discretization but a specific codomain defined by a tubex::Fnc object
*
* \note Due to the slicing implementation of the tube, a wrapping
*       effect will occur to reliably enclose the tubex::Fnc object 
*
* \param x Tube from which the sampling will be duplicated
* \param f tubex::Fnc object that will be enclosed by the tube:
*                     \f$\forall t\in[t_0,t_f], [f](t)\subseteq[x](t)\f$
* \param f_image_id component index of the interval function \f$[f]\f$
*                   (that is possibly multidimensional, first component by default)
*/
)_docs";

const char* DOCS_TUBE_TUBE_TRAJECTORY_DOUBLE=R"_docs(/**
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
)_docs";

const char* DOCS_TUBE_TUBE_TRAJECTORY_TRAJECTORY_DOUBLE=R"_docs(/**
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
)_docs";

const char* DOCS_TUBE_TUBE_STRING=R"_docs(/**
* \brief Restore a scalar tube from serialization
*
* \note The Tube must have been serialized beforehand by the appropriate method serialize()
*
* \param binary_file_name path to the binary file
*/
)_docs";

const char* DOCS_TUBE_TUBE_STRING_TRAJECTORY=R"_docs(/**
* \brief Restore a scalar tube from serialization, together with a Trajectory object
*
* \note The Tube and the Trajectory must have been serialized
*       beforehand by the appropriate method serialize()
*
* \param binary_file_name path to the binary file
* \param traj a pointer to the Trajectory object to be instantiated
*/
)_docs";

const char* DOCS_TUBE_SIZE=R"_docs(/**
* \brief Returns the dimension of the scalar tube (always 1)
*
* \return 1
*/
)_docs";

const char* DOCS_TUBE_PRIMITIVE_INTERVAL=R"_docs(/**
* \brief Returns the primitive Tube of this tube
*
* \param c the constant of integration (0. by default)
* \return a new Tube object with same slicing, enclosing the feasible primitives of this tube
*/
)_docs";

const char* DOCS_TUBE_OPERATOR_TUBE=R"_docs(/**
* \brief Returns a copy of a Tube
*
* \param x the Tube object to be copied
* \return a new Tube object with same slicing and values
*/
)_docs";

const char* DOCS_TUBE_TDOMAIN=R"_docs(/**
* \brief Returns the temporal definition domain of this tube
*
* \return an Interval object \f$[t_0,t_f]\f$
*/
)_docs";

const char* DOCS_TUBE_POLYGON_ENVELOPE=R"_docs(/**
* \brief Returns the polygon envelope of this tube
*
* \return a Polygon object enclosing the slices
*/
)_docs";

const char* DOCS_TUBE_NB_SLICES=R"_docs(/// @}
/// \name Slices structure
/// @{/**
* \brief Returns the number of slices of this tube
*
* \return an integer
*/
)_docs";

const char* DOCS_TUBE_SLICE_INT=R"_docs(/**
* \brief Returns a pointer to the ith Slice object of this tube
*
* \param slice_id the index of the ith Slice
* \return a pointer to the corresponding Slice
*/
)_docs";

const char* DOCS_TUBE_SLICE_INT1=R"_docs(/**
* \brief Returns a constant pointer to the ith Slice object of this tube
*
* \param slice_id the index of the ith Slice
* \return a const pointer to the corresponding Slice
*/
)_docs";

const char* DOCS_TUBE_SLICE_DOUBLE=R"_docs(/**
* \brief Returns a pointer to the Slice object of this tube that is defined at \f$t\f$
*
* \note If two Slices are defined at \f$t\f$ (common domain),
*       then the first Slice is considered
*
* \param t the temporal key (double, must belong to the Tube domain)
* \return a pointer to the corresponding Slice
*/
)_docs";

const char* DOCS_TUBE_SLICE_DOUBLE1=R"_docs(/**
* \brief Returns a constant pointer to the Slice object of this tube that is defined at \f$t\f$
*
* \note If two Slices are defined at \f$t\f$ (common domain),
*       then the first Slice is considered
*
* \param t the temporal key (double, must belong to the Tube domain)
* \return a const pointer to the corresponding Slice
*/
)_docs";

const char* DOCS_TUBE_FIRST_SLICE=R"_docs(/**
* \brief Returns a pointer to the first Slice object of this tube
*
* \return a pointer to the corresponding Slice
*/
)_docs";

const char* DOCS_TUBE_FIRST_SLICE1=R"_docs(/**
* \brief Returns a constant pointer to the first Slice object of this tube
*
* \return a const pointer to the corresponding Slice
*/
)_docs";

const char* DOCS_TUBE_LAST_SLICE=R"_docs(/**
* \brief Returns a pointer to the last Slice object of this tube
*
* \return a pointer to the corresponding Slice
*/
)_docs";

const char* DOCS_TUBE_LAST_SLICE1=R"_docs(/**
* \brief Returns a constant pointer to the last Slice object of this tube
*
* \return a const pointer to the corresponding Slice
*/
)_docs";

const char* DOCS_TUBE_WIDER_SLICE=R"_docs(/**
* \brief Returns a pointer to the Slice defined over the wider temporal domain
*
* \note If two Slice objects have the same domain's width, then the first one is considered
*
* \return a pointer to the corresponding Slice
*/
)_docs";

const char* DOCS_TUBE_WIDER_SLICE1=R"_docs(/**
* \brief Returns a constant pointer to the Slice defined over the wider temporal domain
*
* \note If two Slice objects have the same domain's width, then the first one is considered
*
* \return a const pointer to the corresponding Slice
*/
)_docs";

const char* DOCS_TUBE_LARGEST_SLICE=R"_docs(/**
* \brief Returns a pointer to the Slice object of this tube for which
*        the interval value is the more uncertain
*
* \return a pointer to the corresponding Slice
*/
)_docs";

const char* DOCS_TUBE_LARGEST_SLICE1=R"_docs(/**
* \brief Returns a constant pointer to the Slice object of this tube for
*        which the interval value is the more uncertain
*
* \return a const pointer to the corresponding Slice
*/
)_docs";

const char* DOCS_TUBE_SLICE_TDOMAIN_INT=R"_docs(/**
* \brief Returns the temporal definition domain of the ith Slice of this tube
*
* \param slice_id the index of the ith Slice
* \return an Interval object \f$[t_0^i,t_f^i]\f$
*/
)_docs";

const char* DOCS_TUBE_INPUT2INDEX_DOUBLE=R"_docs(/**
* \brief Returns the Slice index related to the temporal key \f$t\f$
*
* \param t the temporal key (double, must belong to the Tube domain)
* \return an integer
*/
)_docs";

const char* DOCS_TUBE_INDEX_SLICE=R"_docs(/**
* \brief Returns the Slice index related to the Slice pointer
*
* \param slice a const pointer to a Slice object of this tube
* \return an integer
*/
)_docs";

const char* DOCS_TUBE_SAMPLE_DOUBLE=R"_docs(/**
* \brief Samples this tube at \f$t\f$
*
* \note Without any effect if two Slice objects are already defined at \f$t\f$
*       (if the gate \f$[x](t)\f$ already exists)
*
* \param t the temporal key (double, must belong to the Tube domain)
*/
)_docs";

const char* DOCS_TUBE_SAMPLE_DOUBLE_SLICE=R"_docs(/**
* \brief Samples this tube at \f$t\f$ from a pointer to the corresponding slice
*
* Reduces the complexity of related methods by providing a direct access
* to the Slice object covering the input \f$t\f$.
*
* \note Without any effect if two Slice objects are already defined at \f$t\f$
*       (if the gate \f$[x](t)\f$ already exists)
*
* \param t the temporal key (double, must belong to the Tube domain)
* \param slice_to_be_sampled a pointer to the Slice whose domain contains \f$t\f$
*/
)_docs";

const char* DOCS_TUBE_SAMPLE_DOUBLE_INTERVAL=R"_docs(/**
* \brief Samples this tube at \f$t\f$ with a specific gate value
*
* \note Without any sampling effect if two Slice objects are already defined at \f$t\f$
*       (if the gate \f$[x](t)\f$ already exists)
*
* \param t the temporal key (double, must belong to the Tube domain)
* \param gate the Interval value of this tube at \f$t\f$
*/
)_docs";

const char* DOCS_TUBE_SAMPLE_TUBE=R"_docs(/**
* \brief Samples this tube so that it will share the same sampling of \f$[x](\cdot)\f$
*
* \note The previous sampling of this tube is preserved
*
* \param x the Tube from which the new sampling will come from
*/
)_docs";

const char* DOCS_TUBE_GATE_EXISTS_DOUBLE=R"_docs(/**
* \brief Tests if a gate exists at time \f$t\f$
*
* \param t time input to test
* \return `true` if a gate exists
*/
)_docs";

const char* DOCS_TUBE_CODOMAIN=R"_docs(/// @}
/// \name Accessing values
/// @{/**
* \brief Returns the interval of feasible values
*
* \return an Interval object \f$[x]([t_0,t_f])\f$
*/
)_docs";

const char* DOCS_TUBE_VOLUME=R"_docs(/**
* \brief Returns the volume of this tube
*
* \note returns POS_INFINITY if the codomain is unbounded
* \note returns 0 if the tube is flat (and so without wrapping effect)
*
* \return volume defined as \f$w([t_0,t_f])\times w([x]([t_0,t_f]))\f$
*/
)_docs";

const char* DOCS_TUBE_OPERATOR_CALL_INT=R"_docs(/**
* \brief Returns the value of the ith slice
*
* \param slice_id the index of the ith slice
* \return Interval value of \f$[x](i)\f$
*/
)_docs";

const char* DOCS_TUBE_OPERATOR_CALL_DOUBLE=R"_docs(/**
* \brief Returns the evaluation of this tube at \f$t\f$
*
* \param t the temporal key (double, must belong to the Tube domain)
* \return Interval value of \f$[x](t)\f$
*/
)_docs";

const char* DOCS_TUBE_OPERATOR_CALL_INTERVAL=R"_docs(/**
* \brief Returns the interval evaluation of this tube over \f$[t]\f$
*
* \param t the subdomain (Interval, must be a subset of the Tube domain)
* \return Interval envelope \f$[x]([t])\f$
*/
)_docs";

const char* DOCS_TUBE_EVAL_INTERVAL=R"_docs(/**
* \brief Returns the interval evaluations of the bounds of the
*        tube \f$\underline{x^-}(\cdot)\f$ and \f$\overline{x^+}(\cdot)\f$ over \f$[t]\f$
*
* \param t the subdomain (Interval, must be a subset of the Tube domain)
* \return the pair \f$\big([\underline{x^-}]([t]),[\overline{x^+}]([t])\big)\f$
*/
)_docs";

const char* DOCS_TUBE_INTERPOL_DOUBLE_TUBE=R"_docs(/**
* \brief Returns the optimal evaluation of this tube at \f$t\f$,
*        based on the derivative information \f$\dot{x}(\cdot)\f$
*
* \todo Change the name of this method?
*
* \param t the temporal key (double, must belong to the Tube domain)
* \param v the derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
* \return Interval value of \f$[x](t)\f$
*/
)_docs";

const char* DOCS_TUBE_INTERPOL_INTERVAL_TUBE=R"_docs(/**
* \brief Returns the optimal interval evaluation of this tube over \f$[t]\f$,
*        based on the derivative information \f$\dot{x}(\cdot)\f$
*
* \todo Change the name of this method?
*
* \param t the subdomain (Interval, must be a subset of the Tube domain)
* \param v the derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
* \return Interval value of \f$[x]([t])\f$
*/
)_docs";

const char* DOCS_TUBE_INVERT_INTERVAL_INTERVAL=R"_docs(/**
* \brief Returns the interval inversion \f$[x]^{-1}([y])\f$
*
* \note If the inversion results in several pre-images, their union is returned
*
* \param y the interval codomain
* \param search_domain the optional interval domain on which the inversion will be performed
* \return the hull of \f$[x]^{-1}([y])\f$
*/
)_docs";

const char* DOCS_TUBE_INVERT_INTERVAL_VECTOR_INTERVAL__INTERVAL=R"_docs(/**
* \brief Computes the set of continuous values of the inversion \f$[x]^{-1}([y])\f$
*
* \param y the interval codomain
* \param v_t the vector of the sub-domains \f$[t_k]\f$ for which
*            \f$\forall t\in[t_k] \mid x(t)\in[y], x(\cdot)\in[x](\cdot)\f$
* \param search_domain the optional interval domain on which the inversion will be performed
*/
)_docs";

const char* DOCS_TUBE_INVERT_INTERVAL_TUBE_INTERVAL=R"_docs(/**
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
)_docs";

const char* DOCS_TUBE_INVERT_INTERVAL_VECTOR_INTERVAL__TUBE_INTERVAL=R"_docs(/**
* \brief Computes the set of continuous values of the optimal inversion \f$[x]^{-1}([y])\f$
*
* \note The knowledge of the derivative tube \f$[v](\cdot)\f$ allows finer inversions
*
* \param y the interval codomain
* \param v_t the vector of the sub-domains \f$[t_k]\f$ for which
*            \f$\exists t\in[t_k] \mid x(t)\in[y], x(\cdot)\in[x](\cdot), \dot{x}(\cdot)\in[v](\cdot)\f$
* \param v the derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
* \param search_domain the optional interval domain on which the inversion will be performed
*/
)_docs";

const char* DOCS_TUBE_MAX_DIAM=R"_docs(/**
* \brief Returns the diameter of the interval value \f$[x](t)\f$ that is the more uncertain
*
* \return the maximal thickness of this tube
*/
)_docs";

const char* DOCS_TUBE_MAX_GATE_DIAM_DOUBLE=R"_docs(/**
* \brief Returns the diameter of the gate of this tube that is the more uncertain
*
* \param t the temporal key of the corresponding uncertain gate
* \return the maximal thickness of the gate
*/
)_docs";

const char* DOCS_TUBE_DIAM_BOOL=R"_docs(/**
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

const char* DOCS_TUBE_DIAM_TUBE=R"_docs(/**
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

const char* DOCS_TUBE_EQ_TUBE=R"_docs(/// @}
/// \name Tests
/// @{/**
* \brief Returns true if this tube is equal to \f$[x](\cdot)\f$
*
* \note Equality is obtained if the tubes share
*       the same bounds, domain and sampling
*
* \param x the Tube object
* \return true in case of equality
*/
)_docs";

const char* DOCS_TUBE_NE_TUBE=R"_docs(/**
* \brief Returns true if this tube is different from \f$[x](\cdot)\f$
*
* \note The two tubes are different if they do not share
*       the same bounds, domain or sampling
*
* \param x the Tube object
* \return true in case of difference
*/
)_docs";

const char* DOCS_TUBE_IS_SUBSET_TUBE=R"_docs(/**
* \brief Returns true if this tube is a subset of \f$[x](\cdot)\f$
*
* \note The two tubes may not share the same slicing,
*       but must have the same definition domain
*
* \param x the Tube object
* \return true in case of subset
*/
)_docs";

const char* DOCS_TUBE_IS_STRICT_SUBSET_TUBE=R"_docs(/**
* \brief Returns true if this tube is a subset of \f$[x](\cdot)\f$,
*        and not \f$[x](\cdot)\f$ itself
*
* \note The two tubes may not share the same slicing,
*       but must have the same definition domain
*
* \param x the Tube object
* \return true in case of strict subset
*/
)_docs";

const char* DOCS_TUBE_IS_INTERIOR_SUBSET_TUBE=R"_docs(/**
* \brief Returns true if this tube is a subset of the interior of \f$[x](\cdot)\f$
*
* \note The two tubes may not share the same slicing,
*       but must have the same definition domain
*
* \param x the Tube object
* \return true in case of interior subset
*/
)_docs";

const char* DOCS_TUBE_IS_STRICT_INTERIOR_SUBSET_TUBE=R"_docs(/**
* \brief Returns true if this tube is a subset of the interior
*        of \f$[x](\cdot)\f$, and not \f$[x](\cdot)\f$ itself
*
* \note The two tubes may not share the same slicing,
*       but must have the same definition domain
*
* \param x the Tube object
* \return true in case of strict interior subset
*/
)_docs";

const char* DOCS_TUBE_IS_SUPERSET_TUBE=R"_docs(/**
* \brief Returns true if this tube is a superset of \f$[x](\cdot)\f$
*
* \note The two tubes may not share the same slicing,
*       but must have the same definition domain
*
* \param x the Tube object
* \return true in case of superset
*/
)_docs";

const char* DOCS_TUBE_IS_STRICT_SUPERSET_TUBE=R"_docs(/**
* \brief Returns true if this tube is a superset of \f$[x](\cdot)\f$,
*        and not \f$[x](\cdot)\f$ itself
*
* \note The two tubes may not share the same slicing,
*       but must have the same definition domain
*
* \param x the Tube object
* \return true in case of strict superset
*/
)_docs";

const char* DOCS_TUBE_IS_EMPTY=R"_docs(/**
* \brief Returns true if this tube is empty
*
* \note If \f$[x](t)=\varnothing\f$ for some \f$t\f$, then the tube
*       \f$[x](\cdot)\f$ is considered empty
*
* \return true in case of emptiness
*/
)_docs";

const char* DOCS_TUBE_CONTAINS_TRAJECTORY=R"_docs(/**
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
)_docs";

const char* DOCS_TUBE_OVERLAPS_TUBE_FLOAT=R"_docs(/**
* \brief Returns true if this tube overlaps the tube \f$[x](\cdot)\f$
*
* \param x the other tube
* \param ratio an optional overlapping ratio between 0 and 1 (1 by default).
*              For instance, if ratio=0.3 and there is an overlapping of at least 30%,
*              then the function returns true
* \return true in case of overlapping with respect to the defined ratio
*/
)_docs";

const char* DOCS_TUBE_SET_INTERVAL=R"_docs(/// @}
/// \name Setting values
/// @{/**
* \brief Sets a constant interval value for this tube: \f$\forall t, [x](t)=[y]\f$
*
* \note The sampling of this tube is preserved
*
* \param y Interval value of the slices
*/
)_docs";

const char* DOCS_TUBE_SET_INTERVAL_INT=R"_docs(/**
* \brief Sets the interval value of the ith slice of this tube
*
* \param y Interval value of the ith slice
* \param slice_id index of the ith Slice
*/
)_docs";

const char* DOCS_TUBE_SET_INTERVAL_DOUBLE=R"_docs(/**
* \brief Sets the interval value of this tube at \f$t\f$: \f$[x](t)=[y]\f$
*
* \note It may create a gate (and so one more slice) if the tube
*       is not already sampled at \f$t\f$. Otherwise, it will
*       update the value of the already existing gate.
*
* \param y Interval value of the gate
* \param t the temporal key (double, must belong to the Tube domain)
*/
)_docs";

const char* DOCS_TUBE_SET_INTERVAL_INTERVAL=R"_docs(/**
* \brief Sets the interval value of this tube over \f$[t]\f$: \f$\forall t\in[t], [x](t)=[y]\f$
*
* \note It may create two gates (and so further slices) if the tube
*       is not already sampled at \f$t^-\f$ and \f$t^+\f$. This is
*       done to ensure that \f$\forall t\in[t], [x](t)=[y]\f$.
*
* \param y Interval value to be set
* \param t the subdomain (Interval, must be a subset of the Tube domain)
*/
)_docs";

const char* DOCS_TUBE_SET_EMPTY=R"_docs(/**
* \brief Sets this tube to the empty set
*
* \note By convention, all slices will be set to the empty set
*/
)_docs";

const char* DOCS_TUBE_INFLATE_DOUBLE=R"_docs(/**
* \brief Inflates this tube by adding \f$[-rad,+rad]\f$ to all its codomain components
*
* \note All slices and gates will be inflated
*
* \param rad half of the inflation
* \return *this
*/
)_docs";

const char* DOCS_TUBE_INFLATE_TRAJECTORY=R"_docs(/**
* \brief Inflates this tube by adding non-constant uncertainties defined in a trajectory 
*
* \note From the trajectory \f$a(\cdot)\f$, the function will 
*       inflate this tube such that \f$\forall t, [x](t):=[x](t)+[-a(t),+a(t)]\f$
* \note All slices and gates will be inflated
*
* \param rad the Trajectory object defining the non-constant inflation
* \return *this
*/
)_docs";

const char* DOCS_TUBE_SHIFT_TDOMAIN_DOUBLE=R"_docs(/**
* \brief Shifts the domain \f$[t_0,t_f]\f$ of \f$[x](\cdot)\f$
*
* \param a the offset value so that \f$[t_0,t_f]:=[t_0-a,t_f-a]\f$ 
*/
)_docs";

const char* DOCS_TUBE_REMOVE_GATE_DOUBLE=R"_docs(/**
* \brief Removes the gate at \f$t\f$ and merges the two related slices
*
* \param t time input where the gate to remove is
*/
)_docs";

const char* DOCS_TUBE_BISECT_DOUBLE_FLOAT=R"_docs(/// @}
/// \name Bisection
/// @{/**
* \brief Bisects this tube
*
* \note The tube is bisected along the codomain and according to a defined ratio
* \note The bisection is performed on the interval value of the gate \f$[x](t)\f$
* \note If the tube is not already sampled at \f$t\f$, then a sampling is performed
*
* \param t the temporal key (double, must belong to the Tube domain)
* \param ratio the bisection ratio (default value: 0.55)
* \return a pair of two Tube objects resulting from the bisection
*/
)_docs";

const char* DOCS_TUBE_IADD_INTERVAL=R"_docs(/// @}
/// \name Assignments operators
/// @{/**
* \brief Operates +=
*
* \param x Interval
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TUBE_IADD_TRAJECTORY=R"_docs(/**
* \brief Operates +=
*
* \param x Trajectory
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TUBE_IADD_TUBE=R"_docs(/**
* \brief Operates +=
*
* \param x Tube
* \return (*this)+=x
*/
)_docs";

const char* DOCS_TUBE_ISUB_INTERVAL=R"_docs(/**
* \brief Operates -=
*
* \param x Interval
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TUBE_ISUB_TRAJECTORY=R"_docs(/**
* \brief Operates -=
*
* \param x Trajectory
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TUBE_ISUB_TUBE=R"_docs(/**
* \brief Operates -=
*
* \param x Tube
* \return (*this)-=x
*/
)_docs";

const char* DOCS_TUBE_IMUL_INTERVAL=R"_docs(/**
* \brief Operates *=
*
* \param x Interval
* \return (*this)*=x
*/
)_docs";

const char* DOCS_TUBE_IMUL_TRAJECTORY=R"_docs(/**
* \brief Operates *=
*
* \param x Trajectory
* \return (*this)*=x
*/
)_docs";

const char* DOCS_TUBE_IMUL_TUBE=R"_docs(/**
* \brief Operates *=
*
* \param x Tube
* \return (*this)*=x
*/
)_docs";

const char* DOCS_TUBE_ITRUEDIV_INTERVAL=R"_docs(/**
* \brief Operates /=
*
* \param x Interval
* \return (*this)/=x
*/
)_docs";

const char* DOCS_TUBE_ITRUEDIV_TRAJECTORY=R"_docs(/**
* \brief Operates /=
*
* \param x Trajectory
* \return (*this)/=x
*/
)_docs";

const char* DOCS_TUBE_ITRUEDIV_TUBE=R"_docs(/**
* \brief Operates /=
*
* \param x Tube
* \return (*this)/=x
*/
)_docs";

const char* DOCS_TUBE_IOR_INTERVAL=R"_docs(/**
* \brief Operates |=
*
* \param x Interval
* \return (*this)|=x
*/
)_docs";

const char* DOCS_TUBE_IOR_TRAJECTORY=R"_docs(/**
* \brief Operates |=
*
* \param x Trajectory
* \return (*this)|=x
*/
)_docs";

const char* DOCS_TUBE_IOR_TUBE=R"_docs(/**
* \brief Operates |=
*
* \param x Tube
* \return (*this)|=x
*/
)_docs";

const char* DOCS_TUBE_IAND_INTERVAL=R"_docs(/**
* \brief Operates &=
*
* \param x Interval
* \return (*this)&=x
*/
)_docs";

const char* DOCS_TUBE_IAND_TRAJECTORY=R"_docs(/**
* \brief Operates &=
*
* \param x Trajectory
* \return (*this)&=x
*/
)_docs";

const char* DOCS_TUBE_IAND_TUBE=R"_docs(/**
* \brief Operates &=
*
* \param x Tube
* \return (*this)&=x
*/
)_docs";

const char* DOCS_TUBE_CLASS_NAME=R"_docs(/// @}
/// \name String
/// @{/**
* \brief Returns the name of this class
*
* \note Only used for some generic display method
*
* \return the predefined name
*/
)_docs";

const char* DOCS_TUBE_OPERATOR_OSTREAM_TUBE=R"_docs(/**
* \brief Displays a synthesis of this tube
*
* \param str ostream
* \param x tube to be displayed
* \return ostream
*/
)_docs";

const char* DOCS_TUBE_ENABLE_SYNTHESIS_BOOL=R"_docs(/**
* \brief Enables the computation of a synthesis tree
*
* \note The synthesis tree speeds up computations such as integrals or evaluations
*
* \param enable boolean
*/
)_docs";

const char* DOCS_TUBE_INTEGRAL_DOUBLE=R"_docs(/// @}
/// \name Integration
/// @{/**
* \brief Computes the interval integral \f$\int_0^t[x](\tau)d\tau\f$
*
* \note From the monotonicity of the integral operator, 
*       \f$\int_0^t[x](\tau)d\tau=[\int_0^t x^-(\tau)d\tau,\int_0^t x^+(\tau)d\tau]\f$
*
* \param t the temporal key (double, must belong to the Tube domain)
* \return the set of feasible integral values
*/
)_docs";

const char* DOCS_TUBE_INTEGRAL_INTERVAL=R"_docs(/**
* \brief Computes the interval integral \f$\int_0^{[t]}[x](\tau)d\tau\f$
*
* \note From the monotonicity of the integral operator, 
*       \f$\int_0^{[t]}[x](\tau)d\tau=[\int_0^{[t]}x^-(\tau)d\tau,\int_0^{[t]}x^+(\tau)d\tau]\f$
*
* \param t the subdomain (Interval, must be a subset of the Tube domain)
* \return the set of feasible integral values
*/
)_docs";

const char* DOCS_TUBE_INTEGRAL_INTERVAL_INTERVAL=R"_docs(/**
* \brief Computes the interval integral \f$\int_{[t_1]}^{[t_2]}[x](\tau)d\tau\f$
*
* \note From the monotonicity of the integral operator, 
*       \f$\int_{[t_1]}^{[t_2]}[x](\tau)d\tau=[\int_{[t_1]}^{[t_2]}x^-(\tau)d\tau,\int_{[t_1]}^{[t_2]}x^+(\tau)d\tau]\f$
*
* \param t1 lower bound, subset of the Tube domain
* \param t2 upper bound, subset of the Tube domain
* \return the set of feasible integral values
*/
)_docs";

const char* DOCS_TUBE_PARTIAL_INTEGRAL_INTERVAL=R"_docs(/**
* \brief Computes the partial interval integral \f$\int_{0}^{[t]}[x](\tau)d\tau\f$
*
* \note From the monotonicity of the integral operator, 
*       \f$\int_{0}^{[t]}[x](\tau)d\tau=[\int_{0}^{[t]}x^-(\tau)d\tau,\int_{0}^{[t]}x^+(\tau)d\tau]\f$
*
* \param t interval upper bound, subset of the Tube domain
* \return the pair \f$\big([i^-],[i^+]\big)\f$,
*         where \f$[i^-]=\int_{0}^{[t]}x^-(\tau)d\tau\f$ 
*         and \f$[i^+]=\int_{0}^{[t]}x^+(\tau)d\tau\f$
*/
)_docs";

const char* DOCS_TUBE_PARTIAL_INTEGRAL_INTERVAL_INTERVAL=R"_docs(/**
* \brief Computes the partial interval integral \f$\int_{[t_1]}^{[t_2]}[x](\tau)d\tau\f$
*
* \note From the monotonicity of the integral operator, 
*       \f$\int_{[t_1]}^{[t_2]}[x](\tau)d\tau=[\int_{[t_1]}^{[t_2]}x^-(\tau)d\tau,\int_{[t_1]}^{[t_2]}x^+(\tau)d\tau]\f$
*
* \param t1 interval lower bound, subset of the Tube domain
* \param t2 interval upper bound, subset of the Tube domain
* \return the pair \f$\big([i^-],[i^+]\big)\f$,
*         where \f$[i^-]=\int_{[t_1]}^{[t_2]}x^-(\tau)d\tau\f$ 
*         and \f$[i^+]=\int_{[t_1]}^{[t_2]}x^+(\tau)d\tau\f$
*/
)_docs";

const char* DOCS_TUBE_SERIALIZE_STRING_INT=R"_docs(/// @}
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

const char* DOCS_TUBE_SERIALIZE_STRING_TRAJECTORY_INT=R"_docs(/**
* \brief Serializes this tube together with a Trajectory object
*
* \note The values and sampling (slices and gates) are serialized
* \note The serialization of a Trajectory defined from a tubex::Function object is not supported
* \note The output file will appear in the executable current directory
*
* \param binary_file_name name of the output file (default value: "x.tube")
* \param traj the Trajectory object to serialize (for instance, actual but unknown values)
* \param version_number serialization version (used for tests purposes, default value: last version)
*/
)_docs";

const char* DOCS_TUBE_SAME_SLICING_TUBE_TUBE=R"_docs(/// @}/**
* \brief Tests whether the two Tube objects are sharing the same slicing
*
* \note If true, it means the two tubes are defined with the same
*       amount of slices and identical sampling
*
* \param x1 the first Tube
* \param x2 the second Tube
* \return true in case of same slicing
*/
)_docs";

const char* DOCS_TUBE_ENABLE_SYNTHESES_BOOL=R"_docs(/**
* \brief Enables the computation of a synthesis tree for any Tube object
*
* \note A synthesis tree speeds up computations such as integrals or evaluations
*
* \param enable boolean
*/
)_docs";

const char* DOCS_TUBE_HULL_LIST_TUBE_=R"_docs(/**
* \brief Computes the hull of several tubes
*
* \param l_tubes list of tubes
* \return the tube enveloping the other ones
*/
)_docs";

