/** 
 *  \file
 *  CtcUnion class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCUNION__
#define __CODAC2_CTCUNION__

#include "codac2_BoxCtc.h"
#include "codac2_CollectionCtc.h"

namespace codac2
{
  class CtcUnion : public CollectionCtc, public BoxCtc
  {
    public:

      template<typename... C>
      CtcUnion(const C&... c) : CollectionCtc(c...)
      { }

      void contract(IntervalVector& x) const
      {
        auto result = x;
        result.set_empty();

        for(auto& ci : _v_ctc_ptrs)
        {
          auto saved_x = x;
          ci->contract(saved_x);
          result |= saved_x;
        }

        x = result;
      }

      template<typename C>
      CtcUnion& operator|=(const C& c)
      {
        add_shared_ptr(std::make_shared<C>(c));
        return *this;
      }
  };
}

#endif