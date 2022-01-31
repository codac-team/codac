//============================================================================
//                               P Y I B E X
// File        : Polar separator with projection on the XY-plan
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Jun 29, 2017
//============================================================================

#ifdef WIN32
#define _USE_MATH_DEFINES
#include <cmath>
#endif
#include "pyibex_SepDiskExists.h"

namespace pyibex {


SepDiskExists::SepDiskExists(Interval x0, Interval y0, Interval rho) : x0(x0), y0(y0), rho(rho), Sep(2) {

}


//----------------------------------------------------------------------
void Cstep(Interval& Y, Interval& X)
{
  Y &=Interval(0,1);

  if (!Y.is_empty()){
        Interval Yh, Yb, Xg, Xd;
        Yh= Y & Interval(1); Yb= Y & Interval(0);
        Xg= X & Interval::NEG_REALS; Xd = X & Interval::POS_REALS;
        if (Yh.is_empty()) Xd &= Interval::NEG_REALS;
        if (Yb.is_empty()) Xg &= Interval::POS_REALS;
        X &= Xg | Xd;
    }
      else X.set_empty();
}


Interval Step(const Interval& X)
{ if (X.is_empty()) return Interval::EMPTY_SET;
    if (X.lb()>0) return (Interval(1));
    if (X.ub()<0) return(Interval(0));
    return (Interval(0,1));
}

void SepDiskExists::contract(IntervalVector &box, bool outer){
     Interval P3inf2=rho.lb()*rho.lb();
    Interval P3sup2=rho.ub()*rho.ub();

     /* step 1  */ Interval A1=box[0]-x0.lb();
     /* step 2  */ Interval B1=box[0]-x0.ub();
     /* step 3  */ Interval A2=box[1]-y0.lb();
     /* step 4  */ Interval B2=box[1]-y0.ub();
     /* step 5  */ Interval A12=ibex::sqr(A1);
     /* step 6  */ Interval A22=ibex::sqr(A2);
     /* step 7  */ Interval B12=ibex::sqr(B1);
     /* step 8  */ Interval B22=ibex::sqr(B2);
     /* step 9  */ Interval B1A1=B1*A1;
     /* step 10 */ Interval MinB12A12=ibex::min(B12,A12);
     /* step 11 */ Interval B2A2 = B2*A2;
     /* step 12 */ Interval MinB22A22= ibex::min(B22,A22);
     /* step 13 */ Interval MaxB12A12 = ibex::max(B12,A12);
     /* step 14 */ Interval MaxB22A22 = ibex::max(B22,A22);
    //  /* step 15 */ Interval S11 = Step(B1A1);
    //  /* step 16 */ Interval S22 = Step(B2A2);
     /* step 15 */ Interval S11 = ibex::chi(B1A1, Interval(0), Interval(1));
     /* step 16 */ Interval S22 = ibex::chi(B2A2, Interval(0), Interval(1));
     /* step 17 */ Interval Z11 = S11*MinB12A12;
     /* step 18 */ Interval Z22 = S22*MinB22A22;
     /* step 19 */ Interval Z1 = Z11 + Z22;
     /* step 20 */ Interval Z2 = MaxB12A12 + MaxB22A22;
     /* step 21 */ Interval D1=ibex::min(Z2,P3sup2);
     /* step 22 */ Interval D2=ibex::max(Z1,P3inf2);
     Interval R;
     R = (outer == true) ? Interval::POS_REALS : Interval::NEG_REALS;
     R &= D1 - D2;
    //  D1 &= D2+R;
    //  D2 &= D1-R;
     /* step 23  */ ibex::bwd_sub(R,D1,D2);
    std::cerr  << R << " " << D1  << " " << D2 << "\n";

     ibex::bwd_sub(R,D1,D2);                    /* step 23 */
     ibex::bwd_max(D2,Z1,P3inf2);                  /* step 22 */ //D2=Max(Z1,P3inf2);
     ibex::bwd_min(D1,Z2,P3sup2);                  /* step 21 */ // D1=Min(Z2,P3sup2);
     ibex::bwd_add(Z2, MaxB12A12, MaxB22A22);    /* step 20 */ // Z2 = MaxB12A12 + MaxB22A22;
     ibex::bwd_add(Z1,Z11,Z22);                  /* step 19 */ // Z1 = Z11 + Z22;
     ibex::bwd_mul(Z22,S22,MinB22A22);           /* step 18 */ // Z22 = S22*MinB22A22;
     ibex::bwd_mul(Z11,S11,MinB12A12);           /* step 17 */ // Z11 = S11*MinB12A12;
    //  Cstep(S22,B2A2);                             /* step 16 */ // S22 = Step(B2A2);
    //  Cstep(S11,B1A1);                             /* step 15 */ // S11 = Step(B1A1);
     Interval tmp10(0), tmp11(1), tmp20(0), tmp21(1);
    //  ibex::bwd_chi(S22,B2A2, tmp10, tmp11);
    //  ibex::bwd_chi(S11,B1A1, tmp20, tmp21);                             /* step 15 */ // S11 = Step(B1A1);
     ibex::bwd_sign(S22,B2A2);
     ibex::bwd_sign(S11,B1A1);                             /* step 15 */ // S11 = Step(B1A1);
     ibex::bwd_max(MaxB22A22,B22, A22);           /* step 14 */ // MaxB22A22 = Max(B22,A22);
     ibex::bwd_max(MaxB12A12,B12, A12);           /* step 13 */ // MaxB12A12 = Max(B12,A12);
     ibex::bwd_min(MinB22A22,B22,A22);             /* step 12 */ // MinB22A22= Min(B22,A22);
     ibex::bwd_mul(B2A2,B2,A2);                  /* step 11 */ // B2A2 = B2*A2;
     ibex::bwd_min(MinB12A12,B12,A12);             /* step 10 */ // MinB12A12=Min(B12,A12);
     ibex::bwd_mul(B1A1,B1,A1);                  /* step 9  */ // B1A1=B1*A1;
     ibex::bwd_sqr(B22,B2);                        /* step 8  */ // B22=sqr(B2);
     ibex::bwd_sqr(B12,B1);                        /* step 7  */ // B12=sqr(B1);
     ibex::bwd_sqr(A22,A2);                        /* step 6  */ // A22=sqr(A2);
     ibex::bwd_sqr(A12,A1);                        /* step 5  */ // A12=sqr(A1);
     box[1] &= B2 + y0.ub();               /* step 4  */ // B2=Y-y0.ub();
     box[1] &= A2 + y0.lb();               /* step 3  */ // A2=Y-y0.lb(;
     box[0] &= B1 + x0.ub();               /* step 2  */ // B1=X-x0.ub();
     box[0] &= A1 + x0.lb();               /* step 1  */ // A1=X-x0.lb(;
}

void SepDiskExists::separate(IntervalVector& x_in, IntervalVector& x_out){

  assert(x_out.size() == 2);
  assert(x_in.size() == 2);
  x_out &= x_in;
  x_in &= x_out;

  contract(x_out, true);
  contract(x_in, false);
}

}; // end namespace pyibex
