/** 
 *  \file
 *  CollectionSep class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <type_traits>
#include <memory>
#include "codac2_Sep.h"

namespace codac2
{
  class CollectionSep : public Sep
  {
    public:

      CollectionSep()
      { }

      template<typename... S, // S should be some Sep class
        typename = typename std::enable_if<
          (true && ... && std::is_base_of<Sep,S>::value), void
        >::type>
      CollectionSep(const S&... s)
      {
        (add_shared_ptr(std::make_shared<S>(s)), ...);
      }

      virtual std::shared_ptr<Sep> copy() const
      {
        auto cp = std::make_shared<CollectionSep>();

        for(const auto& si : _v_sep)
          cp->add_shared_ptr(si->copy());

        return cp;
      }

      virtual BoxPair separate(const IntervalVector& x) const
      {
        assert(false && "This virtual method should not be called");
        return { x, x};
      }

      template<typename T>
      void add_shared_ptr(std::shared_ptr<T> shrd_ptr)
      {
        _v_sep.push_back(shrd_ptr);
        _v_sep_ptrs.push_back(shrd_ptr.get());
      }

      void add_raw_ptr(Sep *s) // todo: keep this? (Python binding?)
      {
        _v_sep_ptrs.push_back(s);
      }

    //protected:

      std::vector<std::shared_ptr<Sep>> _v_sep;
      std::vector<Sep*> _v_sep_ptrs;
  };
}