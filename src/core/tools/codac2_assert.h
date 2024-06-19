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

namespace codac2
{
  #if defined FAST_RELEASE & defined NDEBUG

    #define assert_release(ignore_test,ignore_message) ((void)0)

  #else

    #define assert_release(test,error_message) \
    if(!(test)) \
    { \
      std::cerr << "[Assert error] " << error_message << std::endl; \
      abort(); \
    } \
    \

  #endif
}