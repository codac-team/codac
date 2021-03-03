/** 
 *  \file
 *  Exception class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

// For maximum portability, we recommend you use __func__, but provide a fallback definition with the preprocessor: 
// https://gcc.gnu.org/onlinedocs/gcc-4.8.1/gcc/Function-Names.html
#ifndef __CODAC_EXCEPTION_H__
#define __CODAC_EXCEPTION_H__

#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif

#include <iostream>
#include <exception>
#include <string>
#include <sstream>

namespace codac
{
  /**
   * \class Exception
   * \brief Root class of all exceptions raised by Codac
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