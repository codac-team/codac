/* ============================================================================
 *  tubex-lib - Tube arithmetic contractors
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2017
 * ---------------------------------------------------------------------------- */

#ifndef CtcArithmetic_HEADER
#define CtcArithmetic_HEADER

#include "tubex_Tube.h"

namespace tubex
{
  /** Arithmetic contractors **/

  bool ctcAbs(Tube& y, Tube& x);
  bool ctcSqr(Tube& y, Tube& x);
  bool ctcSqrt(Tube& y, Tube& x);
  bool ctcPow(Tube& y, Tube& x, int p);
  bool ctcPow(Tube& y, Tube& x, double p);
  bool ctcPow(Tube& y, Tube& x, ibex::Interval& p);
  bool ctcRoot(Tube& y, Tube& x);
  bool ctcExp(Tube& y, Tube& x);
  bool ctcLog(Tube& y, Tube& x);
  bool ctcCos(Tube& y, Tube& x);
  bool ctcSin(Tube& y, Tube& x);
  bool ctcTan(Tube& y, Tube& x);
  bool ctcAcos(Tube& y, Tube& x);
  bool ctcAsin(Tube& y, Tube& x);
  bool ctcAtan(Tube& y, Tube& x);
  bool ctcCosh(Tube& y, Tube& x);
  bool ctcSinh(Tube& y, Tube& x);
  bool ctcTanh(Tube& y, Tube& x);
  bool ctcAcosh(Tube& y, Tube& x);
  bool ctcAsinh(Tube& y, Tube& x);
  bool ctcAtanh(Tube& y, Tube& x);
  bool ctcAtan2(Tube& theta, Tube& y, Tube& x);
}

#endif