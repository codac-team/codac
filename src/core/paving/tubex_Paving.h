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

#include "tubex_Set.h"

namespace tubex
{
  class Paving : public Set
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
      bool is_leaf() const;

    protected:

      Paving *m_first_subpaving = NULL, *m_second_subpaving = NULL;
  };
}

#endif