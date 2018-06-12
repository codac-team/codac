#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

#define IVP 1
#define BVP 2
#define SOLVER_TEST BVP


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

#endif


int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    vector<Tube> v;

    #if SOLVER_TEST == IVP

      Interval domain(0.,10.);
      float epsilon = 0.1;
      v.push_back(Tube(domain, Interval(-1.,1.)));
      v[0].set(1., 0.); // initial condition

    #elif SOLVER_TEST == BVP

      Interval domain(0.,1.);
      float epsilon = 0.04;
      v.push_back(Tube(domain, Interval(-1.,1.)));

    #endif

  /* =========== SOLVER =========== */

    tubex::Solver solver;
    vector<vector<Tube> > v_solutions = solver.solve(v, &contract, epsilon);

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VibesFigure_Tube fig("Solver");

    #if SOLVER_TEST == IVP

      fig.setProperties(100,100,700,700);

    #elif SOLVER_TEST == BVP

      fig.setProperties(100,100,700,350);
      Trajectory truth1(domain, Function("t", "exp(t)/sqrt(1+exp(2))"));
      fig.addTrajectory(&truth1, "truth1", "blue");
      Trajectory truth2(domain, Function("t", "-exp(t)/sqrt(1+exp(2))"));
      fig.addTrajectory(&truth2, "truth2", "red");

    #endif

    for(int i = 0 ; i < v_solutions.size() ; i++)
    {
      ostringstream o;
      o << "solution_" << i;
      fig.addTube(&v_solutions[i][0], o.str());
      fig.setTubeDerivative(&v_solutions[i][0], &v_solutions[i][0]);
    }

    fig.show(true);
    vibes::endDrawing();

  return EXIT_SUCCESS;
}