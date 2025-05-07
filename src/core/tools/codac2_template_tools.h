/** 
 *  \file codac2_template_tools.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <memory>
#include "codac2_Matrix.h"
#include "codac2_Vector.h"

namespace codac2
{
  template<class T>
  concept IsRealType = (std::is_same_v<double,T>
    || std::is_same_v<Vector,T>
    || std::is_same_v<Matrix,T>);

  inline Index size_of([[maybe_unused]] int x)
  {
    return 1;
  }
  
  inline Index size_of([[maybe_unused]] double x)
  {
    return 1;
  }

  template<typename T>
  inline Index size_of(const T& x)
  {
    return x.size();
  }

  template<typename T1, typename T2>
  inline bool same_size(const T1& x1, const T2& x2)
  {
    return size_of(x1) == size_of(x2);
  }

  template<class... T>
  bool all_same_size(const T&... x)
  {
    return (... && (same_size(std::get<0>(std::make_tuple(x...)), x)));
  }

  template<class... T>
  Index size_first_item(const T&... x)
  {
    return size_of(std::get<0>(std::make_tuple(x...)));
  }

  // Removes duplicates when no comparison operator is available
  template<typename T>
  void remove_duplicates_from_list(std::list<T>& l)
  {
    typename std::list<T>::iterator it = l.begin();
    while(it != l.end())
    {
      if(std::count(l.begin(), l.end(), *it) > 1)
        l.erase(it++);
      else
        ++it;
    }
  }

  template<typename C>
    requires is_ctc_v<C>
  inline Index size_of(const std::shared_ptr<C>& x)
  {
    return x->size();
  }

  template<typename S>
    requires is_sep_v<S>
  inline Index size_of(const std::shared_ptr<S>& x)
  {
    return x->size();
  }

  template<int R,int C>
  inline auto vectorVector_to_vectorIntervalVector(const std::vector<Mat<double,R,C>>& x)
  {
    std::vector<Mat<Interval,R,C>> v(x.size());
    for(size_t i = 0 ; i < x.size() ; i++)
      v[i] = x[i].template cast<Interval>();
    return v;
  }
}