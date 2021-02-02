/** 
 *  Beacon class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include "codac_Beacon.h"

using namespace std;
using namespace ibex;

namespace codac
{
  Beacon::Beacon(const Vector& pos)
    : m_pos(pos), m_pos_box(pos)
  {
    //assert(pos.size() == 3); // todo: clean this structure or remove this class
  }

  Beacon::Beacon(const IntervalVector& pos_box)
    : m_pos(pos_box.mid()), m_pos_box(pos_box)
  {
    assert(!pos_box.is_unbounded());
  }

  Beacon::Beacon(double x, double y, double z)
  {
    m_pos[0] = x; m_pos[1] = y; m_pos[2] = z;
    m_pos_box = IntervalVector(m_pos);
  }

  double Beacon::x() const
  {
    return m_pos[0];
  }

  double Beacon::y() const
  {
    return m_pos[1];
  }

  double Beacon::z() const
  {
    return m_pos[2];
  }

  const Vector& Beacon::pos() const
  {
    return m_pos;
  }

  const IntervalVector& Beacon::pos_box() const
  {
    return m_pos_box;
  }
}