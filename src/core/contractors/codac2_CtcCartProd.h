/** 
 *  \file
 *  CtcCartProd class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Quentin Brateau, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <type_traits>
#include "codac2_CtcWrapper.h"
#include "codac2_Collection.h"

namespace codac2
{
  class CtcCartProd : public Ctc_<IntervalVector>
  {
    public:

      template<typename C1, typename... C, typename = typename std::enable_if<
          std::is_base_of_v<Ctc_<IntervalVector>,C1> && 
          (true && ... && (std::is_base_of_v<Ctc_<IntervalVector>,C>)), void
        >::type>
      CtcCartProd(const C1& c1, const C&... c)
        : Ctc_<IntervalVector>(c1.size()), _ctcs(c...)
      {
        _ctcs.add_shared_ptr(std::make_shared<C1>(c1));
      }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<CtcCartProd>(*this);
      }

      void contract(IntervalVector& x) const
      {
        size_t i = 0;
        for(const auto& ci : _ctcs)
        {
          IntervalVector xi = x.subvector(i,i+ci->size()-1);
          ci->contract(xi);
          x.put(i,xi);
          i += ci->size();
        }
        assert(i == x.size());
      }

    protected:

      Collection<Ctc_<IntervalVector>> _ctcs;
  };

  template<typename... C, typename = typename std::enable_if<(true && ... && (
      std::is_base_of_v<Ctc_<IntervalVector>,C>
    )), void>::type>
  inline CtcCartProd cart_prod(const C&... c)
  {
    return CtcCartProd(c...);
  }
}