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
#include "codac2_TubeVector.h"
#include "codac2_SliceVector.h"

namespace codac2
{
  using codac::TFunction;

  /**
   * \class CtcDiffInclusion
   * \brief ...
   */
  class CtcDiffInclusion
  {
    public:

      CtcDiffInclusion(const TFunction& t);
      void contract(TubeVector& x, const TubeVector& u);
      void contract(SliceVector& x, const SliceVector& u);
      const TFunction& f() const;

    protected:

      friend class SliceVector; // to be removed
      const TFunction _f;
  };
}

#endif