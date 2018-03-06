/* ============================================================================
 *  tubex-lib - CtcCompo class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2017
 * ---------------------------------------------------------------------------- */

#include "tubex_CtcCompo.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcCompo::CtcCompo(Tube *x, Tube *y, Tube *z) : m_x(x), m_y(y), m_z(z)
  {

  }

  bool CtcCompo::contract()
  {
    /* TO APPEAR SOON */

    return false;
  }
}