#include "ibex.h"
#include "tubex.h"
#include "tubex-solver.h"
#include <iomanip>

using namespace std;
using namespace ibex;
using namespace tubex;

void contract(TubeVector& x)
{
  tubex::Function f("x", "-sin(x)");

  CtcPicard ctc_picard;
  ctc_picard.contract(f, x, FORWARD);

  //CtcDeriv ctc_deriv;
  //ctc_deriv.set_fast_mode(true);
  //ctc_deriv.contract(x, f.eval_vector(x), FORWARD | BACKWARD);
}

int main()
{
  /* =========== PARAMETERS =========== */

    Vector epsilon(1, 0.05);
    Interval domain(0.,10.);
    TubeVector x(domain, 0.01, 1);
    TrajectoryVector truth(domain, tubex::Function("2.*atan(exp(-t)*tan(0.5))"));

    IntervalVector x0 = truth(Interval(0.));
    x0 = IntervalVector(Interval(1.));

    cout << setprecision(40) << x0 << endl;
    x.set(x0, 0.); // initial condition

    // Note: use truth(Interval(0.)) instead of truth(0.) for a reliable evaluation

  /* =========== SOLVER =========== */

    contract(x);

    vibes::beginDrawing();
    VibesFigure_TubeVector fig("x", &x, &truth);
    fig.show();

    cout << x.contains(truth) << endl;

    TubeVector y = x;
    y.set_empty();
    y |= truth;

    cout << (x & y).is_empty() << endl;
    cout << (x & TubeVector(truth, 0.005)).is_empty() << endl;
    cout << (x & TubeVector(truth, 0.001)).is_empty() << endl;

    //tubex::Solver solver(epsilon);
    //solver.set_refining_fxpt_ratio(0.9);
    //solver.set_propa_fxpt_ratio(1.);
    //solver.set_cid_fxpt_ratio(0.);
    //solver.figure()->add_trajectoryvector(&truth, "truth");
    //list<TubeVector> l_solutions = solver.solve(x, &contract);


  // Checking if this example still works:
  //return (solver.solutions_contain(l_solutions, truth) == YES) ? EXIT_SUCCESS : EXIT_FAILURE;
}