#include "ibex.h"
#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

void contract(TubeVector& x)
{
  tubex::Function f("x", "-sin(x)");

  CtcPicard ctc_picard(true);
  ctc_picard.contract(f, x, FORWARD);

  CtcDeriv ctc_deriv(true);
  ctc_deriv.contract(x, f.eval(x));
}

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    int n = 1;
    Vector epsilon(n, 0.05);
    Interval domain(0.,10.);
    TubeVector x(domain, n);
    Trajectory truth(domain, tubex::Function("2.*atan(exp(-t)*tan(0.5))"));
    x.set(IntervalVector(n, truth[0.]), 0.); // initial condition

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon, 0.0001, 0.005, 0.005);
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
  return (solver.solution_encloses(v_solutions, truth)) ? EXIT_SUCCESS : EXIT_FAILURE;
}