/** 
 *  \file
 *  Collection class
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

namespace codac2
{
  template<typename T>
  class Collection
  {
    public:

      Collection()
      { }

      template<typename... T_,
        typename = typename std::enable_if<(true && ... && (
          std::is_base_of<T,T_>::value // T_ should be some T class
          && !std::is_same<Collection<T>,T_>::value
        )), void>::type>
      Collection(const T_&... x)
      {
        (add_shared_ptr(std::make_shared<T_>(x)), ...);
      }

      Collection(const Collection<T>& c)
      {
        for(const auto& ci : c._v)
          add_shared_ptr(std::dynamic_pointer_cast<T>(ci->copy()));
      }

      template<typename T_,
        typename = typename std::enable_if<
          std::is_base_of<T,T_>::value
        >::type>
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
        return *_v_raw.front();
      }

      const T& front() const
      {
        return *_v_raw.front();
      }

      T& back()
      {
        return *_v_raw.back();
      }

      const T& back() const
      {
        return *_v_raw.back();
      }

      iterator begin()
      {
        return _v_raw.begin();
      }

      iterator end()
      {
        return _v_raw.end();
      }

      const_iterator begin() const
      {
        return _v_raw.cbegin();
      }

      const_iterator end() const
      {
        return _v_raw.cend();
      }

    private:

      std::vector<std::shared_ptr<T>> _v;
      std::vector<T*> _v_raw;
  };
}