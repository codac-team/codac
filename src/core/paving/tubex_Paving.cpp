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

#include <iostream>
#include "tubex_Paving.h"
#include "ibex_LargestFirst.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Definition
  
  Paving::Paving(const IntervalVector& box, int value)
    : m_box(box), m_value(value)
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

  // Binary tree structure

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
    // todo: check is_leaf
    // todo: check ratio

    LargestFirst bisector(0., ratio);
    pair<IntervalVector,IntervalVector> subboxes = bisector.bisect(m_box);
    m_first_subpaving = new Paving(subboxes.first, m_value);
    m_second_subpaving = new Paving(subboxes.second, m_value);
  }

  // Accessing values

  bool Paving::is_leaf() const
  {
    return m_first_subpaving == NULL;
  }

  int Paving::value() const
  {
    return m_value;
  }

  const IntervalVector& Paving::box() const
  {
    return m_box;
  }

  // Setting values

  void Paving::set_value(int value)
  {
    m_value = value;
  }
}