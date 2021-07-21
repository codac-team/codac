/** 
 *  \file
 *  Set class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_SET_H__
#define __CODAC_SET_H__

#include "codac_IntervalVector.h"

namespace codac
{
  /**
   * \enum SetValue
   * \brief Defines a set of feasible values of a set
   *
   * \note These values can be used as flags for boolean selections.
   * Example of use: `(UNKNOWN | IN)` to get items of these types
   */
  enum class SetValue
  {
    OUT = 0x02,   ///< outside the solution set
    IN = 0x04,     ///< inside the solution set
    UNKNOWN = 0x01, ///< unable to conclude
    PENUMBRA = 0x08, ///< inside the penumbra set
    UNCHECKED = 0x10,
  };

  /**
   * \brief Allows tests on combinations of two SetValue.
   *
   * Used for tests such as `if(set_val & SetValue::OUT) { ... }`
   *
   * \param a first SetValue operand
   * \param b second SetValue operand
   * \return intersection of set values
   */
  inline int operator&(SetValue a, SetValue b)
  { return static_cast<int>(static_cast<int>(a) & static_cast<int>(b)); }

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
       * \param value integer of the set, `SetValue::UNKNOWN` by default
       */
      Set(const IntervalVector& box, SetValue value = SetValue::UNKNOWN);

      /**
       * \brief Set destructor
       */
      ~Set();

      /**
       * \brief Returns the value of this set
       *
       * \return the integer value
       */
      SetValue value() const;

      /**
       * \brief Returns the dimension of the paving
       *
       * \return n
       */
      int size() const;

      /**
       * \brief Returns the n-dimensional box representing this set
       *
       * \return the box
       */
      const IntervalVector& box() const;

      /**
       * \brief Sets the integer value for this set
       *
       * \param value
       */
      void set_value(SetValue value);

    protected:

      SetValue m_value; //!< integer value of this set
      IntervalVector m_box; //!< box representing this set
  };
}

#endif