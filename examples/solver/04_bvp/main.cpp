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
    bounds[0] = x[0](0.);
    bounds[1] = x[0](1.);
    hc4.contract(bounds);
    x.set(IntervalVector(bounds[0]), 0.);
    x.set(IntervalVector(bounds[1]), 1.);
  
  // Differential equation

    tubex::Function f("x", "x");

    CtcPicard ctc_picard;
    //ctc_picard.preserve_slicing(true);
    ctc_picard.contract(f, x);
    
    CtcDeriv ctc_deriv;
    //ctc_deriv.preserve_slicing(true);
    ctc_deriv.set_fast_mode(true);
    ctc_deriv.contract(x, f.eval_vector(x));
}

int main()
{
  /* =========== PARAMETERS =========== */

    Tube::enable_syntheses(false);
    int n = 1;
    Vector epsilon(n, 0.05);
    Interval domain(0.,1.);
    TubeVector x(domain, n);
    TrajectoryVector truth1(domain, tubex::Function("exp(t)/sqrt(1+exp(2))"));
    TrajectoryVector truth2(domain, tubex::Function("-exp(t)/sqrt(1+exp(2))"));

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon);
    solver.set_refining_fxpt_ratio(0.999);
    solver.set_propa_fxpt_ratio(0.5);
    solver.set_cid_fxpt_ratio(0.);
    solver.figure()->add_trajectoryvector(&truth1, "truth1");
    solver.figure()->add_trajectoryvector(&truth2, "truth2");
    list<TubeVector> l_solutions = solver.solve(x, &contract);


  // Checking if this example still works:
  return (solver.solutions_contain(l_solutions, truth1) == YES
       && solver.solutions_contain(l_solutions, truth2) == YES) ? EXIT_SUCCESS : EXIT_FAILURE;
}