/** 
 *  Paving class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include "tubex_Paving.h"
#include "ibex_LargestFirst.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Definition
  
  Paving::Paving(const IntervalVector& box, int value)
    : Set(box, value)
  {

  }

  Paving::~Paving()
  {
    if(m_first_subpaving != NULL)
    {
      delete m_first_subpaving;
      delete m_second_subpaving;
    }
  }
  Paving* Paving::get_first_subpaving()
  {
    return const_cast<Paving*>(static_cast<const Paving&>(*this).get_first_subpaving());
  }

  const Paving* Paving::get_first_subpaving() const
  {
    return m_first_subpaving;
  }

  Paving* Paving::get_second_subpaving()
  {
    return const_cast<Paving*>(static_cast<const Paving&>(*this).get_second_subpaving());
  }

  const Paving* Paving::get_second_subpaving() const
  {
    return m_second_subpaving;
  }

  void Paving::bisect(float ratio)
  {
    assert(Interval(0.,1.).interior_contains(ratio));
    assert(is_leaf() && "only leaves can be bisected");

    LargestFirst bisector(0., ratio);
    pair<IntervalVector,IntervalVector> subboxes = bisector.bisect(m_box);
    m_first_subpaving = new Paving(subboxes.first, m_value);
    m_second_subpaving = new Paving(subboxes.second, m_value);
  }
  bool Paving::is_leaf() const
  {
    return m_first_subpaving == NULL;
  }
}