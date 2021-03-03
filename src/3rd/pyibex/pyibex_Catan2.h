// File temporarily extracted from pyibex repo (Benoît Desrochers)
// todo: make a clean dependency

//============================================================================
//                               P Y I B E X
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPL v3
// Created     : May 25, 2015
// Last Update : Jun 29, 2017
//============================================================================

#ifndef __IBEX_CTC_ANGLE_H__
#define __IBEX_CTC_ANGLE_H__

#include <codac_IntervalVector.h>
#include <codac_Ctc.h>
#include <tuple>
// using namespace std;

using ibex::bwd_imod;

namespace codac {


// Implements interval modulo with double period:  x = y mod(p)
inline bool bwd_imod2(Interval& x, Interval& y, const Interval& p) {
  Interval z1 = (x - y)/p;
  Interval z2 = integer(z1);
  x &= y + z2*p;
  y &= x - z2*p;
  return true;
}


inline Interval Cmod(const Interval& x, const Interval& y){
  Interval xx(x);
  Interval yy(y);
  bwd_imod2(xx,yy,2*Interval::PI);
  return yy;
}

inline Interval Cmod_bwd(const Interval& x, const Interval& y){
  Interval xx(x);
  Interval yy(y);
  bwd_imod2(xx,yy,(2*Interval::PI));
  return xx;
}



inline std::tuple<Interval, Interval, Interval> Catan2(const Interval&x, const Interval&y, const Interval& th){
  static const Interval iZeroPi2 = Interval(0) | (Interval::HALF_PI);
  static const Interval iPi2Pi = (Interval::HALF_PI) | Interval::PI;
  // static const Interval 2PI_UB = 2*Interval::PI.ub();
  if (x.is_empty() || y.is_empty() || th.is_empty()) {
      return std::make_tuple(Interval::EMPTY_SET, Interval::EMPTY_SET, Interval::EMPTY_SET);
  }
  if ( x.is_subset(Interval::POS_REALS) && y.is_subset(Interval::POS_REALS) && th.is_subset(iZeroPi2) ) {

    if (x == Interval::ZERO && y == Interval::ZERO){
      return std::make_tuple(Interval::EMPTY_SET, Interval::EMPTY_SET, Interval::EMPTY_SET);
    }

    Interval th_tmp = Cmod(th, iZeroPi2);

    // trick to keep tan(th) > 0 because we only consider x >= 0 and y>= 0
    Interval tan_lb = tan(Interval(th_tmp.lb()));
    Interval tan_ub = (th_tmp.ub() == Interval::HALF_PI.ub()) ? Interval(std::numeric_limits<double>::max(), POS_INFINITY)  : tan(Interval(th_tmp.ub()));
    Interval tanTh = tan_lb | tan_ub;
    // Interval tanTh = tan(th_tmp) & Interval(0, POS_INFINITY);

    Interval xx = ( y != Interval::ZERO) ? x & y/tanTh : x;
    Interval yy = ( x != Interval::ZERO) ? y & x*tanTh : y;
    Interval thr =  th_tmp & ( ( x != Interval::ZERO ) ? atan(y/x) : Interval::HALF_PI );
    // cout << "XX " << xx << " " << tanTh << y/tanTh << "\n";
    // cout << "YY " << yy << " " << tanTh << x*tanTh << "\n";
    // cout << "TH " << th << " " <<  thr << " " << th_tmp << atan(y/x) << "\n";
    if (xx.is_empty() || yy.is_empty() || thr.is_empty() ){
      return std::make_tuple(Interval::EMPTY_SET, Interval::EMPTY_SET, Interval::EMPTY_SET);
    }

    return std::make_tuple(xx, yy, thr);
  }

  // Divide into four quadrats  and call contractor
  //  x > 0 and y > 0 and th \in [0, PI/2.]
  Interval x1 = x & Interval::POS_REALS;
  Interval y1 = y & Interval::POS_REALS;
  Interval th1 = Cmod(th,iZeroPi2);
  std::tie(x1, y1, th1) = Catan2(x1, y1, th1);
  Interval th11 = Cmod_bwd(th, th1);
  // bwd_imod(th11, th1, ITV2PI);

  // x > 0, y < 0 , th \in [-PI/2., 0]
  Interval x2 = x & Interval::POS_REALS;
  Interval y2 = y & Interval::NEG_REALS;
  Interval th2 = -Cmod(th, -iZeroPi2);
  std::tie(x2, y2, th2) = Catan2(x2, -y2, th2);
  Interval th22 = Cmod_bwd(th, -th2);
  // bwd_imod(th22, -th2, ITV2PI);

  // x < 0, y < 0 , th \in [-PI, -PI/2.]
  Interval x3 = x & Interval::NEG_REALS;
  Interval y3 = y & Interval::NEG_REALS;
  Interval th3 = Interval::PI + Cmod(th, -iPi2Pi);
  std::tie(x3, y3, th3) = Catan2(-x3, -y3, (th3 &  iZeroPi2) );
  Interval th33 = Cmod_bwd(th, th3 - Interval::PI);
  // bwd_imod(th33, th3 - Interval::PI, ITV2PI);

  // x < 0, y > 0 , th \in [PI/2., PI]
  Interval x4 = x & Interval::NEG_REALS;
  Interval y4 = y & Interval::POS_REALS;
  Interval th4 = Interval::PI - Cmod( th,iPi2Pi);
  std::tie(x4, y4, th4) = Catan2(-x4, y4, ( th4 & iZeroPi2) );
  Interval th44 = Cmod_bwd(th, Interval::PI -  th4);
  // bwd_imod(th44, Interval::PI -  th4, ITV2PI);

  Interval xx =   ( x1 | x2 | (-x3) | (-x4) );
  Interval yy =   ( y1 | (-y2) | (-y3) | (y4) );
  Interval thh =  ( th11 | (th22) | ( th33 ) | ( th44 ) );
  return std::make_tuple(xx,yy,thh);

}


}  // namespace pyibex

#endif // __IBEX_CTC_ANGLE_H__
