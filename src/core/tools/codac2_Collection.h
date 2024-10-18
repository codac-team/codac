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
  class Collection
  {
    public:

      Collection()
      { }

      template<typename... T_>
        requires ((std::is_base_of_v<T,T_> // T_ should be some T class
          && !std::is_same_v<Collection<T>,T_>) && ...)
      Collection(const T_&... x)
      {
        (add_shared_ptr(std::make_shared<T_>(x)), ...);
      }

      template<typename... T_>
        requires (std::is_same_v<std::shared_ptr<T>,std::shared_ptr<T_>> && ...)
      Collection(const std::shared_ptr<T_>&... x)
      {
        (add_shared_ptr(x), ...);
      }

      Collection(const Collection<T>& c)
      {
        for(const auto& ci : c._v)
          add_shared_ptr(std::dynamic_pointer_cast<T>(ci->copy()));
      }

      template<typename T_>
        requires std::is_base_of_v<T,T_>
      void add(const T_& x)
      {
        add_shared_ptr(std::make_shared<T>(x));
      }

      void add_shared_ptr(std::shared_ptr<T> shrd_ptr)
      {
        assert(shrd_ptr);
        _v.push_back(shrd_ptr);
        _v_raw.push_back(shrd_ptr.get());
      }

      void add_raw_ptr(T *x)
      {
        assert(x);
        _v_raw.push_back(x);
      }

      using iterator = typename std::vector<T*>::iterator;
      using const_iterator = typename std::vector<T*>::const_iterator;

      T& front()
      {
        return const_cast<T&>(const_cast<const Collection<T>*>(this)->front());
      }

      const T& front() const
      {
        assert(!_v_raw.empty());
        return *_v_raw.front();
      }

      T& back()
      {
        return const_cast<T&>(const_cast<const Collection<T>*>(this)->back());
      }

      const T& back() const
      {
        assert(!_v_raw.empty());
        return *_v_raw.back();
      }

      iterator begin()
      {
        return const_cast<iterator>(const_cast<const Collection<T>*>(this)->begin());
      }

      const_iterator begin() const
      {
        return _v_raw.cbegin();
      }

      const_iterator end() const
      {
        return _v_raw.cend();
      }

      iterator end()
      {
        return const_cast<iterator>(const_cast<const Collection<T>*>(this)->end());
      }

    private:

      std::vector<std::shared_ptr<T>> _v;
      std::vector<T*> _v_raw;
  };
}