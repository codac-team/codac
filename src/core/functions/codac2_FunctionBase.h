/** 
 *  \file codac2_FunctionBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_ExprBase.h"
#include "codac2_analytic_variables.h"
#include "codac2_FunctionArgsList.h"

// todo: remove this:
#include "codac2_analytic_constants.h"

namespace codac2
{
  /**
   * \class FunctionBase
   * \brief A base class for functions (either analytic functions, or set functions).
   * 
   * The ``FunctionBase`` class represents a generic function that operates on expressions. It 
   * allows defining functions that take a list of arguments and apply a specific 
   * expression to those arguments.
   *
   * This class serves as a base for creating specific types of functions by providing the mechanism 
   * to handle arguments and perform the function application.
   * 
   * Note that a function may be invoked in another expression, by using the ``operator()`` which
   * allows compositions of functions.
   * For such call, the arguments of this operator are either other function variables or any expressions.
   * 
   * \tparam E The type of the output expression of this function.
   */
  template<typename E>
  class FunctionBase
  {
    public:

      /**
       * \brief Constructs a ``FunctionBase`` with a list of arguments and a shared pointer to an output expression.
       * 
       * The arguments are stored as a ``FunctionArgsList``, and the expression is deep-copied.
       * 
       * \param args A reference to a vector of ``VarBase`` references representing the function's arguments.
       * \param y A shared pointer to the expression that defines the function.
       */
      FunctionBase(const std::vector<std::reference_wrapper<VarBase>>& args, const std::shared_ptr<E>& y)
        : FunctionBase(FunctionArgsList(args),y)
      { }

      /**
       * \brief Constructs a ``FunctionBase`` with a list of arguments and a shared pointer to an output expression.
       * 
       * The expression is deep-copied.
       * 
       * \param args A ``FunctionArgsList`` containing the function's arguments.
       * \param y A shared pointer to the function's expression.
       */
      FunctionBase(const FunctionArgsList& args, const std::shared_ptr<E>& y)
        : _y(std::dynamic_pointer_cast<E>(y->copy())), _args(args)
      { }

      /**
       * \brief Copy constructor.
       * 
       * The expression is deep-copied.
       * 
       * \param f The ``FunctionBase`` instance to copy.
       */
      FunctionBase(const FunctionBase<E>& f)
        : _y(std::dynamic_pointer_cast<E>(f.expr()->copy())), _args(f.args()) // todo: keep this dynamic_pointer_cast?
      { }
      
      /**
       * \brief Virtual destructor.
       * 
       * The destructor is virtual to ensure proper cleanup of derived classes when an object of a 
       * derived class is deleted through a pointer to ``FunctionBase``.
       */
      virtual ~FunctionBase()
      { }

      /**
       * \brief Returns the arguments of the function.
       * 
       * \return A constant reference to the ``FunctionArgsList`` containing the function's arguments.
       */
      const FunctionArgsList& args() const
      {
        return _args;
      }

      /**
       * \brief Returns the number of arguments of the function.
       * 
       * \return an integer.
       */
      size_t nb_args() const
      {
        return _args.size();
      }

      /**
       * \brief Returns the expression associated with the function.
       * 
       * \return A constant reference to the shared pointer to the function's expression.
       */
      const std::shared_ptr<E>& expr() const
      {
        return _y;
      }

      /**
       * \brief Applies this function to the arguments and returns the resulting expression.
       * 
       * The method deep-copies the expressions provided as arguments and replaces the arguments 
       * in the expression with the provided ones.
       * 
       * \param x The arguments to apply to the function, for instance expressions or variables.
       * \return A shared pointer to the resulting expression after the arguments have been replaced.
       */
      template<typename... X>
      std::shared_ptr<E> operator()(const X&... x) const
      {
        auto expr_copy = expr()->copy();
        size_t i = 0;
        (expr_copy->replace_arg(_args[i++]->unique_id(), this->__get_copy(x)), ...);
        assert_release(i == this->args().size() && 
          "Invalid arguments: wrong number of input arguments");
        return std::dynamic_pointer_cast<E>(expr_copy);
      }

      /**
       * \brief Applies this function to the arguments and returns the resulting expression.
       * 
       * The method deep-copies the expressions provided as arguments and replaces the arguments 
       * in the expression with the provided ones.
       * 
       * This method is mainly used for Python binding.
       * 
       * \param x A vector of shared pointers to arguments to apply to the function, for instance expressions or variables.
       * \return A shared pointer to the resulting expression after the arguments have been replaced.
       */
      std::shared_ptr<E> operator()(const std::vector<std::shared_ptr<ExprBase>>& x) const
      {
        assert_release(x.size() == this->args().size() && 
          "Invalid arguments: wrong number of input arguments");
        auto expr_copy = expr()->copy();
        for(size_t i = 0 ; i < x.size() ; i++)
          expr_copy->replace_arg(_args[i]->unique_id(), x[i]->copy());
        return std::dynamic_pointer_cast<E>(expr_copy);
      }

      /**
       * \brief Calculates the total size of the function arguments,
       * as the sum of the sizes of each argument.
       * 
       * \return The sum of the sizes of all arguments.
       */
      Index input_size() const
      {
        return this->_args.total_size();
      }

    protected:

      /**
       * \brief Helper function to get a copy of a an expression.
       * 
       * \tparam X The type of the expression to copy.
       * \param x A shared pointer to the expression to copy.
       * \return A shared pointer to the copied expression.
       */
      template<typename X>
      std::shared_ptr<X> __get_copy(std::shared_ptr<X> x) const
      {
        return std::dynamic_pointer_cast<X>(x->copy());
      }

      /**
       * \brief Helper function to get a copy of an ``AnalyticExprWrapper``.
       * 
       * \tparam X The type of the expression to copy.
       * \param x The ``AnalyticExprWrapper`` to copy.
       * \return A shared pointer to the copied expression.
       */
      template<typename X>
      std::shared_ptr<ExprBase> __get_copy(const AnalyticExprWrapper<X>& x) const
      {
        return x->copy();
      }

      /**
       * \brief Helper function to get a copy of various expression types.
       * 
       * \tparam A The type of the expression.
       * \param x The expression to copy.
       * \return A shared pointer to the copied expression.
       */
      template<typename A>
      auto __get_copy(const A& x) const
      {
        if constexpr(std::is_base_of_v<VarBase,A>)
          return std::dynamic_pointer_cast<A>(x.copy());
        else
        {
          // todo: make this generic (analytic / set):
          return const_value(x);
        }
      }

      const std::shared_ptr<E> _y; //!< expression that defines the function
      const FunctionArgsList _args; //!< arguments of the function
  };
}