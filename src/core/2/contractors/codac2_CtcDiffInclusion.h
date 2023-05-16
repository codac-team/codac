/** 
 *  \file
 *  CtcDiffInclusion class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCDIFFINCLUSION_H__
#define __CODAC2_CTCDIFFINCLUSION_H__

#include "codac_TFunction.h"
#include "codac_DynCtc.h"
#include "codac2_Tube.h"
#include "codac2_Slice.h"

namespace codac2
{
  using codac::TFunction;
  using codac::TimePropag;

  /**
   * \class CtcDiffInclusion
   * \brief ...
   */
  class CtcDiffInclusion
  {
    public:

      CtcDiffInclusion(const TFunction& t);
      void contract(codac2::Tube<IntervalVector>& x, const codac2::Tube<IntervalVector>& u, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(Slice<IntervalVector>& x, const Slice<IntervalVector>& u, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      const TFunction& f() const;

    protected:

      //friend class Slice; // to be removed
      const TFunction _f;
  };
}

#endif