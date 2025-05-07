/** 
 *  codac2_ExprBase.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_analytic_variables.h"
#include "codac2_AnalyticExprWrapper.h"
#include "codac2_subvector.h"
#include "codac2_arith_sub.h"

using namespace std;
using namespace codac2;


// ScalarVar class

  ScalarVar::ScalarVar(const std::string& name)
     :AnalyticVarExpr<ScalarType>(name)
  { }

  std::shared_ptr<VarBase> ScalarVar::arg_copy() const
  {
    return std::make_shared<ScalarVar>(*this);
  }

  std::shared_ptr<ExprBase> ScalarVar::copy() const
  {
    return std::make_shared<ScalarVar>(*this);
  }

  Index ScalarVar::size() const
  {
    return 1;
  }

  std::pair<Index,Index> ScalarVar::output_shape() const
  {
    return {1,1};
  }


  AnalyticExprWrapper<ScalarType> ScalarVar::operator-() const
  {
    return { std::make_shared<AnalyticOperationExpr<SubOp,ScalarType,ScalarType>>(
      std::dynamic_pointer_cast<AnalyticExpr<ScalarType>>(this->copy())) };
  }


// VectorVar class

  VectorVar::VectorVar(Index n, const std::string& name)
    : AnalyticVarExpr<VectorType>(name), _n(n)
  {
    assert_release(n > 0);
  }

  std::shared_ptr<VarBase> VectorVar::arg_copy() const
  {
    return std::make_shared<VectorVar>(*this);
  }

  std::shared_ptr<ExprBase> VectorVar::copy() const
  {
    return std::make_shared<VectorVar>(*this);
  }

  Index VectorVar::size() const
  {
    return _n;
  }

  std::pair<Index,Index> VectorVar::output_shape() const
  {
    return {_n,1};
  }

  AnalyticExprWrapper<ScalarType> VectorVar::operator[](Index i) const
  {
    assert_release(i >= 0 && i < _n);
    return { std::make_shared<AnalyticOperationExpr<ComponentOp,ScalarType,VectorType>>(
      std::dynamic_pointer_cast<AnalyticExpr<VectorType>>(this->copy()), i) };
  }

  AnalyticExprWrapper<VectorType> VectorVar::subvector(Index i, Index j) const
  {
    assert_release(i >= 0 && i < _n && j >= i && j < _n);
    return { std::make_shared<AnalyticOperationExpr<SubvectorOp,VectorType,VectorType>>(
      std::dynamic_pointer_cast<AnalyticExpr<VectorType>>(this->copy()), i, j) };
  }


// MatrixVar class


  MatrixVar::MatrixVar(Index r, Index c, const std::string& name)
    : AnalyticVarExpr<MatrixType>(name), _r(r), _c(c)
  { }

  std::shared_ptr<VarBase> MatrixVar::arg_copy() const
  {
    return std::make_shared<MatrixVar>(*this);
  }

  std::shared_ptr<ExprBase> MatrixVar::copy() const
  {
    return std::make_shared<MatrixVar>(*this);
  }

  Index MatrixVar::size() const
  {
    return _r*_c;
  }

  Index MatrixVar::rows() const
  {
    return _r;
  }

  Index MatrixVar::cols() const
  {
    return _c;
  }

  std::pair<Index,Index> MatrixVar::output_shape() const
  {
    return {_r,_c};
  }

  AnalyticExprWrapper<ScalarType> MatrixVar::operator()(Index i, Index j) const
  {
    assert_release(i >= 0 && i < _r && j >= 0 && j < _c);
    return { std::make_shared<AnalyticOperationExpr<ComponentOp,ScalarType,MatrixType>>(
      std::dynamic_pointer_cast<AnalyticExpr<MatrixType>>(this->copy()), i, j) };
  }
