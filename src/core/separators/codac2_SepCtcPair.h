/** 
 *  \file codac2_SepCtcPair.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Sep.h"
#include "codac2_CtcWrapper.h"
#include "codac2_Collection.h"
#include "codac2_template_tools.h"

namespace codac2
{
  class SepCtcPair : public Sep
  {
    public:

      template<typename C1, typename C2, typename = typename std::enable_if<(
          (std::is_base_of_v<Ctc_<IntervalVector>,C1> || std::is_same_v<std::shared_ptr<Ctc_<IntervalVector>>,C1>) &&
          (std::is_base_of_v<Ctc_<IntervalVector>,C2> || std::is_same_v<std::shared_ptr<Ctc_<IntervalVector>>,C2>)
        )>::type>
      SepCtcPair(const C1& ctc_in, const C2& ctc_out)
        : Sep(size_of(ctc_in)), _ctc_in_out(ctc_in, ctc_out)
      {
        assert_release(size_of(ctc_in) == size_of(ctc_out));
      }

      std::shared_ptr<Sep> copy() const;
      BoxPair separate(const IntervalVector& x) const;

    protected:

      const Collection<Ctc_<IntervalVector>> _ctc_in_out;
  };
}