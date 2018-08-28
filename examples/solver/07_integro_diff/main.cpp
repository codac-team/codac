#include "ibex.h"
#include "tubex.h"
#include "tubex-solver.h"

using namespace std;
using namespace ibex;
using namespace tubex;

class FncIntegroDiff : public tubex::Fnc
{
  public: 

    FncIntegroDiff() : Fnc(1,1) {};
    const TubeVector eval_vector(const TubeVector& x) const { return Fnc::eval(x); }
    const Interval eval(const Interval& t, const TubeVector& x) const { /* scalar case not defined */ }

    const IntervalVector eval_vector(const Interval& t, const TubeVector& x) const
    {
      return Vector(1, 1.) - 2. * x(t) - 5. * x.integral(t);
      //return Vector(1, 1.) - 2. * sin(x)[t] - 5. * x.integral(t); // unsolved by Mathematica
    }
};

void contract(TubeVector& x)
{
  // Boundary constraints

    Variable vx0, vx1;
    SystemFactory fac;
    fac.add_var(vx0);
    fac.add_var(vx1);
    fac.add_ctr(sqr(vx0) + sqr(vx1) = 1);
    System sys(fac);
    ibex::CtcHC4 hc4(sys);
    IntervalVector bounds(2);
    bounds[0] = x[0](0.);
    bounds[1] = x[0](1.);
    hc4.contract(bounds);
    x.set(IntervalVector(1, bounds[0]), 0.);
    x.set(IntervalVector(1, bounds[1]), 1.);

  // Differential equation

    FncIntegroDiff f;

    CtcPicard ctc_picard(1.1);
    ctc_picard.preserve_slicing(true);
    ctc_picard.contract(f, x, FORWARD | BACKWARD);

    CtcDeriv ctc_deriv;
    ctc_deriv.preserve_slicing(true);
    ctc_deriv.contract(x, f.eval_vector(x), FORWARD | BACKWARD);
}

int main()
{
  /* =========== PARAMETERS =========== */

    int n = 1;
    Vector epsilon(n, 0.1);
    Interval domain(0.,1.);
    TubeVector x(domain, n);
    TrajectoryVector truth1(domain, tubex::Function("0.5*exp(-t)*(-1.78315*cos(2*t)+1.89158*sin(2*t))"));
    TrajectoryVector truth2(domain, tubex::Function("0.5*exp(-t)*(1.97753*cos(2*t)+0.0112371*sin(2*t))"));

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon, 0.005, 0.1, 0.1);
    solver.figure()->add_trajectoryvector(&truth1, "truth", "blue");
    solver.figure()->add_trajectoryvector(&truth2, "truth", "red");
    list<TubeVector> l_solutions = solver.solve(x, &contract);


  // Checking if this example still works:
  return (solver.solution_encloses(l_solutions, truth1)
       && solver.solution_encloses(l_solutions, truth2)) ? EXIT_SUCCESS : EXIT_FAILURE;
}