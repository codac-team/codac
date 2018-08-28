/* ============================================================================
 *  tubex-lib - Ctc class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Ctc.h"

using namespace std;

namespace tubex
{
  Ctc::Ctc()
  {

  }

  void Ctc::preserve_slicing(bool preserve)
  {
    m_preserve_slicing = preserve;
  }

  void Ctc::set_fast_mode(bool fast_mode)
  {
    m_fast_mode = fast_mode;
  }
}