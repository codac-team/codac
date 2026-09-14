/** 
 *  \file codac2_SepPolarCart.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou, from a former implementation of Benoît Desrochers
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepPolarCart.h"
#include "codac2_CtcPolar.h"
#include "codac2_cart_prod.h"
#include "codac2_hull.h"

using namespace codac2;

/**
 * Contracts x with the Cartesian projection of the complement of a polar box.
 *
 * For a polar box [rho]x[theta], the complement is represented as the union
 * of four polar boxes:
 *   - rho <= rho.lb(), with theta in [theta]
 *   - rho >= rho.ub(), with theta in [theta]
 *   - theta before theta.lb(), with rho in R+
 *   - theta after theta.ub(), with rho in R+
 *
 * The angular complement is computed in the 2*pi-wide window centered on [theta].
 */
IntervalVector contract_polar_box_complement(
  const CtcPolar& ctc_polar,
  const IntervalVector& x,
  const IntervalVector& x_pol)
{
  assert_release(x.size() == 2);
  assert_release(x_pol.size() == 2);

  if(x.is_empty())
    return x;

  // Complement of the empty polar box: no point is removed by the inner
  // contractor, so the Cartesian box is left unchanged.
  if(x_pol.is_empty())
    return x;

  const Interval& rho = x_pol[0];
  const Interval& theta = x_pol[1];

  std::list<IntervalVector> parts;

  auto add_polar_part = [&ctc_polar,&parts](const IntervalVector& x,
    const Interval& rho,
    const Interval& theta)
  {
    if(rho.is_empty() || theta.is_empty())
      return;

    Interval rho_(rho), theta_(theta);
    IntervalVector x_(x);
    ctc_polar.contract(x_[0],x_[1],rho_,theta_);
    parts.push_back(x_);
  };

  // Radial complement, restricted to the current angular sector. The angular
  // outside parts below cover the remaining angles.
  if(std::isfinite(rho.lb()) && rho.lb() >= 0)
    add_polar_part(x, {0,rho.lb()}, theta);

  if(std::isfinite(rho.ub()))
    add_polar_part(x, {rho.ub(),oo}, theta);

  // Angular complement. If [theta] already covers at least one full turn,
  // there is no angular complement in the 2*pi-periodic sense.
  if(std::isfinite(theta.lb()) && std::isfinite(theta.ub())
    && theta.ub() - theta.lb() < 2*PI)
  {
    double limit = theta.mid()-PI;
    add_polar_part(x, {0,oo}, {limit,theta.lb()});
    add_polar_part(x, {0,oo}, {theta.ub(),limit+2*PI});
  }

  return x & hull(parts);
}

BoxPair SepPolarCart::separate(const IntervalVector& x) const
{
  assert_release(x.size() == 2);

  BoxPair x_cart_sep { x, x };

  if(x.is_empty())
    return x_cart_sep;

  CtcPolar ctc_polar;
  IntervalVector x_cart(x), x_pol(2);

  // Cartesian input -> polar enclosure
  ctc_polar.contract(x_cart[0], x_cart[1], x_pol[0], x_pol[1]);

  if(x_cart.is_empty() || x_pol.is_empty())
    return x_cart_sep;

  // Separation in the polar space
  const BoxPair x_pol_sep = _sep.front()->separate(x_pol);

  // Outer contraction: keep the Cartesian points whose polar coordinates may
  // belong to the polar separator output
  IntervalVector copy_x_pol_sep = x_pol_sep.outer.subvector(0,1);
  ctc_polar.contract(x_cart_sep.outer[0], x_cart_sep.outer[1], copy_x_pol_sep[0], copy_x_pol_sep[1]);

  // Inner contraction: remove points belonging to the polar set by contracting
  // with the complement of the polar outer box
  x_cart_sep.inner = contract_polar_box_complement(ctc_polar, x_cart_sep.inner, x_pol_sep.outer);

  return x_cart_sep;
}