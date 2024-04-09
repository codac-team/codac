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
  
  class ArgBase
  {
    public:

      virtual int unique_id() const = 0;
      virtual std::shared_ptr<ArgBase> arg_copy() const = 0;
      virtual ~ArgBase() = default;
      virtual size_t size() const = 0;
  };

  template<typename T>
  class ArgExpr : public Expr<T>, public ArgBase
  {
    public:

      ArgExpr()
      { }

      virtual int unique_id() const
      {
        return Expr<T>::unique_id();
      }

      T fwd_eval(ValuesMap& v) const
      {
        return Expr<T>::value(v);
      }
      
      void bwd_eval(ValuesMap& v) const
      { }

      void replace_expr(int old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      { }
  };

  template<typename T>
  class Arg_ : public ArgExpr<T>
  {
    public:

      Arg_() : ArgExpr<T>()
      { }

      Arg_(const Arg_<T>& x)
        : ArgExpr<T>(x)
      { }

      std::shared_ptr<ArgBase> arg_copy() const
      {
        return std::make_shared<Arg_<T>>(*this);
      }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<Arg_<T>>(*this);
      }

      virtual size_t size() const
      {
        return 1;
      }

      operator std::shared_ptr<Expr<T>>()
      {
        return std::dynamic_pointer_cast<Expr<T>>(this->copy());
      }
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

      std::shared_ptr<ArgBase> arg_copy() const
      {
        return std::make_shared<ArgVector>(*this);
      }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<ArgVector>(*this);
      }

      virtual size_t size() const
      {
        return _n;
      }

      std::shared_ptr<Expr<Interval>> operator[](size_t i)
      {
        assert(i >= 0 && i < _n);
        return std::make_shared<OperationExpr<CtcComponent,Interval,IntervalVector>>(std::dynamic_pointer_cast<Expr<IntervalVector>>(this->copy()), i);
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

      std::shared_ptr<ArgBase> arg_copy() const
      {
        return std::make_shared<ArgMatrix>(*this);
      }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<ArgMatrix>(*this);
      }

      virtual size_t size() const
      {
        return _r * _c;
      }

    protected:

      size_t _r, _c;
  };
}