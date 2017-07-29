/* ============================================================================
 *  tubex-lib - TubeException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "TubeException.h"
#include <string>
#include <sstream>

using namespace std;

namespace tubex
{
  TubeException::TubeException(const std::string& function_name, const std::string& custom_message)
  {
    m_what_msg = "in " + function_name + ": " + custom_message;
  }

  const char* TubeException::what() const throw()
  {
    return m_what_msg.c_str();
  }

  std::ostream& operator<<(std::ostream& os, const TubeException& e)
  {
    os << e.what();
    return os;
  }
}