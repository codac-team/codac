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

#ifndef __CODAC2_CTCUNION_H__
#define __CODAC2_CTCUNION_H__

#include <memory>
//#include "ibex_CtcUnion.h"
#include "codac2_Ctc.h"

namespace codac2
{
  class CollectionCtc
  {
    public:

      CollectionCtc()
      { }

      template<typename... C>
      CollectionCtc(const C&... c)
      {
        (add_shared_ptr(std::make_shared<C>(c)), ...);
      }

      template<typename T>
      void add_shared_ptr(std::shared_ptr<T> shrd_ptr)
      {
        _v_ctc.push_back(shrd_ptr);
        _v_ctc_ptrs.push_back(shrd_ptr.get());
      }

      void add_raw_ptr(BoxCtc *c)
      {
        _v_ctc_ptrs.push_back(c);
      }

    protected:

      std::vector<std::shared_ptr<BoxCtc>> _v_ctc;
      std::vector<BoxCtc*> _v_ctc_ptrs;
  };

  class CtcUnion : public CollectionCtc, public BoxCtc
  {
    public:

      template<typename... C>
      CtcUnion(const C&... c) : CollectionCtc(c...)
      { }

      void contract(IntervalVector& x)
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