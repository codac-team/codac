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
// C++20 not fully supported by compilers yet: #include <source_location>

namespace codac2
{
  #if defined FAST_RELEASE & defined NDEBUG

    #define assert_release(ignore_test) ((void)0)

  #else

    #if defined __PRETTY_FUNCTION__

      #define assert_release(test) \
      if(!(test)) \
      { \
        std::string s = std::string("\n=============================================================================") \
          + "\nThe following Codac assertion failed:\n\n\t" + std::string(#test) \
          + "\n \nIn: " + std::string(__FILE__) + ":" + std::to_string(__LINE__) \
          + "\nFunction: " + std::string(__PRETTY_FUNCTION__) \
          + "\nYou need help? Submit an issue on: https://github.com/codac-team/codac/issues" \
          + "\n============================================================================="; \
        throw std::invalid_argument(s); \
        abort(); \
      } \
      \

    #else

      #define assert_release(test) \
      if(!(test)) \
      { \
        std::string s = std::string("\n=============================================================================") \
          + "\nThe following Codac assertion failed:\n\n\t" + std::string(#test) \
          + "\n \nIn: " + std::string(__FILE__) + ":" + std::to_string(__LINE__) \
          + "\nFunction: " + std::string(__func__) \
          + "\nYou need help? Submit an issue on: https://github.com/codac-team/codac/issues" \
          + "\n============================================================================="; \
        throw std::invalid_argument(s); \
        abort(); \
      } \
      \

    #endif

  #endif
}

#if 0

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