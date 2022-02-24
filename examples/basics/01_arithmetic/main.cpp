#include <codac.h>
#include <codac-rob.h>
#include <iomanip>
#include "ibex_CtcHC4.h"
#include "ibex_SystemFactory.h"
#include "ibex_Ctc3BCid.h"
#include <unsupported/Eigen/Splines>

using namespace std;
using namespace codac;
using namespace ibex;

#include <Eigen/Dense>
#include <iostream>
#include <cmath>
#include <vector>
#include <Eigen/QR>

int main()
{
  vibes::beginDrawing();

  double dt = 0.001;
  Interval tdomain(-20.,20.);

  Trajectory lb(tdomain, TFunction("cos(t)-1+0.01*t^2"));
  Trajectory ub(tdomain, TFunction("sin(t+0.5)+1+0.03*t^2"));
  Tube x(lb, ub, dt);
  //Tube x(tdomain, dt, TFunction("t^2+[-1,1]"));
  Tube xtree(x);
  Tube xpoly(x);

  xtree.enable_synthesis(SynthesisMode::BINARY_TREE);
  xpoly.enable_synthesis(SynthesisMode::POLYNOMIAL, 1.e-3);

  cout << setprecision(400) << x(Interval(-10.,15.)) << endl;
  cout << setprecision(400) << xtree(Interval(-10.,15.)) << endl;
  cout << setprecision(400) << xpoly(Interval(-10.,15.)) << endl << endl;

  cout << setprecision(400) << x(Interval(-15.,15.)) << endl;
  cout << setprecision(400) << xtree(Interval(-15.,15.)) << endl;
  cout << setprecision(400) << xpoly(Interval(-15.,15.)) << endl << endl;

  cout << setprecision(400) << x(Interval(15.)) << endl;
  cout << setprecision(400) << xtree(Interval(15.)) << endl;
  cout << setprecision(400) << xpoly(Interval(15.)) << endl << endl;

  vibes::endDrawing();
/*
  Trajectory x_truth(tdomain, TFunction("cos(t)+0.01*t^2+1"));

    VIBesFigTube fig_x("x");
    fig_x.set_properties(100, 100, 600, 300);
    fig_x.add_tube(&x, "x");

  vector<PolynomialSubtraj> v_p_lb = polynomial_traj(x, false);
  vector<PolynomialSubtraj> v_p_ub = polynomial_traj(x, true);
  cout << v_p_lb.size() << " polynoms" << endl;
  for(size_t i = 0 ; i < v_p_lb.size() ; i++)
  {
    cout << "  " << v_p_lb[i].tdomain << endl;
    Trajectory *traj = new Trajectory(create_traj(v_p_lb[i]));
    fig_x.add_trajectory(traj, "traj"+std::to_string(i), "red");
  }

    fig_x.show();

    vector<Interval> v_tests{
      Interval(-15,10),
      Interval(8.,8.),
      Interval(-5.,0.),
      Interval(4.5015),
      tdomain,
      Interval(-15.,15.),
      Interval(7.,7.2)
    };

    for(const auto& t : v_tests)
    {
      clock_t t_start;
      int nb = 10000;

      t_start = clock();
      for(int i = 0 ; i < nb ; i++)
        Interval dump = fast_eval(v_p_lb,v_p_ub,t);
      double tfast = (double)(clock() - t_start)/CLOCKS_PER_SEC;
    
      t_start = clock();
      for(int i = 0 ; i < nb ; i++)
        Interval dump = x(t);
      double traw = (double)(clock() - t_start)/CLOCKS_PER_SEC;
    
      t_start = clock();
      for(int i = 0 ; i < nb ; i++)
        Interval dump = xtree(t);
      double ttree = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      cout << "# " << t << endl;
      cout << "Fast: \t" << fast_eval(v_p_lb,v_p_ub,t) << endl;
      cout << "Tree: \t" << xtree(t) << endl;
      cout << "Raw: \t" << x(t) << endl;
      cout << "is_superset: \t" << fast_eval(v_p_lb,v_p_ub,t).is_superset(x(t)) << endl;
      cout << "perf w.r.t raw: \t" << traw << " " << tfast << " \t" << traw/tfast << endl;
      cout << "perf w.r.t tree: \t" << ttree << " " << tfast << " \t" << ttree/tfast << endl << endl;
      fig_x.draw_box(IntervalVector({t,fast_eval(v_p_lb,v_p_ub,t)}));
    }

    vibes::endDrawing();*/

  return 0;
}