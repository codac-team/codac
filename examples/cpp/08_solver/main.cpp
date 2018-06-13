#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

#define IVP 1
#define BVP 2
#define BVP_TEST 3
#define SOLVER_TEST IVP


#if SOLVER_TEST == IVP

  void contract(vector<Tube>& v_x)
  {
    v_x[0].ctcFwdBwd(-sin(v_x[0]));
  }

#elif SOLVER_TEST == BVP
      
  void contract(vector<Tube>& v_x)
  {
    Variable vx0, vx1;
    SystemFactory fac;
    fac.add_var(vx0);
    fac.add_var(vx1);
    fac.add_ctr(sqr(vx0) + sqr(vx1) = 1);
    System sys(fac);
    ibex::CtcHC4 hc4(sys);

    IntervalVector bounds(2);
    bounds[0] &= v_x[0][0.];
    bounds[1] &= v_x[0][1.];
    hc4.contract(bounds);
    v_x[0].set(bounds[0], 0.);
    v_x[0].set(bounds[1], 1.);

    v_x[0].ctcFwdBwd(v_x[0]);
  }

#elif SOLVER_TEST == BVP_TEST
      
  void contract(vector<Tube>& v_x)
  {
    Variable vt, vu, vudot;
    SystemFactory fac;
    fac.add_var(vt);
    fac.add_var(vu);
    fac.add_var(vudot);
    fac.add_ctr(vudot - vt*cos(vt) + vu - (1.+vt)*sin(vt) = 0.);
    System sys(fac);
    ibex::CtcHC4 hc4(sys);

    tubex::CtcHC4 tube_hc4;
    tube_hc4.contract(v_x[0], v_x[1], hc4);

    v_x[0].ctcFwdBwd(v_x[1]);
  }

#endif


int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    vector<Tube> v;

    #if SOLVER_TEST == IVP

      Interval domain(0.,10.);
      float epsilon = 0.05;
      v.push_back(Tube(domain, Interval(-1.,1.)));
      v[0].set(1., 0.); // initial condition
      bool show_details = true;

    #elif SOLVER_TEST == BVP

      Interval domain(0.,1.);
      float epsilon = 0.05;
      v.push_back(Tube(domain, Interval(-1.,1.)));
      bool show_details = true;

    #elif SOLVER_TEST == BVP_TEST

      Interval domain(0.,2.5);
      float epsilon = 0.2;
      v.push_back(Tube(domain, Interval(0.5,2.5)));
      v.push_back(Tube(domain, Interval(-20.,20.)));
      v[0].set(1., 0.); // initial condition
      bool show_details = true;

    #endif

  /* =========== SOLVER =========== */

    tubex::Solver solver;
    vector<vector<Tube> > v_solutions = solver.solve(v, &contract, epsilon);

    if(v_solutions.size() == 0)
      cout << "no solution found" << endl;

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VibesFigure_Tube fig("Solver");

    #if SOLVER_TEST == IVP

      fig.setProperties(100,100,700,500);

    #elif SOLVER_TEST == BVP

      fig.setProperties(100,100,700,350);
      Trajectory truth1(domain, Function("t", "exp(t)/sqrt(1+exp(2))"));
      fig.addTrajectory(&truth1, "truth1", "blue");
      Trajectory truth2(domain, Function("t", "-exp(t)/sqrt(1+exp(2))"));
      fig.addTrajectory(&truth2, "truth2", "red");

    #elif SOLVER_TEST == BVP_TEST

      fig.setProperties(100,100,700,350);
      Trajectory truth1(domain, Function("t", "exp(-t)+t*sin(t)"));
      fig.addTrajectory(&truth1, "truth1", "blue");

    #endif

    for(int i = 0 ; i < v_solutions.size() ; i++)
    {
      ostringstream o;
      o << "solution_" << i;
      fig.addTube(&v_solutions[i][0], o.str());
      fig.setTubeDerivative(&v_solutions[i][0], &v_solutions[i][0]);
    }

    fig.show(show_details);
    vibes::endDrawing();

  return EXIT_SUCCESS;
}