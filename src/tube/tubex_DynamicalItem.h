/* ============================================================================
 *  tubex-lib - DynamicalItem class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef DynamicalItem_HEADER
#define DynamicalItem_HEADER

#include "ibex.h"

namespace tubex
{
  class DynamicalItem
  {
    public:

      virtual int dim() const = 0;
      virtual const ibex::Interval domain() const = 0; // todo: output const Interval& (reference)
      virtual const ibex::IntervalVector codomainBox() const = 0; // todo: output const Interval& (reference)
  };
}

#endif