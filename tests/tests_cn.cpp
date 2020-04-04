#include "tests.h"
#include "tubex_ContractorNetwork.h"
#include "ibex_CtcFwdBwd.h"
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
#include "tubex_CtcFwdBwd.h"
#include "tubex_CtcFunction.h"
#include "vibes.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

#define VIBES_DRAWING 0

TEST_CASE("CN simple")
{
  SECTION("Simple static case")
  {
    ibex::CtcFwdBwd ctc_plus(*new ibex::Function("a", "b", "c", "a+b-c")); // algebraic constraint a+b=c

    IntervalVector a(2, Interval(0,1)), b(2, Interval(-1,1)), c(2, Interval(1.5,2));

    ContractorNetwork cn;
    cn.add(ctc_plus, {a[0], b[0], c[0]});
    cn.contract();

    CHECK(a[0] == Interval(0.5,1));
    CHECK(b[0] == Interval(0.5,1));
    CHECK(c[0] == Interval(1.5,2));
  }

  SECTION("Dependencies on vector components")
  {
    ibex::CtcFwdBwd ctc_plus(*new ibex::Function("a", "b", "c", "a+b-c")); // algebraic constraint a+b=c

    IntervalVector a(2, Interval(0,1)), b(2, Interval(-1,1)), c(2, Interval(1.5,2)), d(2, Interval(0.)), e(2);

    ContractorNetwork cn;
    // Contractors are added to the graph, 
    // the first one contracts the vector sets, the second one then contracts the components intervals,
    // and so it should trigger again the first one since components have changed.
    cn.add(ctc_plus, {b, d, e});
    cn.add(ctc_plus, {a[0], b[0], c[0]});
    cn.contract();

    CHECK(a[0] == Interval(0.5,1));
    CHECK(b[0] == Interval(0.5,1));
    CHECK(c[0] == Interval(1.5,2));
    CHECK(d[0] == Interval(0));
    CHECK(e[0] == Interval(0.5,1));
  }

  SECTION("Observation in middle of tube")
  {
    double dt = 5.;
    Interval domain(0.,20.);
    Tube x(domain, dt, Interval(-10.,10.)), v(domain, dt, Interval(0.));

    CtcDeriv ctc_deriv;

    ContractorNetwork cn;
    cn.add(ctc_deriv, {x, v});
    cn.add(ctc_deriv, {x, v}); // redundant contractor that should not be added
    cn.contract();
    cn.contract();

    CHECK(v.codomain() == Interval(0.));
    CHECK(x.codomain() == Interval(-10.,10.));
    CHECK(x.nb_slices() == 4);
    CHECK(v.nb_slices() == 4);
    CHECK(cn.nb_ctc() == 12);
    CHECK(cn.nb_dom() == 10);

    CtcEval ctc_eval;
    Interval t1(5.);
    Interval z(2.);

    cn.add(ctc_eval, {t1, z, x, v});
    cn.add(ctc_eval, {t1, z, x, v}); // redundant contractor that should not be added
    cn.contract();

    CHECK(v.codomain() == Interval(0.));
    CHECK(x.codomain() == Interval(2.));
    //CHECK(cn.nb_ctc() == 13); // todo, check this
    CHECK(cn.nb_dom() == 12);
  }

  SECTION("With f")
  {
    double dt = 5.;
    Interval domain(0.,20.);
    Tube x(domain, dt, Interval(-10.,10.)), v(domain, dt, Interval(0.));

    CtcDeriv ctc_deriv;
    CtcFunction ctc_f("x", "xdot", "xdot=-sin(x)");

    ContractorNetwork cn;
    cn.add(ctc_deriv, {x, v});
    cn.add(ctc_f, {x, v});

    CHECK(v.codomain() == Interval(0.));
    CHECK(x.codomain() == Interval(-10.,10.));
    CHECK(x.nb_slices() == 4);
    CHECK(v.nb_slices() == 4);
    CHECK(cn.nb_ctc() == 16);
    CHECK(cn.nb_dom() == 10);

    cn.set_fixedpoint_ratio(0.8);
    cn.contract();
  }

  SECTION("CtcFunction on scalar or vector cases")
  {
    Interval x(0,1), y(-2,3), a(1,20);
    IntervalVector vx(2,x), vy(2,y), va(2,a);
    
    CtcFunction ctc_add("b", "c", "a", "b+c=a");

    {
      ContractorNetwork cn;

      cn.add(ctc_add, {x,y,a});
      cn.contract();

      CHECK(x == Interval(0,1));
      CHECK(y == Interval(0,3));
      CHECK(a == Interval(1,4));
    }

    {
      ContractorNetwork cn;

      cn.add(ctc_add, {vx,vy,va});
      cn.contract();

      CHECK(vx == IntervalVector(2,Interval(0,1)));
      CHECK(vy == IntervalVector(2,Interval(0,3)));
      CHECK(va == IntervalVector(2,Interval(1,4)));
    }
  }

  SECTION("CtcFunction on heterogeneous variables")
  {
    IntervalVector x{{0,1},{-2,3}};
    Interval a(1,20);
    
    CtcFunction ctc_add("b[2]", "a", "b[0]+b[1]=a");

    ContractorNetwork cn;
    cn.add(ctc_add, {x,a});
    cn.contract();

    CHECK(x[0] == Interval(0,1));
    CHECK(x[1] == Interval(0,3));
    CHECK(a == Interval(1,4));
  }

  SECTION("Subvector")
  {
    IntervalVector x{{0,1},{-2,3},{1,20}};

    CtcFunction ctc_add("b", "c", "a", "b+c=a");

    ContractorNetwork cn;
    IntervalVector& sub_x = cn.subvector(x,1,2);

    CHECK(sub_x[0] == Interval(-2,3));
    CHECK(sub_x[1] == Interval(1,20));

    cn.add(ctc_add, {x[0],x[1],x[2]});
    cn.contract();

    CHECK(x[0] == Interval(0,1));
    CHECK(x[1] == Interval(0,3));
    CHECK(x[2] == Interval(1,4));

    CHECK(sub_x[0] == Interval(0,3));
    CHECK(sub_x[1] == Interval(1,4));

    sub_x[0] = Interval(1,2);
    cn.set_all_contractors_active();
    cn.contract();
    CHECK(x[1] == Interval(1,2));
  }
}