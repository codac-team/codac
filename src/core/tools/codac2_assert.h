/** 
 *  \file codac2_assert.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <cstdlib>
#include <cassert>
#include <sstream>
#include <source_location>

namespace codac2
{
  #if defined FAST_RELEASE & defined NDEBUG

    #define assert_release(ignore_test,ignore_message) ((void)0)

  #else

    #define assert_release(test) \
    if(!(test)) \
    { \
      auto l = std::source_location::current(); \
      std::ostringstream s; \
      s << l.file_name() << '(' \
        << l.line() << ':' \
        << l.column() << ")\n  " \
        << l.function_name() << "\n"; \
      throw std::invalid_argument("Wrong assertion in the following function:\n  " + s.str()); \
      abort(); \
    } \
    \

  #endif
}