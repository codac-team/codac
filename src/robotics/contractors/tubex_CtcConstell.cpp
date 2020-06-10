/** 
 *  CtcConstell class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include "tubex_CtcConstell.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcConstell::CtcConstell(const vector<IntervalVector>& map)
    : Ctc(2), m_map(map)
  {
    // todo: binary tree for logarithmic complexity
  }

  CtcConstell::CtcConstell(const list<IntervalVector>& map)
    : Ctc(2)
  {
    for(const auto& b : map)
      m_map.push_back(b);
    // todo: binary tree for logarithmic complexity
  }

  CtcConstell::~CtcConstell()
  {

  }

  void CtcConstell::contract(IntervalVector &beacon_box)
  {
    assert(beacon_box.size() == 2);
    IntervalVector envelope_beacons(2, Interval::EMPTY_SET);
    for(const auto m : m_map)
      envelope_beacons |= beacon_box & m.subvector(0,1);
    beacon_box = envelope_beacons;
  }
}