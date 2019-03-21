/** 
 *  Set class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

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