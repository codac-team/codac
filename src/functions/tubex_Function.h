/* ============================================================================
 *  tubex-lib - Function class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef Function_HEADER
#define Function_HEADER

#include "ibex_Function.h"
#include "tubex_Fnc.h"

namespace tubex
{
  class Function : public Fnc
  {
    public:

      Function(const char* y);
      Function(const char* x1, const char* y);
      ~Function();

      TubeVector eval(const TubeVector& x) const;

    protected:

      ibex::Function *m_ibex_f;
  };
}

#endif