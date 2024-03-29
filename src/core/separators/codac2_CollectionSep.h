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

#ifndef __CODAC2_COLLECTIONSEP_H__
#define __CODAC2_COLLECTIONSEP_H__

#include <memory>
#include "codac2_Sep.h"

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
}

#endif