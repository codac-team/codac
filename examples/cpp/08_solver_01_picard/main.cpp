#include "ibex.h"
#include "tubex.h"

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

int main(int argc, char *argv[])
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
    vector<TubeVector> v_solutions = solver.solve(x, &contract);

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VibesFigure_Tube fig("Solver", x.dim());
    fig.set_properties(100,100,700,500);
    fig.add_trajectory(&truth, "truth", "blue");

    for(int i = 0 ; i < v_solutions.size() ; i++)
    {
      ostringstream o; o << "solution_" << i;
      fig.add_tube(&v_solutions[i], o.str());
    }

    fig.show(true);
    vibes::endDrawing();


  // Checking if this example is still working:
  return (v_solutions.size() == 1
       && solver.solution_encloses(v_solutions, truth)) ? EXIT_SUCCESS : EXIT_FAILURE;
}