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
  class CtcArithmetic
  {
    public:
      
      /** Arithmetic contractors **/

      static bool ctcAbs(Tube& y, Tube& x);
      static bool ctcSqr(Tube& y, Tube& x);
      static bool ctcSqrt(Tube& y, Tube& x);
      static bool ctcPow(Tube& y, Tube& x, int p);
      static bool ctcPow(Tube& y, Tube& x, double p);
      static bool ctcPow(Tube& y, Tube& x, ibex::Interval& p);
      static bool ctcRoot(Tube& y, Tube& x, int p);
      static bool ctcExp(Tube& y, Tube& x);
      static bool ctcLog(Tube& y, Tube& x);
      static bool ctcCos(Tube& y, Tube& x);
      static bool ctcSin(Tube& y, Tube& x);
      static bool ctcTan(Tube& y, Tube& x);
      static bool ctcAcos(Tube& y, Tube& x);
      static bool ctcAsin(Tube& y, Tube& x);
      static bool ctcAtan(Tube& y, Tube& x);
      static bool ctcCosh(Tube& y, Tube& x);
      static bool ctcSinh(Tube& y, Tube& x);
      static bool ctcTanh(Tube& y, Tube& x);
      static bool ctcAcosh(Tube& y, Tube& x);
      static bool ctcAsinh(Tube& y, Tube& x);
      static bool ctcAtanh(Tube& y, Tube& x);
      static bool ctcAtan2(Tube& theta, Tube& y, Tube& x);
            
      /**
       * \brief Contract tubes given algebraic constraints.
       *
       * Constraints are defined thanks to an ibex function that has to vanish.
       * 
       * \param x* the tubes (variables) involved in the constraints
       * \param f the ibex function defining the constraints, the variables of which being the tubes
       * \return true if a contraction has been done, false otherwise
       */
      static bool contract(Tube& x1, Tube& x2, const ibex::Function& f);
      static bool contract(Tube& x1, Tube& x2, Tube& x3, const ibex::Function& f);
      static bool contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, const ibex::Function& f);
      static bool contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, const ibex::Function& f);
      static bool contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, const ibex::Function& f);
      static bool contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, Tube& x7, const ibex::Function& f);
      static bool contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, Tube& x7, Tube& x8, const ibex::Function& f);
  };
}

#endif