/** 
 *  \file codac2_inversion.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <ostream>
#include "codac2_inversion.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalMatrix.h"

using namespace std;

namespace codac2
{
  IntervalMatrix infinite_sum_enclosure(const IntervalMatrix& A, double &mrad) {
      assert_release(A.is_squared());
      Index N = A.rows();

      Matrix B = A.mag();
      
      // modified Floyd algorithm
      for (Index k=0;k<N;k++) {
         for (Index r=0;r<N;r++) {
            if (r==k) continue;
            if (B(k,r)>0.0 && B(k,r)<B(k,k)) B(k,r)=B(k,k);
            if (B(r,k)>0.0 && B(r,k)<B(k,k)) B(r,k)=B(k,k);
         } 
         for (Index c=0;c<N;c++) {
            if (c==k) continue;
	    if (B(k,c)==0.0) continue;
            for (Index r=0;r<N;r++) {
                if (r==k) continue;
                if (B(r,k)==0.0) continue;
                if (B(r,c)<B(r,k)) B(r,c)=B(r,k);
                if (B(r,c)<B(k,c)) B(r,c)=B(k,c);
            }
         }
      }

      IntervalMatrix res(N,N);
      Interval v(-1.0,1.0);
      mrad=0.0;
      for (Index c=0;c<N;c++) {
         for (Index r=0;r<N;r++) {
            Interval sum=0.0;
            for (Index k=0;k<N;k++) {
               sum += std::min(B(r,k),B(k,c));
            }
            if (sum==0.0) {
               res(r,c)=A(r,c);
            } else if (sum.ub()>=1.0) {
               mrad=oo;
               res(r,c)=Interval(); 
            } else {
               Interval sumprod=1.0/(1.0-sum);
               if (sumprod.ub()>mrad) mrad=sumprod.ub();
               res(r,c)=(v*B(r,c))*sumprod;
            }
         }
      }
      return res;
  }
}
