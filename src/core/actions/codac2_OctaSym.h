/** 
 *  \file codac2_OctaSym.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include "codac2_matrices.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_CtcWrapper.h"
#include "codac2_AnalyticFunction.h"
#include "codac2_SampledTraj.h"

namespace codac2
{
  class CtcAction;
  class SepBase;
  class SepAction;
  class SetExpr;
  class OctaSymOp;

  /**
   * \class Action
   */
  class Action
  { };

  /**
   * \class OctaSym
   */
  class OctaSym : public std::vector<int>, public Action
  {
    public:

      OctaSym(std::initializer_list<int> s);
      OctaSym(const std::vector<int>& s);

      OctaSym invert() const;

      OctaSym operator*(const OctaSym& s) const;

      Matrix permutation_matrix() const;

      template<typename Derived>
        requires (Derived::ColsAtCompileTime == 1)
      Mat<typename Derived::Scalar,-1,1> operator()(const Eigen::MatrixBase<Derived>& x) const
      {
        assert_release(x.size() == (Index)size());
        Mat<typename Derived::Scalar,-1,1> x_(x);
        for(size_t i = 0 ; i < size() ; i++)
          x_[i] = sign((*this)[i])*x[std::abs((*this)[i])-1];
        return x_;
      }

      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      CtcAction operator()(const C& c) const;
      // -> is defined in CtcAction class

      template<typename S>
        requires is_sep_v<S>
      SepAction operator()(const S& s) const;
      // -> is defined in SepAction class

      std::shared_ptr<SetExpr> operator()(const std::shared_ptr<SetExpr>& x1) const;
      // -> is defined in set operations file

      template<typename T>
      SampledTraj<T> operator()(const SampledTraj<T>& x) const
      {
        auto y = x;
        for(auto& [ti,yi] : y)
          yi = (*this)(yi);
        return y;
      }

      template<typename V>
        // To avoid ambiguity with operator()(const Eigen::MatrixBase<Derived>& x):
        requires (std::is_same_v<V,VectorExpr> || std::is_same_v<V,VectorVar>)
      inline VectorExpr operator()(const V& x1) const
      {
        if constexpr(std::is_same_v<V,VectorExpr>)
          assert_release((Index)this->size() == x1->output_shape().first);
        else
          assert_release((Index)this->size() == x1.output_shape().first);
        return { std::make_shared<AnalyticOperationExpr<OctaSymOp,VectorType,VectorType>>(*this, x1) };
      }

      friend std::ostream& operator<<(std::ostream& str, const OctaSym& s)
      {
        str << "(";
        for(size_t i = 0 ; i < s.size() ; i++)
          str << (i != 0 ? " " : "") << s[i];
        str << ")" << std::flush;
        return str;
      }
  };
}