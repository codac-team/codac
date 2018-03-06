/* ============================================================================
 *  tubex-lib - CtcIntertemp class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_CtcIntertemp.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcIntertemp::CtcIntertemp(Interval *t1, Interval *t2, Tube *y, const Tube *w) : m_t1(t1), m_t2(t2), m_z(z), m_y(y), m_w(w)
  {

  }

  bool CtcIntertemp::contract()
  {
    /* TO APPEAR SOON */

    return false;
  }
}