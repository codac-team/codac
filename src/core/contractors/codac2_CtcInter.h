/** 
 *  \file
 *  CtcInter class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <type_traits>
#include "codac2_CtcWrapper.h"
#include "codac2_CollectionCtc.h"

namespace codac2
{
  template<typename X>
  class CtcInter : public CollectionCtc<X>//, public Ctc_<X>
  {
    public:

      template<typename... C, // C should be some Ctc_<X> class
        typename = typename std::enable_if<
          (true && ... && std::is_base_of<Ctc_<X>,C>::value), void
        >::type>
      CtcInter(const C&... c)
        : CollectionCtc<X>(c...)
      { }

      //explicit CtcInter(const CtcInter<X>& c)
      //  : CollectionCtc<X>(c)
      //{ }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::dynamic_pointer_cast<CtcInter<X>>(this->CollectionCtc<X>::copy());
      }

      void contract(X& x) const
      {
        for(auto& ci : this->_v_ctc_ptrs)
          ci->contract(x);
      }

      template<typename C, // C should be some Ctc_<X> class
        typename = typename std::enable_if<std::is_base_of<Ctc_<X>,C>::value>::type>
      CtcInter<X>& operator&=(const C& c)
      {
        this->add_shared_ptr(std::make_shared<C>(c));
        return *this;
      }
  };

  template<typename X,
    typename C1, // C1 should be some Ctc class
    typename = typename std::enable_if<std::is_base_of<Ctc_<X>,C1>::value>::type,
    typename C2, // C2 should be some Ctc class
    typename = typename std::enable_if<std::is_base_of<Ctc_<X>,C2>::value>::type>
  inline CtcInter<X> operator&(const C1& c1, const C2& c2)
  {
    return CtcInter<X>(c1,c2);
  }
}