#include "ibex.h"
#include "tubex.h"
#include "tubex-solver.h"

using namespace std;
using namespace ibex;
using namespace tubex;

void contract(TubeVector& x)
{
  // Boundary constraints

    Variable vx0, vx1;
    SystemFactory fac;
    fac.add_var(vx0);
    fac.add_var(vx1);
    fac.add_ctr(sqr(vx0) + sqr(vx1) = 1);
    System sys(fac);
    ibex::CtcHC4 hc4(sys);
    IntervalVector bounds(2);
    bounds[0] = x[0.][0];
    bounds[1] = x[1.][0];
    hc4.contract(bounds);
    x.set(IntervalVector(1,bounds[0]), 0.);
    x.set(IntervalVector(1,bounds[1]), 1.);
  
  // Differential equation

    tubex::Function f("x", "x");

    CtcPicard ctc_picard(true);
    ctc_picard.contract(f, x);
    
    CtcDeriv ctc_deriv(true);
    ctc_deriv.contract(x, f.eval(x));
}

int main()
{
  /* =========== PARAMETERS =========== */

    int n = 1;
    Vector epsilon(n, 0.05);
    Interval domain(0.,1.);
    TubeVector x(domain, n);
    Trajectory truth1(domain, tubex::Function("exp(t)/sqrt(1+exp(2))"));
    Trajectory truth2(domain, tubex::Function("-exp(t)/sqrt(1+exp(2))"));

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon, 0.005, 0.005, 1.);
    solver.figure()->add_trajectory(&truth1, "truth1", "blue");
    solver.figure()->add_trajectory(&truth2, "truth2", "red");
    list<TubeVector> l_solutions = solver.solve(x, &contract);


  // Checking if this example is still working:
  return (solver.solution_encloses(l_solutions, truth1)
       && solver.solution_encloses(l_solutions, truth2)) ? EXIT_SUCCESS : EXIT_FAILURE;
}