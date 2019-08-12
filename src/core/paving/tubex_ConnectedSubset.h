/** 
 *  \file
 *  ConnectedSubset class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CONNECTEDSUBSET_H__
#define __TUBEX_CONNECTEDSUBSET_H__

#include "tubex_Set.h"

namespace tubex
{
  class Paving;

  /**
   * \class ConnectedSubset
   * \brief Multi-dimensional paving representation of a connected subset
   *
   * A connected subset is a topological space that cannot be represented
   * as the union of two or more disjoint non-empty open subsets.
   *
   * This object gathers a set of Paving leaves items that represent a subset of
   * a paving structure.
   */
  class ConnectedSubset : public Set
  {
    public:

      /// \name Basics
      /// @{

      /**
       * \brief Creates a connected subset
       *
       * \param v_subset_items set of Paving leaves this object will be made of
       */
      ConnectedSubset(const std::vector<const Paving*>& v_subset_items);

      /**
       * \brief ConnectedSubset destructor
       */
      ~ConnectedSubset();

      /**
       * \brief Returns the n-dimensional box enclosing this subset
       *
       * \return the box
       */
      const ibex::IntervalVector box() const;

      /**
       * \brief Tests if this subset is strictly included in its paving structure
       *
       * \return `true` in case of strict inclusion
       */
      bool is_strictly_included_in_paving() const;

      /**
       * \brief Returns a const pointer to the paving structure
       *
       * \return a const pointer to the Paving object
       */
      const Paving* get_paving() const;

      /**
       * \brief Returns the set of Paving leaves this subset is made of
       *
       * \return a vector of Paving leaves items
       */
      const std::vector<const Paving*>& get_items() const; // items of type k

      /**
       * \brief Returns the set of Paving boxes this subset is made of
       *
       * \return a vector of boxes
       */
      std::vector<ibex::IntervalVector> get_boxes() const; // items of type k

      /**
       * \brief Returns the boundary boxes of this subset
       *
       * Note that the returned boxes are degenerated as
       * they represent subpaving's edges.
       *
       * \param value_boundary value of the items representing the boundary of the subset
       * \param value_out value of the items representing no-solution boxes in the paving
       * \return a vector of boxes
       */
      std::vector<ibex::IntervalVector> get_boundary(int value_boundary = VALUE_MAYBE, int value_out = VALUE_OUT) const; // items of type k-1

      /// @}
      /// \name Methods related to topological degree
      /// @{

      bool zero_proven(ibex::IntervalVector (*f)(const ibex::IntervalVector& b));
      int zeros_number(ibex::IntervalVector (*f)(const ibex::IntervalVector& b), ibex::IntervalMatrix (*Jf)(const ibex::IntervalVector& b), float precision);

      /// @}

    protected:

      /// \name Methods related to topological degree
      /// @{

      int topological_degree(ibex::IntervalVector (*f)(const ibex::IntervalVector& b));
      bool all_positive_signs(const std::vector<int>& v_s) const;
      int orientation(const ibex::IntervalVector& b, const ibex::IntervalVector& parent_coface, int orientation) const;
      int compute_local_degree(ibex::IntervalVector (*f)(const ibex::IntervalVector& b), const ibex::IntervalVector& b, const ibex::IntervalVector& common_cocoface) const;
      std::vector<int> sign_vector(ibex::IntervalVector (*f)(const ibex::IntervalVector& b), const ibex::IntervalVector& b, const ibex::IntervalVector& common_cocoface) const;
      std::vector<ibex::IntervalVector> get_cofaces(const ibex::IntervalVector& b) const;
      std::vector<ibex::IntervalVector> get_cofaces(const ibex::IntervalVector& b, const ibex::IntervalVector& common_cocoface) const;
      int box_dimension(const ibex::IntervalVector& b) const;
      bool non_singular_jacobian(ibex::IntervalMatrix (*Jf)(const ibex::IntervalVector& b), float precision);

      /// @}

    protected:

      std::vector<const Paving*> m_v_subset_items;
  };
}

#endif