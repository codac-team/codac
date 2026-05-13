/** 
 *  codac2_Zonotope.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Zonotope.h"

namespace codac2
{
  Zonotope::Zonotope(const Vector& c_, const Matrix& A_)
    : c(c_), A(A_)
  {
    assert_release(c.size() == A.rows());
  }

  Zonotope Zonotope::empty(Index n)
  {
    Zonotope z(Vector::Constant(n, std::numeric_limits<double>::quiet_NaN()), Matrix::Identity(n,n));
    z.empty_flag = true;
    return z;
  }

  bool Zonotope::is_empty() const
  {
    return empty_flag;
  }

  void Zonotope::set_empty()
  {
    empty_flag = true;
    c = Vector::Constant(c.size(), std::numeric_limits<double>::quiet_NaN());
  }

  IntervalVector Zonotope::box() const
  {
    return c + (A.template cast<Interval>())*IntervalVector::constant(A.cols(),{-1,1});
  }

  Zonotope Zonotope::proj(const std::vector<Index>& indices) const
  {
    assert_release(*std::min_element(indices.begin(), indices.end()) >= 0 && "indices out of range");
    assert_release(*std::max_element(indices.begin(), indices.end()) <= c.size() && "indices out of range");

    Matrix A_cropped (indices.size(), A.cols());
    Vector c_cropped (indices.size());

    for (size_t i = 0; i < indices.size(); ++i)
    {
      A_cropped.row(i) = A.row(indices[i]);
      c_cropped[i] = c[indices[i]];
    }

    return Zonotope(c_cropped, A_cropped);
  }

  Zonotope Zonotope::operator+(const Zonotope& zonotope)
  {
    assert_release(c.size() == zonotope.c.size() && "Zonotopes must have the same dimension");

    Vector c_sum = c + zonotope.c;
    Matrix A_sum (A.rows(), A.cols() + zonotope.A.cols());

    A_sum << A, zonotope.A;

    return Zonotope(c_sum, A_sum);
  }

  std::ostream& operator<<(std::ostream& str, const Zonotope& z)
  {
    str << "{ ";
    
    if (z.is_empty())
      str << "empty " << (z.size()) << "-d zonotope";
    else
      str << "Zonotope center: \n" << z.c.transpose() << "\nZonotope shape matrix: \n" << z.A;

    str << " }";
    return str;
  }
}
