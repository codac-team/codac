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

#ifndef __TUBEX_DYNAMICALITEM_H__
#define __TUBEX_DYNAMICALITEM_H__

#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"

namespace tubex
{
  class DynamicalItem
  {
    public:

      virtual int dim() const = 0;
      virtual const ibex::Interval domain() const = 0;
      virtual const ibex::IntervalVector codomain_box() const = 0;
      virtual const std::string class_name() const = 0;
  };
}

#endif