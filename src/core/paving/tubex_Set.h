/* ============================================================================
 *  tubex-lib - Set class
 * ============================================================================
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
  enum SetValue
  {
    // The following values can be used as flags for boolean selections
    VALUE_MAYBE = 0x01, // 1
    VALUE_OUT = 0x02,   // 2
    VALUE_IN = 0x04     // 4

    // Example: use (VALUE_MAYBE | VALUE_IN) to get items of these types
  };

  // To avoid the need to cast for boolean operations:
  inline SetValue operator|(SetValue a, SetValue b)
  { return static_cast<SetValue>(static_cast<int>(a) | static_cast<int>(b)); }


  class Set
  {
    public:

      // Definition
      Set(const ibex::IntervalVector& box, int value = VALUE_MAYBE);
      ~Set();

      // Accessing values
      int value() const;
      const ibex::IntervalVector& box() const;

      // Setting values
      void set_value(int value);

    protected:

      int m_value;
      const ibex::IntervalVector m_box;
  };
}

#endif