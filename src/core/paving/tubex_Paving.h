/** 
 *  \file
 *  Paving class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_PAVING_H__
#define __TUBEX_PAVING_H__

#include "tubex_Set.h"
#include "tubex_ConnectedSubset.h"

namespace tubex
{
  /**
   * \class Paving
   * \brief Multi-dimensional paving as representation of a set
   *
   * The paving is made of a set of multi-dimensional boxes.
   * The implementation of this paving is made as a binary tree.
   */
  class Paving : public Set
  {
    public:

      /// \name Basics
      /// @{

      /**
       * \brief Creates a paving
       *
       * \param box n-dimensional box defining the paving
       * \param value integer of the set, `SetValue::VALUE_MAYBE` by default
       */
      Paving(const ibex::IntervalVector& box, SetValue value = SetValue::MAYBE);

      /**
       * \brief Paving destructor
       */
      ~Paving();

      /// @}
      /// \name Binary tree structure
      /// @{

      /**
       * \brief Points to the first subpaving of the binary tree implementing this paving set
       *
       * \return a pointer to the first Paving object, or NULL is this set is a leaf
       */
      Paving* get_first_subpaving();

      /**
       * \brief Points to the first subpaving of the binary tree implementing this paving set
       *
       * \return a const pointer to the first Paving object, or NULL is this set is a leaf
       */
      const Paving* get_first_subpaving() const;

      /**
       * \brief Points to the second subpaving of the binary tree implementing this paving set
       *
       * \return a pointer to the second Paving object, or NULL is this set is a leaf
       */
      Paving* get_second_subpaving();

      /**
       * \brief Points to the second subpaving of the binary tree implementing this paving set
       *
       * \return a const pointer to the second Paving object, or NULL is this set is a leaf
       */
      const Paving* get_second_subpaving() const;

      /**
       * \brief Returns a pointer to the root of the paving structure
       *
       * \return a pointer to the Paving object
       */
      Paving* get_root();

      /**
       * \brief Returns a pointer to the root of the paving structure
       *
       * \return a const pointer to the Paving object
       */
      const Paving* get_root() const;

      /**
       * \brief Points to the first leaf of the specified value
       *
       * \param val the value of the leaf we are looking for
       * \param without_flag optional research mode: select the first leaf among non-flagged items only
       * \return a pointer to the Paving object
       */
      Paving* get_first_leaf(SetValue val, bool without_flag = false);

      /**
       * \brief Points to the first leaf of the paving structure
       *
       * \param val the value of the leaf we are looking for
       * \param without_flag optional research mode: select the first leaf among non-flagged items only
       * \return a const pointer to the Paving object
       */
      const Paving* get_first_leaf(SetValue val, bool without_flag = false) const;

      /**
       * \brief Bisects this paving into two subpavings with some ratio bisection
       *
       * Will create two nodes in the binary tree implementing this set.
       *
       * \param ratio the bisection ratio (default value: 0.49)
       */
      void bisect(float ratio = 0.49);

      /**
       * \brief Returns true if this paving is made of two subpavings
       *
       * \return `true` if this is a leaf, `false` otherwise
       */
      bool is_leaf() const;

      /// @}
      /// \name Flags
      /// @{

      /**
       * \brief Tests whether this paving has been flagged or not
       *
       * \return `true` if flagged
       */
      bool flag() const;

      /**
       * \brief Flags this paving and all its children
       */
      void set_flag() const;

      /**
       * \brief Removes the flags of this Paving and all its children
       */
      void reset_flags() const;

      /// @}
      /// \name Extract methods
      /// @{

      /**
       * \brief Returns a set of Paving leaves of some value and intersecting a given box
       *
       * \param val the value of the leaves we are looking for
       * \param box_to_intersect the box the returned leaves will intersect
       * \param v_subpavings the set of returned objects
       * \param no_degenerated_intersection if `true`, then the objects for which the 
       *                                    intersection amounts to a point will not be returned
       */
      void get_pavings_intersecting(SetValue val,
          const ibex::IntervalVector& box_to_intersect,
          std::vector<const Paving*>& v_subpavings,
          bool no_degenerated_intersection = false) const;

      /**
       * \brief Returns the neighbors (adjacent items) of this Paving, having some value
       *
       * \param v_neighbours the set of leaves to be returned
       * \param val optional value of the leaves we are looking for, `-1` for no restriction
       * \param without_flag optional research mode: select the leaves among non-flagged items only
       */
      void get_neighbours(std::vector<const Paving*>& v_neighbours,
          SetValue val = SetValue::IN | SetValue::OUT | SetValue::MAYBE, // all by default
          bool without_flag = false) const;

      /**
       * \brief Returns the set of connected subsets of this paving
       *
       * A connected subset is a topological space that cannot be represented
       * as the union of two or more disjoint non-empty open subsets.
       *
       * \note Note that this method is preferably called from the root Paving.
       *
       * \param sort_by_size (optional) if `true` then the subsets will be
       *                     sort by the number of boxes they are made of
       * \return the set of connected subsets
       */
      std::vector<ConnectedSubset> get_connected_subsets(bool sort_by_size = false) const;

      /// @}

    protected:

      mutable bool m_flag; //!< optional flag, can be used by search algorithms
      Paving *m_root = NULL; //!< pointer to the root
      Paving *m_first_subpaving = NULL, *m_second_subpaving = NULL; //!< tree structure
  };
}

#endif