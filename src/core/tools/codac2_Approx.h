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
#include "codac2_IntervalVector.h"

namespace codac2
{
  // The following functions are used for templates
  inline double _lb(const Interval& x) { return x.lb(); }
  inline double _lb(const double& x)   { return x;      }
  inline double _ub(const Interval& x) { return x.ub(); }
  inline double _ub(const double& x)   { return x;      }

  template<typename T>
  class Approx
  {
    public:

      explicit Approx(const T& x, double eps = std::numeric_limits<double>::epsilon()*10)
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
          return ((std::fabs(_lb(x1)-_lb(x2._x)) < x2._eps) && _ub(x1) == _ub(x2._x))
            || ((std::fabs(_ub(x1)-_ub(x2._x)) < x2._eps) && _lb(x1) == _lb(x2._x))
            || ((std::fabs(_lb(x1)-_lb(x2._x)) < x2._eps) && std::fabs(_ub(x1)-_ub(x2._x)) < x2._eps);
        }

        else
        {
          if constexpr(std::is_same_v<T,IntervalVector> || std::is_same_v<T,Vector>)
          {
            for(size_t i = 0 ; i < x1.size() ; i++)
              if(!(((std::fabs(_lb(x1[i])-_lb(x2._x[i])) < x2._eps) && _ub(x1[i]) == _ub(x2._x[i]))
                || ((std::fabs(_ub(x1[i])-_ub(x2._x[i])) < x2._eps) && _lb(x1[i]) == _lb(x2._x[i]))
                || ((std::fabs(_lb(x1[i])-_lb(x2._x[i])) < x2._eps) && std::fabs(_ub(x1[i])-_ub(x2._x[i])) < x2._eps)))
                return false;
          }

          else
          {
            for(size_t i = 0 ; i < x1.nb_rows() ; i++)
              for(size_t j = 0 ; j < x1.nb_cols() ; j++)
                if(!(((std::fabs(_lb(x1(i,j))-_lb(x2._x(i,j))) < x2._eps) && _ub(x1(i,j)) == _ub(x2._x(i,j)))
                  || ((std::fabs(_ub(x1(i,j))-_ub(x2._x(i,j))) < x2._eps) && _lb(x1(i,j)) == _lb(x2._x(i,j)))
                  || ((std::fabs(_lb(x1(i,j))-_lb(x2._x(i,j))) < x2._eps) && std::fabs(_ub(x1(i,j))-_ub(x2._x(i,j))) < x2._eps)))
                  return false;
          }

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