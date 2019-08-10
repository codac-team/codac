/** 
 *  \file
 *  Paving class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
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
   * \brief Multi-dimensional paving representation of a set
   *
   * The paving is made of a set of multi-dimensional boxes.
   * The implementation of this paving is made as a binary tree.
   */
  class Paving : public Set
  {
    public:

      /**
       * \brief Creates a paving
       *
       * \param box n-dimensional box defining the paving
       * \param value integer of the set, `SetValue::VALUE_MAYBE` by default
       */
      Paving(const ibex::IntervalVector& box, int value = VALUE_MAYBE);

      /**
       * \brief Set destructor
       */
      ~Paving();

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


      const Paving* get_first_leaf(int val, bool without_flag) const;
      void get_pavings_intersecting(int val, const ibex::IntervalVector& box_to_intersect, std::vector<const Paving*>& v_subpavings, bool no_degenerated_intersection = false) const;
      void get_neighbours(std::vector<const Paving*>& v_neighbours, int val = -1, bool without_flag = false) const; // -1 = all

      bool flag() const;
      void set_flag() const;
      void reset_flags() const;

      std::vector<ConnectedSubset> get_connected_subsets(bool sort_by_size = false) const;

    protected:

      mutable bool m_flag;
      Paving *m_root = NULL;
      Paving *m_first_subpaving = NULL, *m_second_subpaving = NULL; //!< tree structure
  };
}

#endif