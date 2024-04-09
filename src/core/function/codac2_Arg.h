/** 
 *  \file
 *  Arg class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include "codac2_Expr.h"

namespace codac2
{
  template<typename T>
  struct Wrapper
  { };

  template<>
  struct Wrapper<int> {
    using Domain = Interval;
  };

  template<>
  struct Wrapper<size_t> {
    using Domain = Interval;
  };

  template<>
  struct Wrapper<double> {
    using Domain = Interval;
  };

  template<>
  struct Wrapper<Interval> {
    using Domain = Interval;
  };

  template<>
  struct Wrapper<Vector> {
    using Domain = IntervalVector;
  };

  template<>
  struct Wrapper<IntervalVector> {
    using Domain = IntervalVector;
  };
  
  template<typename T>
  class ArgExpr : public Expr<T>
  {
    public:

      ArgExpr()
      { }

      std::shared_ptr<Expr<T>> copy()
      {
        return Expr<T>::shared_from_this();
      }

      T fwd_eval(ValuesMap& v) const
      {
        return Expr<T>::value(v);
      }
      
      void bwd_eval(ValuesMap& v) const
      { }

      void replace_expr(const std::shared_ptr<const ExprBase>& old_expr, const std::shared_ptr<ExprBase>& new_expr)
      { }
  };

  class ArgBase
  {
    public:

      ArgBase()
        : _unique_id(_unique_id_counter++)
      { }

      virtual size_t size() const = 0;
      virtual std::shared_ptr<ExprBase> exprbase_ptr() const = 0;

      bool operator==(const ArgBase& x) const
      {
        return _unique_id == x._unique_id;
      }

      int unique_id() const
      {
        return _unique_id;
      }

    protected:

      const int _unique_id;
      static int _unique_id_counter;
  };

  int ArgBase::_unique_id_counter = 0;

  template<typename T>
  class Arg_ : public ArgBase
  {
    public:

      Arg_() : ArgBase(), _arg_expr(std::make_shared<ArgExpr<T>>())
      { }

      Arg_(const Arg_<T>& x)
        : ArgBase(x)
      { }

      virtual size_t size() const
      {
        return 1;
      }

      std::shared_ptr<ExprBase> exprbase_ptr() const
      {
        return _arg_expr;
      }

      operator std::shared_ptr<Expr<T>>()
      {
        return _arg_expr;
      }

    protected:

      std::shared_ptr<ArgExpr<T>> _arg_expr;
  };

  using Arg = Arg_<Interval>;

  class ArgVector : public Arg_<IntervalVector>
  {
    public:

      ArgVector(size_t n)
        : _n(n)
      { }

      ArgVector(const ArgVector& x)
        : Arg_<IntervalVector>(x), _n(x._n)
      { }

      virtual size_t size() const
      {
        return _n;
      }

      std::shared_ptr<Expr<Interval>> operator[](size_t i)
      {
        assert(i >= 0 && i < _n);
        return std::make_shared<OperationExpr<CtcComponent,Interval,IntervalVector>>(_arg_expr, i);
      }

    protected:

      size_t _n;
  };

  class ArgMatrix : public Arg_<IntervalMatrix>
  {
    public:

      ArgMatrix(size_t r, size_t c)
        : _r(r), _c(c)
      { }

      ArgMatrix(const ArgMatrix& x)
        : Arg_<IntervalMatrix>(x), _r(x._r), _c(x._c)
      { }

      virtual size_t size() const
      {
        return _r * _c;
      }

    protected:

      size_t _r, _c;
  };
}