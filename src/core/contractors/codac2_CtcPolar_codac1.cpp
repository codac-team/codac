// The following is originated from the former codac::CtcPolar
// by Benoit Desrochers

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
      bwd_imod2(x_, y_, 2*Interval::pi());
      return y_;
    }

    Interval Cmod_bwd(const Interval& x, const Interval& y)
    {
      Interval x_(x);
      Interval y_(y);
      bwd_imod2(x_, y_, 2*Interval::pi());
      return x_;
    }

    tuple<Interval,Interval,Interval> Catan2(const Interval& x, const Interval& y, const Interval& th)
    {
      static const Interval i_0_pi2 = Interval(0) | Interval::half_pi();
      static const Interval i_pi_2pi = Interval::half_pi() | Interval::pi();

      if(x.is_empty() || y.is_empty() || th.is_empty())
        return make_tuple(Interval::empty(), Interval::empty(), Interval::empty());
      
      if(x.is_subset(Interval(0,oo)) && y.is_subset(Interval(0,oo)) && th.is_subset(i_0_pi2))
      {
        if(x == Interval(0.) && y == Interval(0.))
          return make_tuple(Interval::empty(), Interval::empty(), Interval::empty());

        Interval th_tmp = Cmod(th, i_0_pi2);

        // trick to keep tan(th) > 0 because we only consider x >= 0 and y>= 0
        Interval tan_lb = tan(Interval(th_tmp.lb()));
        Interval tan_ub = (th_tmp.ub() == Interval::half_pi().ub()) ?
          Interval(numeric_limits<double>::max(), oo) : tan(Interval(th_tmp.ub()));
        Interval tanTh = tan_lb | tan_ub;

        Interval x_ = (y != Interval(0.)) ? x & y/tanTh : x;
        Interval y_ = (x != Interval(0.)) ? y & x*tanTh : y;
        Interval thr = th_tmp & ((x != Interval(0.)) ? atan(y/x) : Interval::half_pi());

        if(x_.is_empty() || y_.is_empty() || thr.is_empty())
          return make_tuple(Interval::empty(), Interval::empty(), Interval::empty());

        return make_tuple(x_, y_, thr);
      }

      // Divide into four quadrants and call contractor

        // x > 0 and y > 0 and th \in [0, PI/2.]
        Interval x1 = x & Interval(0,oo);
        Interval y1 = y & Interval(0,oo);
        Interval th1 = Cmod(th, i_0_pi2);
        tie(x1, y1, th1) = Catan2(x1, y1, th1);
        Interval th11 = Cmod_bwd(th, th1);

        // x > 0, y < 0 , th \in [-PI/2., 0]
        Interval x2 = x & Interval(0,oo);
        Interval y2 = y & Interval(-oo,0);
        Interval th2 = -Cmod(th, -i_0_pi2);
        tie(x2, y2, th2) = Catan2(x2, -y2, th2);
        Interval th22 = Cmod_bwd(th, -th2);

        // x < 0, y < 0 , th \in [-PI, -PI/2.]
        Interval x3 = x & Interval(-oo,0);
        Interval y3 = y & Interval(-oo,0);
        Interval th3 = Interval::pi() + Cmod(th,-i_pi_2pi);
        tie(x3, y3, th3) = Catan2(-x3, -y3, (th3 & i_0_pi2));
        Interval th33 = Cmod_bwd(th, th3 - Interval::pi());

        // x < 0, y > 0 , th \in [PI/2., PI]
        Interval x4 = x & Interval(-oo,0);
        Interval y4 = y & Interval(0,oo);
        Interval th4 = Interval::pi() - Cmod(th,i_pi_2pi);
        tie(x4, y4, th4) = Catan2(-x4, y4, (th4 & i_0_pi2));
        Interval th44 = Cmod_bwd(th, Interval::pi() - th4);

      Interval x_ =  (x1 | x2 | (-x3) | (-x4));
      Interval y_ =  (y1 | (-y2) | (-y3) | y4);
      Interval thh = (th11 | th22 | th33 | th44);
      return make_tuple(x_, y_, thh);
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