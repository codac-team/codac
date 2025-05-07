/** 
 *  \file codac2_Collection.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include <cassert>
#include <vector>
#include <memory>

namespace codac2
{
  template<typename T>
  class Collection : public std::vector<std::shared_ptr<T>>
  {
    public:

      Collection()
      { }

      template<typename... T_>
        requires ((std::is_base_of_v<T,T_> // T_ should be some T class
          && !std::is_same_v<Collection<T>,T_>) && ...)
      Collection(const T_&... x)
      {
        (push_object_back(x), ...);
      }

      template<typename... T_>
        requires (std::is_same_v<std::shared_ptr<T>,std::shared_ptr<T_>> && ...)
      Collection(const std::shared_ptr<T_>&... x)
        : std::vector<std::shared_ptr<T>>({x...})
      { }

      Collection(const Collection<T>& c)
        : std::vector<std::shared_ptr<T>>(c.size())
      {
        for(size_t i = 0 ; i < c.size() ; i++)
          (*this)[i] = std::dynamic_pointer_cast<T>(c[i]->copy());
      }

      template<typename T_>
        requires std::is_base_of_v<T,T_>
      void push_object_back(const T_& x)
      {
        this->push_back(std::make_shared<T_>(x));
      }
  };
}