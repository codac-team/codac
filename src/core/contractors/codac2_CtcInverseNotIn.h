/** 
 *  \file codac2_CtcInverseNotIn.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_CtcUnion.h"
#include "codac2_CtcInverse.h"

namespace codac2
{
  template<typename Y,typename X=IntervalVector>
  class CtcInverseNotIn : public CtcUnion<X>
  {
    public:

      CtcInverseNotIn(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const Y& y, bool with_centered_form = true)
        : CtcUnion<X>(f.args()[0]->size() /* f must have only one arg, see following assert */)
      {
        assert_release(f.args().size() == 1 && "f must have only one arg");
        bool is_not_in = true;

        for(const auto& complem_y : y.complementary())
          *this |= CtcInverse_<Y,X>(f, complem_y, with_centered_form, is_not_in);
      }

      template<typename C, typename = typename std::enable_if<
          std::is_base_of_v<Ctc_<X>,C>
        >::type>
      CtcInverseNotIn(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const C& ctc_compl, bool with_centered_form = true)
        : CtcUnion<X>(f.args()[0]->size() /* f must have only one arg, see following assert */)
      {
        assert_release(f.args().size() == 1 && "f must have only one arg");
        bool is_not_in = true;
        *this |= CtcInverse_<Y,X>(f, ctc_compl, with_centered_form, is_not_in);
      }

      std::shared_ptr<Ctc_<X>> copy() const
      {
        return std::make_shared<CtcInverseNotIn<Y,X>>(*this);
      }

      void contract(X& x) const
      {
        CtcUnion<X>::contract(x);
      }
  };
}