/** 
 *  \file codac2_SepTransform.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_Sep.h"
#include "codac2_Collection.h"
#include "codac2_AnalyticFunction.h"
#include "codac2_CtcWrapper.h"
#include "codac2_CtcInverse.h"
#include "codac2_template_tools.h"

namespace codac2
{
  class SepTransform : public Sep<SepTransform>
  {
    public:

      template<typename S>
        requires IsSepBaseOrPtr<S>
      SepTransform(const S& s, const AnalyticFunction<VectorOpValue>& f, const AnalyticFunction<VectorOpValue>& f_inv)
        : Sep<SepTransform>(f.args()[0]->size() /* f must have only one arg, see following assert */),
          _sep(s), _ctc_inv(f, IntervalVector(f_inv.args()[0]->size())), _f(f), _f_inv(f_inv)
      {
        assert_release(f.args().size() == 1 && "f must have only one arg");
        assert_release(f.args().size() == f_inv.args().size());
      }

      BoxPair separate(const IntervalVector& x) const;

    protected:

      const Collection<SepBase> _sep;
      const CtcInverse_<IntervalVector> _ctc_inv;
      const AnalyticFunction<VectorOpValue> _f, _f_inv;
  };
}