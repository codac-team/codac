/** 
 *  \file codac2_ExprBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include <memory>
#include <cassert>
#include <utility>
#include "codac2_Domain.h"
#include "codac2_Index.h"

namespace codac2
{
  class ExprBase;
  class VarBase;

  /**
   * \class ExprID
   * \brief A class representing a unique identifier for expressions.
   * 
   * The ``ExprID`` class is used to assign and manage a unique identifier (``Index`` type) for
   * expressions. This identifier helps in distinguishing one expression from another, independently
   * of its memory address. It is used for tracking and comparison of expressions within a larger system.
   */
  class ExprID
  {
    public:

      /**
       * \brief Default constructor.
       * 
       * The default constructor generates a new unique ``ExprID`` by assigning it a new ID
       * from a static counter. Each ``ExprID`` object created will have a unique identifier.
       */
      ExprID();

      /**
       * \brief Retrieves the unique identifier of the expression.
       * 
       * \return The unique identifier.
       */
      Index id() const;

      /**
       * \brief Equality operator.
       * 
       * \param i The ``ExprID`` object to compare with.
       * \return ``true`` if the two ``ExprID`` objects have the same identifier, ``false`` otherwise.
       */
      bool operator==(const ExprID& i) const;

      /**
       * \brief Comparison operator.
       * 
       * This operator is used in sorting operations or other contexts requiring ordering.
       * 
       * \param i The ``ExprID`` object to compare with.
       * \return ``true`` if the unique identifier of the current object is less than that of ``i``, 
       *         ``false`` otherwise.
       */
      bool operator<(const ExprID& i) const;

    protected:

      const Index _id; //!< unique identifier, cannot be modified after initialization
      static Index _id_counter; //!< static counter used to generate unique IDs for each ``ExprID`` object
  };

  /**
   * \class ExprBase
   * \brief Abstract base class for representing an expression.
   * 
   * The ``ExprBase`` class serves as the base class for all expressions involved in functions.
   * It provides methods for managing expressions, including copying, replacing sub-expressions, and 
   * ensuring each expression has a unique identifier.
   */
  class ExprBase : public std::enable_shared_from_this<ExprBase>
  {
    public:

      /**
       * \brief Default constructor.
       * 
       * The constructor initializes the expression by generating its unique identifier.
       */
      ExprBase();

      /**
       * \brief Creates a copy of the current expression.
       * 
       * This is a pure virtual function that must be implemented by derived classes to 
       * create a deep copy of the expression.
       * 
       * \return A shared pointer to the new copy of the expression.
       */
      virtual std::shared_ptr<ExprBase> copy() const = 0;

      /**
       * \brief Replaces a variable by a new expression.
       * 
       * This is a pure virtual function that must be implemented by derived classes to 
       * replace, in the current expression, a specific variable
       * (corresponding to the ``old_arg_id``) with a new expression.
       * 
       * \param old_arg_id The ``ExprID`` of the variable to replace.
       * \param new_expr A shared pointer to the new expression to insert.
       */
      virtual void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr) = 0;

      /**
       * \brief Returns the unique identifier of the expression.
       * 
       * \return A constant reference to the unique identifier of the expression.
       */
      const ExprID& unique_id() const;

      /**
       * \brief Equality operator for comparing two expressions.
       * 
       * This operator compares two ``ExprBase`` objects by their unique identifiers to 
       * check if they represent the same expression.
       * 
       * \param e The ``ExprBase`` object to compare with.
       * \return ``true`` if the two expressions have the same unique identifier, ``false`` otherwise.
       */
      bool operator==(const ExprBase& e) const;

      /**
       * \brief Virtual destructor.
       * 
       * The destructor is virtual to ensure proper cleanup of derived classes when an 
       * object of a derived class is deleted through a pointer to ``ExprBase``.
       */
      virtual ~ExprBase() = default;

    protected:

      const ExprID _unique_id; //!< unique identifier for this expression
  };

  /**
   * \class OperationExprBase
   * \brief A base class for expressions representing operations with multiple operands.
   * 
   * The ``OperationExprBase`` class is a templated class designed to represent operations 
   * (*e.g.*, addition, multiplication, squared root) that involve one or multiple operand expressions.
   * The class holds a tuple of operand expressions, supports copying of operands, and provides 
   * functionality to replace specific expressions within the operation. The class is designed 
   * to be inherited by concrete operation expression classes, where the operands will be 
   * specialized.
   *
   * \tparam X The types of the operands in the operation. It supports an arbitrary number 
   *          of operand types via variadic templates.
   */
  template<typename... X>
  class OperationExprBase
  {
    public:

      /**
       * \brief Constructs an ``OperationExprBase`` with operand expressions.
       * 
       * This constructor initializes the operation expression with a set of operand 
       * expressions, which are passed as shared pointers. These operands are stored in 
       * a tuple for efficient access and manipulation.
       * 
       * \param x A variadic list of shared pointers to the operand expressions.
       */
      OperationExprBase(std::shared_ptr<X>... x)
        : _x(std::make_tuple((x)...))
      { }

      /**
       * \brief Copy constructor.
       * 
       * This constructor creates a new ``OperationExprBase`` by copying the operand expressions 
       * from another instance. Each operand is deep-copied using the ``copy()`` method of the 
       * underlying expression objects, ensuring that the new instance has independent copies 
       * of the operands.
       * 
       * \param e The ``OperationExprBase`` instance to copy from.
       */
      OperationExprBase(const OperationExprBase<X...>& e)
        : _x(e._x)
      {
        std::apply(
          [](auto &&... x)
          {
            ((x = __copy(x)), ...);
          }, _x);
      }

      /**
       * \brief Replaces a variable by a new expression.
       * 
       * This method replaces, in the current expression, a specific variable
       * (corresponding to the ``old_arg_id``) with a new expression.
       * 
       * \param old_arg_id The ``ExprID`` of the variable to replace.
       * \param new_expr A shared pointer to the new expression to insert.
       */
      void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        std::apply(
          [old_arg_id,new_expr](auto &&... x)
          {
            (__replace_arg(x,old_arg_id,new_expr), ...);
          }, _x);
      }

    protected:

      /**
       * \brief Helper function to copy a single operand expression.
       * 
       * This function performs a deep copy of an operand expression.
       * 
       * \tparam X_ The type of the operand expression.
       * \param x A shared pointer to the operand expression to copy.
       * \return A shared pointer to the new copied expression.
       */
      template<typename X_>
      static std::shared_ptr<X_> __copy(const std::shared_ptr<X_>& x)
      {
        return std::dynamic_pointer_cast<X_>(x->copy());
      }

      /**
       * \brief Helper function to replace a variable by a new expression.
       * 
       * This function replaces, in the expression pointed by ``x``, a specific variable
       * (an argument, corresponding to the ``old_arg_id``) with a new expression.
       * 
       * \tparam D The type of the current expression.
       * \param x A reference to the shared pointer of the current expression.
       * \param old_arg_id The ``ExprID`` of the variable to replace.
       * \param new_expr A shared pointer to the new expression to insert.
       */
      template<typename D>
      static void __replace_arg(std::shared_ptr<D>& x, const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        if(x->unique_id() == old_arg_id)
        {
          assert(std::dynamic_pointer_cast<VarBase>(x) && "this subexpr should be some variable");
          x = std::dynamic_pointer_cast<D>(new_expr);
        }
        else
          x->replace_arg(old_arg_id, new_expr);
      }

      std::tuple<std::shared_ptr<X>...> _x; //!< tuple storing the operand expressions
  };
}