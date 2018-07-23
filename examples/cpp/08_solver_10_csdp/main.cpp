#include "ibex.h"
#include "tubex.h"
#include "tubex-solver.h"

using namespace std;
using namespace ibex;
using namespace tubex;

void contract(TubeVector& x)
{
  tubex::Function f("y1", "y2", "(-0.7*y1 ; 0.7*y1 - (ln(2)/5.)*y2)");

  CtcPicard ctc_picard(false);
  ctc_picard.contract(f, x, FORWARD | BACKWARD);

  TubeVector v = f.eval(x);

  CtcDeriv ctc_deriv(false);
  ctc_deriv.contract(x, v, FORWARD | BACKWARD);

  CtcEval ctc_eval(false);
  Interval t(1.,3.);
  IntervalVector z(2);
  z[1] = Interval(1.1,1.3);

  ctc_eval.contract(t, z, x, v);
}

int main()
{
  /* =========== PARAMETERS =========== */

    int n = 2;
    Vector epsilon(n, 0.05);
    Interval domain(0.,6.);
    TubeVector x(domain, IntervalVector(n, Interval(-0.1,1.6)));

    IntervalVector init = x[x.domain().lb()];
    init[0] &= Interval(1.25);
    x.set(init, x.domain().lb());

    IntervalVector max_enclosure(3);
    max_enclosure[0] = Interval(1.,3.);
    max_enclosure[2] = Interval(1.1,1.3);

    IntervalVector a = x[max_enclosure[0]];
    a[1] &= Interval(NEG_INFINITY, max_enclosure[2].ub());
    x.set(a, max_enclosure[0]);

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon, 0.001, 0.005, 0.01);
    solver.figure()->draw_box(max_enclosure, "blue");
    list<TubeVector> l_solutions = solver.solve(x, &contract);

  return EXIT_SUCCESS;
}