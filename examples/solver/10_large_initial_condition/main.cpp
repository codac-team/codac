#include "ibex.h"
#include "tubex.h"
#include "tubex-solver.h"

using namespace std;
using namespace ibex;
using namespace tubex;

void contract(TubeVector& x)
{
  tubex::Function f("x", "-x");

  CtcPicard ctc_picard;
  ctc_picard.preserve_slicing(true);
  ctc_picard.contract(f, x);

  CtcDeriv ctc_deriv;
  ctc_deriv.preserve_slicing(true);
  ctc_deriv.contract(x, f.eval_vector(x));
}

int main()
{
  /* =========== PARAMETERS =========== */

    int n = 1;
    Vector epsilon(n, 0.1);
    Interval domain(0.,1.);
    TubeVector x(domain, n);
    x.set(IntervalVector(n, Interval(0.5,1.)), 0.); // initial condition
    TrajectoryVector truth1(domain, tubex::Function("1.0*exp(-t)"));
    TrajectoryVector truth2(domain, tubex::Function("0.5*exp(-t)"));

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon);
    solver.set_refining_fxpt_ratio(0.005);
    solver.set_propa_fxpt_ratio(0.005);
    solver.set_cid_fxpt_ratio(0.002);
    solver.figure()->add_trajectoryvector(&truth1, "truth1", "blue");
    solver.figure()->add_trajectoryvector(&truth2, "truth2", "blue");
    list<TubeVector> l_solutions = solver.solve(x, &contract);


  // Checking if this example still works:
  Tube hull = TubeVector::hull(l_solutions)[0];
  Tube f_hull = Tube(domain, 0.0001, tubex::Function("[0.5,1.0]*exp(-t)"));
  return f_hull.is_subset(hull) ? EXIT_SUCCESS : EXIT_FAILURE;
}