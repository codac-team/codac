/** 
 *  \file codac2_SampledTraj.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_SampledTraj.h"
#include "codac2_template_tools.h"

namespace codac2
{
  template<typename T,typename X,bool READONLY=false>
  class SampledTrajEval
  {
    public:

      using OUTPUT_HULL = typename Wrapper<T>::Domain;

      template<bool C=READONLY>
        requires (C == false && std::is_same_v<X,double>)
      // eval operation not yet supported for interval domains
      SampledTrajEval<T,X,READONLY>& operator=(const T& x)
      {
        assert_release(size_of(x) == _x->size());
        _x->set(_t, x);
        return *this;
      }
      
      inline operator T() const
      {
        static_assert(std::is_same_v<X,double>);
        return _const_x->eval(_t);
      }

      inline operator OUTPUT_HULL() const
      {
        return _const_x->eval(_t);
      }

    protected:

      X _t;
      SampledTraj<T> *_x = nullptr;
      const SampledTraj<T> *_const_x;

      friend class SampledTraj<T>;
  };
  
  template<typename T,typename X,bool READONLY>
  inline std::ostream& operator<<(std::ostream& os, const SampledTrajEval<T,X,READONLY>& x)
  {
    if constexpr(std::is_same_v<X,double>)
      os << (T)x << std::flush;
    else if constexpr(std::is_same_v<X,Interval>)
      os << (typename Wrapper<T>::Domain)x << std::flush;
    return os;
  }
}