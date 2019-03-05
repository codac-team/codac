/** 
 *  @file
 *  Set class
 * ================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_SET_H__
#define __TUBEX_SET_H__

#include "ibex_IntervalVector.h"

namespace tubex
{
  /**
   * \enum SetValue
   * \brief Defines a set of feasible values of a set
   *
   * \note These values can be used as flags for boolean selections.
   * Example of use: `(VALUE_MAYBE | VALUE_IN)` to get items of these types
   */
  enum SetValue
  {
    VALUE_MAYBE = 0x01, ///< unable to conclude
    VALUE_OUT = 0x02,   ///< outside the solution set
    VALUE_IN = 0x04     ///< inside the solution set
  };

  /**
   * \brief Union of two SetValue values.
   *
   * Avoids the need to cast for boolean union operations.
   *
   * \param a
   * \param b
   * \return the interval union `a|b`
   */
  inline SetValue operator|(SetValue a, SetValue b)
  { return static_cast<SetValue>(static_cast<int>(a) | static_cast<int>(b)); }

  /**
   * \class Set
   * \brief Multi-dimensional interval-based representation of a set
   *
   * Its value is represented by an integer. 
   */
  class Set
  {
    public:

      /**
       * \brief Creates a set
       *
       * \param box n-dimensional box representing the set
       * \param value integer of the set, `SetValue::VALUE_MAYBE` by default
       */
      Set(const ibex::IntervalVector& box, int value = VALUE_MAYBE);

      /**
       * \brief Set destructor
       */
      ~Set();

      /**
       * \brief Returns the value of this set
       *
       * \return the integer value
       */
      int value() const;

      /**
       * \brief Returns the n-dimensional box representing this set
       *
       * \return the box
       */
      const ibex::IntervalVector& box() const;

      /**
       * \brief Sets the integer value for this set
       *
       * \param value
       */
      void set_value(int value);

    protected:

      int m_value; //!< integer value of this set
      const ibex::IntervalVector m_box; //!< box representing this set
  };
}

#endif