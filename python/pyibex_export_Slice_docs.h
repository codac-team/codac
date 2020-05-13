
/** 
 *  ile
 *  Slice documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 const char* DOCS_SLICE=R"_docs(/**
* \class Slice
* \brief Slice \f$\llbracket x\rrbracket(\cdot)\f$ of a one dimensional tube and made of an envelope and two gates
*/
)_docs";

const char* DOCS_SLICE_SLICE_INTERVAL_INTERVAL=R"_docs(/// \name Definition
/// @{/**
* \brief Creates a slice \f$\llbracket x\rrbracket\f$
*
* \param domain Interval domain \f$[t^k_0,t^k_f]\f$
* \param codomain Interval value of the slice (all reals \f$[-\infty,\infty]\f$ by default)
*/
)_docs";

const char* DOCS_SLICE_SLICE_SLICE=R"_docs(/**
* \brief Creates a copy of the slice \f$\llbracket x\rrbracket\f$
*
* \param x Slice to be duplicated
*/
)_docs";

const char* DOCS_SLICE_SIZE=R"_docs(/**
* \brief Returns the dimension of the slice (always 1)
*
* \return 1
*/
)_docs";

const char* DOCS_SLICE_OPERATOR_SLICE=R"_docs(/**
* \brief Returns a copy of a Slice
*
* \param x the Slice object to be copied
* \return a new Slice object with same domain and codomain
*/
)_docs";

const char* DOCS_SLICE_TDOMAIN=R"_docs(/**
* \brief Returns the temporal definition domain of this slice
*
* \return an Interval object \f$[t_0,t_f]\f$
*/
)_docs";

const char* DOCS_SLICE_PREV_SLICE=R"_docs(/// @}
/// \name Slices structure
/// @{/**
* \brief Points to the previous slice \f$\llbracket x\rrbracket(k-1)\f$
*
* \return a pointer to a Slice object, or NULL is this slice is the first one
*/
)_docs";

const char* DOCS_SLICE_PREV_SLICE1=R"_docs(/**
* \brief Points to the previous slice \f$\llbracket x\rrbracket(k-1)\f$
*
* \return a const pointer to a Slice object, or NULL is this slice is the first one
*/
)_docs";

const char* DOCS_SLICE_NEXT_SLICE=R"_docs(/**
* \brief Points to the next slice \f$\llbracket x\rrbracket(k+1)\f$
*
* \return a pointer to a Slice object, or NULL is this slice is the last one
*/
)_docs";

const char* DOCS_SLICE_NEXT_SLICE1=R"_docs(/**
* \brief Points to the next slice \f$\llbracket x\rrbracket(k+1)\f$
*
* \return a const pointer to a Slice object, or NULL is this slice is the last one
*/
)_docs";

const char* DOCS_SLICE_INPUT_GATE=R"_docs(/**
* \brief Returns the value of the input gate \f$\llbracket x\rrbracket(t_0)\f$ of this slice
*
* \return the interval input gate
*/
)_docs";

const char* DOCS_SLICE_OUTPUT_GATE=R"_docs(/**
* \brief Returns the value of the output gate \f$\llbracket x\rrbracket(t_f)\f$ of this slice
*
* \return the interval output gate
*/
)_docs";

const char* DOCS_SLICE_POLYGON_SLICE=R"_docs(/**
* \brief Computes a convex polygon that optimaly encloses the values of the slice
*        according to the knowledge of the derivative slice \f$\llbracket v\rrbracket\f$
*
* \todo Store the polygon in cache memory?
*
* \param v the derivative slice
* \return a ConvexPolygon object
*/
)_docs";

const char* DOCS_SLICE_CODOMAIN=R"_docs(/// @}
/// \name Accessing values
/// @{/**
* \brief Returns the envelope of the slice
*
* \return the Interval object \f$\llbracket x\rrbracket([t_0,t_f])\f$
*/
)_docs";

const char* DOCS_SLICE_BOX=R"_docs(/**
* \brief Computes the two-dimensional box \f$[t_0,t_f]\times\llbracket x\rrbracket([t_0,t_f])\f$
*
* \return the slice box
*/
)_docs";

const char* DOCS_SLICE_DIAM_INTERVAL=R"_docs(/**
* \brief Returns the diameter of an interval (IBEX overload)
*
* \note returns POS_INFINITY if the interval is unbounded
* \note returns 0 if the interval is empty
*
* \param interval set to be evaluated
* \return the diameter
*/
)_docs";

const char* DOCS_SLICE_DIAM=R"_docs(/**
* \brief Returns the diameter of this slice
*
* \note returns POS_INFINITY if the codomain is unbounded
* \note returns 0 if the slice is empty
*
* \return the diameter
*/
)_docs";

