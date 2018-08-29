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
}

int main()
{
  /* =========== PARAMETERS =========== */

    int n = 1;
    Vector epsilon(n, 0.05);
    Interval domain(0.,1.);
    TubeVector x(domain, n);
    x.set(IntervalVector(n, 1.), 0.); // initial condition
    TrajectoryVector truth(domain, tubex::Function("exp(-t)"));

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon);
    solver.set_refining_fxpt_ratio(0.005);
    solver.set_propa_fxpt_ratio(0.005);
    solver.set_cid_fxpt_ratio(1.);
    solver.figure()->add_trajectoryvector(&truth, "truth", "blue");
    list<TubeVector> l_solutions = solver.solve(x, &contract);


  // Checking if this example still works:
  return (l_solutions.size() == 1
       && solver.solutions_contain(l_solutions, truth)) ? EXIT_SUCCESS : EXIT_FAILURE;
}