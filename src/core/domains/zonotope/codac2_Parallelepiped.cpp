/** 
 *  codac2_Parallelepiped.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Parallelepiped.h"
#include "codac2_inversion.h"

using namespace std;

namespace codac2
{
  Parallelepiped::Parallelepiped(const Vector& c_, const Matrix& A_)
    : Zonotope(c_, A_)
  {
    assert_release(A.cols() <= c.size() && "too many vectors, you are describing a zonotope");
  }

  Parallelepiped Parallelepiped::empty(Index n)
  {
    Parallelepiped p(Vector::Constant(n, std::numeric_limits<double>::quiet_NaN()), Matrix::Identity(n,n));
    p.empty_flag = true;
    return p;
  }

  void generate_vertices(Index i, Index n, const Vector& c, const Matrix& A, std::vector<Vector>& L_v)
  {
    if (i == n)
    {
      L_v.push_back(c);
    }
    else if (i<n)
    {
      generate_vertices(i+1, n, c + A.col(i), A, L_v);
      generate_vertices(i+1, n, c - A.col(i), A, L_v);
    }
  }

  std::vector<Vector> Parallelepiped::vertices() const
  {
    std::vector<Vector> L_v;
    generate_vertices(0, size(),c,A,L_v);
    return L_v;
  }

  BoolInterval Parallelepiped::contains(const Vector& v) const
  {
    return is_superset(v.template cast<Interval>());
  }

  BoolInterval Parallelepiped::is_superset(const IntervalVector& x) const
  {
    assert_release(A.rows() == A.cols() && "Matrix A must be square to check containment.");
    assert_release(x.size() == size() && "Point dimension must match parallelepiped dimension.");

    IntervalVector B = inverse_enclosure(A)*(x - c);
    IntervalVector IV = IntervalVector::constant(A.cols(),{-1,1});

    if (!(B.intersects(IV)))
      return BoolInterval::FALSE;

    else
    {
      if (B.is_subset(IV))
        return BoolInterval::TRUE;
      else
        return BoolInterval::UNKNOWN;
    }
  }

  Parallelepiped Parallelepiped::operator&(const IntervalVector& x) const
  {
    assert_release(A.rows() == A.cols() && "Matrix A must be square to compute intersection.");
    assert_release(x.size() == size() && "Box dimension must match parallelepiped dimension.");

    if (is_empty() || x.is_empty())
      return Parallelepiped::empty(size());

    IntervalVector x_p = inverse_enclosure(A)*(x-c);
    x_p &= IntervalVector::constant(c.size(),Interval(-1,1));

    if (x_p.is_empty())
      return Parallelepiped::empty(size());

    Vector c_i = c + A*x_p.mid();

    Matrix A_i (A);
    for (int i = 0; i < c.size(); ++i)
      A_i.col(i) *= x_p[i].rad();

    return Parallelepiped(c_i,A_i);
  }

  IntervalVector operator&(const IntervalVector& x, const Parallelepiped& p)
  {
    assert_release(p.A.rows() == p.A.cols() && "Matrix A must be square to compute intersection.");
    assert_release(x.size() == p.size() && "Box dimension must match parallelepiped dimension.");

    if (p.is_empty() || x.is_empty())
      return IntervalVector::empty(x.size());

    IntervalVector x_p = inverse_enclosure(p.A)*(x-p.c);
    x_p &= IntervalVector::constant(p.c.size(),Interval(-1,1));

    if (x_p.is_empty())
      return IntervalVector::empty(x.size());

    IntervalVector x_i = p.c + p.A*x_p;
    return x_i & x;
  }
}
