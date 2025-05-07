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


namespace codac2
{
  class CtcAction;
  class SepBase;
  class SepAction;
  class SetExpr;

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

      int _sign(int a) const
      {
        return (a > 0) ? 1 : ((a < 0) ? -1 : 0);
      }

      template<typename T>
      Mat<T,-1,1> operator()(const Mat<T,-1,1>& x) const
      {
        assert_release(x.size() == (Index)size());
        Mat<T,-1,1> x_(size());
        for(size_t i = 0 ; i < size() ; i++)
          x_[i] = _sign((*this)[i])*x[std::abs((*this)[i])-1];
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