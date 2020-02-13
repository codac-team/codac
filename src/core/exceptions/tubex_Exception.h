/** 
 *  \file
 *  Exception class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_EXCEPTION_H__
#define __TUBEX_EXCEPTION_H__

#include <iostream>
#include <exception>
#include <string>
#include <sstream>

namespace tubex
{
  /**
   * \class Exception
   * \brief Root class of all exceptions raised by Tubex
   */
  class Exception : public std::exception
  {
    public:

      /**
       * \brief Creates an `std::exception` related to a C++ function and some custom error message
       *
       * \param function_name the name of the function raising the Exception
       * \param custom_message the related error message
       */
      Exception(const std::string& function_name, const std::string& custom_message);

      /**
       * \brief Exception destructor
       */
      ~Exception() throw() {};
      
      /**
       * \brief Returns the exception message
       *
       * \return exception message
       */
      virtual const char* what() const throw();

    protected:

      std::string m_what_msg;
  };

  std::ostream& operator<<(std::ostream& os, const Exception& e);
}

#endif