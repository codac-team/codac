/** 
 *  \file
 *  CtcEllipse class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include "codac2_CtcWrapper.h"
#include "codac2_OctaSym.h"

namespace codac2
{
  class CtcEllipse0 : public Ctc_<IntervalVector>
  {
    public:

      CtcEllipse0(const IntervalVector& q);
      virtual std::shared_ptr<Ctc> copy() const;
      void contract(IntervalVector& x) const;

    protected:

      const IntervalVector _q, _qs;
  };

  class CtcEllipse : public Ctc_<IntervalVector>
  {
    public:

      CtcEllipse(const Vector& q);
      virtual std::shared_ptr<Ctc> copy() const;
      void contract(IntervalVector& x) const;
      const Vector& q() const;

    protected:

      IntervalVector contract_ith(const OctaSym& i, const IntervalVector& x) const;

      const Vector _q;
  };
}