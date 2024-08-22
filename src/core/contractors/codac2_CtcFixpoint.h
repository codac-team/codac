/** 
 *  \file codac2_CtcFixpoint.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_CtcWrapper.h"
#include "codac2_Collection.h"
#include "codac2_IntervalVector.h"
#include "codac2_template_tools.h"

namespace codac2
{
  class CtcFixpoint : public Ctc_<IntervalVector>
  {
    public:

      template<typename C, typename = typename std::enable_if<
          std::is_base_of_v<Ctc_<IntervalVector>,C> || std::is_same_v<std::shared_ptr<Ctc_<IntervalVector>>,C>
        >::type>
      CtcFixpoint(const C& c, double ratio = 0.1)
        : Ctc_<IntervalVector>(size_of(c)), _ctc(c),
          _r(ratio)
      { }

      std::shared_ptr<Ctc_<IntervalVector>> copy() const;
      void contract(IntervalVector& x) const;

    protected:

      const Collection<Ctc_<IntervalVector>> _ctc;
      const double _r;
  };
}