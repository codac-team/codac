/* ============================================================================
 *  IBEX-Robotics - PSetNode class
 * ============================================================================
 *  Copyright : Simon Rohou, Benoit Desrochers
 *  License   : This program can be distributed under the terms of
 *              the MIT license. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou, Benoît Desrochers
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __PSETNODE_H__
#define __PSETNODE_H__

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <ibex_IntervalVector.h>
#include <ibex_LargestFirst.h>
#include <pyibex_PavingVisitor.h>
#include <ibex_Pdc.h>

using ibex::IntervalVector;

namespace pyibex {

class PSetNode
{
  public:

    /**
     * \brief Create a paving containing one given box.
     *
     * \param box an IntervalVector representing paving's domain
     * \param value a value representing the paving
     */
    PSetNode(const ibex::IntervalVector& box);

    /**
     * \brief Create a paving containing one given box.
     *
     * \param box an IntervalVector representing paving's domain
     * \param value a value representing the paving
     */
    PSetNode(const ibex::IntervalVector& box_in, const ibex::IntervalVector& box_out);

    /**
     * \brief Copy constructor
     */
    PSetNode(const PSetNode& n);

    /**
     * \brief move constructor
     */
    PSetNode(PSetNode&& other);
    /**
     * \brief assignement operators
     */
    // PSetNode& operator=(const PSetNode& n);

    /**
     * \brief Dselete this paving.
     */
    ~PSetNode();

    /**
     *  Move assignement operator
     */
    PSetNode &operator=(const PSetNode &&other);

    /**
     * \brief Return paving's domain.
     *
     * \return an IntervalVector representing paving's domain
     */
    ibex::IntervalVector& getBoxIn();

    /**
     * \brief Return paving's domain.
     *
     * \return an IntervalVector representing paving's domain
     */
    ibex::IntervalVector& getBoxOut();

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
    PSetNode* left() const;

    /**
     * \brief Return a pointer to the second part of the bisected domain.
     *
     * \return the second half of this.
     */
    PSetNode* right() const;

    /**
     * \brief Set paving's value.
     *
     * \param value the value of the paving
     * \param propagate_to_leafs if false, children are not updated
     */
    // void setValue(V value, bool propagate_to_leafs = true);

    /**
     * \brief Get leafs number (i.e. get number of subpavings that constitute the paving) of the given value.
     *
     * \param value the specified value to count
     * \return an integer
     */
    // int getSubpavingsNumber(V value) const;

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

    void bisect_max(ibex::Bsc& bisector);
    void bisect_around(ibex::Bsc& bisector, IntervalVector& box);



    ibex::BoolInterval isInside(const ibex::Vector& p);
		/**
		 * \brief Visit the current Node
		 *
		 * \param visitor which will perform action with node
		 *
		 */
     template<typename T>
		 void visit(T& visitor);

		/**
		 * \brief Number of leaves below the Pnode
		 */
		 int countLeaves();

		/**
		 *	\brief Remove_sons
		 */
		void remove_children();

    /**
     * Remove_sons
     */
    void removeNode();
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
    static PSetNode*  load(std::ifstream& inf);

    /**
     *  \brief returns True if the Xin & Xout si empty
     */
    bool is_empty();

    ibex::IntervalVector m_box_in, m_box_out;

  protected:

    PSetNode *m_left, *m_right;
};

// #include "PSetNode_impl.hpp" // this is needed with C++ templates

template<typename T>
inline void PSetNode::visit(T& visitor){
  if(isLeaf()){
    visitor.visit_leaf(m_box_in, m_box_out);
  }else {
    visitor.visit_node(m_box_in, m_box_out);
    left()->visit(visitor);
    right()->visit(visitor);
  }
}

}; // end namespace pyibex
#endif //__PNODE_H__
