/** 
 *  codac2_arith_mul.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_arith_mul.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  void MulOp::bwd(const Interval& y, IntervalRow& x1, IntervalVector& x2)
  {
    assert(x1.size() == x2.size());

    const Index n = x1.size();
    std::vector<Interval> sums(n), prods(n);

    // Forward propagation

      for(Index i = 0 ; i < n ; i++)
      {
        prods[i] = x1[i]*x2[i];
        sums[i] = prods[i];
        if(i > 0) sums[i] += sums[i-1];
      }

    // Backward propagation

      sums[n-1] &= y;

      for(Index i = n-1 ; i >= 0 ; i--)
      {
        if(i > 0) AddOp::bwd(sums[i],sums[i-1],prods[i]);
        else prods[0] &= sums[0];
        MulOp::bwd(prods[i],x1[i],x2[i]);
      }
  }

  void MulOp::bwd(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2)
  {
    assert(x1.rows() == y.size());
    assert(x1.cols() == x2.size());

    /*if(x1.is_squared()) // not working for any squared x1
    {
      CtcGaussElim ctc_ge;
      CtcLinearPrecond ctc_gep(ctc_ge);
      IntervalVector y_(y);
      ctc_gep.contract(x1,x2,y_);
    }*/

    if(x1.rows() > x1.cols())
    {
      #if 0

        Index last_row = 0;
        Index i = 0;

        do
        {
          double vol_x2 = x2.volume();
          IntervalRow row_i = x1.row(i);
          MulOp::bwd(y[i],row_i,x2);

          if(row_i.is_empty())
          {
            x1.set_empty();
            return;
          }

          else
            x1.row(i) = row_i;

          if(x2.volume()/vol_x2 < 0.98)
            last_row = i;
          i = (i+1)%y.size();
        } while(i != last_row);

      #else

        // Interval FullPiv attempt, could be used for any possibility.
        // Only contracts x2.
        IntvFullPivLU LUdec(x1);
        IntervalMatrix copy_x2(x2);
        LUdec.solve(y,copy_x2);
        x2 = copy_x2.col(0);

      #endif
    }

    else
    {
      #if 0

        IntvFullPivLU LUdec(x1);
        IntervalMatrix copy_x2(x2);
        LUdec.solve(y,copy_x2);
        x2 = copy_x2.col(0);

      #else

        // "Tree matrix" attempt, works well in example.
        // Only contract x2.
        IntervalMatrix P = gauss_jordan(x1.mid());
        IntervalVector b_tilde = P*y;
        IntervalMatrix A_tilde = P*x1;
        // A_tilde should be a tree matrix
            
        #if 1

          for(Index i = b_tilde.size()-1 ; i>=0 ; i--)
          {
            IntervalRow R = A_tilde.row(i);
            MulOp::bwd(b_tilde[i], R, x2);
            if(x2.is_empty()) { x1.set_empty(); x2.set_empty(); return; }
          }

          for(Index i=0 ; i<b_tilde.size() ; i++)
          {
            IntervalRow R = A_tilde.row(i);
            MulOp::bwd(b_tilde[i], R, x2);
            if(x2.is_empty()) { x1.set_empty(); x2.set_empty(); return; }
          }

        #else

          bool contraction;
          do
          {
            contraction = false;

            for(Index i = 0 ; i < x2.size() ; i++)
            {
              for(Index k = 0 ; k < b_tilde.size() ; k++)
              {
                Interval u = b_tilde[k];

                for(Index j = 0 ; j < x2.size() ; j++)
                  if(i != j)
                    u -= x2[j]*A_tilde(k,j);

                Interval x2i_old = x2[i];
                MulOp::bwd(u, x2[i], A_tilde(k,i));
                contraction |= x2i_old != x2[i];
              }
            }

          } while(contraction);

        #endif
      #endif
    }

    // Basic contraction on x1
    for(Index i=0 ; i<x1.rows() ; i++)
    {
      IntervalRow R = x1.row(i);
      MulOp::bwd(y[i], R, x2);
      if(R.is_empty())
      {
        x1.set_empty();
        return;
      }
      x1.row(i) = R;
    }
  }
}
