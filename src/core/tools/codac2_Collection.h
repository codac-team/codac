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
#include <list>
#include <memory>

namespace codac2
{
  template<typename T>
  class Collection : public std::list<std::shared_ptr<T>>
  {
    public:

      Collection()
      { }

      template<typename... T_>
        requires ((!std::is_same_v<Collection<T>,T_>) && ...)
      Collection(const T_&... x)
      {
        (this->push_back(x), ...);
      }

      Collection(std::initializer_list<std::shared_ptr<T>> init)
        : std::list<std::shared_ptr<T>>(init)
      { }

      Collection(const Collection<T>& c)
        : std::list<std::shared_ptr<T>>()
      {
        for(const auto& ci : c)
        {
          assert(ci);
          this->push_back(std::dynamic_pointer_cast<T>(ci->copy()));
        }
      }

      template<typename T_>
        requires std::is_base_of_v<T,T_>
      void push_back(const T_& x)
      {
        this->push_back(std::make_shared<T_>(x));
      }

      template<typename T_>
        requires std::is_base_of_v<T,T_>
      void push_back(const std::shared_ptr<T_>& x)
      {
        this->std::list<std::shared_ptr<T>>::push_back(x);
      }
  };
}