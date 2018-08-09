/* ============================================================================
 *  tubex-lib - AbstractTube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_AbstractTube.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  bool AbstractTube::share_same_slicing(const AbstractTube& x1, const AbstractTube& x2)
  {
    // todo
  }

  // String
  
  ostream& operator<<(ostream& str, const AbstractTube& x)
  {
    str << x.class_name() << " " << x.domain() << "↦" << x.codomain_box()
        << ", " << x.nb_slices()
        << " slice" << (x.nb_slices() > 1 ? "s" : "")
        << flush;
    return str;
  }
}