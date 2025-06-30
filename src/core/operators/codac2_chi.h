/** 
 *  \file codac2_chi.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"
#include "codac2_analytic_variables.h"

namespace codac2
{
  // chi([x1],[x2],[x3]) = [x2] if (x1^+)<=0, [x3] if (x1^-)>0, hull([x2],[x3]) else

  struct ChiOp
  {
    template<typename X1,typename X2,typename X3>
    static std::string str(const X1& x1, const X2& x2, const X3& x3)
    {
      return "χ(" + x1->str() + "," + x2->str() + "," + x3->str() + ")";
    }

    template<typename X1, typename X2, typename X3>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1, const X2& s2, [[maybe_unused]] const X3& s3)
    {
      auto shape2 = s2->output_shape();
      assert(shape2 == s3->output_shape());
      return shape2;
    }

    template<typename T>
    static inline T fwd(const Interval& x1, const T& x2, const T& x3)
    {
      // Copy of chi from codac2_Interval_operations_impl.h
      if(x1.ub() <= 0)
        return x2;
      else if(x1.lb() > 0)
        return x3;
      else
        return x2 | x3; 
    }

    template<typename T>
    static inline T fwd_natural(const ScalarType& x1, const T& x2, const T& x3)
    {
      return {
        fwd(x1.a,x2.a,x3.a),
        x1.def_domain
          && ((x1.a.lb()>0.0)  ? x2.def_domain : true)
          && ((x1.a.ub()<=0.0) ? x3.def_domain : true)
      };
    }

    template<typename T>
    static inline T fwd_centered(const ScalarType& x1, const T& x2, const T& x3)
    {
      if(centered_form_not_available_for_args(x1,x2,x3))
        return fwd_natural(x1,x2,x3);
      
      assert(x2.da.rows() == x3.da.rows());
 
      return {
        fwd(x1.m,x2.m,x3.m),
        fwd(x1.a,x2.a,x3.a),
        (x1.a.lb()>0 ? x3.da : ((x1.a.ub()<=0) ? x2.da : (x2.da | x3.da))),
        x1.def_domain
          && ((x1.a.lb()>0.0)  ? x2.def_domain : true)
          && ((x1.a.ub()<=0.0) ? x3.def_domain : true)
      };
    }

    template<typename T>
    static inline void bwd(const T& y, Interval& x1, T& x2, T& x3)
    {
      // The content of this function comes from the IBEX library.
      // See ibex::Interval (IBEX lib, main author: Gilles Chabert)
      //   https://ibex-lib.readthedocs.io
      // Some updates have been proposed by Damien Massé.

      if(x1.ub() <= 0)
      {
        if((x2 &= y).is_empty())
        {
          x1.set_empty();
          x3.set_empty();
        }
      }

      else if(x1.lb() > 0)
      {
        if((x3 &= y).is_empty())
        {
          x1.set_empty();
          x2.set_empty();
        }
      }

      if(y.is_disjoint(x2))
      {
        if((x1 &= Interval(0,oo)).is_empty())
        {
          x2.set_empty();
          x3.set_empty();
        }
  
        if((x3 &= y).is_empty())
        {
          x1.set_empty();
          x2.set_empty();
        }
      }

      if(y.is_disjoint(x3))
      {
        if((x1 &= Interval(-oo,0)).is_empty())
        {
          x2.set_empty();
          x3.set_empty();
        }

        if((x2 &= y).is_empty())
        {
          x1.set_empty();
          x3.set_empty();
        }
      }
    }
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  template<typename T2,typename T3,typename T=ExprType<T2>::Type>
  inline AnalyticExprWrapper<T>
  chi(const ScalarExpr& x1, const T2& x2, const T3& x3)
  {
    return { std::make_shared<AnalyticOperationExpr<ChiOp,T,ScalarType,T,T>>(
      x1,(AnalyticExprWrapper<T>)x2,(AnalyticExprWrapper<T>)x3) };
  }
  
}
