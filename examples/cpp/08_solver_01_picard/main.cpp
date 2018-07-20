#include "ibex.h"
#include "tubex.h"
#include "tubex-solver.h"

using namespace std;
using namespace ibex;
using namespace tubex;

void contract(TubeVector& x)
{
  tubex::Function f("x", "-x");

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
    x.set(IntervalVector(n, 1.), 0.); // initial condition
    Trajectory truth(domain, tubex::Function("exp(-t)"));

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon, 0.005, 0.005, 1.);
    solver.figure()->add_trajectory(&truth, "truth", "blue");
    list<TubeVector> l_solutions = solver.solve(x, &contract);


  // Checking if this example is still working:
  return (l_solutions.size() == 1
       && solver.solution_encloses(l_solutions, truth)) ? EXIT_SUCCESS : EXIT_FAILURE;
}