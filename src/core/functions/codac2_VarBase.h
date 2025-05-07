/** 
 *  \file codac2_VarBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_ExprBase.h"

namespace codac2
{
  /**
   * \class VarBase
   * \brief Abstract base class for representing variables in analytic or set functions.
   *
   * The ``VarBase`` class serves as the abstract base for various types of variable representations,
   * such as scalar, vector, and matrix variables.
   */
  class VarBase
  {
    public:

      VarBase(const std::string& name)
        : _name(name)
      { }

      const std::string& name() const
      {
        return _name;
      }

      /**
       * \brief Virtual destructor for the ``VarBase`` class.
       * 
       * The destructor is virtual to ensure proper cleanup of derived classes.
       */
      virtual ~VarBase() = default;

      /**
       * \brief Returns the unique identifier of the variable.
       * 
       * Each instance of a derived class should have a unique identifier to distinguish it 
       * from other variables. This ID is used to identify the variable (or any expression)
       * independently of its memory address.
       * 
       * \return A constant reference to the unique identifier of the variable.
       */
      virtual const ExprID& unique_id() const = 0;

      /**
       * \brief Creates a copy of the argument.
       * 
       * This method should return a deep copy of the variable.
       * 
       * \return A shared pointer to a new instance of the variable (copy of the original).
       */
      virtual std::shared_ptr<VarBase> arg_copy() const = 0;

      /**
       * \brief Returns the mathematical size of the variable.
       * 
       * For scalar variables, the size is 1.
       * For vector variables of \f$\mathbb{R}^n\f$, the size is \f$n\f$.
       * For matrix variables of \f$r\f$ rows and \f$c\f$ columns, the size is \f$r\times c\f$.
       * 
       * \return The size of the variable, typically the number of components.
       */
      virtual Index size() const = 0;

    protected:

      friend class FunctionArgsList;

      std::string _name;
  };
}