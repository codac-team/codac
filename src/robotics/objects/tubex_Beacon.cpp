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

#include <iostream>
#include "tubex_Beacon.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Beacon::Beacon(const Vector& pos)
    : m_pos(pos), m_pos_box(pos)
  {
    assert(pos.size() == 3);
  }

  Beacon::Beacon(double x, double y, double z)
  {
    m_pos[0] = x; m_pos[1] = y; m_pos[2] = z;
    m_pos_box = IntervalVector(m_pos);
  }

  const ibex::Vector& Beacon::pos() const
  {
    return m_pos;
  }

  const ibex::IntervalVector& Beacon::pos_box() const
  {
    return m_pos_box;
  }
}