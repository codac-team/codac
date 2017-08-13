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

#include "Tube.h"
#include "VibesFigure_Tube.h"

using namespace std;
using namespace ibex;
using namespace tubex;

void displayBeaconsMap(const Tube& x, const Tube& y, int fig_x, int fig_y);

bool ctcLoc(const pair<float,float>& beacon,
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

  Tube::contract(xdot, v, theta, Function("xdot", "v", "theta", "xdot - v*cos(theta)"));
  Tube::contract(ydot, v, theta, Function("ydot", "v", "theta", "ydot - v*sin(theta)"));

  Tube::contract(x, y, g, beacon_x, beacon_y,
      Function("x", "y", "g", "bx", "by", "g - sqrt((x-bx)^2+(y-by)^2)"));
  Tube::contract(x, xdot, y, ydot, gdot, beacon_x, beacon_y,
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
}

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    double tube_dt = 0.01;
    Interval domain(0., 64.);

  /* =========== MEASUREMENTS =========== */

    vector<pair<float,float> > v_beacons;
    v_beacons.push_back(make_pair(30.,20.));
    v_beacons.push_back(make_pair(80.,-5.));
    v_beacons.push_back(make_pair(125.,20.));

  /* =========== MEASUREMENTS =========== */

    vector<pair<int,IntervalVector> > v_obs;
    IntervalVector ti_zi(2);

    ti_zi[0] = Interval(14.75, 15.55); ti_zi[1] = Interval(11.69, 12.69);
    v_obs.push_back(make_pair(1, ti_zi));
    ti_zi[0] = Interval(20.8, 21.6);   ti_zi[1] = Interval(15.4, 16.4);
    v_obs.push_back(make_pair(0, ti_zi));
    ti_zi[0] = Interval(23.8, 24.6);   ti_zi[1] = Interval(10.62, 11.62);
    v_obs.push_back(make_pair(0, ti_zi));
    ti_zi[0] = Interval(26.8, 27.6);   ti_zi[1] = Interval(11.05, 12.05);
    v_obs.push_back(make_pair(0, ti_zi));
    ti_zi[0] = Interval(29.8, 30.6);   ti_zi[1] = Interval(11.87, 12.87);
    v_obs.push_back(make_pair(0, ti_zi));
    ti_zi[0] = Interval(32.8, 33.6);   ti_zi[1] = Interval(15.31, 16.31);
    v_obs.push_back(make_pair(0, ti_zi));
    ti_zi[0] = Interval(44.35, 45.15); ti_zi[1] = Interval(13.65, 14.65);
    v_obs.push_back(make_pair(2, ti_zi));
    ti_zi[0] = Interval(47.35, 48.15); ti_zi[1] = Interval(13.32, 14.32);
    v_obs.push_back(make_pair(2, ti_zi));
    ti_zi[0] = Interval(50.35, 51.15); ti_zi[1] = Interval(12.03, 13.03);
    v_obs.push_back(make_pair(2, ti_zi));
    ti_zi[0] = Interval(53.35, 54.15); ti_zi[1] = Interval(15.98, 16.98);
    v_obs.push_back(make_pair(2, ti_zi));
    ti_zi[0] = Interval(56.75, 57.55); ti_zi[1] = Interval(17.45, 18.45);
    v_obs.push_back(make_pair(1, ti_zi));

  /* =========== TUBES =========== */

    Tube u1(domain, tube_dt, Function("t", "-0.45 * cos(0.2*t) + [-0.001,0.001]"));
    Tube u2(domain, tube_dt, Function("t", "0.1 + sin(0.25*t) + [-0.001,0.001]"));

    Tube x(u1, Interval::ALL_REALS), y(x), v(x), theta(x);
    Tube xdot(x), ydot(x);

    vector<Tube> v_g, v_gdot;
    for(int i = 0 ; i < v_beacons.size() ; i++)
    {
      Tube g(u1, Interval::ALL_REALS), gdot(g);
      v_g.push_back(g);
      v_gdot.push_back(gdot);
    }

    bool contraction;
    int k = 0;
    do
    {
      cout << "Localization step " << (k+1) << "..." << endl;
      contraction = false;
      
      for(int i = 0 ; i < v_beacons.size() ; i++)
      {
        for(int j = 0 ; j < v_obs.size() ; j++)
          if(v_obs[j].first == i)
            contraction |= v_g[i].ctcObs(v_gdot[i],
                                         v_obs[j].second[0],
                                         v_obs[j].second[1]);

        contraction |= ctcLoc(v_beacons[i], x, xdot, y, ydot, theta, u1, v, u2, v_g[i], v_gdot[i]);
      }

      k++;
    } while(contraction && k < 7);

  /* =========== GRAPHICS =========== */

    VibesFigure_Tube::show(&x, "Tube [x](·)", 100, 100);
    VibesFigure_Tube::show(&y, "Tube [y](·)", 150, 150);
    displayBeaconsMap(x, y, 200, 200);
    VibesFigure_Tube::endDrawing();
    
  // Checking if this example is still working:
  return (fabs(x.volume() - 604.8) < 1e-2
       && fabs(y.volume() - 665.358) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void displayBeaconsMap(const Tube& x, const Tube& y, int fig_x, int fig_y)
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
}