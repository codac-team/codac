/* ============================================================================
 *  tubex-lib - CtcDelay class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2017
 * ---------------------------------------------------------------------------- */

#include "tubex_CtcDelay.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDelay::CtcDelay(Interval *a, Tube *x, Tube *y) : m_a(a), m_x(x), m_y(y)
  {

  }

  bool CtcDelay::contract()
  {
    /* TO APPEAR SOON */
    
    return false;
  }
}