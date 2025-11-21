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
#include "codac2_matrices.h"
#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "codac2_Row.h"
#include "codac2_IntervalRow.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_Polygon.h"

namespace codac2
{
  #define DEFAULT_EPS std::numeric_limits<double>::epsilon()*10

  template<typename T>
  class Approx
  {
    public:

      explicit Approx(const T& x, double eps = DEFAULT_EPS)
        : _x(x), _eps(eps)
      { }

      template<typename OtherDerived>
      explicit Approx(const Eigen::MatrixBase<OtherDerived>& x, double eps = DEFAULT_EPS)
        : _x(x.eval()), _eps(eps)
      { }

      friend bool operator==(const T& x1, const Approx<T>& x2)
      {
        if constexpr(std::is_same_v<T,double>)
          return std::fabs(x1-x2._x) < x2._eps;

        else if(x1.size() != x2._x.size())
          return false;

        else if(x1 == x2._x)
          return true;

        else if constexpr(std::is_same_v<T,Interval>)
        {
          if((x1.is_empty() && !x2._x.is_empty()) || (!x1.is_empty() && x2._x.is_empty()))
            return false;
          return (x1.lb() == x2._x.lb() || x1.lb() == Approx<double>(x2._x.lb(),x2._eps))
              && (x1.ub() == x2._x.ub() || x1.ub() == Approx<double>(x2._x.ub(),x2._eps));
        }

        else if constexpr(std::is_same_v<T,Vector>
          || std::is_same_v<T,IntervalVector>
          || std::is_same_v<T,Row>
          || std::is_same_v<T,IntervalRow>
          || std::is_same_v<T,Matrix>
          || std::is_same_v<T,IntervalMatrix>)
        {
          for(Index i = 0 ; i < x1.rows() ; i++)
            for(Index j = 0 ; j < x1.cols() ; j++)
              if(!(x1(i,j) == Approx<typename T::Scalar>(x2._x(i,j), x2._eps)))
                return false;
          return true;
        }

        else
        {
          assert_release_constexpr(false && "Approx::operator== unhandled case");
          return false;
        }
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
  
  template<>
  class Approx<Segment>
  {
    public:

      explicit Approx(const Segment& x, double eps = DEFAULT_EPS)
        : _x(x), _eps(eps)
      { }

      friend bool operator==(const Segment& p1, const Approx<Segment>& p2)
      {
        return (Approx<IntervalVector>(p1[0], p2._eps) == p2._x[0] && Approx<IntervalVector>(p1[1], p2._eps) == p2._x[1])
          || (Approx<IntervalVector>(p1[1], p2._eps) == p2._x[0] && Approx<IntervalVector>(p1[0], p2._eps) == p2._x[1]);
      }

      friend std::ostream& operator<<(std::ostream& os, const Approx<Segment>& x)
      {
        os << "Approx(" << x._x << ")";
        return os;
      }
    
    private:

      const Segment _x;
      const double _eps;
  };
  
  template<typename P>
    requires std::is_base_of_v<Polygon,P>
  class Approx<P>
  {
    public:

      explicit Approx(const P& x, double eps = DEFAULT_EPS)
        : _x(x), _eps(eps)
      { }

      friend bool operator==(const P& p1, const Approx<P>& p2)
      {
        size_t n = p1.edges().size();
        if(p2._x.edges().size() != n)
          return false;

        size_t i; // looking for same reference of first value
        for(i = 0 ; i < n ; i++)
          if(Approx<Segment>(p1.edges()[0], p2._eps) == p2._x.edges()[i])
            break;

        int way = 1;
        if(n > 1)
          way = (Approx<Segment>(p1.edges()[1], p2._eps) == p2._x.edges()[(i+1)%n]) ? 1 : -1;

        for(size_t j = 0 ; j < n ; j++)
          if(Approx<Segment>(p1.edges()[j], p2._eps) != p2._x.edges()[(i+way*j+2*n)%n])
            return false;

        return true;
      }

      friend std::ostream& operator<<(std::ostream& os, const Approx<P>& x)
      {
        os << "Approx(" << x._x << ")";
        return os;
      }
    
    private:

      const P _x;
      const double _eps;
  };
  
  template<typename T>
  class Approx<std::pair<T,T>>
  {
    public:

      explicit Approx(const std::pair<T,T>& x, double eps = DEFAULT_EPS)
        : _x(x), _eps(eps)
      { }

      friend bool operator==(const std::pair<T,T>& p1, const Approx<std::pair<T,T>>& p2)
      {
        return Approx<T>(p2._x.first,p2._eps) == p1.first && Approx<T>(p2._x.second,p2._eps) == p1.second;
      }

      friend std::ostream& operator<<(std::ostream& os, const Approx<std::pair<T,T>>& x)
      {
        os << "Approx(" << x._x.first << "," << x._x.second << ")";
        return os;
      }
    
    private:

      const std::pair<T,T> _x;
      const double _eps;
  };

  Approx(const Polygon&, double) -> 
    Approx<Polygon>;
}