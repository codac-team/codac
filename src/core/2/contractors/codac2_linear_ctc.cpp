/** 
 *  Contractors for linear systems of equations
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_linear_ctc.h"

using namespace std;
using namespace codac2;

void CtcGaussElim::contract(IntervalMatrix& A, IntervalVector& x, IntervalVector& b)
{
  assert(A.rows() == A.cols() && A.rows() == x.size() && A.rows() == b.size());

  IntervalMatrix A_ = A;
  IntervalVector b_ = b;

  size_t n = A_.rows();
  for(size_t i = 0 ; i < n ; i++)
    if(A_(i,i).contains(0.))
      return;

  for(size_t i = 0 ; i < n-1 ; i++)
    for(size_t j = i+1 ; j < n ; j++)
    {
      Interval aj = A_(j,i)/A_(i,i);
      b_[j] -= aj*b_[i];
      for(size_t k = i+1 ; k < n ; k++)
        A_(j,k) -= aj*A_(i,k);
    }

  for(int i = n-1 ; i >= 0 ; i--)
  {
    Interval sum = 0.;
    for(size_t j = i+1 ; j < n ; j++)
      sum += A_(i,j)*x[j];
    x[i] &= (b_[i]-sum)/A_(i,i);
  }
}

void CtcGaussSeidel::contract(IntervalMatrix& A, IntervalVector& x, IntervalVector& b)
{
  assert(A.rows() == A.cols() && A.rows() == x.size() && A.rows() == b.size());

  auto ext_diag = A;
  for(size_t i = 0 ; i < A.rows() ; i++)
    ext_diag(i,i) = 0.;
  x &= A.diagonal_matrix().inverse().toDenseMatrix()*(b-ext_diag*x);
}