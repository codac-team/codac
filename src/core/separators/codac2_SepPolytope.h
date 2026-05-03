/** 
 *  \file codac2_SepPolygon.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Polytope.h"
#include "codac2_CtcPolytope.h"
#include "codac2_SepWrapper.h"

namespace codac2
{
  class SepPolytope : public Sep<SepPolytope>
  {
    public:

      SepPolytope(const Polytope& p);
      BoxPair separate(const IntervalVector& x) const;

    protected:
      const CtcPolytope _ctc;

  };
}
