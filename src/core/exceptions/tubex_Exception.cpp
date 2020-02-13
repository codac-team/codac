/** 
 *  Exception class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <string>
#include <sstream>
#include "tubex_Exception.h"

using namespace std;

namespace tubex
{
  Exception::Exception(const std::string& function_name, const std::string& custom_message)
  {
    m_what_msg = "in " + function_name + ": " + custom_message;
  }

  const char* Exception::what() const throw()
  {
    return m_what_msg.c_str();
  }

  std::ostream& operator<<(std::ostream& os, const Exception& e)
  {
    os << e.what();
    return os;
  }
}