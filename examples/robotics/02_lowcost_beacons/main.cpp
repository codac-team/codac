/* ============================================================================
 *  tubex-lib - Example: robot localization among low-cost beacons
 *
 *  Example from the paper "Reliable Non-Linear State Estimation Involving Time Uncertainties"
 *  Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "ibex.h"
#include "tubex.h"
#include "tubex-robotics.h"

using namespace std;
using namespace ibex;
using namespace tubex;

//void displayBeaconsMap(const Tube& x, const Tube& y, int fig_x, int fig_y);

/*bool ctcLoc(const pair<float,float>& beacon,
            Tube& x, Tube& xdot,
            Tube& y, Tube& ydot,
            Tube& theta, const Tube& thetadot,
            Tube& v, const Tube& vdot,
            Tube& g, Tube& gdot)
{
  Tube x_old(x), xdot_old(xdot),
       y_old(y), ydot_old(ydot),
       theta_old(theta), thetadot_old(thetadot),
       v_old(v), vdot_old(vdot),
       g_old(g), gdot_old(gdot);

  Tube beacon_x(x, beacon.first);
  Tube beacon_y(y, beacon.second);

  CtcArithmetic::contract(xdot, v, theta, Function("xdot", "v", "theta", "xdot - v*cos(theta)"));
  CtcArithmetic::contract(ydot, v, theta, Function("ydot", "v", "theta", "ydot - v*sin(theta)"));

  CtcArithmetic::contract(x, y, g, beacon_x, beacon_y,
      Function("x", "y", "g", "bx", "by", "g - sqrt((x-bx)^2+(y-by)^2)"));
  CtcArithmetic::contract(x, xdot, y, ydot, gdot, beacon_x, beacon_y,
      Function("x", "xdot", "y", "ydot", "gdot", "bx", "by",
               "gdot - ((sign(x-bx)/sqrt(1+(((y-by)/(x-bx))^2)))*xdot + (sign(y-by)/sqrt(1+(((x-bx)/(y-by))^2)))*ydot)"));

  x.ctcFwdBwd(xdot);
  y.ctcFwdBwd(ydot);
  g.ctcFwdBwd(gdot);

  Interval theta0 = Interval(M_PI/2.).inflate(0.01);
  Interval v0 = Interval(0.).inflate(0.01);

  theta.ctcFwdBwd(thetadot, theta0);
  v.ctcFwdBwd(vdot, v0);

  return x_old != x || xdot_old != xdot || y_old != y || ydot_old != ydot || theta_old != theta || thetadot_old != thetadot || v_old != v || vdot_old != vdot || g_old != g || gdot_old != gdot;
}*/

