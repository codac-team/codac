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
    #define assert_release_constexpr(ignore_test) ((void)0)

  #else

    // In the following, do..while(0) is used for syntax protection

    #define CODAC_ASSERT_MESSAGE(test, file, line, function)                  \
    do {                                                                      \
      throw std::invalid_argument(                                            \
        std::string("\n=============================================================================") \
        + "\nThe following Codac assertion failed:\n\n\tIn function: " + std::string(function) + "\n\n\t" + std::string(test) \
        + "\n\nIn file: " + std::string(file) + ":" + std::to_string(line)         \
        + "\nYou need help? Submit an issue on: https://github.com/codac-team/codac/issues" \
        + "\n=============================================================================" \
      );                                                                      \
    } while (0)

    #if defined(__PRETTY_FUNCTION__)

      #define assert_release(test) \
        do {                       \
          if(!(test)) {            \
            CODAC_ASSERT_MESSAGE(#test, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
          }                        \
        } while (0)

      #define assert_release_constexpr(test) \
        do {                       \
          if constexpr(!(test)) {            \
            CODAC_ASSERT_MESSAGE(#test, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
          }                        \
        } while (0)

    #else

      #define assert_release(test) \
        do {                       \
          if(!(test)) {            \
            CODAC_ASSERT_MESSAGE(#test, __FILE__, __LINE__, __func__); \
          }                        \
        } while (0)

      #define assert_release_constexpr(test) \
        do {                       \
          if constexpr(!(test)) {            \
            CODAC_ASSERT_MESSAGE(#test, __FILE__, __LINE__, __func__); \
          }                        \
        } while (0)

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