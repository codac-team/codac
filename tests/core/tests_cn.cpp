#include "catch_interval.hpp"
#include "codac_Variable.h"
#include "codac_ContractorNetwork.h"
#include "codac_CtcDeriv.h"
#include "codac_CtcEval.h"
#include "codac_CtcFunction.h"
#include "codac_predef_values.h"
#include "vibes.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace codac;

#define VIBES_DRAWING 0

TEST_CASE("CN simple")
{
  SECTION("Most simple case")
  {
    CtcFunction ctc_plus(Function("a", "b", "c", "a+b-c"));
    Interval a(0,1), b(-1,1), c(1.5,2);

    ContractorNetwork cn;
    cn.add(ctc_plus, {a, b, c}); 
    cn.contract();

    CHECK(a == Interval(0.5,1));
    CHECK(b == Interval(0.5,1));
    CHECK(c == Interval(1.5,2));

    CHECK(cn.nb_dom() == 3);
    CHECK(cn.nb_ctc() == 1);
  }

  SECTION("Simple static case")
  {
    CtcFunction ctc_plus(Function("a", "b", "c", "a+b-c")); // algebraic constraint a+b=c

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
    CtcFunction ctc_plus(Function("a", "b", "c", "a+b-c")); // algebraic constraint a+b=c

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
    cn.add(ctc_eval, {t1, z, x, v}); // redundant contractor that should not be added
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
    CtcFunction ctc_f(Function("x", "xdot", "xdot+sin(x)"));

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
    CtcFunction ctc_add(Function("b", "c", "a", "b+c-a"));

    {
      Interval x(0,1), y(-2,3), a(1,20);
    
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

// todo:     { // With const case
// todo:       Interval x(0,1), y(-2,3);
// todo:       const Interval a(1,20);
// todo:     
// todo:       ContractorNetwork cn;
// todo: 
// todo:       cn.add(ctc_add, {x,y,a});
// todo:       cn.add(ctc_add, {x,y,a});
// todo:       cn.contract();
// todo: 
// todo:       CHECK(x == Interval(0,1));
// todo:       CHECK(y == Interval(0,3));
// todo:       CHECK(a == Interval(1,20));
// todo:       CHECK(cn.nb_dom() == 3);
// todo:       CHECK(cn.nb_ctc() == 1);
// todo:     }

    {
      Interval x(0,1), y(-2,3), a(1,20);
      IntervalVector vx(2,x), vy(2,y), va(2,a);
      Vector vec(4,0.5);
    
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
    
    CtcFunction ctc_add(Function("b[2]", "a", "b[0]+b[1]-a"));

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

    CtcFunction ctc_add(Function("b", "c", "a", "b+c-a"));

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
    cn.trigger_all_contractors();
    cn.contract();
    CHECK(x[1] == Interval(1,2));
  }

  SECTION("Singleton variables: double or Vector")
  {
    CtcFunction ctc_add(Function("b", "c", "a", "b+c-a"));

    {
      Interval x(0,1), y(-2,3), a(1,20);
      double double_y = 1.;
      
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
      Interval x(0,1), a(1,20);
      Vector vector_y(2, 1.);
      IntervalVector ivx(2,x), iva(2,a);
    
      ContractorNetwork cn;

      cn.add(ctc_add, {ivx,vector_y,iva});
      cn.add(ctc_add, {ivx,vector_y,iva}); // redundant adding
      cn.add(ctc_add, {ivx,vector_y,iva}); // redundant adding
      cn.contract();

      CHECK(ivx == IntervalVector(2,Interval(0,1)));
      CHECK(vector_y == Vector(2,1.));
      CHECK(iva == IntervalVector(2,Interval(1,2)));
      // todo: reactivate this test: CHECK(cn.nb_dom() == 3*3);
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

      CtcFunction ctc_add(Function("x", "a", "b", "x+a-b"));

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

      CtcFunction ctc_add(Function("x", "a", "b", "x+a-b"));

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

      CtcFunction ctc_add(Function("x", "a", "b", "x+a-b"));

      ContractorNetwork cn;
      cn.add(ctc_add, {x,a,b});
      cn.contract();

      CHECK(x == Interval(2.,2.5));
      CHECK(a.codomain() == Interval(4.5,5.));
      CHECK(b.codomain() == Interval(7.));
    }
    {
      Interval x(2.,2.5);
      Tube a(tdomain, dt, TFunction("cos(t)")), b(tdomain, dt);

      CtcFunction ctc_add(Function("x", "a", "b", "x+a-b"));

      ContractorNetwork cn;
      cn.add(ctc_add, {x,a,b});
      cn.contract();
      
      Tube result(tdomain, dt, TFunction("cos(t)"));
      result += x;

      CHECK(x == Interval(2.,2.5));
      CHECK(b == result);
    }
  }

  SECTION("Adding continuous data")
  {
    double dt = 1.;
    Interval tdomain(0.,5.);

    Tube x(tdomain, dt), v(tdomain, dt);
    x.set(0., 0.);

    CtcDeriv ctc_deriv;
    ContractorNetwork cn;
    cn.add(ctc_deriv, {x,v});

    cn.set_name(x, "x");
    cn.set_name(v, "v");
    cn.print_dot_graph("cn_adddata");

    cn.contract();

    CHECK((x.codomain() == Interval::all_reals()
        || x.codomain() == Interval(-99999, 99999))); // todo: remove this
    CHECK((v.codomain() == Interval::all_reals()
        || x.codomain() == Interval(-99999, 99999))); // todo: remove this
    CHECK(cn.nb_ctc_in_stack() == 0);

    // Adding data
    cn.add_data(v, 0., Interval(0.));
    CHECK(cn.nb_ctc_in_stack() == 0);
    cn.add_data(v, 0.3, Interval(0.));
    CHECK(cn.nb_ctc_in_stack() == 0);
    cn.add_data(v, 0.4, Interval(0.));
    cn.add_data(v, 0.5, Interval(0.));
    // cn.add_data(x, 0.5, Interval(0.5))); // should fail
    CHECK(cn.nb_ctc_in_stack() == 0);
    cn.add_data(v, 0.99, Interval(0.));
    CHECK(cn.nb_ctc_in_stack() == 0);
    cn.add_data(v, 1.3, Interval(0.));
    CHECK(cn.nb_ctc_in_stack() == 3);
    cn.add_data(v, 1.5, Interval(0.));
    CHECK(cn.nb_ctc_in_stack() == 3);
    cn.add_data(v, 4.5, Interval(-3.)); // accross two slices
    CHECK(cn.nb_ctc_in_stack() == 9);
    cn.add_data(v, 5.5, Interval(1.)); // after tf
    CHECK(cn.nb_ctc_in_stack() == 10);

    CHECK(v(0) == Interval(0.));
    CHECK(v(1) == Interval(-0.5,0.));
    CHECK(v(2) == Interval(-1.5,-0.5));
    CHECK(v(3) == Interval(-2.5,-1.5));
    CHECK(v(4) == Interval(-3.,-1.));
  }

  SECTION("create_interm_var Tube")
  {
    double dt = 0.1;
    Interval tdomain(0.,10.);
    Tube x(tdomain, dt);
    x.set(0., 0.);

    CHECK(x.codomain() == Interval());

    ContractorNetwork cn;
    CtcDeriv ctc_deriv;

    {
      Tube v_local(tdomain, dt, TFunction("0"));
      Tube &v_inside = cn.create_interm_var(v_local);
      //cn.add(ctc_deriv, {x,v_inside});
    }

    //cn.contract();
    //CHECK(x.codomain() == Interval(0.));
  }

  /*SECTION("create_interm_var TubeVector")
  {
    double dt = 0.1;
    Interval tdomain(0.,10.);
    TubeVector x(tdomain, dt, 2);
    x.set(Vector(2,0.), 0.);

    CHECK(x.codomain() == IntervalVector(2));

    ContractorNetwork cn;
    CtcDeriv ctc_deriv;

    {
      TubeVector v_local(tdomain, dt, TFunction("(0.;0.)"));
      TubeVector &v_inside = cn.create_interm_var(v_local);
      cn.add(ctc_deriv, {x,v_inside});
    }

    //cn.contract();
    CHECK(x.codomain() == IntervalVector(2, 0.));
  }*/


  /*SECTION("Desired order")
  {
    Interval x(0.,1.), y(4.,5.), d(-10.,10.);
    Interval x_(x), y_(y);

    CtcFunction ctc_sqr(Function("x", "y", "sqr(x)-y"));
    CtcFunction ctc_sqrt(Function("x", "y", "sqrt(x)-y"));
    CtcFunction ctc_plus(Function("x", "y", "z", "x+y-z"));

    ContractorNetwork cn;

    Interval& a = cn.create_interm_var(Interval());
    Interval& b = cn.create_interm_var(Interval());
    Interval& c = cn.create_interm_var(Interval());

    cn.add(ctc_sqr, {y, b});
    cn.add(ctc_sqr, {x, a});
    cn.add(ctc_plus, {a, b, c});
    cn.add(ctc_sqrt, {c, d});

    cn.set_name(ctc_sqrt, "sqrt");
    cn.set_name(ctc_plus, "+");
    cn.set_name(ctc_sqr, "sqr");

    cn.set_fixedpoint_ratio(0.);
    cn.contract_ordered_mode();

    CHECK(x == x_);
    CHECK(y == y_);
    CHECK(d == sqrt(sqr(x_)+sqr(y_)));
    CHECK(cn.iteration_nb() == 1);
  }

  SECTION("Desired order (reverse)")
  {
    Interval x(0.,0.), y(4.,6.), d(4.);
    Interval d_(d);

    CtcFunction ctc_sqr(Function("x", "y", "sqr(x)-y"));
    CtcFunction ctc_sqrt(Function("x", "y", "sqrt(x)-y"));
    CtcFunction ctc_plus(Function("x", "y", "z", "x+y-z"));

    ContractorNetwork cn;

    Interval& a = cn.create_interm_var(Interval());
    Interval& b = cn.create_interm_var(Interval());
    Interval& c = cn.create_interm_var(Interval());

    cn.add(ctc_sqr, {y, b});
    cn.add(ctc_sqr, {x, a});
    cn.add(ctc_plus, {a, b, c});
    cn.add(ctc_sqrt, {c, d});

    cn.set_name(ctc_sqrt, "sqrt");
    cn.set_name(ctc_plus, "+");
    cn.set_name(ctc_sqr, "sqr");

    cn.set_fixedpoint_ratio(1.);
    cn.contract_ordered_mode();

    CHECK(x == Interval(0.));
    CHECK(y == Interval(4.));
    CHECK(d == d_);
    CHECK(cn.iteration_nb() == 1);
  }

  SECTION("Desired order (reverse, needs two iterations)")
  {
    Interval x(0.,0.), y(4.,6.), d(4.);
    Interval d_(d);

    CtcFunction ctc_sqr(Function("x", "y", "sqr(x)-y"));
    CtcFunction ctc_sqrt(Function("x", "y", "sqrt(x)-y"));
    CtcFunction ctc_plus(Function("x", "y", "z", "x+y-z"));

    ContractorNetwork cn;

    Interval& a = cn.create_interm_var(Interval());
    Interval& b = cn.create_interm_var(Interval());
    Interval& c = cn.create_interm_var(Interval());

    cn.add(ctc_sqrt, {c, d});
    cn.add(ctc_plus, {a, b, c});
    cn.add(ctc_sqr, {x, a});
    cn.add(ctc_sqr, {y, b});

    cn.set_name(ctc_sqrt, "sqrt");
    cn.set_name(ctc_plus, "+");
    cn.set_name(ctc_sqr, "sqr");

    cn.set_fixedpoint_ratio(0.);
    cn.contract_ordered_mode();

    CHECK(x == Interval(0.));
    CHECK(y == Interval(4.));
    CHECK(d == d_);
    CHECK(cn.iteration_nb() <= 2);
  }*/

// todo: this test does not work on GitHub actions only:  SECTION("Reset method")
// todo: this test does not work on GitHub actions only:  {
// todo: this test does not work on GitHub actions only:    Interval x(0.,0.), y(4.,6.), d(4.);
// todo: this test does not work on GitHub actions only:    Interval d_(d);
// todo: this test does not work on GitHub actions only:    
// todo: this test does not work on GitHub actions only:    CtcFunction ctc_sqr(Function("x", "y", "sqr(x)-y"));
// todo: this test does not work on GitHub actions only:    CtcFunction ctc_sqrt(Function("x", "y", "sqrt(x)-y"));
// todo: this test does not work on GitHub actions only:    CtcFunction ctc_plus(Function("x", "y", "z", "x+y-z"));
// todo: this test does not work on GitHub actions only:    
// todo: this test does not work on GitHub actions only:    ContractorNetwork cn;
// todo: this test does not work on GitHub actions only:    
// todo: this test does not work on GitHub actions only:    Interval& a = cn.create_interm_var(Interval(-55.,55.));
// todo: this test does not work on GitHub actions only:    cn.set_name(a, "a");
// todo: this test does not work on GitHub actions only:    Interval& b = cn.create_interm_var(Interval());
// todo: this test does not work on GitHub actions only:    Interval& c = cn.create_interm_var(Interval());
// todo: this test does not work on GitHub actions only:    
// todo: this test does not work on GitHub actions only:    cn.set_name(b, "b");
// todo: this test does not work on GitHub actions only:    cn.set_name(c, "c");
// todo: this test does not work on GitHub actions only:    
// todo: this test does not work on GitHub actions only:    cn.add(ctc_sqrt, {c, d});
// todo: this test does not work on GitHub actions only:    cn.add(ctc_plus, {a, b, c});
// todo: this test does not work on GitHub actions only:    cn.add(ctc_sqr, {x, a});
// todo: this test does not work on GitHub actions only:    cn.add(ctc_sqr, {y, b});
// todo: this test does not work on GitHub actions only:    
// todo: this test does not work on GitHub actions only:    cn.set_name(a, "a");
// todo: this test does not work on GitHub actions only:    cn.set_name(b, "b");
// todo: this test does not work on GitHub actions only:    cn.set_name(c, "c");
// todo: this test does not work on GitHub actions only:    cn.set_name(d, "d");
// todo: this test does not work on GitHub actions only:    cn.set_name(x, "x");
// todo: this test does not work on GitHub actions only:    cn.set_name(y, "y");
// todo: this test does not work on GitHub actions only:    
// todo: this test does not work on GitHub actions only:    CHECK(cn.nb_dom() == 6);
// todo: this test does not work on GitHub actions only:    
// todo: this test does not work on GitHub actions only:    cn.contract();
// todo: this test does not work on GitHub actions only:    
// todo: this test does not work on GitHub actions only:    CHECK(x == Interval(0.));
// todo: this test does not work on GitHub actions only:    CHECK(y == Interval(4.));
// todo: this test does not work on GitHub actions only:    CHECK(d == d_);
// todo: this test does not work on GitHub actions only:    CHECK(a == Interval(0.));
// todo: this test does not work on GitHub actions only:    CHECK(b == Interval(16.));
// todo: this test does not work on GitHub actions only:    CHECK(c == Interval(16.));
// todo: this test does not work on GitHub actions only:    
// todo: this test does not work on GitHub actions only:    CHECK(cn.nb_ctc_in_stack() == 0);
// todo: this test does not work on GitHub actions only:    CHECK(cn.nb_dom() == 6);
// todo: this test does not work on GitHub actions only:    CHECK(cn.nb_ctc() == 4);
// todo: this test does not work on GitHub actions only:    cn.reset_interm_vars();
// todo: this test does not work on GitHub actions only:    CHECK(cn.nb_ctc() == 4);
// todo: this test does not work on GitHub actions only:    CHECK(cn.nb_ctc_in_stack() != 0);
// todo: this test does not work on GitHub actions only:    CHECK(x == Interval(0.));
// todo: this test does not work on GitHub actions only:    CHECK(y == Interval(4.));
// todo: this test does not work on GitHub actions only:    CHECK(d == d_);
// todo: this test does not work on GitHub actions only:    CHECK(a == Interval(-55.,55.));
// todo: this test does not work on GitHub actions only:    CHECK(b == Interval());
// todo: this test does not work on GitHub actions only:    CHECK(c == Interval());
// todo: this test does not work on GitHub actions only:    
// todo: this test does not work on GitHub actions only:    cn.contract();
// todo: this test does not work on GitHub actions only:    CHECK(x == Interval(0.));
// todo: this test does not work on GitHub actions only:    CHECK(y == Interval(4.));
// todo: this test does not work on GitHub actions only:    CHECK(d == d_);
// todo: this test does not work on GitHub actions only:    CHECK(a == Interval(0.));
// todo: this test does not work on GitHub actions only:    CHECK(b == Interval(16.));
// todo: this test does not work on GitHub actions only:    CHECK(c == Interval(16.));
// todo: this test does not work on GitHub actions only:  }

  SECTION("Reset method, vector case")
  {
    CtcFunction ctc_plus(Function("x", "y", "z", "x+y-z"));

    ContractorNetwork cn;

    IntervalVector& a = cn.create_interm_var(IntervalVector(
      {{3.,3.},{-oo,oo},{4.,4.}}));
    IntervalVector& b = cn.create_interm_var(IntervalVector(
      {{-oo,oo},{2.,2.},{4.,4.}}));
    IntervalVector& c = cn.create_interm_var(IntervalVector(
      {{3.,3.},{2.,2.},{-oo,oo}}));

    cn.set_name(a, "a");
    cn.set_name(b, "b");
    cn.set_name(c, "c");

    cn.add(ctc_plus, {a, b, c});

    CHECK(cn.nb_dom() == 12);
    CHECK(cn.nb_ctc() == 6);
    CHECK(cn.nb_ctc_in_stack() != 0);

    cn.contract();

    CHECK(cn.nb_ctc_in_stack() == 0);
    CHECK(cn.nb_dom() == 12);

    CHECK(a == IntervalVector(
      {{3.,3.},{0.,0.},{4.,4.}}));
    CHECK(b == IntervalVector(
      {{0.,0.},{2.,2.},{4.,4.}}));
    CHECK(c == IntervalVector(
      {{3.,3.},{2.,2.},{8.,8.}}));

    cn.reset_interm_vars();
    CHECK(cn.nb_ctc_in_stack() != 0);
    CHECK(cn.nb_dom() == 12);

    CHECK(a == IntervalVector(
      {{3.,3.},{-oo,oo},{4.,4.}}));
    CHECK(b == IntervalVector(
      {{-oo,oo},{2.,2.},{4.,4.}}));
    CHECK(c == IntervalVector(
      {{3.,3.},{2.,2.},{-oo,oo}}));

    cn.contract();
    CHECK(cn.nb_ctc_in_stack() == 0);
    CHECK(cn.nb_dom() == 12);

    CHECK(a == IntervalVector(
      {{3.,3.},{0.,0.},{4.,4.}}));
    CHECK(b == IntervalVector(
      {{0.,0.},{2.,2.},{4.,4.}}));
    CHECK(c == IntervalVector(
      {{3.,3.},{2.,2.},{8.,8.}}));
  }

  SECTION("Variables in CN")
  {
    CtcFunction ctc_plus(Function("a", "b", "c", "a+b-c"));
    Interval a1(0,1), b1(-1,1), a2(-1,1), b2(0,1), c(1.5,2);

    ContractorNetwork cn;
    IntervalVar a;
    CtcDeriv ctc_deriv;
    cn.add(ctc_plus, {a, b1, c}); 

    cn.contract({
      {a, a1}
    });    

    CHECK(a1 == Interval(0.5,1));
    CHECK(b1 == Interval(0.5,1));
    CHECK(c == Interval(1.5,2));

    a1 |= Interval(0,1);
    b1 |= Interval(-1,1);

    cn.contract({
      {a, a1}
    });

    CHECK(a1 == Interval(0.5,1));
    CHECK(b1 == Interval(0.5,1));
    CHECK(c == Interval(1.5,2));

    cn.contract({
      {a, a2},
      {b1, b2}
    });

    CHECK(a1 == Interval(0.5,1));
    CHECK(b1 == Interval(0.5,1));
    CHECK(a2 == Interval(0.5,1));
    CHECK(b2 == Interval(0.5,1));
    CHECK(c == Interval(1.5,2));

    CHECK(cn.nb_dom() == 3);
    CHECK(cn.nb_ctc() == 1);
  }

  SECTION("Variables and CtcFunction on heterogeneous variables")
  {
    IntervalVar a;
    IntervalVectorVar x(2);
    
    CtcFunction ctc_add(Function("b[2]", "c", "b[0]+b[1]-c"));

    ContractorNetwork cn;
    cn.add(ctc_add, {x,a});

    IntervalVector x1{{0,1},{-2,3}};
    Interval a1(1,20);
    cn.contract({
      {a, a1},
      {x, x1}
    });

    CHECK(x1[0] == Interval(0,1));
    CHECK(x1[1] == Interval(0,3));
    CHECK(a1 == Interval(1,4));

    IntervalVector x2(2);
    x2[0] = Interval(10,10.5);
    x2[1] = Interval(22,99);

    Interval a2(32,33);
    cn.contract({
      {x, x2},
      {a, a2}
    });

    //CHECK(a == Interval());
    //CHECK(x == IntervalVector(2));

    CHECK(x1[0] == Interval(0,1)); // remain unchanged
    CHECK(x1[1] == Interval(0,3)); // remain unchanged
    CHECK(a1 == Interval(1,4));    // remain unchanged
    CHECK(ApproxIntv(x2[0]) == Interval(10,10.5));
    CHECK(x2[1] == Interval(22,23));
    CHECK(a2 == Interval(32,33));
  }

  SECTION("IntervalVectorVar linked only with components")
  {
    Function f1("x[2]","(x[0]+1;x[1]+2)");
    ibex::CtcFwdBwd ctc_f1(f1);
    Function f2("x","(x+3)");
    ibex::CtcFwdBwd ctc_f2(f2);

    ContractorNetwork cn;
    IntervalVectorVar box(3);
    cn.add(ctc_f1,{box[0],box[1]});
    cn.add(ctc_f2,{box[2]});

    IntervalVector x({{-10,10},{-10,10},{-10,10}});
    cn.contract({{box,x}});

    CHECK(x[0] == Interval(-1));
    CHECK(x[1] == Interval(-2));
    CHECK(x[2] == Interval(-3));
  }
}