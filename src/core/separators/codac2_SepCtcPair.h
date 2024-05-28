/** 
 *  \file
 *  SepCtcPair class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include "codac2_Sep.h"
#include "codac2_CtcWrapper.h"
#include "codac2_Collection.h"

namespace codac2
{
  class SepCtcPair : public Sep
  {
    public:

      template<typename C1, typename C2, typename = typename std::enable_if<(
          std::is_base_of_v<Ctc_<IntervalVector>,C1> &&
          std::is_base_of_v<Ctc_<IntervalVector>,C2>
        )>::type>
      SepCtcPair(const C1& ctc_in, const C2& ctc_out)
        : Sep(ctc_in.size()), _ctc_in_out(ctc_in, ctc_out)
      {
        assert(ctc_in.size() == ctc_out.size());
      }

      virtual std::shared_ptr<Sep> copy() const;
      virtual BoxPair separate(const IntervalVector& x) const;

    protected:

      Collection<Ctc_<IntervalVector>> _ctc_in_out;
  };
}