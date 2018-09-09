#include "ibex.h"
#include "tubex.h"
#include "tubex-solver.h"

using namespace std;
using namespace ibex;
using namespace tubex;

void contract(TubeVector& x)
{
  tubex::Function f("y1", "y2", "(-0.7*y1 ; 0.7*y1 - (ln(2)/5.)*y2)");
  TubeVector v = f.eval_vector(x);

  /*
  double a = 0.7;
  double b = ln(2.)/5.;
  double k = (1. - 0.75*e^(-6.*b)) / (e^(-6.*a) - e^(-6.*b));
  y1 = ((b/a) - 1.)*k*exp(-a*t)
  */

  CtcPicard ctc_picard;
  ctc_picard.preserve_slicing(true);
  ctc_picard.contract(f, x, FORWARD | BACKWARD);

  CtcDeriv ctc_deriv;
  ctc_deriv.contract(x, v, FORWARD | BACKWARD);

  // Check if the following is useful:
  CtcEval ctc_eval;
  ctc_eval.preserve_slicing(true);
  Interval t(1.,3.);
  IntervalVector z(2);
  z[0] = Interval(-999.,999.);
  z[1] = Interval(1.1,1.3);
  ctc_eval.contract(Interval(1.,3.), Interval(1.1,1.3), x[1], v[1]);
}

int main()
{
  /* =========== PARAMETERS =========== */

    int n = 2;
    Interval domain(0.,6.);
    TubeVector x(domain, n); // todo: remove bounds
    Vector epsilon(n); epsilon[0] = 0.05; epsilon[1] = 0.1;

    IntervalVector init = x(x.domain().lb());
    init[0] = 1.25;
    x.set(init, x.domain().lb());

    IntervalVector max_restriction(2);
    max_restriction[1] = Interval(1.1,1.3);
    x.set(max_restriction, Interval(1.,3.));

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon);
    solver.set_refining_fxpt_ratio(0.99);
    solver.set_propa_fxpt_ratio(0.8);
    solver.set_cid_fxpt_ratio(0.);

    IntervalVector max_restrict_box(3);
    max_restrict_box[0] = Interval(1.,3.);
    max_restrict_box.put(1, max_restriction);
    solver.figure()->draw_box(max_restrict_box, "blue");

    list<TubeVector> l_solutions = solver.solve(x, &contract);


  return EXIT_SUCCESS;
}