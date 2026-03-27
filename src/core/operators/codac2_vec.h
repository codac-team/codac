/** 
 *  \file codac2_vector.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>

#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"

namespace codac2
{
  struct VectorOp
  {
    template<typename... X>
    static inline std::string str(const X&... x)
    {
      std::string s = (("\t" + x->str() + ",\n") + ...);
      s.pop_back(); s.pop_back(); // removes last separation
      return "[\n" + s + "\n]";
    }

    template<typename... X>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X&... x)
    {
      return { sizeof...(X), 1 };
    }

    template<typename... X>
      requires (std::is_base_of_v<Interval,X> && ...)
    static inline IntervalVector fwd(const X&... x)
    {
      return IntervalVector({x...});
    }

    template<typename... X>
      requires (std::is_base_of_v<ScalarType,X> && ...)
    static inline VectorType fwd_natural(const X&... x)
    {
      bool def_domain = true;
      ((def_domain &= x.def_domain), ...);

      return {
        fwd(x.a...),
        def_domain
      };
    }

    template<typename... X>
      requires (std::is_base_of_v<ScalarType,X> && ...)
    static inline VectorType fwd_centered(const X&... x)
    {
      if(centered_form_not_available_for_args(x...))
        return fwd_natural(x...);

      IntervalMatrix d(sizeof...(X),std::get<0>(std::tie(x...)).da.cols());
      Index i = 0;
      ((d.row(i++) = x.da), ...);

      bool def_domain = true;
      ((def_domain &= x.def_domain), ...);

      return {
        fwd(x.m...),
        fwd(x.a...),
        d,
        def_domain
      };
    }

    template<typename... X>
      requires (std::is_base_of_v<Interval,X> && ...)
    static inline void bwd(const IntervalVector& y, X&... x)
    {
      Index i = 0;
      ((x &= y[i++]), ...);
    }
  };

  namespace detail
  {
    inline void replace_scalar_child(std::shared_ptr<AnalyticExpr<ScalarType>>& x,
      const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
    {
      if(x->unique_id() == old_arg_id)
      {
        auto new_x = std::dynamic_pointer_cast<AnalyticExpr<ScalarType>>(new_expr);
        assert_release(new_x);
        x = new_x;
      }

      else
        x->replace_arg(old_arg_id, new_expr);
    }

    class DynamicVectorExpr final : public AnalyticExpr<VectorType>
    {
      public:

        explicit DynamicVectorExpr(const std::vector<ScalarExpr>& xs)
        {
          _xs.reserve(xs.size());
          for(const auto& x : xs)
          {
            auto sx = std::dynamic_pointer_cast<AnalyticExpr<ScalarType>>(x);
            assert_release(sx);
            _xs.push_back(sx);
          }
        }

        DynamicVectorExpr(const DynamicVectorExpr& e)
        {
          _xs.reserve(e._xs.size());
          for(const auto& x : e._xs)
          {
            auto sx = std::dynamic_pointer_cast<AnalyticExpr<ScalarType>>(x->copy());
            assert_release(sx);
            _xs.push_back(sx);
          }
        }

        std::shared_ptr<ExprBase> copy() const override
        {
          return std::make_shared<DynamicVectorExpr>(*this);
        }

        void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr) override
        {
          for(auto& x : _xs)
            replace_scalar_child(x, old_arg_id, new_expr);
        }

        VectorType fwd_eval(ValuesMap& v, Index total_input_size, bool natural_eval) const override
        {
          if(natural_eval)
            return this->init_value(v, natural_fwd(v, total_input_size));

          std::vector<ScalarType> vals;
          vals.reserve(_xs.size());
          bool centered_available = true;
          for(const auto& x : _xs)
          {
            vals.push_back(x->fwd_eval(v, total_input_size, false));
            centered_available &= (vals.back().da.size() != 0);
          }

          if(!centered_available)
            return this->init_value(v, natural_fwd(v, total_input_size));

          const Index n = static_cast<Index>(_xs.size());
          const Index input_cols = vals.empty() ? total_input_size : vals.front().da.cols();
          IntervalVector m(n), a(n);
          IntervalMatrix da(n, input_cols);
          bool def_domain = true;

          for(Index i = 0 ; i < n ; ++i)
          {
            const auto& xi = vals[static_cast<std::size_t>(i)];
            m[i] = xi.m;
            a[i] = xi.a;
            da.row(i) = xi.da;
            def_domain &= xi.def_domain;
          }

          return this->init_value(v, VectorType(m, a, da, def_domain));
        }

        void bwd_eval(ValuesMap& v) const override
        {
          for(const auto& x : _xs)
            x->bwd_eval(v);
        }

        std::pair<Index,Index> output_shape() const override
        {
          return { static_cast<Index>(_xs.size()), 1 };
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

        VectorType natural_fwd(ValuesMap& v, Index total_input_size) const
        {
          const Index n = static_cast<Index>(_xs.size());
          IntervalVector a(n);
          bool def_domain = true;
          for(Index i = 0 ; i < n ; ++i)
          {
            auto xi = _xs[static_cast<std::size_t>(i)]->fwd_eval(v, total_input_size, true);
            a[i] = xi.a;
            def_domain &= xi.def_domain;
          }

          return { a, def_domain };
        }

        std::vector<std::shared_ptr<AnalyticExpr<ScalarType>>> _xs;
    };
  }

  // Analytic operator
  // The following functions can be used to build analytic expressions.

  inline const ScalarExpr& _add_to_vec(const ScalarExpr& x)
  {
    return x;
  }

  inline ScalarExpr _add_to_vec(double x)
  {
    return const_value(x);
  }

  // Variadic:

  template<typename... X>
    requires ((std::is_same_v<typename ExprType<X>::Type,ScalarType>) && ...)
  inline VectorExpr
  vec(const X&... x)
  {
    return { std::make_shared<AnalyticOperationExpr<VectorOp,VectorType,typename ExprType<X>::Type...>>(_add_to_vec(x)...) };
  }

  // Dynamic:

  inline VectorExpr
  vec(const std::vector<ScalarExpr>& x)
  {
    return { std::make_shared<detail::DynamicVectorExpr>(x) };
  }
}
