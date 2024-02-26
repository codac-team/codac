/** 
 *  Set class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include "codac_Set.h"

using namespace std;
using namespace ibex;

namespace codac
{
  Set::Set(const IntervalVector& box, SetValue value)
    : m_value(value), m_box(box)
  {

  }

  Set::~Set()
  {

  }

  SetValue Set::value() const
  {
    return m_value;
  }
  
  int Set::size() const
  {
    return m_box.size();
  }

  const IntervalVector& Set::box() const
  {
    return m_box;
  }

  void Set::set_value(SetValue value)
  {
    m_value = value;
  }
}