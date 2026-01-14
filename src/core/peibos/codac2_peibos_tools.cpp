/** 
 *  codac2_peibos_tools.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_peibos_tools.h"
#include "codac2_template_tools.h"
#include "codac2_inversion.h"
#include "codac2_IntvFullPivLU.h"

using namespace codac2;

namespace codac2
{
  double split (const IntervalVector& X, double eps, std::vector<IntervalVector>& boxes)
  {
    if (X.max_diam()<=eps)
    {
      boxes.push_back(X);
      return X.max_diam();
    }
    else
    {
      auto p = X.bisect_largest(0.5);
      double diam1 = split(p.first,eps,boxes);
      double diam2 = split (p.second,eps,boxes);
      return std::max(diam1,diam2);
    }
  }

  double error_peibos(const IntervalVector& Y, const Vector& z, const IntervalMatrix& Jf, const Matrix& A, const IntervalVector& X)
  {
    Vector xc = X.mid();

    IntervalVector dX = X-xc;

    IntervalVector E = (Y - z) + (Jf - A)*dX;

    return E.norm().ub();
  }

  double condition_number(const Matrix& A) 
  {
    Eigen::JacobiSVD<Matrix> svd(A);
    double sigma_max = svd.singularValues()(0);
    double sigma_min = svd.singularValues()(svd.singularValues().size() - 1);
    return sigma_max / sigma_min;
  }

  Matrix inflate_flat_parallelepiped(const Matrix& A, const Vector& e_vec, double rho)
  {
    Index m = A.cols();
    Index n = A.rows();

    IntvFullPivLU LUdec((Matrix) A.transpose());
    IntervalMatrix N = LUdec.kernel();

    std::vector<int> col_indices;

    for (int i = 0; i < m; i++)
      if (! (A.col(i).isZero()) )
        col_indices.push_back(i);

    // A_reduced is composed of the non empty vectors of A

    Matrix A_reduced (n, col_indices.size());
    for (int i = 0; i < (int) col_indices.size(); i++)
      A_reduced.col(i) = A.col(col_indices[i]);

    // Construct the new matrix A_tild = [A_reduced | N]
    Matrix A_tild (n,n);
    A_tild << A_reduced,N.mid();

    IntervalMatrix Q = inverse_enclosure(A_tild.transpose() * A_tild);

    IntervalMatrix mult = IntervalMatrix::zero(n,n);
    for (int i = 0; i < n; i++)
      mult(i,i) = rho*sqrt(Q(i,i));

    for (int i = 0; i < m; i++)
      mult(i,i) += e_vec(i);

    return ((A_tild.template cast<Interval>()) * mult).smag();
  }

}