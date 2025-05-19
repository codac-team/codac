/** 
 *  \file codac2_AnalyticFunction_impl.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

namespace codac2
{
  inline EvalMode operator&(EvalMode a, EvalMode b)
  { return static_cast<EvalMode>(static_cast<int>(a) & static_cast<int>(b)); }

  inline EvalMode operator|(EvalMode a, EvalMode b)
  { return static_cast<EvalMode>(static_cast<int>(a) | static_cast<int>(b)); }


  // Public methods

    template<typename T>
      requires std::is_base_of_v<AnalyticTypeBase,T>
    template<typename... X>
    inline AnalyticExprWrapper<T> AnalyticFunction<T>::operator()(const X&... x) const
    {
      return { this->FunctionBase<AnalyticExpr<T>>::operator()(x...) };
    }

    template<typename T>
      requires std::is_base_of_v<AnalyticTypeBase,T>
    template<typename... Args>
    inline auto AnalyticFunction<T>::real_eval(const Args&... x) const
    {
      return eval(x...).mid();
    }

    template<typename T>
      requires std::is_base_of_v<AnalyticTypeBase,T>
    template<typename... Args>
    inline typename T::Domain AnalyticFunction<T>::eval(const EvalMode& m, const Args&... x) const
    {
      check_valid_inputs(x...);

      switch(m)
      {
        case EvalMode::NATURAL:
        {
          return eval_<true>(x...).a;
        }

        case EvalMode::CENTERED:
        {
          auto x_ = eval_<false>(x...);
          auto flatten_x = cart_prod(x...);
          assert(x_.da.rows() == x_.a.size() && x_.da.cols() == flatten_x.size());
          
          if constexpr(std::is_same_v<T,ScalarType>)
            return x_.m + (x_.da*(flatten_x-flatten_x.mid()))[0];
          else
            return x_.m + (x_.da*(flatten_x-flatten_x.mid())).col(0);
        }

        case EvalMode::DEFAULT:
        default:
        {
          auto x_ = eval_<false>(x...);

          if(x_.da.size() == 0) // if the centered form is not available for this expression
            return eval(EvalMode::NATURAL, x...);

          else
          {
            auto flatten_x = cart_prod(x...);
            if constexpr(std::is_same_v<T,ScalarType>)
              return x_.a & (x_.m + (x_.da*(flatten_x-flatten_x.mid()))[0]);
            else
            {
              assert(x_.da.rows() == x_.a.size() && x_.da.cols() == flatten_x.size());
              return x_.a & (x_.m + (x_.da*(flatten_x-flatten_x.mid())).col(0));
            }
          }
        }
      }
    }

    template<typename T>
      requires std::is_base_of_v<AnalyticTypeBase,T>
    template<typename... Args>
    inline typename T::Domain AnalyticFunction<T>::eval(const Args&... x) const
    {
      return eval(EvalMode::NATURAL | EvalMode::CENTERED, x...);
    }

    template<typename T>
      requires std::is_base_of_v<AnalyticTypeBase,T>
    template<typename... Args>
    inline auto AnalyticFunction<T>::diff(const Args&... x) const
    {
      check_valid_inputs(x...);
      return eval_<false>(x...).da;
    }

    template<typename T>
      requires std::is_base_of_v<AnalyticTypeBase,T>
    inline Index AnalyticFunction<T>::output_size() const
    {
      if constexpr(std::is_same_v<T,ScalarType>)
        return 1;

      else if constexpr(std::is_same_v<T,VectorType>)
      {
        assert_release(this->args().size() == 1 && "unable (yet) to compute output size for multi-arg functions");

        // A dump evaluation is performed to estimate the dimension
        // of the image of this function. A natural evaluation is assumed
        // to be faster.

        if(dynamic_cast<ScalarVar*>(this->args()[0].get())) // if the argument is scalar
          return eval(EvalMode::NATURAL, Interval()).size();
        else
          return eval(EvalMode::NATURAL, IntervalVector(this->input_size())).size();
      }

      else
      {
        assert_release(false && "unable to estimate output size");
        return 0;
      }
    }

    template<typename U>
    std::ostream& operator<<(std::ostream& os, [[maybe_unused]] const AnalyticFunction<U>& f)
    {
      if constexpr(std::is_same_v<U,ScalarType>) 
        os << "scalar function";
      else if constexpr(std::is_same_v<U,VectorType>) 
        os << "vector function";
      return os;
    }

  // Protected methods

    template<typename T>
      requires std::is_base_of_v<AnalyticTypeBase,T>
    template<typename D>
    inline void AnalyticFunction<T>::add_value_to_arg_map(ValuesMap& v, const D& x, Index i) const
    {
      assert(i >= 0 && i < (Index)this->args().size());
      assert_release(size_of(x) == this->args()[i]->size() && "provided arguments do not match function inputs");

      IntervalMatrix d = IntervalMatrix::zero(size_of(x), this->args().total_size());
      
      Index p = 0, j = 0;
      for( ; j < i ; j++)
        p += this->args()[j]->size();

      for(Index k = p ; k < p+size_of(x) ; k++)
        d(k-p,k) = 1.;

      using D_DOMAIN = typename ExprType<D>::Type;

      v[this->args()[i]->unique_id()] = 
        std::make_shared<D_DOMAIN>(typename D_DOMAIN::Domain(x).mid(), x, d, true);
    }

    template<typename T>
      requires std::is_base_of_v<AnalyticTypeBase,T>
    template<typename... Args>
    inline void AnalyticFunction<T>::fill_from_args(ValuesMap& v, const Args&... x) const
    {
      Index i = 0;
      (add_value_to_arg_map(v, x, i++), ...);
    }

    template<typename T>
      requires std::is_base_of_v<AnalyticTypeBase,T>
    template<typename D>
    inline void AnalyticFunction<T>::intersect_value_from_arg_map(const ValuesMap& v, D& x, Index i) const
    {
      assert(v.find(this->args()[i]->unique_id()) != v.end() && "argument cannot be found");
      x &= std::dynamic_pointer_cast<typename ExprType<D>::Type>(v.at(this->args()[i]->unique_id()))->a;
    }

    template<typename T>
      requires std::is_base_of_v<AnalyticTypeBase,T>
    template<typename... Args>
    inline void AnalyticFunction<T>::intersect_from_args(const ValuesMap& v, Args&... x) const
    {
      Index i = 0;
      (intersect_value_from_arg_map(v, x, i++), ...);
    }

    template<typename T>
      requires std::is_base_of_v<AnalyticTypeBase,T>
    template<bool NATURAL_EVAL,typename... Args>
    inline auto AnalyticFunction<T>::eval_(const Args&... x) const
    {
      ValuesMap v;

      if constexpr(sizeof...(Args) == 0)
        return this->expr()->fwd_eval(v, 0, NATURAL_EVAL);

      else
      {
        fill_from_args(v, x...);
        return this->expr()->fwd_eval(v, cart_prod(x...).size(), NATURAL_EVAL); // todo: improve size computation
      }
    }

    template<typename T>
      requires std::is_base_of_v<AnalyticTypeBase,T>
    template<typename... Args>
    inline void AnalyticFunction<T>::check_valid_inputs(const Args&... x) const
    {
      [[maybe_unused]] Index n = 0;
      ((n += size_of(x)), ...);

      assert_release(this->_args.total_size() == n && 
        "Invalid arguments: wrong number of input arguments");
    }
}