/** 
 *  \file codac2_CtcEllipse.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Luc Jaulin, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_CtcWrapper.h"
#include "codac2_OctaSym.h"

namespace codac2
{
  class CtcEllipse0 : public Ctc<CtcEllipse0,IntervalVector>
  {
    public:

      CtcEllipse0(const IntervalVector& q);
      void contract(IntervalVector& x) const;

    protected:

      const IntervalVector _q, _qs;
  };

  class CtcEllipse : public Ctc<CtcEllipse,IntervalVector>
  {
    public:

      CtcEllipse(const Vector& q);
      void contract(IntervalVector& x) const;
      const Vector& q() const;

    protected:

      IntervalVector contract_ith(const OctaSym& i, const IntervalVector& x) const;

      const Vector _q;
  };
}