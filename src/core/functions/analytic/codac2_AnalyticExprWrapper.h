/** 
 *  \file codac2_AnalyticExprWrapper.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_AnalyticExpr.h"
#include "codac2_ValueType.h"
#include "codac2_analytic_variables.h"
#include "codac2_component.h"
#include "codac2_subvector.h"
#include "codac2_analytic_constants.h"

namespace codac2
{
  class ScalarVar;
  class VectorVar;

  template<typename T>
  struct AnalyticExprWrapper : public std::shared_ptr<AnalyticExpr<T>>
  {    
    AnalyticExprWrapper(const AnalyticExprWrapper<T>& e)
      : std::shared_ptr<AnalyticExpr<T>>(e)
    { }
    
    AnalyticExprWrapper(const std::shared_ptr<AnalyticExpr<T>>& e)
      : std::shared_ptr<AnalyticExpr<T>>(e)
    { }
    
    template<typename V>
      requires std::is_base_of_v<AnalyticVarExpr<T>,V>
    AnalyticExprWrapper(const V& e)
      : std::shared_ptr<AnalyticExpr<T>>({ std::dynamic_pointer_cast<AnalyticExpr<T>>(e.copy()) })
    { }
    
    template<typename C>
      requires (!std::is_base_of_v<AnalyticVarExpr<T>,C>)
    AnalyticExprWrapper(const C& e)
      requires std::is_same_v<typename ValueType<C>::Type,T>
      : std::shared_ptr<AnalyticExpr<T>>(const_value(e))
    { }
    
    inline AnalyticExprWrapper<ScalarType> operator[](Index i) const
      requires std::is_same_v<T,VectorType>
    {
      return { std::make_shared<AnalyticOperationExpr<ComponentOp,ScalarType,VectorType>>(*this,i) };
    }

    inline AnalyticExprWrapper<VectorType> subvector(Index i, Index j) const
      requires std::is_same_v<T,VectorType>
    {
      return { std::make_shared<AnalyticOperationExpr<SubvectorOp,VectorType,VectorType>>(*this,i,j) };
    }
    
    inline AnalyticExprWrapper<ScalarType> operator()(Index i, Index j) const
      requires std::is_same_v<T,MatrixType>
    {
      return { std::make_shared<AnalyticOperationExpr<ComponentOp,ScalarType,MatrixType>>(*this,i,j) };
    }
  };

  using ScalarExpr = AnalyticExprWrapper<ScalarType>;
  using VectorExpr = AnalyticExprWrapper<VectorType>;
  using MatrixExpr = AnalyticExprWrapper<MatrixType>;

  template<class X>
  concept IsScalarExprOrVar = (std::is_base_of_v<VarBase,X> || std::is_base_of_v<ScalarExpr,X>);

  template<class X>
  concept IsVectorExprOrVar = (std::is_base_of_v<VarBase,X> || std::is_base_of_v<VectorExpr,X>);

  template<class X>
  concept IsMatrixExprOrVar = (std::is_base_of_v<VarBase,X> || std::is_base_of_v<MatrixExpr,X>);
}