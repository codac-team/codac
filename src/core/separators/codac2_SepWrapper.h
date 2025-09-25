/** 
 *  \file codac2_SepWrapper.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou, Quentin Brateau
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Sep.h"
#include "codac2_SepCtcPair.h"
#include "codac2_CtcWrapper.h"
#include "codac2_CtcUnion.h"
#include "codac2_Paving.h"

namespace codac2
{
  /// \cond SepWrapper<T>

  template<typename T>
  class SepWrapper : public Sep<SepWrapper<T>>
  {
    public:

      SepWrapper(const T& y)
        : Sep<SepWrapper<T>>(y.size()), _y(y)
      { }

    protected:

      const T _y;
  };

  /// \endcond

  template<>
  class SepWrapper<IntervalVector> : public SepCtcPair
  {
    public:

      SepWrapper(const IntervalVector& y)
        : SepCtcPair([y]()
              {
                // Inner contractor:
                CtcUnion<IntervalVector> cu(y.size());
                for(const auto& complem_y : y.complementary())
                  cu |= CtcWrapper<IntervalVector>(complem_y);
                return cu;
              }(),
            CtcWrapper<IntervalVector>(y))
      { }

      BoxPair separate(const IntervalVector& x) const;
      // ^ is referenced here for Doxygen purpose
  };

  template<>
  class SepWrapper<PavingInOut>: public Sep<SepWrapper<PavingInOut>>
  {
    public:

      SepWrapper(const PavingInOut& P)
        : Sep<SepWrapper<PavingInOut>>(P.size()), _P(P)
      { }

      BoxPair separate(const IntervalVector& x) const;

    protected:
    
      const PavingInOut _P;
  };
}