/** 
 *  codac2_GaussJordan.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Luc Jaulin, Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <iostream>
#include "codac2_GaussJordan.h"

using namespace std;

namespace codac2
{
  Matrix rising(const Matrix& R_, const Matrix& U_, const Matrix& A)
  {
    Matrix R = R_, U = U_;
    Index n = A.rows(), m = A.cols();
    Index p = m-n;

    for(Index i = n-1 ; i > 0 ; i--)
    {
      Matrix K = U(i,i+p)*Matrix::Identity(n,n);
      K.block(0,i,i,1) = -U.block(0,i+p,i,1);
      R = K*R;
      U = R*A;
    }

    return R;
  }

  Matrix precond(const Matrix& P, const Matrix& L, const Matrix& U)
  {
    auto P_inv = P.inverse();
    Matrix A = P_inv*(L*U);
    Matrix R = (P_inv*L).inverse();
    return rising(R,U,A);
  }

  Matrix gauss_jordan(const Matrix& A)
  {
    Index n = A.rows(), m = A.cols();
    Eigen::FullPivLU<Matrix> lu(A);

    Matrix L = Matrix::Identity(n,n);
    if(std::pow(L.determinant(),2) < 1e-5)
    {
      cout << "[Matrix gauss_jordan(const Matrix& A)] -> eye Matrix" << endl;
      return Matrix::eye(n,n);
    }

    L.block(0,0,n,std::min(m,n)).triangularView<Eigen::StrictlyLower>() = 
        lu.matrixLU().block(0,0,n,std::min(m,n));

    Matrix P = lu.permutationP();
    Matrix U = lu.matrixLU().triangularView<Eigen::Upper>();

    return precond(P,L,U);
  }
}