/** 
 *  \file codac2_matrix.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>

#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"

namespace codac2
{
  struct MatrixOp
  {
    template<typename... X>
    static std::string str(const X&... x)
    {
      std::string s = (("\t" + x->str() + ",\n") + ...);
      s.pop_back(); s.pop_back(); // removes last separation
      return "[\n" + s + "\n]";
    }

    template<typename X1, typename... X>
    static std::pair<Index,Index> output_shape(const X1& s1, [[maybe_unused]] const X&... s)
    {
      auto shape1 = s1->output_shape();
      assert(shape1.second == 1);
      return { shape1.first, 1+sizeof...(X) };
    }

    static inline void set_col_i(IntervalMatrix& m, const IntervalVector& x, Index i)
    {
      assert(i >= 0 && i < m.cols());
      assert_release(x.size() == m.rows());
      m.col(i) = x;
    }

    template<typename X1,typename... X>
      requires ((X1::ColsAtCompileTime == 1) && ((X::ColsAtCompileTime == 1) && ...))
    static inline IntervalMatrix fwd(const X1& x1, const X&... x)
    {
      IntervalMatrix m(x1.size(), 1+sizeof...(X));
      Index i = 0;
      MatrixOp::set_col_i(m, x1, i++); // first column x1
      (MatrixOp::set_col_i(m, x, i++), ...); // other columns
      return m;
    }

    template<typename... X>
      requires (std::is_base_of_v<VectorType,X> && ...)
    static inline MatrixType fwd_natural(const X&... x)
    {
      return {
        MatrixOp::fwd(x.a...),
        (x.def_domain && ...)
      };
    }

    static inline void fill_diff_matrix(IntervalMatrix &d, 
  const IntervalMatrix &dax, Index &l) {
       d.middleRows(l,dax.rows())=dax;
       l += dax.rows();
    }


    template<typename X1, typename... X>
      requires (std::is_base_of_v<VectorType,X1> 
    && (std::is_base_of_v<VectorType,X> && ...))
    static inline MatrixType fwd_centered(const X1& x1, const X&... x)
    {
      if (centered_form_not_available_for_args(x1,x...))
        return fwd_natural(x1,x...);

      IntervalMatrix d(x1.a.size()*(1+sizeof...(X)),x1.da.cols());
      Index l=0;
      d.topRows(x1.da.rows()) = x1.da;
      l += x1.da.rows();
      ( MatrixOp::fill_diff_matrix(d,x.da,l) , ...);
      assert (l==d.rows());

      return {
        MatrixOp::fwd(x1.m,x.m...),
        MatrixOp::fwd(x1.a,x.a...),
        d,
        (x1.def_domain && (x.def_domain && ...))
      };
    }

    template<typename... X>
      requires (std::is_base_of_v<IntervalVector,X> && ...)
    static inline void bwd(const IntervalMatrix& y, X&... x)
    {
      Index i = 0;
      ((x &= y.col(i++)), ...);
    }
  };

  namespace detail
  {
    inline void replace_vector_child(std::shared_ptr<AnalyticExpr<VectorType>>& x,
      const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
    {
      if(x->unique_id() == old_arg_id)
      {
        auto new_x = std::dynamic_pointer_cast<AnalyticExpr<VectorType>>(new_expr);
        assert_release(new_x);
        x = new_x;
      }

      else
        x->replace_arg(old_arg_id, new_expr);
    }

    class DynamicMatrixExpr final : public AnalyticExpr<MatrixType>
    {
      public:
        
        explicit DynamicMatrixExpr(const std::vector<VectorExpr>& xs)
        {
          _xs.reserve(xs.size());
          for(const auto& x : xs)
          {
            auto vx = std::dynamic_pointer_cast<AnalyticExpr<VectorType>>(x);
            assert_release(vx);
            _xs.push_back(vx);
          }
        }

        DynamicMatrixExpr(const DynamicMatrixExpr& e)
        {
          _xs.reserve(e._xs.size());
          for(const auto& x : e._xs)
          {
            auto vx = std::dynamic_pointer_cast<AnalyticExpr<VectorType>>(x->copy());
            assert_release(vx);
            _xs.push_back(vx);
          }
        }

        std::shared_ptr<ExprBase> copy() const override
        {
          return std::make_shared<DynamicMatrixExpr>(*this);
        }

        void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr) override
        {
          for(auto& x : _xs)
            replace_vector_child(x, old_arg_id, new_expr);
        }

        MatrixType fwd_eval(ValuesMap& v, Index total_input_size, bool natural_eval) const override
        {
          if(natural_eval)
            return this->init_value(v, natural_fwd(v, total_input_size));

          std::vector<VectorType> vals;
          vals.reserve(_xs.size());
          bool centered_available = true;
          for(const auto& x : _xs)
          {
            vals.push_back(x->fwd_eval(v, total_input_size, false));
            centered_available &= (vals.back().da.size() != 0);
          }

          if(!centered_available)
            return this->init_value(v, natural_fwd(v, total_input_size));

          const Index cols = static_cast<Index>(_xs.size());
          const Index rows = vals.empty() ? 0 : vals.front().a.size();
          const Index input_cols = vals.empty() ? total_input_size : vals.front().da.cols();
          IntervalMatrix m(rows, cols), a(rows, cols), da(rows*cols, input_cols);
          bool def_domain = true;
          Index l = 0;

          for(Index j = 0 ; j < cols ; ++j)
          {
            const auto& xj = vals[static_cast<std::size_t>(j)];
            m.col(j) = xj.m;
            a.col(j) = xj.a;
            da.middleRows(l, xj.da.rows()) = xj.da;
            l += xj.da.rows();
            def_domain &= xj.def_domain;
          }

          return this->init_value(v, MatrixType(m, a, da, def_domain));
        }

        void bwd_eval(ValuesMap& v) const override
        {
          for(const auto& x : _xs)
            x->bwd_eval(v);
        }

        std::pair<Index,Index> output_shape() const override
        {
          if(_xs.empty())
            return { 0, 0 };

          const auto shape = _xs.front()->output_shape();
          assert(shape.second == 1);
          return { shape.first, static_cast<Index>(_xs.size()) };
        }

        bool belongs_to_args_list(const FunctionArgsList& args) const override
        {
          bool ok = true;
          for(const auto& x : _xs)
            ok &= x->belongs_to_args_list(args);
          return ok;
        }

        std::string str(bool in_parentheses = false) const override
        {
          if(_xs.empty())
            return in_parentheses ? "([])" : "[]";

          std::string s;
          for(const auto& x : _xs)
            s += "\t" + x->str() + ",\n";

          s.pop_back();
          s.pop_back();

          s = "[\n" + s + "\n]";
          return in_parentheses ? "(" + s + ")" : s;
        }

        bool is_str_leaf() const override
        {
          return false;
        }

        std::vector<std::shared_ptr<ExprBase>> children_expr_base() const override
        {
          std::vector<std::shared_ptr<ExprBase>> children;
          children.reserve(_xs.size());
          for(const auto& x : _xs)
            children.push_back(std::dynamic_pointer_cast<ExprBase>(x));
          return children;
        }

      private:

        MatrixType natural_fwd(ValuesMap& v, Index total_input_size) const
        {
          const Index cols = static_cast<Index>(_xs.size());
          const Index rows = _xs.empty() ? 0 : _xs.front()->output_shape().first;
          IntervalMatrix a(rows, cols);
          bool def_domain = true;

          for(Index j = 0 ; j < cols ; ++j)
          {
            auto xj = _xs[static_cast<std::size_t>(j)]->fwd_eval(v, total_input_size, true);
            a.col(j) = xj.a;
            def_domain &= xj.def_domain;
          }

          return { a, def_domain };
        }

        std::vector<std::shared_ptr<AnalyticExpr<VectorType>>> _xs;
    };
  }

  // Analytic operator
  // The following function can be used to build analytic expressions.

  // Variadic (cannot handle const values (int, double) for now)

  template<typename... X>
  inline MatrixExpr
  mat(const std::shared_ptr<AnalyticExpr<X>>&... x)
  {
    return { std::make_shared<AnalyticOperationExpr<MatrixOp,MatrixType,X...>>(
      AnalyticOperationExpr<MatrixOp,MatrixType,X...>(x...)) };
  }

  // Dynamic:

  inline MatrixExpr
  mat(const std::vector<VectorExpr>& x)
  {
    return { std::make_shared<detail::DynamicMatrixExpr>(x) };
  }
}
