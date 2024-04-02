/** 
 *  \file
 *  CollectionCtc class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <type_traits>
#include <cassert>
#include <vector>
#include <memory>
#include "codac2_CtcWrapper.h"

namespace codac2
{
  template<typename X>
  class CollectionCtc : public Ctc_<X>
  {
    public:

      CollectionCtc()
      { }

      template<typename... C, // C should be some Ctc_<X> class
        typename = typename std::enable_if<
          (true && ... && std::is_base_of<Ctc_<X>,C>::value), void
        >::type>
      CollectionCtc(const C&... c)
      {
        (add_shared_ptr(std::make_shared<C>(c)), ...);
      }

      virtual std::shared_ptr<Ctc> copy() const
      {
        auto cp = std::make_shared<CollectionCtc<X>>();

        for(const auto& ci : _v_ctc)
          cp->add_shared_ptr(std::dynamic_pointer_cast<Ctc_<X>>(ci->copy()));

        return cp;
      }

      void contract(X& x) const
      { 
        assert(false && "This virtual method should not be called");
      }

      void add_shared_ptr(std::shared_ptr<Ctc_<X>> shrd_ptr)
      {
        _v_ctc.push_back(shrd_ptr);
        _v_ctc_ptrs.push_back(shrd_ptr.get());
      }

      void add_raw_ptr(Ctc_<X> *c)
      {
        _v_ctc_ptrs.push_back(c);
      }

    protected:

      std::vector<std::shared_ptr<Ctc_<X>>> _v_ctc;
      std::vector<Ctc_<X>*> _v_ctc_ptrs;
  };
}