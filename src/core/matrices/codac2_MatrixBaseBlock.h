/** 
 *  \file codac2_MatrixBaseBlock.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <ostream>
#include "codac2_eigen.h"
#include "codac2_assert.h"
#include "codac2_MatrixBase.h"

namespace codac2
{
  template<typename S,typename T>
  class MatrixBase;
  
  template<typename Q,typename T>
  struct MatrixBaseBlock
  {
    Q _m;
    size_t _i,_j,_p,_q;

    MatrixBaseBlock(Q m, size_t i, size_t j, size_t p, size_t q)
      : _m(m), _i(i), _j(j), _p(p), _q(q)
    {
      assert(i >= 0 && p > 0 && i+p <= (size_t)m.rows());
      assert(j >= 0 && q > 0 && j+q <= (size_t)m.cols());
    }

    size_t nb_rows() const
    {
      return _p;
    }

    size_t nb_cols() const
    {
      return _q;
    }

    template<typename S_>
    void operator=(const MatrixBase<S_,T>& x)
    {
      assert_release(x.nb_rows() == _p && x.nb_cols() == _q);
      _m.block(_i,_j,_p,_q) = x._e;
    }

    template<typename OtherDerived>
    void operator=(const Eigen::MatrixBase<OtherDerived>& x)
    {
      assert_release(x.rows() == _p && x.cols() == _q);
      _m.block(_i,_j,_p,_q) = x;
    }

    // The following replaces operator=, that does not work in Python
    // (assignment on function output Matrix::block())
    template<typename S_>
    void init(const S_& x)
    {
      _m.block(_i,_j,_p,_q) = x;
    }

    auto eval() const
    {
      return _m.block(_i,_j,_p,_q).eval();
    }

    template<typename M>
    bool operator==(const M& x) const
    {
      return this->eval() == x;
    }
  };

  template<typename Q,typename T>
  std::ostream& operator<<(std::ostream& os, const MatrixBaseBlock<Q,T>& x)
  {
    os << x.eval();
    return os;
  }

  template<typename Q,typename T>
  auto eigen(const MatrixBaseBlock<Q,T>& x)
  {
    return x._m.block(x._i,x._j,x._p,x._q);
  }
}