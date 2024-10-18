/** 
 *  CtcPolar class
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Benoit Desrochers, Mohamed Saad Ibn Seddik
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_CtcPolar.h"

namespace codac
{
  // The following is originated from the former Catan2.h file

  // Implements interval modulo with double period: x = y mod(p)

    bool bwd_imod2(Interval& x, Interval& y, const Interval& p)
    {
      Interval z1 = (x-y)/p;
      Interval z2 = integer(z1);
      x &= y + z2*p;
      y &= x - z2*p;
      return true;
    }

    Interval Cmod(const Interval& x, const Interval& y)
    {
      Interval x_(x);
      Interval y_(y);
      bwd_imod2(x_, y_, 2*Interval::PI);
      return y_;
    }

    Interval Cmod_bwd(const Interval& x, const Interval& y)
    {
      Interval x_(x);
      Interval y_(y);
      bwd_imod2(x_, y_, 2*Interval::PI);
      return x_;
    }

    tuple<Interval,Interval,Interval> Catan2(const Interval& x, const Interval& y, const Interval& th)
    {
      static const Interval i_0_pi2 = Interval(0) | Interval::HALF_PI;
      static const Interval i_pi_2pi = Interval::HALF_PI | Interval::PI;

      if(x.is_empty() || y.is_empty() || th.is_empty())
        return make_tuple(Interval::EMPTY_SET, Interval::EMPTY_SET, Interval::EMPTY_SET);
      
      if(x.is_subset(Interval::POS_REALS) && y.is_subset(Interval::POS_REALS) && th.is_subset(i_0_pi2))
      {
        if(x == Interval::ZERO && y == Interval::ZERO)
          return make_tuple(Interval::EMPTY_SET, Interval::EMPTY_SET, Interval::EMPTY_SET);

        Interval th_tmp = Cmod(th, i_0_pi2);

        // trick to keep tan(th) > 0 because we only consider x >= 0 and y>= 0
        Interval tan_lb = tan(Interval(th_tmp.lb()));
        Interval tan_ub = (th_tmp.ub() == Interval::HALF_PI.ub()) ?
          Interval(numeric_limits<double>::max(), POS_INFINITY) : tan(Interval(th_tmp.ub()));
        Interval tanTh = tan_lb | tan_ub;

        Interval x_ = (y != Interval::ZERO) ? x & y/tanTh : x;
        Interval y_ = (x != Interval::ZERO) ? y & x*tanTh : y;
        Interval thr = th_tmp & ((x != Interval::ZERO) ? atan(y/x) : Interval::HALF_PI);

        if(x_.is_empty() || y_.is_empty() || thr.is_empty())
          return make_tuple(Interval::EMPTY_SET, Interval::EMPTY_SET, Interval::EMPTY_SET);

        return make_tuple(x_, y_, thr);
      }

      // Divide into four quadrants and call contractor

        // x > 0 and y > 0 and th \in [0, PI/2.]
        Interval x1 = x & Interval::POS_REALS;
        Interval y1 = y & Interval::POS_REALS;
        Interval th1 = Cmod(th, i_0_pi2);
        tie(x1, y1, th1) = Catan2(x1, y1, th1);
        Interval th11 = Cmod_bwd(th, th1);

        // x > 0, y < 0 , th \in [-PI/2., 0]
        Interval x2 = x & Interval::POS_REALS;
        Interval y2 = y & Interval::NEG_REALS;
        Interval th2 = -Cmod(th, -i_0_pi2);
        tie(x2, y2, th2) = Catan2(x2, -y2, th2);
        Interval th22 = Cmod_bwd(th, -th2);

        // x < 0, y < 0 , th \in [-PI, -PI/2.]
        Interval x3 = x & Interval::NEG_REALS;
        Interval y3 = y & Interval::NEG_REALS;
        Interval th3 = Interval::PI + Cmod(th,-i_pi_2pi);
        tie(x3, y3, th3) = Catan2(-x3, -y3, (th3 & i_0_pi2));
        Interval th33 = Cmod_bwd(th, th3 - Interval::PI);

        // x < 0, y > 0 , th \in [PI/2., PI]
        Interval x4 = x & Interval::NEG_REALS;
        Interval y4 = y & Interval::POS_REALS;
        Interval th4 = Interval::PI - Cmod(th,i_pi_2pi);
        tie(x4, y4, th4) = Catan2(-x4, y4, (th4 & i_0_pi2));
        Interval th44 = Cmod_bwd(th, Interval::PI - th4);

      Interval x_ =  (x1 | x2 | (-x3) | (-x4));
      Interval y_ =  (y1 | (-y2) | (-y3) | y4);
      Interval thh = (th11 | th22 | th33 | th44);
      return make_tuple(x_, y_, thh);
    }


  // CtcPolar implementation

  CtcPolar::CtcPolar() : Ctc(4)
  {

  }

  // Contracts x,y,rho,theta wrt the polar constraint
  // Return false if the result is an empty set
  bool _contract(Interval &x, Interval& y, Interval& rho, Interval& theta)
  {
    tie(x, y, theta) = Catan2(x, y, theta);

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

    if(x.is_empty() || y.is_empty() || rho.is_empty() || theta.is_empty())
    {
      x.set_empty();
      y.set_empty();
      rho.set_empty();
      theta.set_empty();
      return false;
    }

    return true;
  }

  void CtcPolar::contract(Interval& x, Interval& y, Interval& rho, Interval& theta)
  {
    _contract(x,y,rho,theta);
    _contract(x,y,rho,theta);
  }

  void CtcPolar::contract(IntervalVector& x)
  {
    _contract(x[0],x[1],x[2],x[3]);
    _contract(x[0],x[1],x[2],x[3]);
  }
}