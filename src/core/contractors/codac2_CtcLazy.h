/** 
 *  \file codac2_CtcLazy.h
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

namespace codac2
{
  class CtcLazy : public Ctc_<IntervalVector>
  {
    public:

      template<typename C, typename = typename std::enable_if<
          std::is_base_of_v<Ctc_<IntervalVector>,C>
        >::type>
      CtcLazy(const C& c)
        : Ctc_<IntervalVector>(c.size()), _ctc(c),
          _r(1./std::pow(2,c.size()-1))
      { }

      std::shared_ptr<Ctc> copy() const;
      void contract(IntervalVector& x) const;

    protected:

      const Collection<Ctc_<IntervalVector>> _ctc;
      const double _r;
  };
}