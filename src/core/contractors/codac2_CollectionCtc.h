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

#ifndef __CODAC2_COLLECTIONCTC__
#define __CODAC2_COLLECTIONCTC__

#include <vector>
#include <memory>
#include "codac2_BoxCtc.h"

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
}

#endif