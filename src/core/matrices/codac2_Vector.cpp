/** 
 *  codac2_Vector.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Vector.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  Vector::Vector(size_t n)
    : Vector(n,0.)
  {
    assert_release(n >= 0);
  }

  Vector::Vector(size_t n, double x)
    : MatrixBase<Vector,double>(n,1,x),
      VectorBase<Vector,Matrix,double>(n)
  {
    assert_release(n >= 0);
  }

  Vector::Vector(std::initializer_list<double> l)
    : MatrixBase<Vector,double>(l.size(),1),
      VectorBase<Vector,Matrix,double>(l)
  {
    assert_release(!std::empty(l));
  }

  Vector::Vector(const std::vector<double>& l)
    : MatrixBase<Vector,double>(l.size(),1),
      VectorBase<Vector,Matrix,double>(l)
  {
    assert_release(!std::empty(l));
  }

  Vector::Vector(const MatrixBase<Vector,double>& x)
    : Vector(x._e)
  { }

  size_t Vector::min_coeff_index() const
  {
    size_t r,c;
    this->_e.minCoeff(&r,&c);
    assert(c == 0);
    return r;
  }

  size_t Vector::max_coeff_index() const
  {
    size_t r,c;
    this->_e.maxCoeff(&r,&c);
    assert(c == 0);
    return r;
  }

  std::ostream& operator<<(std::ostream& os, const Vector& x)
  {
    os << "(";
    for(size_t i = 0 ; i < x.size() ; i++)
      os << x[i] << (i<x.size()-1 ? " ; " : "");
    os << ")";
    return os;
  }
}