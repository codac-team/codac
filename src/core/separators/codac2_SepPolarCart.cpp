/** 
 *  codac2_SepPolarCart.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Benoît Desrochers, (Simon Rohou)
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepPolarCart.h"
#include "codac2_CtcPolar.h"

using namespace codac2;

BoxPair SepPolarCart::separate(const IntervalVector& x) const
{
  assert_release(x.size() == 2);

  CtcPolar ctc_polar;
  IntervalVector x_cart(2), x_pol(x);

  ctc_polar.contract(x_cart[0], x_cart[1], x_pol[0], x_pol[1]);
  auto x_cart_sep = _sep.front()->separate(x_cart);
  BoxPair x_polar_sep { x, x };
  ctc_polar.contract(x_cart_sep.inner[0], x_cart_sep.inner[1], x_polar_sep.inner[0], x_polar_sep.inner[1]);
  ctc_polar.contract(x_cart_sep.outer[0], x_cart_sep.outer[1], x_polar_sep.outer[0], x_polar_sep.outer[1]);

  return x_polar_sep;
}


SepPolarXY::SepPolarXY(Interval rho, Interval theta) : rho(rho), theta(theta), Sep(2) {
  rho_m = Interval(0, rho.lb());
  rho_p = Interval(rho.ub(), POS_INFINITY);
  double limit = theta.mid() - M_PI;
  theta_m = Interval(limit, theta.lb());
  theta_p = Interval(theta.ub(), limit + 2*M_PI);
  cmpl = Interval(0, 2*M_PI);
}


void SepPolarXY::contractOut(IntervalVector &x_out){
  Interval th = this->theta;
  Interval r = this->rho;
  this->ctc.contract(x_out[0], x_out[1], r, th);
  if(x_out[0].is_empty() || x_out[1].is_empty())
    x_out.set_empty();
}


void SepPolarXY::contractIn(IntervalVector &x_in){
  Interval x1(x_in[0]); Interval y1(x_in[1]);
  Interval x2(x_in[0]); Interval y2(x_in[1]);
  Interval x3(x_in[0]); Interval y3(x_in[1]);
  Interval x4(x_in[0]); Interval y4(x_in[1]);
  
  Interval ALLREALS1 = Interval::POS_REALS;
  Interval ALLREALS2 = Interval::POS_REALS;
  Interval cmpl1(cmpl);
  Interval cmpl2(cmpl);
  Interval theta_m_tmp(theta_m);
  Interval theta_p_tmp(theta_p);
  Interval rho_m_tmp(rho_m);
  Interval rho_p_tmp(rho_p);

  this->ctc.contract(x1, y1, ALLREALS1, theta_m_tmp);
  this->ctc.contract(x2, y2, ALLREALS2, theta_p_tmp);
  this->ctc.contract(x3, y3, rho_m_tmp, cmpl1);
  this->ctc.contract(x4, y4, rho_p_tmp, cmpl2);
  x_in[0] &= (x1 | x2 | x3 | x4);
  x_in[1] &= (y1 | y2 | y3 | y4);
  if(x_in[0].is_empty() || x_in[1].is_empty())
    x_in.set_empty();
}
