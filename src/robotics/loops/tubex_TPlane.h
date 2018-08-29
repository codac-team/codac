/* ============================================================================
 *  tubex-lib - TPlane class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : -
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_TPLANE_H__
#define __TUBEX_TPLANE_H__

#include "tubex_Paving.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  class TPlane : public Paving
  {
    public:

      TPlane(const ibex::IntervalVector& tbox);
      void compute(float precision, const TubeVector& x, const TubeVector& v);

    protected:

  };
}

#endif