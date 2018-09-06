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
  ctc_picard.contract(f, x, BACKWARD);

  CtcDeriv ctc_deriv;
  ctc_deriv.set_fast_mode(true);
  ctc_deriv.contract(x, f.eval_vector(x), BACKWARD);
}

int main()
{
  /* =========== PARAMETERS =========== */

    Vector epsilon(1, 0.5);
    Interval domain(0.,10.);
    TubeVector x(domain, 1);
    TrajectoryVector truth(domain, tubex::Function("exp(-t)"));
    x.set(IntervalVector(truth(Interval(10.))), 10.); // initial condition

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon);
    solver.set_refining_fxpt_ratio(1.);
    solver.set_propa_fxpt_ratio(0.1);
    solver.set_cid_fxpt_ratio(0.);
    solver.figure()->add_trajectoryvector(&truth, "truth");
    list<TubeVector> l_solutions = solver.solve(x, &contract);


  // Checking if this example still works:
  return (l_solutions.size() == 1
       && solver.solutions_contain(l_solutions, truth)) ? EXIT_SUCCESS : EXIT_FAILURE;
}