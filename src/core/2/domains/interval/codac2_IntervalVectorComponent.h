/** 
 *  \file
 *  
 *  This class reuses many of the functions developed for ibex::IntervalVector. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalVector (IBEX lib, author: G. Chabert)
 * 
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_INTERVALVECTORCOMPONENT_H__
#define __CODAC2_INTERVALVECTORCOMPONENT_H__

#include <codac2_IntervalVector.h>

namespace codac2
{
  template<int N>
  class IntervalVector_;

  template<int N=Dynamic>
  class IntervalVectorComponent_ : public Interval
  {
    public:

      IntervalVectorComponent_(IntervalVector_<N>& x, size_t i)
        : _x(x), _i(i), Interval(_x.i(_i))
      {
        set_name(x.name() + "[" + std::to_string(i) + "]");
      }

      IntervalVectorComponent_<N>& operator=(const IntervalVectorComponent_<N>& xi)
      {
        _x.i(_i) = xi;
        return *this;
      }

      IntervalVectorComponent_<N>& operator=(const Interval& xi)
      {
        _x.i(_i) = xi;
        return *this;
      }

      bool operator==(const IntervalVectorComponent_<N>& xi)
      {
        return &_x == &xi._x && &_i == &xi._i;
      }

      bool operator!=(const IntervalVectorComponent_<N>& xi)
      {
        return &_x != &xi._x || &_i != &xi._i;
      }

      IntervalVectorComponent_<N>& operator&=(const Interval& xi)
      {
        _x.i(_i) &= xi;
        return *this;
      }

      IntervalVectorComponent_<N>& operator|=(const Interval& xi)
      {
        _x.i(_i) |= xi;
        return *this;
      }

      IntervalVectorComponent_<N>& operator+=(const Interval& xi)
      {
        _x.i(_i) += xi;
        return *this;
      }

      IntervalVectorComponent_<N>& operator-=(const Interval& xi)
      {
        _x.i(_i) -= xi;
        return *this;
      }

      IntervalVectorComponent_<N>& operator*=(const Interval& xi)
      {
        _x.i(_i) *= xi;
        return *this;
      }

      IntervalVectorComponent_<N>& operator/=(const Interval& xi)
      {
        _x.i(_i) /= xi;
        return *this;
      }

      /*template<typename T>
      Interval operator+(const T& x) const
      {
        return operator const Interval&() + x;
      }

      template<typename T>
      Interval operator-(const T& x) const
      {
        return operator const Interval&() - x;
      }*/

      Interval& operator()()
      {
        return _x.i(_i);
      }

      /*const Interval& operator()() const
      {
        return _x.i(_i);
      }*/

      /*operator Interval()
      {
        return _x.i(_i);
      }*/

      /*operator Interval&()
      {
        return _x.i(_i);
      }*/

      /*operator const Interval&() const
      {
        return _x.i(_i);
      }*/

      IntervalVectorComponent_<N>& inflate(double r)
      {
        _x.i(_i).inflate(r);
        return *this;
      }

      double volume() const
      {
        return _x.i(_i).diam();
      }

      friend std::ostream& operator<<(std::ostream& os, const IntervalVectorComponent_<N>& p)
      {
        os << p.operator const Interval&();
        return os;
      }

    //protected:

      IntervalVector_<N>& _x;
      const size_t _i;
  };

  using IntervalVectorComponent = IntervalVectorComponent_<>;


  template<int N=Dynamic>
  class IntervalVectorConstComponent_ : public Interval
  {
    public:

      IntervalVectorConstComponent_(const IntervalVector_<N>& x, size_t i)
        : Interval(x.i(i)), _x(x), _i(i)
      {
        set_name(x.name() + "[" + std::to_string(i) + "]");
      }

      IntervalVectorConstComponent_<N>& operator=(const IntervalVectorConstComponent_<N>& xi) = delete;
      IntervalVectorConstComponent_<N>& operator=(const Interval& xi) = delete;

      bool operator==(const IntervalVectorConstComponent_<N>& xi)
      {
        return &_x == &xi._x && &_i == &xi._i;
      }

      bool operator!=(const IntervalVectorConstComponent_<N>& xi)
      {
        return &_x != &xi._x || &_i != &xi._i;
      }

    //protected:

      const IntervalVector_<N>& _x;
      const size_t _i;
  };

  using IntervalVectorConstComponent = IntervalVectorConstComponent_<>;

} // namespace codac

#endif