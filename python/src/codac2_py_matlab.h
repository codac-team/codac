/** 
 *  \file
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#define FOR_MATLAB false

namespace codac2
{
  #if FOR_MATLAB
    using size_t_type = double;
    using int_type = double;
  #else
    using size_t_type = size_t;
    using int_type = int;
  #endif

  namespace matlab
  {
    template<typename I>
    void test_integer(I x)
    {
      if constexpr(FOR_MATLAB)
        if((int)x != x)
          throw pybind11::index_error("provided value is not an integer");
    }

    template<typename I>
    void test_integer(I x1, I x2)
    {
      test_integer(x1);
      test_integer(x2);
    }

    template<typename I>
    size_t input_index(const I& x)
    {
      if constexpr(FOR_MATLAB)
        return x-1;
      return x;
    }

    template<typename I>
    I output_index(const I& x)
    {
      if constexpr(FOR_MATLAB)
        return x+1;
      return x;
    }
  }
}