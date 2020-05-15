#include "catch_interval.hpp"
#include "tubex_ContractorNetwork.h"
#include "ibex_CtcFwdBwd.h"
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
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

    CHECK(cn.nb_dom() == 3);
    CHECK(cn.nb_ctc() == 1);
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
    cn.add(ctc_plus, {b, d, e});
    cn.add(ctc_plus, {b, d, e});
    cn.add(ctc_plus, {a[0], b[0], c[0]});
    cn.add(ctc_plus, {a[0], b[0], c[0]});
    cn.add(ctc_plus, {a[0], b[0], c[0]});
    cn.add(ctc_plus, {a[0], b[0], c[0]});
    cn.contract();

    CHECK(a[0] == Interval(0.5,1));
    CHECK(b[0] == Interval(0.5,1));
    CHECK(c[0] == Interval(1.5,2));
    CHECK(d[0] == Interval(0));
    CHECK(e[0] == Interval(0.5,1));

    // Before setting names (some vectors not added entirely):
    CHECK(cn.nb_dom() == 3*3+2);
    CHECK(cn.nb_ctc() == 3   // vector items contractors
                        +2   // ctc_plus in array mode
                        +1); // ctc_plus on scalar values);

    cn.set_name(a, "a");
    cn.set_name(b, "b");
    cn.set_name(c, "c");
    cn.set_name(d, "d");
    cn.set_name(e, "e");
    cn.set_name(ctc_plus, "+");
    cn.print_dot_graph("cn_vector_dependencies");

    CHECK(cn.nb_dom() == 3*5);
    CHECK(cn.nb_ctc() == 5   // vector items contractors
                        +2   // ctc_plus in array mode
                        +1); // ctc_plus on scalar values);
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
    cn.add(ctc_eval, {t1, z, x, v});
    cn.add(ctc_eval, {t1, z, x, v});
    cn.add(ctc_eval, {t1, z, x, v}); // redundant contractor that should not be added
    cn.contract();

    cn.set_name(t1, "t_1");
    cn.set_name(z, "z");
    cn.set_name(x, "x");
    cn.set_name(v, "v");
    cn.print_dot_graph("cn_observation");

    CHECK(v.codomain() == Interval(0.));
    CHECK(x.codomain() == Interval(2.));
    CHECK(cn.nb_ctc() == 13);
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
    Vector vec(4,0.5);
    
    CtcFunction ctc_add("b", "c", "a", "b+c=a");
    CtcFunction ctc_cos("b", "c", "a", "b+c=a");
    CtcFunction ctc_minus("b", "c", "b-c=0");

    {
      ContractorNetwork cn;

      cn.add(ctc_add, {x,y,a});
      cn.add(ctc_add, {x,y,a});
      cn.contract();

      CHECK(x == Interval(0,1));
      CHECK(y == Interval(0,3));
      CHECK(a == Interval(1,4));
      CHECK(cn.nb_dom() == 3);
      CHECK(cn.nb_ctc() == 1);
    }

    {
      ContractorNetwork cn;
      cn.add(ctc_add, {vx,vy,va});
      cn.add(ctc_add, {vx[0],vy[0],va[0]});
      cn.contract();

      CHECK(vx == IntervalVector(2,Interval(0,1)));
      CHECK(vy == IntervalVector(2,Interval(0,3)));
      CHECK(va == IntervalVector(2,Interval(1,4)));

      CHECK(cn.nb_dom() == 3*3);
      CHECK(cn.nb_ctc() == 3+2);

      cn.set_name(va, "a");
      cn.set_name(vx, "x");
      cn.set_name(vy, "y");
      cn.print_dot_graph("cn_function");
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

  SECTION("Singleton variables: double or Vector")
  {
    Interval x(0,1), y(-2,3), a(1,20);
    double double_y = 1.;
    Vector vector_y(2, 1.);
    IntervalVector ivx(2,x), ivy(2,y), iva(2,a);
    
    CtcFunction ctc_add("b", "c", "a", "b+c=a");

    {
      ContractorNetwork cn;

      cn.add(ctc_add, {x,double_y,a});
      cn.add(ctc_add, {x,double_y,a}); // redundant adding
      cn.add(ctc_add, {x,double_y,a}); // redundant adding
      cn.contract();

      CHECK(x == Interval(0,1));
      CHECK(double_y == 1.);
      CHECK(a == Interval(1,2));
      CHECK(cn.nb_dom() == 3);
      CHECK(cn.nb_ctc() == 1);
    }

    {
      ContractorNetwork cn;

      cn.add(ctc_add, {ivx,vector_y,iva});
      cn.add(ctc_add, {ivx,vector_y,iva}); // redundant adding
      cn.add(ctc_add, {ivx,vector_y,iva}); // redundant adding
      cn.contract();

      CHECK(ivx == IntervalVector(2,Interval(0,1)));
      CHECK(vector_y == Vector(2,1.));
      CHECK(iva == IntervalVector(2,Interval(1,2)));
      CHECK(cn.nb_dom() == 3*3);
      CHECK(cn.nb_ctc() == 3+2);

      cn.set_name(vector_y, "y");
      cn.set_name(ivx, "x");
      cn.set_name(iva, "a");
      cn.set_name(ctc_add, "+");
      cn.print_dot_graph("cn_singleton");
    }
  }

  SECTION("Heterogeneous variables with CtcFunction")
  {
    double dt = 0.1;
    Interval tdomain(0.,10.);
    
    {
      Interval x(2,3);
      Tube a(tdomain, dt, Interval(4.,5.)), b(tdomain, dt, Interval(5.,9.));

      CtcFunction ctc_add("x", "a", "b", "x+a=b");

      ContractorNetwork cn;
      cn.add(ctc_add, {x,a,b});
      cn.contract();

      CHECK(x == Interval(2.,3.));
      CHECK(a.codomain() == Interval(4.,5.));
      CHECK(b.codomain() == Interval(6.,8.));
    }
    {
      Interval x(-1.,3.);
      Tube a(tdomain, dt, Interval(6.,7.)), b(tdomain, dt, Interval(7.));

      CtcFunction ctc_add("x", "a", "b", "x+a=b");

      ContractorNetwork cn;
      cn.add(ctc_add, {x,a,b});
      cn.contract();

      cn.set_name(x, "x");
      cn.set_name(a, "a");
      cn.set_name(b, "b");

      CHECK(x == Interval(0.,1.));
      CHECK(a.codomain() == Interval(6.,7.));
      CHECK(b.codomain() == Interval(7.));
    }
    {
      Interval x(2.,2.5);
      Tube a(tdomain, dt, Interval(0.,10.)), b(tdomain, dt, Interval(7.));

      CtcFunction ctc_add("x", "a", "b", "x+a=b");

      ContractorNetwork cn;
      cn.add(ctc_add, {x,a,b});
      cn.contract();

      CHECK(x == Interval(2.,2.5));
      CHECK(a.codomain() == Interval(4.5,5.));
      CHECK(b.codomain() == Interval(7.));
    }
    {
      Interval x(2.,2.5);
      Tube a(tdomain, dt, tubex::TimeFunction("cos(t)")), b(tdomain, dt);

      CtcFunction ctc_add("x", "a", "b", "x+a=b");

      ContractorNetwork cn;
      cn.add(ctc_add, {x,a,b});
      cn.contract();
      
      Tube result(tdomain, dt, tubex::TimeFunction("cos(t)"));
      result += x;

      CHECK(x == Interval(2.,2.5));
      CHECK(b == result);
    }
  }
}