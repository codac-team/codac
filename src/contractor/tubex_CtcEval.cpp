/* ============================================================================
 *  tubex-lib - CtcEval class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_CtcEval.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcEval::CtcEval(Interval *t, Interval *z, Tube *y, const Tube *w) : m_t(t), m_z(z), m_y(y), m_w(w)
  {

  }

  bool CtcEval::contract()
  {
    /* TO APPEAR SOON */

    return false;
  }
}