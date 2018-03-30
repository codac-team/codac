/* ============================================================================
 *  tubex-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Tube.h"
#include "tubex_TubeSlice.h"

using namespace std;
using namespace ibex;

namespace tubex
{
    
    bool TubeTree::isSlice() const
    {
      return false;
    }
  /*Tube::Tube(const Interval& domain) : TubeNode(domain, Interval::ALL_REALS)
  {
    m_first_tubenode = new TubeSlice(domain, Interval::ALL_REALS);
  }

  Interval Tube::operator()(double t) const
  {
    return Interval::ALL_REALS;
  }

  void Tube::sample(double t, const Interval& gate)
  {

  }*/
    
    Interval TubeTree::invert(const Interval& y, const Interval& t) const
    {
      if(!m_domain.intersects(t) || !codomain().intersects(y))
        return Interval::EMPTY_SET;

      else
        return m_first_tubenode->invert(y, t) | m_second_tubenode->invert(y, t);
    }
}