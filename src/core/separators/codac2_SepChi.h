/** 
 *  \file codac2_SepChi.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <memory>
#include "codac2_Collection.h"
#include "codac2_IntervalVector.h"
#include "codac2_Sep.h"
#include "codac2_template_tools.h"

namespace codac2
{
  /**
   * \brief A separator on the Chi constraint
   * If [x] in Sa, then Sb, else Sc
   */
  class SepChi : public Sep<SepChi>
  {
    public:

      template<typename S1, typename S2, typename S3>
        requires IsSepBaseOrPtr<S1> && IsSepBaseOrPtr<S2> && IsSepBaseOrPtr<S3>
      SepChi(const S1& Sa, const S2& Sb, const S3& Sc)
        : Sep<SepChi>(size_of(Sa)), _seps({Sa, Sb, Sc})
      { 
        assert_release(size_of(Sa) == size_of(Sb) && size_of(Sa) == size_of(Sc)
          && "size of Sa, Sb, and Sc should be the same");
      }

      BoxPair separate(const IntervalVector& x) const;

    protected:

      const Collection<SepBase> _seps;
  };
}