const char* DOCS_SLICE_VOLUME=R"_docs(/**
* \brief Returns the volume of this slice
*
* \note returns POS_INFINITY if the codomain is unbounded
* \note returns 0 if the slice is flat (and so without wrapping effect)
*
* \return volume defined as \f$w([t_0,t_f])\times w(\llbracket x\rrbracket([t_0,t_f]))\f$
*/
)_docs";

const char* DOCS_SLICE_OPERATOR_CALL_DOUBLE=R"_docs(/**
* \brief Returns the evaluation of this slice at \f$t\f$
*
* \note The returned value is either the input gate, the envelope or the output gate
*
* \param t the temporal key (double, must belong to the Slice domain)
* \return Interval value of \f$\llbracket x\rrbracket(t)\f$
*/
)_docs";

const char* DOCS_SLICE_OPERATOR_CALL_INTERVAL=R"_docs(/**
* \brief Returns the interval evaluation of this slice over \f$[t]\f$
*
* \param t the subdomain (Interval, must be a subset of the Slice domain)
* \return Interval envelope \f$\llbracket x\rrbracket([t])\f$
*/
)_docs";

const char* DOCS_SLICE_EVAL_INTERVAL=R"_docs(/**
* \brief Returns the interval evaluations of the bounds of the slice over \f$[t]\f$
*
* \note It includes the lower and upper bounds of the gates
*
* \param t the subdomain (Interval, must be a subset of the Slice domain)
* \return the pair \f$\big(\llbracket\underline{x^-}\rrbracket([t]),\llbracket\overline{x^+}\rrbracket([t])\big)\f$
*/
)_docs";

const char* DOCS_SLICE_INTERPOL_DOUBLE_SLICE=R"_docs(/**
* \brief Returns the optimal evaluation of this slice at \f$t\f$,
*        based on the derivative information \f$\dot{x}(\cdot)\f$
*
* \todo Change the name of this method?
*
* \param t the temporal key (double, must belong to the Slice domain)
* \param v the derivative slice such that \f$\dot{x}(\cdot)\in\llbracket v\rrbracket(\cdot)\f$
* \return Interval value of \f$\llbracket x\rrbracket(t)\f$
*/
)_docs";

const char* DOCS_SLICE_INTERPOL_INTERVAL_SLICE=R"_docs(/**
* \brief Returns the optimal evaluation of this slice over \f$[t]\f$,
*        based on the derivative information \f$\dot{x}(\cdot)\f$
*
* \todo Change the name of this method?
*
* \param t the subdomain (Interval, must be a subset of the Slice domain)
* \param v the derivative slice such that \f$\dot{x}(\cdot)\in\llbracket v\rrbracket(\cdot)\f$
* \return Interval value of \f$\llbracket x\rrbracket([t])\f$
*/
)_docs";

const char* DOCS_SLICE_INVERT_INTERVAL_INTERVAL=R"_docs(/**
* \brief Returns the interval inversion \f$\llbracket x\rrbracket^{-1}([y])\f$
*
* \param y the interval codomain
* \param search_domain the optional interval domain on which the inversion will be performed
* \return the hull of \f$\llbracket x\rrbracket^{-1}([y])\f$
*/
)_docs";

const char* DOCS_SLICE_INVERT_INTERVAL_SLICE_INTERVAL=R"_docs(/**
* \brief Returns the optimal interval inversion \f$\llbracket x\rrbracket^{-1}([y])\f$
*
* \note The knowledge of the derivative slice \f$\llbracket v\rrbracket(\cdot)\f$ allows a finer inversion
*
* \param y the interval codomain
* \param v the derivative slice such that \f$\dot{x}(\cdot)\in\llbracket v\rrbracket(\cdot)\f$
* \param search_domain the optional interval domain on which the inversion will be performed
* \return the hull of \f$\llbracket x\rrbracket^{-1}([y])\f$
*/
)_docs";

const char* DOCS_SLICE_EQ_SLICE=R"_docs(/// @}
/// \name Tests
/// @{/**
* \brief Returns true if this slice is equal to \f$\llbracket x\rrbracket(\cdot)\f$
*
* \note Equality is obtained if the slices share
*       the same gates, codomain and domain
*
* \param x the Slice object
* \return true in case of equality
*/
)_docs";

const char* DOCS_SLICE_NE_SLICE=R"_docs(/**
* \brief Returns true if this slice is different from \f$\llbracket x\rrbracket(\cdot)\f$
*
* \note The two slices are different if they do not share
*       the same gates, codomain or domain
*
* \param x the Slice object
* \return true in case of difference
*/
)_docs";

