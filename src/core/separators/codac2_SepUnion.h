/** 
 *  \file
 *  SepUnion class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_SEPUNION_H__
#define __CODAC2_SEPUNION_H__

#include <memory>
#include "codac2_CtcFunction.h"

namespace codac2
{
  class CollectionSep
  {
    public:

      CollectionSep()
      { }

      template<typename... S>
      CollectionSep(const S&... s)
      {
        (add_shared_ptr(std::make_shared<S>(s)), ...);
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

    protected:

      std::vector<std::shared_ptr<Sep>> _v_sep;
      std::vector<Sep*> _v_sep_ptrs;
  };

  class SepUnion : public CollectionSep, public Sep
  {
    public:

      template<typename... S>
      SepUnion(const S&... s) : CollectionSep(s...)
      { }

      BoxPair separate(const IntervalVector& x) const
      {
        auto x_in = x;
        auto x_out = x;
        x_out.set_empty();

        for(auto& si : _v_sep_ptrs)
        {
          auto x_sep = si->separate(x);
          x_out |= x_sep.out;
          x_in &= x_sep.in;
        }

        return { x_in, x_out };
      }

      template<typename S>
      SepUnion& operator|=(const S& s)
      {
        add_shared_ptr(std::make_shared<S>(s));
        return *this;
      }
  };

  template<typename S2>
  inline SepUnion operator|(const SepUnion& s1, const S2& s2)
  {
    return SepUnion(s1,s2);
  }
}

#endif