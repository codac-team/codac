/** 
 *  \file codac2_Approx.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <limits>

namespace codac2
{
  template<typename T>
  class Approx
  {
    public:

      explicit Approx(const T& x)
        : _x(x ), _eps(std::numeric_limits<double>::epsilon()*10)
      { }

      explicit Approx(const T& x, double eps)
        : _x(x ), _eps(eps)
      { }

      friend bool operator==(const T& x1, const Approx<T>& x2)
      {
        if(x1.size() != x2._x.size())
          return false;
        if(x1 == x2._x)
          return true;

        if constexpr(std::is_same_v<T,Interval>)
        {
          return ((std::fabs(x1.lb()-x2._x.lb()) < x2._eps) && x1.ub() == x2._x.ub())
            || ((std::fabs(x1.ub()-x2._x.ub()) < x2._eps) && x1.lb() == x2._x.lb())
            || ((std::fabs(x1.lb()-x2._x.lb()) < x2._eps) && std::fabs(x1.ub()-x2._x.ub()) < x2._eps);
        }

        else
        {
          for(size_t i = 0 ; i < x1.size() ; i++)
            if(!(((std::fabs(x1[i].lb()-x2._x[i].lb()) < x2._eps) && x1[i].ub() == x2._x[i].ub())
              || ((std::fabs(x1[i].ub()-x2._x[i].ub()) < x2._eps) && x1[i].lb() == x2._x[i].lb())
              || ((std::fabs(x1[i].lb()-x2._x[i].lb()) < x2._eps) && std::fabs(x1[i].ub()-x2._x[i].ub()) < x2._eps)))
              return false;
          return true;
        }
      }

      friend bool operator==(const Approx<T>& x1, const T& x2)
      {
        return operator==(x2, x1);
      }

      friend bool operator!=(const T& x1, const Approx<T>& x2)
      {
        return !operator==(x1, x2);
      }

      friend bool operator!=(const Approx<T>& x1, const T& x2)
      {
        return !operator==(x2, x1);
      }

      friend std::ostream& operator<<(std::ostream& os, const Approx<T>& x)
      {
        os << "Approx(" << x._x << ")";
        return os;
      }

    private:

      const T _x;
      const double _eps;
  };
  
}