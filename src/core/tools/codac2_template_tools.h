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
#include "codac2_Sep.h"
#include "codac2_Ctc.h"
#include "codac2_Matrix.h"
#include "codac2_Vector.h"

namespace codac2
{
  template<class C,class X>
  concept IsCtcBaseOrPtr = (std::is_base_of_v<CtcBase<X>,C>
      || std::is_same_v<std::shared_ptr<CtcBase<X>>,C>);

  template<class C,class X>
  concept IsCtcBase = std::is_base_of_v<CtcBase<X>,C>;

  template<class S>
  concept IsSepBaseOrPtr = (std::is_base_of_v<SepBase,S>
    || std::is_base_of_v<S,std::shared_ptr<SepBase>>);

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

  inline Index size_of(const std::shared_ptr<CtcBase<IntervalVector>>& x)
  {
    return x->size();
  }

  inline Index size_of(const std::shared_ptr<SepBase>& x)
  {
    return x->size();
  }

  template<typename T>
    requires (!std::is_base_of_v<std::shared_ptr<CtcBase<IntervalVector>>,T>
      && !std::is_base_of_v<std::shared_ptr<SepBase>,T>
      && !std::is_same_v<int,T> && !std::is_same_v<double,T>)
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
}


