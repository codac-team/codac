/** 
 *  \file codac2_vector.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"

namespace codac2
{
  struct VectorOp
  {
    template<typename... X>
    static inline std::string str(const X&... x)
    {
      std::string s = (("\t" + x->str() + ",\n") + ...);
      s.pop_back(); s.pop_back(); // removes last separation
      return "[\n" + s + "\n]";
    }

    template<typename... X>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X&... x)
    {
      return { sizeof...(X), 1 };
    }

    template<typename... X>
      requires (std::is_base_of_v<Interval,X> && ...)
    static inline IntervalVector fwd(const X&... x)
    {
      return IntervalVector({x...});
    }

    template<typename... X>
      requires (std::is_base_of_v<ScalarType,X> && ...)
    static inline VectorType fwd_natural(const X&... x)
    {
      bool def_domain = true;
      ((def_domain &= x.def_domain), ...);
      
      return {
        fwd(x.a...),
        def_domain
      };
    }

    template<typename... X>
      requires (std::is_base_of_v<ScalarType,X> && ...)
    static inline VectorType fwd_centered(const X&... x)
    {
      if(centered_form_not_available_for_args(x...))
        return fwd_natural(x...);

      IntervalMatrix d(sizeof...(X),std::get<0>(std::tie(x...)).da.cols());
      Index i = 0;
      ((d.row(i++) = x.da), ...);

      bool def_domain = true;
      ((def_domain &= x.def_domain), ...);
      
      return {
        fwd(x.m...),
        fwd(x.a...),
        d,
        def_domain
      };
    }

    template<typename... X>
      requires (std::is_base_of_v<Interval,X> && ...)
    static inline void bwd(const IntervalVector& y, X&... x)
    {
      Index i = 0;
      ((x &= y[i++]), ...);
    }
  };

  // Analytic operator
  // The following functions can be used to build analytic expressions.

  inline const ScalarExpr& _add_to_vec(const ScalarExpr& x)
  {
    return x;
  }

  inline ScalarExpr _add_to_vec(double x)
  {
    return const_value(x);
  }

  template<typename... X>
    requires ((std::is_same_v<typename ValueType<X>::Type,ScalarType>) && ...)
  inline VectorExpr
  vec(const X&... x)
  {
    return { std::make_shared<AnalyticOperationExpr<VectorOp,VectorType,typename ValueType<X>::Type...>>(_add_to_vec(x)...) };
  }
}