int main()
{
  /* =========== PARAMETERS =========== */

    double tube_dt = 0.01;
    Interval domain(0., 64.);

  /* =========== MAP =========== */

    vector<Beacon> v_beacons;
    v_beacons.push_back(Beacon(30.,20.));
    v_beacons.push_back(Beacon(80.,-5.));
    v_beacons.push_back(Beacon(125.,20.));

  /* =========== MEASUREMENTS =========== */

    IntervalVector ti_zi(2);
    map<int,vector<IntervalVector> > m_obs;
    ti_zi[0] = Interval(14.75, 15.55); ti_zi[1] = Interval(11.69, 12.69); m_obs[1].push_back(ti_zi);
    ti_zi[0] = Interval(20.8, 21.6);   ti_zi[1] = Interval(15.4, 16.4);   m_obs[0].push_back(ti_zi);
    ti_zi[0] = Interval(23.8, 24.6);   ti_zi[1] = Interval(10.62, 11.62); m_obs[0].push_back(ti_zi);
    ti_zi[0] = Interval(26.8, 27.6);   ti_zi[1] = Interval(11.05, 12.05); m_obs[0].push_back(ti_zi);
    ti_zi[0] = Interval(29.8, 30.6);   ti_zi[1] = Interval(11.87, 12.87); m_obs[0].push_back(ti_zi);
    ti_zi[0] = Interval(32.8, 33.6);   ti_zi[1] = Interval(15.31, 16.31); m_obs[0].push_back(ti_zi);
    ti_zi[0] = Interval(44.35, 45.15); ti_zi[1] = Interval(13.65, 14.65); m_obs[2].push_back(ti_zi);
    ti_zi[0] = Interval(47.35, 48.15); ti_zi[1] = Interval(13.32, 14.32); m_obs[2].push_back(ti_zi);
    ti_zi[0] = Interval(50.35, 51.15); ti_zi[1] = Interval(12.03, 13.03); m_obs[2].push_back(ti_zi);
    ti_zi[0] = Interval(53.35, 54.15); ti_zi[1] = Interval(15.98, 16.98); m_obs[2].push_back(ti_zi);
    ti_zi[0] = Interval(56.75, 57.55); ti_zi[1] = Interval(17.45, 18.45); m_obs[1].push_back(ti_zi);

  /* =========== STATE ESTIMATION =========== */

    TubeVector x(domain, tube_dt, 4); // state vector
    TubeVector xdot(domain, tube_dt, 4); // state vector
    TubeVector u(domain, tube_dt,     // command vector
      tubex::Function("(-0.45 * cos(0.2*t) + [-0.001,0.001] ; 0.1 + sin(0.25*t) + [-0.001,0.001])"));

    xdot.put(2, u);

    IntervalVector x0(4);
    x0[2] = (M_PI / 2.) + Interval(-0.01, 0.01);
    x0[3] = Interval(-0.01, 0.01);

    x.set(x0, 0.);


    // One observation tube by beacon:
   // vector<TubeVector> v_g;
   // for(int i = 0 ; i < v_beacons.size() ; i++)
   //   v_g.push_back(TubeVector(domain, tube_dt, 2));

    int k = 0;
    bool contraction;
    CtcEval ctc_eval(false, false);

    const char* xdyn[8];
    xdyn[0] = "x"; xdyn[1] = "y";
    xdyn[2] = "xdot"; xdyn[3] = "ydot";
    xdyn[4] = "bx"; xdyn[5] = "by";
    xdyn[6] = "g"; xdyn[7] = "gdot";
    tubex::Function f_g(8, xdyn, "(sqrt((x-bx)^2+(y-by)^2) ; \
                                  xdot*sign(x-bx)/sqrt(1+((y-by)^2)/((x-bx)^2)) \
                                + ydot*sign(y-by)/sqrt(1+((x-bx)^2)/((y-by)^2)))");

    xdyn[0] = "x"; xdyn[1] = "y";
    xdyn[2] = "v"; xdyn[3] = "theta";
    tubex::Function f_f(4, xdyn, "(v*cos(theta) ; \
                                   v*sin(theta) ; \
                                   -0.45 * cos(0.2*t) + [-0.001,0.001] ; \
                                   0.1 + sin(0.25*t) + [-0.001,0.001])");

    map<int,TubeVector*> m_x;
    for(int i = 0 ; i < v_beacons.size() ; i++)
    {
      m_x[i] = new TubeVector(x);
      m_x[i]->resize(8);

      (*m_x[i])[4].set(v_beacons[i].pos_box()[0]);
      (*m_x[i])[5].set(v_beacons[i].pos_box()[1]);

      for(int k = 0 ; k < m_obs[i].size() ; k++)
        ctc_eval.contract(m_obs[i][k][0], m_obs[i][k][1], (*m_x[i])[6], (*m_x[i])[7]);
    }

    Variable vt, vx, vy, vxdot, vydot, vbx, vby, vg, vgdot, vv, vtheta;

    SystemFactory fac_f;
    fac_f.add_var(vt);
    fac_f.add_var(vx); fac_g.add_var(vy);
    fac_f.add_var(vxdot); fac_g.add_var(vydot);
    fac_f.add_var(vbx); fac_g.add_var(vby);
    fac_f.add_var(vg); fac_g.add_var(vgdot);
    fac_f.add_ctr(vxdot = v*cos(theta));
    fac_f.add_ctr(vydot = v*sin(theta));
    System sys_f(fac_f);
    ibex::CtcHC4 hc4(sys_f);

    SystemFactory fac_g;
    fac_g.add_var(vt);
    fac_g.add_var(vx); fac_g.add_var(vy);
    fac_g.add_var(vxdot); fac_g.add_var(vydot);
    fac_g.add_var(vbx); fac_g.add_var(vby);
    fac_g.add_var(vg); fac_g.add_var(vgdot);
    fac_g.add_ctr(vg = sqrt(sqr(vx-vbx)+sqr(vy-vby)));
    fac_g.add_ctr(vgdot = (vxdot*sign(vx-vbx)/sqrt(1+(sqr(vy-vby)/sqr(vx-vbx)))) 
                      + (vydot*sign(vy-vby)/sqrt(1+(sqr(vx-vbx)/sqr(vy-vby)))));
    System sys_g(fac_g);
    ibex::CtcHC4 hc4(sys_g);

    tubex::CtcHC4 ctc_hc4;
/*
    for(int i = 0 ; i < v_beacons.size() ; i++)
    {
      cout << "Seamark " << (i+1) << endl;

      ctc_hc4.contract(hc4, *m_x[i]);

      (*m_x[i])[0].ctc_deriv((*m_x[i])[3]);
      (*m_x[i])[1].ctc_deriv((*m_x[i])[4]);
      (*m_x[i])[6].ctc_deriv((*m_x[i])[7]);

      for(int j = 0 ; j < 4 ; j++)
        x[j] &= (*m_x[i])[j];
    }

  /* =========== GRAPHICS =========== */

    //VibesFigure_Tube::draw("Tube [x](·)", &x, 100, 100);
    //VibesFigure_Tube::draw("Tube [y](·)", &y, 150, 150);
    //displayBeaconsMap(x, y, 200, 200);
    //VibesFigure_Tube::end_drawing();
    
  // Checking if this example is still working:
  //return (fabs(x.volume() - 604.8) < 1e-2
  //     && fabs(y.volume() - 665.358) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*void displayBeaconsMap(const Tube& x, const Tube& y, int fig_x, int fig_y)
{
  const string fig_name = "Map (top view): [x](·)x[y](·)";
  const int slices_number_to_display = 500;

  vibes::newFigure(fig_name);
  vibes::setFigureProperties(
            vibesParams("figure", fig_name, "x", fig_x, "y", fig_y, "width", 900, "height", 600));

  // Robot's tubes projection
  int startpoint;
  for(int i = 0 ; i < x.size() ; i += max((int)(x.size() / slices_number_to_display), 1))
    startpoint = i;

  for(int i = startpoint ; i >= 0; i -= max((int)(x.size() / slices_number_to_display), 1))
  {
    Interval intv_x = x[i];
    Interval intv_y = y[i];
    if(!intv_x.is_unbounded() && !intv_y.is_unbounded())
      vibes::drawBox(intv_x.lb(), intv_x.ub(), intv_y.lb(), intv_y.ub(),
                     "lightGray[white]", vibesParams("figure", fig_name));
  }
}*/