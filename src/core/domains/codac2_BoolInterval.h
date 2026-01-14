/** 
 *  \file codac2_BoolInterval.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <ostream>

namespace codac2
{
  /**
   * \enum BoolInterval
   * \brief Enumeration representing a boolean interval.
   * 
   * The logical operators ``&`` and ``|`` can be used to combine ``BoolInterval`` values.

   * Set operators ``&`` (intersection) and ``|`` (union) and logical operators ``&&`` (AND) and ``||`` (OR)
   * are overloaded to combine ``BoolInterval`` values consistently.
   */
  enum class BoolInterval
  {
    FALSE = 0x01,
    TRUE = 0x02,
    /** ``EMPTY`` is equivalent to the operation ``TRUE & FALSE``. */
    EMPTY = 0x00,
    /** ``UNKNOWN`` is equivalent to the operation ``TRUE | FALSE``. */
    UNKNOWN = 0x01 | 0x02
  };

  /**
   * \brief Intersection operator for ``BoolInterval`` sets.
   *
   * Performs a bitwise AND on the integer representations of two ``BoolInterval`` values.
   * 
   * \param x The left-hand ``BoolInterval`` operand.
   * \param y The right-hand ``BoolInterval`` operand.
   * \return The result of the intersection.
   */
  constexpr BoolInterval operator&(BoolInterval x, BoolInterval y)
  {
    return static_cast<BoolInterval>(static_cast<int>(x) & static_cast<int>(y));
  }

  /**
   * \brief Union operator for ``BoolInterval`` sets.
   *
   * Performs a bitwise OR on the integer representations of two ``BoolInterval`` values.
   * 
   * \param x The left-hand ``BoolInterval`` operand.
   * \param y The right-hand ``BoolInterval`` operand.
   * \return The result of the union.
   */
  constexpr BoolInterval operator|(BoolInterval x, BoolInterval y)
  {
    return static_cast<BoolInterval>(static_cast<int>(x) | static_cast<int>(y));
  }

  /**
   * \brief Logical AND operator for ``BoolInterval`` sets.
   *
   * \note Not to be confused with the bitwise AND operator ``&`` 
   * (which represents an interval *intersection* rather than a logical conjunction).
   * 
   * \param x The left-hand ``BoolInterval`` operand.
   * \param y The right-hand ``BoolInterval`` operand.
   * \return The logical AND result as a ``BoolInterval``.
   */
  constexpr BoolInterval operator&&(BoolInterval x, BoolInterval y)
  {
    if((x == BoolInterval::EMPTY) || (y == BoolInterval::EMPTY))
      return BoolInterval::EMPTY;
    if((x == BoolInterval::FALSE) || (y == BoolInterval::FALSE))
      return BoolInterval::FALSE;
    if((x == BoolInterval::UNKNOWN) || (y == BoolInterval::UNKNOWN))
      return BoolInterval::UNKNOWN;
    return BoolInterval::TRUE;
  }

  /**
   * \brief Logical OR operator for ``BoolInterval`` sets.
   *
   * \note Not to be confused with the bitwise OR operator ``|`` 
   * (which represents an interval *union* rather than a logical disjunction).
   * 
   * \param x The left-hand ``BoolInterval`` operand.
   * \param y The right-hand ``BoolInterval`` operand.
   * \return The logical OR result as a ``BoolInterval``.
   */
  constexpr BoolInterval operator||(BoolInterval x, BoolInterval y)
  {
    if((x == BoolInterval::EMPTY) || (y == BoolInterval::EMPTY))
      return BoolInterval::EMPTY;
    if((x == BoolInterval::TRUE) || (y == BoolInterval::TRUE))
      return BoolInterval::TRUE;
    if((x == BoolInterval::UNKNOWN) || (y == BoolInterval::UNKNOWN))
      return BoolInterval::UNKNOWN;
    return BoolInterval::FALSE;
  }

  /**
   * \brief Returns the complementary of a BoolInterval
   * 
   * \param x the boolean interval
   * \return the complementary
   */
  inline BoolInterval operator~(BoolInterval x)
  {
    switch(x)
    {
      case BoolInterval::FALSE:
        return BoolInterval::TRUE;
      case BoolInterval::TRUE:
        return BoolInterval::FALSE;
      default:
        return x;
    }
  }
  
  /**
   * \brief Streams out a BoolInterval
   * 
   * \param os the stream to be updated
   * \param x the boolean interval to stream out
   * \return a reference to the updated stream
   */
  inline std::ostream& operator<<(std::ostream& os, const BoolInterval& x)
  {
    switch(x)
    {
      case BoolInterval::EMPTY:
        os << "[ empty ]";
        break;
      case BoolInterval::FALSE:
        os << "[ false ]";
        break;
      case BoolInterval::TRUE:
        os << "[ true ]";
        break;
      case BoolInterval::UNKNOWN:
        os << "[ true, false ]";
        break;
    }
    return os;
  }
}