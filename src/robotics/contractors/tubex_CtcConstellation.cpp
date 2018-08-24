/* ============================================================================
 *  tubex-lib - CtcConstellation class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : -
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include <list>
#include "tubex_CtcConstellation.h"
#include "tubex_SlicingException.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcConstellation::CtcConstellation()
  {

  }

  bool CtcConstellation::contract(IntervalVector &beacon_box, const vector<Beacon>& map) const
  {
    IntervalVector envelope_beacons(2, Interval::EMPTY_SET);
    for(int i = 0 ; i < map.size() ; i++)
      if(beacon_box.contains(map[i].pos().subvector(0,1)))
        envelope_beacons |= map[i].pos().subvector(0,1);
    beacon_box = envelope_beacons;
    // todo: return value
  }
}