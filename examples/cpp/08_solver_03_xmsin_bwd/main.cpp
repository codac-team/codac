#include "ibex.h"
#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

void contract(TubeVector& x)
{
  tubex::Function f("x", "-sin(x)");

  CtcPicard ctc_picard;
  ctc_picard.contract_bwd(f, x);

  CtcDeriv ctc_deriv;
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
    x.set(IntervalVector(n, truth[10.]), 10.); // initial condition

  /* =========== SOLVER =========== */

    tubex::Solver solver;
    vector<TubeVector> v_solutions = solver.solve(x, &contract, epsilon);

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VibesFigure_Tube fig("Solver", x.dim());
    fig.set_properties(100,100,700,500);
    fig.add_trajectory(&truth, "truth", "blue");

    for(int i = 0 ; i < v_solutions.size() ; i++)
    {
      cout << (i+1) << ": " << v_solutions[i] <<  ", tf↦" << v_solutions[i][v_solutions[i].domain().ub()] << endl;
      ostringstream o; o << "solution_" << i;
      fig.add_tube(&v_solutions[i], o.str());
    }

    fig.show(true);
    vibes::endDrawing();


  // Checking if this example is still working:
  return (solver.solution_encloses(v_solutions, truth)) ? EXIT_SUCCESS : EXIT_FAILURE;
}