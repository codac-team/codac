//============================================================================
//                               P Y I B E X
// File        : PavingVisitor.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPLv3 See the LICENSE file
// Created     : 08/08/2016
//============================================================================

#ifndef __PYIBEX_PAVING_VISITOR_H___
#define __PYIBEX_PAVING_VISITOR_H___

// #include "ThickBoolean.h"

#include <ibex_IntervalVector.h>

using ibex::IntervalVector;
//namespace codac {

// class T;
/**
 * \ingroup set
 * \brief Set visitor
 */
template<typename T, typename V>
class PavingVisitor {
public:
	/**
	 * \brief Delete this.
	 */
	virtual ~PavingVisitor() { }

	/**
	 * \brief Visit an intermediate node
	 *
	 * By default, does nothing.
	 */
	virtual void visit_node(const IntervalVector& box) { }

	/**
	* \brief Visit a node
	*/
	virtual void visit_node(const IntervalVector& boxIn, const IntervalVector& boxOut) {};

	/**
	 * \brief Visit a leaf
	 */
	virtual void visit_leaf(const IntervalVector& box, V status)=0;


	/**
	 * \brief Do some stuff before visiting the Set
	 *
	 * By default, does nothing
	 * Take bounding box of the subpaving
	 */
	virtual void pre_visit(const T& p){ };

	/**
	 * \brief Do some stuff after visiting the Set
	 *
	 * By default, does nothing
	 */
	virtual void post_visit(const T& p){ };

};

//} // namespace ibex

#endif // __IBEX_PAVING_VISITOR_H__
