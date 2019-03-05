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

#include <iostream>
#include "tubex_Set.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Set::Set(const IntervalVector& box, int value)
    : m_box(box), m_value(value)
  {

  }

  Set::~Set()
  {

  }

  int Set::value() const
  {
    return m_value;
  }

  const IntervalVector& Set::box() const
  {
    return m_box;
  }

  void Set::set_value(int value)
  {
    m_value = value;
  }
}