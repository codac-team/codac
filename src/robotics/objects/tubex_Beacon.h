/* ============================================================================
 *  tubex-lib - Beacon class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_BEACON_H__
#define __TUBEX_BEACON_H__

#include "ibex_IntervalVector.h"

namespace tubex
{
  class Beacon
  {
    public:

      Beacon(const ibex::Vector& pos);
      Beacon(double x, double y, double z = 0.);
      const ibex::Vector& pos() const;
      const ibex::IntervalVector& pos_box() const;

    protected:

      ibex::Vector m_pos = ibex::Vector(3, 0.);
      ibex::IntervalVector m_pos_box = ibex::IntervalVector(3);
  };
}

#endif