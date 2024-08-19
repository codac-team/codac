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

namespace codac2
{
  inline size_t size_of(int x)
  {
    return 1;
  }
  
  inline size_t size_of(double x)
  {
    return 1;
  }

  inline size_t size_of(const std::shared_ptr<Ctc_<IntervalVector>>& x)
  {
    return x->size();
  }

  inline size_t size_of(const std::shared_ptr<Sep>& x)
  {
    return x->size();
  }

  template<typename T, typename = typename std::enable_if<(
      (!std::is_same_v<std::shared_ptr<Ctc_<IntervalVector>>,T>
    && !std::is_same_v<std::shared_ptr<Sep>,T>
    && !std::is_same_v<int,T>
    && !std::is_same_v<double,T>)
    ), void>::type>
  inline size_t size_of(const T& x)
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
  size_t size_first_item(const T&... x)
  {
    return size_of(std::get<0>(std::make_tuple(x...)));
  }
}