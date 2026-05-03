/** 
 *  \file codac2_CtcPolytope.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Damien Massé
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Polytope.h"
#include "codac2_CtcWrapper.h"

namespace codac2
{
  class CtcPolytope : public Ctc<CtcPolytope,IntervalVector>
  {
    public:

      CtcPolytope(const Polytope& p);

      void contract(IntervalVector& x) const;
      inline const Polytope& p() const { return _p; };

    protected:

      const Polytope _p;
  };

}
