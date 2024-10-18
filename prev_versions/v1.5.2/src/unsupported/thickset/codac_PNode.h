/* ============================================================================
 *  IBEX-Robotics - PNode class
 * ============================================================================
 *  Copyright : Simon Rohou, Benoit Desrochers
 *  License   : This program can be distributed under the terms of
 *              the MIT license. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou, Benoît Desrochers
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __PNODE_H__
#define __PNODE_H__

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <ibex_IntervalVector.h>
#include <ibex_LargestFirst.h>
#include <codac_PavingVisitor.h>

template <typename V>
class PNode
{
  public:

    /**
     * \brief Create a paving containing one given box.
     *
     * \param box an IntervalVector representing paving's domain
     * \param value a value representing the paving
     */
    PNode(const ibex::IntervalVector& box, V value);
    /**
     * \brief Copy constructor
     */
    PNode(const PNode& n);

    /**
     * \brief move constructor
     */
    PNode(PNode&& other);
    /**
     * \brief assignement operators
     */
    // PNode& operator=(const PNode& n);

    /**
     * \brief Dselete this paving.
     */
    ~PNode();

    /**
     *  Move assignement operator
     */
    PNode &operator=(const PNode &&other);

    /**
     * \brief Return paving's domain.
     *
     * \return an IntervalVector representing paving's domain
     */
    const ibex::IntervalVector& getBox() const;

    /**
     * \brief Return paving's value.
     *
     * \return the value of the paving
     */
    V value() const;

    /**
     * \brief Test if this does not have subpavings (i.e. if this is a leaf in a tree structure).
     *
     * \return true if this paving is a leaf, false otherwise
     */
    bool isLeaf() const;

    /**
     * \brief Return the height of the tree structure.
     *
     * A leaf has a height equal to 1.
     *
     * \return an integer
     */
    int height() const;

    /**
     * \brief Return a pointer to the first part of the bisected domain.
     *
     * \return the first half of this.
     */
    PNode* left() const;

    /**
     * \brief Return a pointer to the second part of the bisected domain.
     *
     * \return the second half of this.
     */
    PNode* right() const;

    /**
     * \brief Set paving's value.
     *
     * \param value the value of the paving
     * \param propagate_to_leafs if false, children are not updated
     */
    void setValue(V value, bool propagate_to_leafs = true);

    /**
     * \brief Get leafs number (i.e. get number of subpavings that constitute the paving) of the given value.
     *
     * \param value the specified value to count
     * \return an integer
     */
    int getSubpavingsNumber(V value) const;

    /**
     * \brief Perform a simplification on the tree structure.
     *
     * \return true if a simplification has been done, false otherwise
     */
    bool reunite();

    /**
     * \brief Perform a bisection on the current leaf.
     *
     * Bisection is done only if this is a leaf.
     *
     * \param ratio the ratio between the diameters of the left and the right parts of the bisected box ; 0.45 by default
     */
    void bisect(ibex::Bsc& bisector);

		/**
		 * \brief Visit the current Node
		 *
		 * \param visitor which will perform action with node
		 *
		 */
     template<typename T>
		 void visit( T& visitor);

		/**
		 * \brief Number of leaves below the Pnode
		 */
		 int countLeaves();

		/**
		 *	\brief Remove_sons
		 */
		void remove_children();

		/**
		 *	\ brief Clear Paving
		 * ie. remove all children
		 */
		void clear();

    /**
     * \brief Save node into file stream
     */
    void save(std::ofstream& of);

    /**
     * \brief read node from file stream
     */
    static PNode<V>*  load(std::ifstream& inf);

  protected:

    V m_value;
    PNode *m_left, *m_right;
    ibex::IntervalVector m_box;
};

#include "codac_PNode_impl.hpp" // this is needed with C++ templates


#endif //__PNODE_H__
