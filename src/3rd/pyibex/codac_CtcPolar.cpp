// File temporarily extracted from pyibex repo (Benoît Desrochers)
// todo: make a clean dependency

//============================================================================
//                                 P Y I B E X
// Authors      : Mohamed Saad Ibn Seddik, Benoit Desrochers
//       Inspired from the work of  Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPL v3
// Created     : May 24, 2015
// Last Update : May 24, 2015
//============================================================================

#include "codac_CtcPolar.h"
#include "codac_Catan2.h"

namespace codac {

CtcPolar::CtcPolar() : Ctc(4) {}

CtcPolar::~CtcPolar() {}

// contract x,y,rho,theta wrt the polar constraints
// return false if the result is an empty set
bool _contract(Interval &x, Interval& y,
                        Interval& rho, Interval& theta){
  // const double d2PI   = (2*Interval::PI).ub();

  // Interval theta_tmp = atan2(y, x);
  // bwd_imod(theta_tmp, theta, d2PI);
  // bwd_angle(theta, y, x);

  std::tie(x,   y, theta) = Catan2(x, y, theta);


  Interval a1 = sqr(x);
  Interval a2 = sqr(y);
  Interval a3 = a1 + a2;
  Interval a4 = sqr(rho);

  a4 &= a3;
  a3 &= a4;

  bwd_sqr(a4, rho);
  bwd_add(a3, a2, a1);
  bwd_sqr(a1, x);
  bwd_sqr(a2, y);

  Interval x1 = rho*cos(theta);
  Interval y1 = rho*sin(theta);
  x &= x1;
  y &= y1;
  if(x.is_empty() || y.is_empty() || rho.is_empty() || theta.is_empty()){
    x.set_empty(); y.set_empty();
    rho.set_empty(); theta.set_empty();
    return false;
  }
  return true;
}

void CtcPolar::contract(Interval &x, Interval& y,
                        Interval& rho, Interval& theta){
  // std::cout << rho << " -- " ;
  _contract(x,y,rho, theta);
  _contract(x,y,rho, theta);
  // std::cout << rho << "\n";

}

IntervalVector CtcPolar::RTfromXY(Interval x, Interval y){

  Interval rho(0, POS_INFINITY);
  Interval theta(-Interval::PI.lb(), Interval::PI.lb());
  contract(x, y, rho, theta);
  IntervalVector res(2);
  res[0] = rho; res[1] = theta;
  return res;
}


void CtcPolar::contract(IntervalVector &box) {
  _contract(box[0], box[1], box[2], box[3]);
  _contract(box[0], box[1], box[2], box[3]);
}

void CtcPolarXY::contract(IntervalVector& box){
  // rho, theta copy
  Interval rho_tmp(rho), theta_tmp(theta);
  _contract(box[0], box[1], rho_tmp, theta_tmp);
  _contract(box[0], box[1], rho_tmp, theta_tmp);
}

void CtcPolarXY_landmark::contract(IntervalVector& box){
  // rho, theta copy
  Interval rho_tmp(rho), theta_tmp(theta);
  Interval x_tmp(mx - box[0]);
  Interval y_tmp(my - box[1]);
  _contract(x_tmp, y_tmp, rho_tmp, theta_tmp);
  if ( x_tmp.is_empty() ){
    box.set_empty();
    return;
  }
  _contract(x_tmp, y_tmp, rho_tmp, theta_tmp);
  box[0] &= (mx - x_tmp);
  box[1] &= (my - y_tmp);
  if (box[0].is_empty() || box[1].is_empty()){
    box.set_empty();
  }

}

void CtcPolarXYT_landmark::contract(IntervalVector& box){
  // rho, theta copy
  Interval rho_tmp(rho);
  Interval x_tmp(mx - box[0]);
  Interval y_tmp(my - box[1]);
  Interval theta_tmp(theta + box[3]);

  _contract(x_tmp, y_tmp, rho_tmp, theta_tmp);
  if ( x_tmp.is_empty() ){
    box.set_empty();
    return;
  }
  _contract(x_tmp, y_tmp, rho_tmp, theta_tmp);
  box[0] &= (mx - x_tmp);
  box[1] &= (my - y_tmp);
  box[2] &= (theta_tmp - theta);
  if (box[0].is_empty() || box[1].is_empty() || box[2].is_empty()){
    box.set_empty();
  }

}

}  // namespace pyibex