const char* DOCS_SLICE_IS_SUBSET_SLICE=R"_docs(/**
* \brief Returns true if this slice is a subset of \f$\llbracket x\rrbracket(\cdot)\f$
*
* \note The two slices must have the same definition domain
*
* \param x the Slice object
* \return true in case of subset
*/
)_docs";

const char* DOCS_SLICE_IS_STRICT_SUBSET_SLICE=R"_docs(/**
* \brief Returns true if this slice is a subset of \f$\llbracket x\rrbracket(\cdot)\f$,
*        and not \f$\llbracket x\rrbracket(\cdot)\f$ itself
*
* \note The two slices must have the same definition domain
*
* \param x the Slice object
* \return true in case of strict subset
*/
)_docs";

const char* DOCS_SLICE_IS_INTERIOR_SUBSET_SLICE=R"_docs(/**
* \brief Returns true if this slice is a subset of the interior of \f$\llbracket x\rrbracket(\cdot)\f$
*
* \note The two slices must have the same definition domain
*
* \param x the Slice object
* \return true in case of interior subset
*/
)_docs";

const char* DOCS_SLICE_IS_STRICT_INTERIOR_SUBSET_SLICE=R"_docs(/**
* \brief Returns true if this slice is a subset of the interior of \f$\llbracket x\rrbracket(\cdot)\f$,
*        and not \f$\llbracket x\rrbracket(\cdot)\f$ itself
*
* \note The two slices must have the same definition domain
*
* \param x the Slice object
* \return true in case of strict interior subset
*/
)_docs";

const char* DOCS_SLICE_IS_SUPERSET_SLICE=R"_docs(/**
* \brief Returns true if this slice is a superset of \f$\llbracket x\rrbracket(\cdot)\f$
*
* \note The two slices must have the same definition domain
*
* \param x the Slice object
* \return true in case of superset
*/
)_docs";

const char* DOCS_SLICE_IS_STRICT_SUPERSET_SLICE=R"_docs(/**
* \brief Returns true if this slice is a superset of \f$\llbracket x\rrbracket(\cdot)\f$,
*        and not \f$\llbracket x\rrbracket(\cdot)\f$ itself
*
* \note The two slices must have the same definition domain
*
* \param x the Slice object
* \return true in case of strict superset
*/
)_docs";

const char* DOCS_SLICE_IS_EMPTY=R"_docs(/**
* \brief Returns true if this slice is empty
*
* \note If the input gate, the envelope or the output gate is empty,
*       then the slice is considered empty
*
* \return true in case of emptiness
*/
)_docs";

const char* DOCS_SLICE_CONTAINS_TRAJECTORY=R"_docs(/**
* \brief Returns true if this slice contains the trajectory \f$x(\cdot)\f$
*
* \note Due to the reliable numerical representation of a trajectory, some wrapping effect
*       may appear for its evaluations (either if it is defined by a map of values
*       or an analytic function). Hence, this "contains" test may not be able to
*       conclude, if the thin envelope of \f$x(\cdot)\f$ overlaps a boundary of the slice.
* \note The domain of this slice must be a subset of the domain of \f$x(\cdot)\f$
*
* \param x the trajectory that might be crossing this slice
* \return BoolInterval::YES (or BoolInterval::NO) if this slice contains \f$x(\cdot)\f$
*         (or does not contain) and BoolInterval::MAYBE in case of ambiguity
*/
)_docs";

const char* DOCS_SLICE_SET_INTERVAL=R"_docs(/// @}
/// \name Setting values
/// @{/**
* \brief Sets a constant interval value for this slice: \f$\forall t, \llbracket x\rrbracket(t)=[y]\f$
*
* \note The gates and the envelope will be set to \f$[y]\f$
*
* \param y Interval value of the slice
*/
)_docs";

const char* DOCS_SLICE_SET_EMPTY=R"_docs(/**
* \brief Sets this slice to the empty set
*
* \note The domain will not be affected
*/
)_docs";

const char* DOCS_SLICE_SET_ENVELOPE_INTERVAL_BOOL=R"_docs(/**
* \brief Sets the interval value of the envelope of this slice
*
* \note The gates may be contracted if the new envelope is not their superset
*       and if the boolean parameter is set to true
*
* \param envelope Interval value of the envelope
* \param slice_consistency if 'true', the value of the gates may be affected
*/
)_docs";

const char* DOCS_SLICE_SET_INPUT_GATE_INTERVAL_BOOL=R"_docs(/**
* \brief Sets the interval value of the input gate of this slice
*
* \note The value to be set will be intersected by the envelopes of the related slices
*       if the boolean parameter is set to true
* \note The envelopes of the related slices will not be affected
*
* \param input_gate Interval value of the input gate
* \param slice_consistency if 'true', the value of the gate will
*        be computed according to the connected slices
*/
)_docs";

