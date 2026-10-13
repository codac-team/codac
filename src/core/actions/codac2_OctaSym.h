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
  struct OctaSymOp;

  /**
   * \class Action
   */
  class Action
  { };

  /**
   * \class OctaSym
   * \brief Represents an hyperoctahedral symmetry.
   */
  class OctaSym : public std::vector<int>, public Action
  {
    public:

      /**
       * \brief Constructs an hyperoctahedral symmetry from a list of integers. The list represents the second line in the Cauchy' representation.
       * It is supposed that the first line of the representation is (1 2 3 ... n).
       * 
       * \param s The list of integers representing the hyperoctahedral symmetry.
       */
      OctaSym(std::initializer_list<int> s);

      /**
       * \brief Constructs an hyperoctahedral symmetry from a vector of integers. The vector represents the second line in the Cauchy' representation.
       * It is supposed that the first line of the representation is (1 2 3 ... n).
       * 
       * \param s The vector of integers representing the hyperoctahedral symmetry.
       */
      OctaSym(const std::vector<int>& s);

      /**
       * \brief Inverts of the hyperoctahedral symmetry.
       * 
       * \return The inverse of the hyperoctahedral symmetry.
       */
      OctaSym invert() const;

      /**
       * \brief Composes the hyperoctahedral symmetry with another one.
       * 
       * \param s The hyperoctahedral symmetry to compose with.
       * \return The composition of the two hyperoctahedral symmetries.
       */
      OctaSym operator*(const OctaSym& s) const;

      /**
       * \brief Computes the permutation matrix associated to the hyperoctahedral symmetry.
       * 
       * \return The permutation matrix associated to the hyperoctahedral symmetry.
       */
      Matrix permutation_matrix() const;

      /**
       * \brief Applies the hyperoctahedral symmetry to a vector.
       * 
       * \param x The Vector or IntervalVector to which the hyperoctahedral symmetry is applied.
       * \return The result of the application of the hyperoctahedral symmetry to the vector.
       */
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

      /**
       * \brief Applies the hyperoctahedral symmetry to a Contractor.
       * 
       * \param c The Contractor to which the hyperoctahedral symmetry is applied.
       * \return The result of the application of the hyperoctahedral symmetry to the Contractor.
       */
      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      CtcAction operator()(const C& c) const;
      // -> is defined in CtcAction class

      /**
       * \brief Applies the hyperoctahedral symmetry to a Separator.
       * 
       * \param s The Separator to which the hyperoctahedral symmetry is applied.
       * \return The result of the application of the hyperoctahedral symmetry to the Separator.
       */
      template<typename S>
        requires is_sep_v<S>
      SepAction operator()(const S& s) const;
      // -> is defined in SepAction class

      /**
       * \brief Applies the hyperoctahedral symmetry to a SetExpr.
       * 
       * \param x1 The SetExpr to which the hyperoctahedral symmetry is applied.
       * \return The result of the application of the hyperoctahedral symmetry to the SetExpr.
       */
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

      /**
       * \brief Applies the hyperoctahedral symmetry to a VectorExpr or a VectorVar.
       * 
       * \param x1 The VectorExpr or VectorVar to which the hyperoctahedral symmetry is applied.
       * \return The result of the application of the hyperoctahedral symmetry to the VectorExpr or VectorVar.
       */
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

      /**
       * \brief Overloads the stream insertion operator to print the hyperoctahedral symmetry in a human-readable format.
       * 
       * \param str The output stream to which the hyperoctahedral symmetry is printed.
       * \param s The hyperoctahedral symmetry to print.
       * \return The output stream to which the hyperoctahedral symmetry is printed.
       */
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
