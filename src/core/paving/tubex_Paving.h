/** 
 *  @file
 *  Paving class
 * ================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_PAVING_H__
#define __TUBEX_PAVING_H__

#include "tubex_Set.h"

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

    protected:

      Paving *m_first_subpaving = NULL, *m_second_subpaving = NULL; //!< tree structure
  };
}

#endif