const char* DOCS_SLICE_SET_OUTPUT_GATE_INTERVAL_BOOL=R"_docs(/**
* \brief Sets the interval value of the output gate of this slice
*
* \note The value to be set will be intersected by the envelopes of the related slices
*       if the boolean parameter is set to true
* \note The envelopes of the related slices will not be affected
*
* \param output_gate Interval value of the output gate
* \param slice_consistency if 'true', the value of the gate will
*        be computed according to the connected slices
*/
)_docs";

const char* DOCS_SLICE_INFLATE_DOUBLE=R"_docs(/**
* \brief Inflates this slice by adding \f$[-rad,+rad]\f$ to all its codomain components
*
* \note The envelope and the gates will be inflated
*
* \param rad half of the inflation
* \return *this
*/
)_docs";

const char* DOCS_SLICE_IADD_DOUBLE=R"_docs(/// @}
/// \name Assignments operators
/// @{/**
* \brief Operates +=
*
* \param x double
* \return (*this)+=x
*/
)_docs";

const char* DOCS_SLICE_IADD_TRAJECTORY=R"_docs(/**
* \brief Operates +=
*
* \param x Trajectory
* \return (*this)+=x
*/
)_docs";

const char* DOCS_SLICE_IADD_SLICE=R"_docs(/**
* \brief Operates +=
*
* \param x Slice
* \return (*this)+=x
*/
)_docs";

const char* DOCS_SLICE_ISUB_DOUBLE=R"_docs(/**
* \brief Operates -=
*
* \param x double
* \return (*this)-=x
*/
)_docs";

const char* DOCS_SLICE_ISUB_TRAJECTORY=R"_docs(/**
* \brief Operates -=
*
* \param x Trajectory
* \return (*this)-=x
*/
)_docs";

const char* DOCS_SLICE_ISUB_SLICE=R"_docs(/**
* \brief Operates -=
*
* \param x Slice
* \return (*this)-=x
*/
)_docs";

const char* DOCS_SLICE_IMUL_DOUBLE=R"_docs(/**
* \brief Operates *=
*
* \param x double
* \return (*this)*=x
*/
)_docs";

const char* DOCS_SLICE_IMUL_TRAJECTORY=R"_docs(/**
* \brief Operates *=
*
* \param x Trajectory
* \return (*this)*=x
*/
)_docs";

const char* DOCS_SLICE_IMUL_SLICE=R"_docs(/**
* \brief Operates *=
*
* \param x Slice
* \return (*this)*=x
*/
)_docs";

const char* DOCS_SLICE_ITRUEDIV_DOUBLE=R"_docs(/**
* \brief Operates /=
*
* \param x double
* \return (*this)/=x
*/
)_docs";

const char* DOCS_SLICE_ITRUEDIV_TRAJECTORY=R"_docs(/**
* \brief Operates /=
*
* \param x Trajectory
* \return (*this)/=x
*/
)_docs";

const char* DOCS_SLICE_ITRUEDIV_SLICE=R"_docs(/**
* \brief Operates /=
*
* \param x Slice
* \return (*this)/=x
*/
)_docs";

const char* DOCS_SLICE_IOR_DOUBLE=R"_docs(/**
* \brief Operates |=
*
* \param x double
* \return (*this)|=x
*/
)_docs";

const char* DOCS_SLICE_IOR_TRAJECTORY=R"_docs(/**
* \brief Operates |=
*
* \param x Trajectory
* \return (*this)|=x
*/
)_docs";

const char* DOCS_SLICE_IOR_SLICE=R"_docs(/**
* \brief Operates |=
*
* \param x Slice
* \return (*this)|=x
*/
)_docs";

const char* DOCS_SLICE_IAND_DOUBLE=R"_docs(/**
* \brief Operates &=
*
* \param x double
* \return (*this)&=x
*/
)_docs";

const char* DOCS_SLICE_IAND_TRAJECTORY=R"_docs(/**
* \brief Operates &=
*
* \param x Trajectory
* \return (*this)&=x
*/
)_docs";

const char* DOCS_SLICE_IAND_SLICE=R"_docs(/**
* \brief Operates &=
*
* \param x Slice
* \return (*this)&=x
*/
)_docs";

const char* DOCS_SLICE_CLASS_NAME=R"_docs(/// @}
/// \name String
/// @{/**
* \brief Returns the name of this class
*
* \note Only used for some generic display method
*
* \return the predefined name
*/
)_docs";

const char* DOCS_SLICE_OPERATOR_OSTREAM_SLICE=R"_docs(/**
* \brief Displays a synthesis of this slice
*
* \param str ostream
* \param x slice to be displayed
* \return ostream
*/
)_docs";

