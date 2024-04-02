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
#include "codac2_CollectionCtc.h"

namespace codac2
{
  class SepCtcPair : public Sep
  {
    public:

      template<typename C1, // C1 should be some Ctc_<IntervalVector> class
        typename = typename std::enable_if<std::is_base_of<Ctc_<IntervalVector>,C1>::value>::type,
               typename C2, // C2 should be some Ctc_<IntervalVector> class
        typename = typename std::enable_if<std::is_base_of<Ctc_<IntervalVector>,C2>::value>::type>
      SepCtcPair(const C1& ctc_in, const C2& ctc_out)
        : SepCtcPair(std::make_shared<C1>(ctc_in),std::make_shared<C2>(ctc_out))
      { }

      SepCtcPair(std::shared_ptr<Ctc_<IntervalVector>> ctc_in, std::shared_ptr<Ctc_<IntervalVector>> ctc_out);

      virtual std::shared_ptr<Sep> copy() const;
      virtual BoxPair separate(const IntervalVector& x) const;

    protected:

      const std::shared_ptr<Ctc_<IntervalVector>> _ctc_in, _ctc_out;
  };
}