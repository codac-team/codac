#include "ibex.h"
#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

class FncIntegroDiff : public tubex::Fnc
{
  public: 

    FncIntegroDiff() : Fnc(1,1)
    {

    }
    
    const TubeVector eval(const TubeVector& x) const
    {
      return Fnc::eval(x);
    }

    const IntervalVector eval(const Interval& t, const TubeVector& x) const
    {
      return IntervalVector(x.dim(), 1.) - 2. * x[t] - 5. * x.integral(t);
    }
};

void contract(TubeVector& x)
{
  FncIntegroDiff f;

  CtcPicard ctc_picard(true, 1.1);
  ctc_picard.contract(f, x, FORWARD | BACKWARD);

  CtcDeriv ctc_deriv(true);
  ctc_deriv.contract(x, IntervalVector(x.dim(), 1.) - 2.*x - 5.*x.primitive(), FORWARD | BACKWARD);
}

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    int n = 1;
    Vector epsilon(n, 0.2);
    Interval domain(0.,2.);
    TubeVector x(domain, IntervalVector(n, Interval(-999.,999.)));
    Trajectory truth(domain, tubex::Function("0.5*exp(-t)*sin(2*t)"));
    x.set(IntervalVector(n, truth[domain.lb()]), domain.lb());
    x.set(IntervalVector(n, truth[domain.ub()]), domain.ub());

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon, 0.01, 0.1, 0.1);
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