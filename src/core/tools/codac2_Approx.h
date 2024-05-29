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
        : _x(x ), _eps(T(x).init(std::numeric_limits<double>::epsilon()*10))
      { }

      explicit Approx(const T& x, double eps)
        : _x(x ), _eps(T(x).init(eps))
      { }

      friend bool operator==(const T& x1, const Approx<T>& x2)
      {
        return x1 == x2._x
          || (fabs(x1.lb()-x2._x.lb()) < x2._eps.ub() && x1.ub() == x2._x.ub())
          || (fabs(x1.ub()-x2._x.ub()) < x2._eps.ub() && x1.lb() == x2._x.lb())
          || (fabs(x1.lb()-x2._x.lb()) < x2._eps.ub() && fabs(x1.ub()-x2._x.ub()) < x2._eps.ub() && x1.lb());
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
      const T _eps;
  };
  
}