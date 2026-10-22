/** 
 *  \file codac2_AnalyticFunction.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_AnalyticExpr.h"
#include "codac2_Domain.h"
#include "codac2_analytic_variables.h"
#include "codac2_FunctionBase.h"
#include "codac2_template_tools.h"
#include "codac2_AnalyticExprWrapper.h"
#include "codac2_operators.h"
#include "codac2_cart_prod.h"
#include "codac2_vec.h"
#include "codac2_Wrapper.h"
#include "codac2_Parallelepiped.h"
#include "codac2_peibos_tools.h"

namespace codac2
{
  enum class EvalMode
  {
    NATURAL = 0x01,
    CENTERED = 0x02,
    DEFAULT = 0x03, // corresponds to (NATURAL|CENTERED)
    MIXED = 0x04,  // note: not compatible with CENTERED
    MIXED_FULL = 0x05 // corresponds to (NATURAL|MIXED)
  };

  inline EvalMode operator&(EvalMode a, EvalMode b)
  { return static_cast<EvalMode>(static_cast<int>(a) & static_cast<int>(b)); }

  inline EvalMode operator|(EvalMode a, EvalMode b)
  { return static_cast<EvalMode>(static_cast<int>(a) | static_cast<int>(b)); }

  /* partial mid to build the a field */
  inline Interval partial_mid(const Interval &a,[[maybe_unused]] Index i) {
      return a;
  }
  inline IntervalVector partial_mid(const IntervalVector &a,Index i) {
      IntervalVector m(a);
      Index sz = m.size();
      for (Index j=i+1;j<sz;j++) m[j]=m[j].mid();
      return m;
  }
  inline IntervalMatrix partial_mid(const IntervalMatrix &a,Index i) {
      IntervalMatrix m(a);
      Index sz = m.reshaped().size();
      for (Index j=i+1;j<sz;j++) m.reshaped()[j]=m.reshaped()[j].mid();
      return m;
  }
  inline Interval partial_mid(const double &a,[[maybe_unused]] Index i) {
      return Interval(a);
  }
  inline Interval partial_mid(const int &a,[[maybe_unused]] Index i) {
      return Interval(a);
  }
  inline IntervalVector partial_mid(const Vector &a,[[maybe_unused]] Index i) {
      return IntervalVector(a);
  }
  inline IntervalMatrix partial_mid(const Matrix &a,[[maybe_unused]] Index i) {
      return IntervalMatrix(a);
  }


  struct ScalarExprList : public AnalyticExprWrapper<VectorType>
  {
    // Mainly used to take advantage of initializer lists in AnalyticFunction constructors.
    template<typename... S>
      requires (std::is_same_v<typename ExprType<S>::Type,ScalarType> && ...)
    ScalarExprList(const S&... y)
      : AnalyticExprWrapper<VectorType>(vec(y...))
    { }
  };

  template<typename T>
  class SampledTraj;

  template<typename T>
  class SlicedTube;

  template<typename T>
    requires std::is_base_of_v<AnalyticTypeBase,T>
  class AnalyticFunction : public FunctionBase<AnalyticExpr<T>>
  {
    public:

      AnalyticFunction(const FunctionArgsList& args, const ScalarExprList& y)
        requires(std::is_same_v<T,VectorType>)
        : FunctionBase<AnalyticExpr<T>>(args, y)
      {
        assert_release(y->belongs_to_args_list(this->args()) && 
          "Invalid argument: variable not present in input arguments");
        update_var_names();
      }

      AnalyticFunction(const FunctionArgsList& args, const AnalyticExprWrapper<T>& y)
        : FunctionBase<AnalyticExpr<T>>(args, y)
      {
        assert_release(y->belongs_to_args_list(this->args()) && 
          "Invalid argument: variable not present in input arguments");
        update_var_names();
      }

      AnalyticFunction(const AnalyticFunction<T>& f)
        : FunctionBase<AnalyticExpr<T>>(f)
      { }

      template<typename... X>
      AnalyticExprWrapper<T> operator()(const X&... x) const
      {
        return { this->FunctionBase<AnalyticExpr<T>>::operator()(x...) };
      }

      template<typename... Args>
      auto real_eval(const Args&... x) const
      {
        return eval(x...).mid();
      }

      template<typename... Args>
      typename T::Domain eval(const EvalMode& m, const Args&... x) const
      {
        check_valid_inputs(x...);

        switch(m)
        {
          case EvalMode::NATURAL:
          {
            return eval_<true,false>(x...).get_a();
          }

          case EvalMode::CENTERED:
          {
            auto x_ = eval_<false,false>(x...);
            auto flatten_x = IntervalVector(cart_prod(x...));
            assert(x_.da.rows() == x_.a.size() && x_.da.cols() == flatten_x.size());
            
            if constexpr(std::is_same_v<T,ScalarType>)
              return x_.m + (x_.da*(flatten_x-flatten_x.mid()))[0];

            else if constexpr(std::is_same_v<T,VectorType>)
              return x_.m + (x_.da*(flatten_x-flatten_x.mid())).col(0);

            else
            {
              static_assert(std::is_same_v<T,MatrixType>);
              return x_.m + (x_.da*(flatten_x-flatten_x.mid()))
                .reshaped(x_.m.rows(), x_.m.cols());
            }
          }

	  case EvalMode::MIXED:
          case EvalMode::MIXED_FULL:
          {
            auto x_ = eval_<false, true>(x...);
            // If the centered form is not available for this expression...
            if(m==EvalMode::MIXED_FULL && (x_.da.size() == 0 // .. because some parts have not yet been implemented,
              || !x_.def_domain)) // .. or due to restrictions in the derivative definition domain
              return x_.get_a(); // natural evaluation

            else {
              auto flatten_x = IntervalVector(cart_prod(x...));
              assert(x_.da.rows() == x_.a.size() && x_.da.cols() == flatten_x.size());
            
              if constexpr(std::is_same_v<T,ScalarType>) {
                Interval ret = x_.m + (x_.da*(flatten_x-flatten_x.mid()))[0];
                return (m==EvalMode::MIXED_FULL ? x_.get_a() & ret : ret);
              }
              else if constexpr(std::is_same_v<T,VectorType>) {
                IntervalVector ret = x_.m + (x_.da*(flatten_x-flatten_x.mid())).col(0);
                return (m==EvalMode::MIXED_FULL ? x_.get_a() & ret : ret);
              }
              else
              {
                static_assert(std::is_same_v<T,MatrixType>);
                IntervalMatrix ret =  x_.m + (x_.da*(flatten_x-flatten_x.mid()))
                  .reshaped(x_.m.rows(), x_.m.cols());
                return (m==EvalMode::MIXED_FULL ? x_.get_a() & ret : ret);
              }
            }
          }

          case EvalMode::DEFAULT:
          default:
          {
            auto x_ = eval_<false,false>(x...);

            // If the centered form is not available for this expression...
            if(x_.da.size() == 0 // .. because some parts have not yet been implemented,
              || !x_.def_domain) // .. or due to restrictions in the derivative definition domain
              return x_.get_a(); // natural evaluation

            else
            {
              auto flatten_x = IntervalVector(cart_prod(x...));

              if constexpr(std::is_same_v<T,ScalarType>)
                return x_.get_a() & (x_.m + (x_.da*(flatten_x-flatten_x.mid()))[0]);

              else if constexpr(std::is_same_v<T,VectorType>)
              {
                assert(x_.da.rows() == x_.a.size() && x_.da.cols() == flatten_x.size());
                return x_.get_a() & (x_.m + (x_.da*(flatten_x-flatten_x.mid())).col(0));
              }

              else
              {
                static_assert(std::is_same_v<T,MatrixType>);
                assert(x_.da.rows() == x_.a.size() && x_.da.cols() == flatten_x.size());
                return x_.get_a() & (x_.m +(x_.da*(flatten_x-flatten_x.mid()))
                  .reshaped(x_.m.rows(),x_.m.cols()));
              }
            }
          }
        }
      }

      template<typename... X>
      typename T::Domain eval_default_codomain() const
      {
        static_assert(sizeof...(X) > 0);
        assert_release(this->args().size() == sizeof...(X)
          && "eval_default_codomain: function arity does not match contractor signature");

        return eval_default_codomain_impl<X...>(
          std::index_sequence_for<X...>{}
        );
      }

      template<bool MIXED_EVAL=false,typename... Args>
      auto diff(const Args&... x) const
      {
        check_valid_inputs(x...);
        return eval_<false,MIXED_EVAL>(x...).da;
      }

      template<typename... Args>
      typename T::Domain eval(const Args&... x) const
      {
        return eval(EvalMode::NATURAL | EvalMode::CENTERED, x...);
      }

      template<typename... Args>
      auto traj_eval(const SampledTraj<Args>&... x) const
      {
        SampledTraj<typename T::Scalar> y;
        for(const auto& [ti,xi] : std::get<0>(std::tie(x...)))
          y.set(this->real_eval(x(ti)...),ti);
        return y;
      }

      template<typename... Args>
      auto tube_eval(const SlicedTube<Args>&... x) const
      {
        auto tdomain = std::get<0>(std::tie(x...)).tdomain();

        SlicedTube<typename T::Domain> y(
          tdomain, (typename T::Domain)(this->output_size())
        );

        for(auto it = tdomain->begin() ; it != tdomain->end() ; it++)
          y.slice(it)->codomain() = this->eval(x.slice(it)->codomain()...);

        return y;
      }

      template<bool MIXED_EVAL=false,typename... Args>
        requires std::is_same_v<VectorType,T> && ((!std::is_same_v<MatrixType,typename ExprType<Args>::Type>) && ...)
      Parallelepiped parallelepiped_eval(const Args&... x) const
      {
        this->check_valid_inputs(x...);
        assert_release(this->input_size() <= this->output_size() &&
                      "Parallelepiped evaluation can not take more input than output.");
        assert_release(this->input_size() > 0 &&
                    "Parallelepiped evaluation requires at least one input.");

        IntervalVector Y = this->eval(((typename Wrapper<Args>::Domain)(x)).mid()...);
        Vector c = Y.mid();

        Matrix A = this->diff<MIXED_EVAL>(((typename Wrapper<Args>::Domain)(x)).mid()...).mid();

        // Maximum error computation
        double rho = error_peibos(Y, c, this->diff<MIXED_EVAL>(x...), A, cart_prod(x...));
	// TODO : check the correction of this

        // Inflation of the parallelepiped
        Matrix A_inf = inflate_flat_parallelepiped(A, (cart_prod(x...).template cast<Interval>()).rad(), rho);

        return Parallelepiped(c, A_inf);
      }

      Index output_size() const
      {
        if constexpr(std::is_same_v<T,ScalarType>)
          return 1;

        else {
          std::pair<Index,Index> oshape = output_shape();
          return oshape.first * oshape.second;
        }
      }

      std::pair<Index,Index> output_shape() const 
      {
        if constexpr(std::is_same_v<T,ScalarType>)
          return {1,1};
        else return this->expr()->output_shape();
      }

      friend std::ostream& operator<<(std::ostream& os, [[maybe_unused]] const AnalyticFunction<T>& f)
      {
        os << "(";
        for(size_t i = 0 ; i < f.args().size() ; i++)
          os << (i!=0 ? "," : "") << f.args()[i]->name();
        os << ") -> " << f.expr()->str();
        return os;
      }

      // not working with Clang: template<typename Y, typename... X>
      // not working with Clang:   requires (sizeof...(X) > 0)
      // not working with Clang: friend class CtcInverse;

      // So, the following methods are temporarily public

      // protected:

      template<typename... Args>
      void fill_from_args(ValuesMap& v, const Args&... x) const
      {
        Index i = 0;
        (add_value_to_arg_map(v, x, i++), ...);
      }

      template<typename... Args>
      void intersect_from_args(const ValuesMap& v, Args&... x) const
      {
        Index i = 0;
        (intersect_value_from_arg_map(v, x, i++), ...);
      }

    protected:

      template<typename D>
      void add_value_to_arg_map(ValuesMap& v, const D& x, Index i) const
      {
        assert(i >= 0 && i < (Index)this->args().size());
        assert_release(size_of(x) == this->args()[i]->size() && "provided arguments do not match function inputs");

        using D_TYPE = typename ExprType<D>::Type;

        IntervalMatrix d = IntervalMatrix::zero(size_of(x), this->args().total_size());
        
        Index p = 0;
        for(Index j = 0 ; j < i ; j++)
          p += this->args()[j]->size();

        for(Index k = p ; k < p+size_of(x) ; k++)
          d(k-p,k) = 1.;
 
        std::vector<typename D_TYPE::Domain> vecx;
        vecx.reserve(this->args().total_size());
			
        typename D_TYPE::Domain midx = typename D_TYPE::Domain(x).mid();
        Index l;
        for(l=0;l<p;l++) {
             vecx.push_back(midx);
        }
        for(;l<p+size_of(x)-1;l++) {
             vecx.push_back(
		(static_cast<typename D_TYPE::Domain(*)(const D&,Index)>(partial_mid))(x,l-p));
        }
        for(;l<this->args().total_size();l++) {
             vecx.push_back(x);
        }

        v[this->args()[i]->unique_id()] = 
          std::make_shared<D_TYPE>(midx,
			vecx, d, true);
      }

      template<typename D>
      void intersect_value_from_arg_map(const ValuesMap& v, D& x, Index i) const
      {
        assert(v.find(this->args()[i]->unique_id()) != v.end() && "argument cannot be found");
        x &= std::dynamic_pointer_cast<typename ExprType<D>::Type>(v.at(this->args()[i]->unique_id()))->get_a();
      }

      template<bool NATURAL_EVAL,bool MIXED_DIFF,typename... Args>
      auto eval_(const Args&... x) const
      {
        ValuesMap v;

        if constexpr(sizeof...(Args) == 0)
          return this->expr()->fwd_eval(v, 0, NATURAL_EVAL, MIXED_DIFF);

        else
        {
          fill_from_args(v, x...);
          return this->expr()->fwd_eval(v, this->input_size(), NATURAL_EVAL, MIXED_DIFF);
        }
      }

      template<typename... Args>
      void check_valid_inputs(const Args&... x) const
      {
        [[maybe_unused]] Index n = 0;
        ((n += size_of(x)), ...);

        assert_release(this->_args.total_size() == n && 
          "Invalid arguments: wrong number of input arguments");
      }
      
      inline void update_var_names()
      {
        for(const auto& v : this->_args) // variable names are automatically computed in FunctionArgsList,
          // so we propagate them to the expression
          this->_y->replace_arg(v->unique_id(), std::dynamic_pointer_cast<ExprBase>(v));
      }

      template<typename X>
      static X default_eval_input(const std::shared_ptr<VarBase>& arg)
      {
        if constexpr(std::is_same_v<X,Interval>)
          return Interval();
        else if constexpr(std::is_same_v<X,IntervalVector>)
          return IntervalVector(arg->size());
        else
        {
          static_assert(!std::is_same_v<X,X>,
            "default_eval_input: unsupported input type");
        }
      }

      template<typename... X, std::size_t... I>
      typename T::Domain eval_default_codomain_impl(std::index_sequence<I...>) const
      {
        return this->eval(default_eval_input<X>(this->args()[I])...);
      }
  };

  AnalyticFunction(const FunctionArgsList&, std::initializer_list<ScalarExpr>) -> 
    AnalyticFunction<VectorType>;

  template<typename T>
  AnalyticFunction(const FunctionArgsList&, const T&) -> 
    AnalyticFunction<typename ExprType<T>::Type>;

}
