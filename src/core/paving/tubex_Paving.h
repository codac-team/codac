/* ============================================================================
 *  tubex-lib - Paving class
 * ============================================================================
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

#include "ibex_IntervalVector.h"

namespace tubex
{
  enum PavingValue
  {
    // The following values can be used as flags for boolean selections
    VALUE_MAYBE = 0x01, // 1
    VALUE_OUT = 0x02,   // 2
    VALUE_IN = 0x04     // 4

    // Example: use (VALUE_MAYBE | VALUE_IN) to get subpavings of these types
  };

  // To avoid the need to cast for boolean operations:
  inline PavingValue operator|(PavingValue a, PavingValue b)
  { return static_cast<PavingValue>(static_cast<int>(a) | static_cast<int>(b)); }


  class Paving
  {
    public:

      // Definition
      Paving(const ibex::IntervalVector& box, int value = VALUE_MAYBE);
      ~Paving();

      // Binary tree structure
      Paving* get_first_subpaving();
      const Paving* get_first_subpaving() const;
      Paving* get_second_subpaving();
      const Paving* get_second_subpaving() const;
      void bisect(float ratio = 0.49);

      // Accessing values
      bool is_leaf() const;
      int value() const;
      const ibex::IntervalVector& box() const;

      // Setting values
      void set_value(int value);

    protected:

      int m_value;
      const ibex::IntervalVector m_box;
      Paving *m_first_subpaving = NULL, *m_second_subpaving = NULL;
  };
}

#endif