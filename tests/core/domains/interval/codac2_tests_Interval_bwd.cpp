/** 
 *  Codac tests
 *  
 *  Most of these tests come from the IBEX library (Gilles Chabert)
 *  See more: https://ibex-lib.readthedocs.io
 *  They have been revised to fit with Codac (v2)
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Gilles Chabert, (Simon Rohou)
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Interval.h>
#include <codac2_Interval_operations.h>
#include <codac2_Approx.h>
#include <limits>

using namespace std;
using namespace codac2;

#define PI 3.1415926535897932384626433832795028L // todo: use std::numbers::pi with C++20

const double MAX_DOUBLE = std::numeric_limits<double>::max();

void CHECK_bwd_trigo(const Interval& y, const Interval& x, const Interval& expected_x)
{
  Interval _x;
  _x = x; bwd_sin(y,_x); CHECK(Approx(_x, 1e-5) == expected_x);
  _x = -x; bwd_sin(-y,_x); CHECK(Approx(_x, 1e-5) == -expected_x);
  _x = x-Interval::half_pi(); bwd_cos(y,_x); CHECK(Approx(_x, 1e-5) == expected_x-Interval::half_pi());
  _x = x+Interval::half_pi(); bwd_cos(-y,_x); CHECK(Approx(_x, 1e-5) == expected_x+Interval::half_pi());
}

void CHECK_bwd_pow(const Interval& y, const Interval& x, const Interval& expected_x, int p)
{
  Interval _x;
  _x = x; bwd_pow(y,_x,p); CHECK(Approx(_x,1e-5) == expected_x);
  _x = -x; bwd_pow((p%2 == 0) ? y : -y,_x,p); CHECK(Approx(_x,1e-5) == -expected_x);
}

void CHECK_bwd_mul(const Interval& y,
  const Interval& x1, const Interval& x2, const Interval& expected_x1, const Interval& expected_x2)
{
  Interval _x1, _x2;

  _x1 = x1; _x2 = x2; bwd_mul(y,_x1,_x2);
  CHECK(Approx(_x1) == expected_x1);
  CHECK(Approx(_x2) == expected_x2);

  _x1 = x1; _x2 = x2; bwd_mul(y,_x2,_x1);
  CHECK(Approx(_x1) == expected_x1);
  CHECK(Approx(_x2) == expected_x2);

  _x1 = -x1; _x2 = x2; bwd_mul(-y,_x2,_x1);
  CHECK(Approx(_x1) == -expected_x1);
  CHECK(Approx(_x2) == expected_x2);

  _x1 = x1; _x2 = -x2; bwd_mul(-y,_x2,_x1);
  CHECK(Approx(_x1) == expected_x1);
  CHECK(Approx(_x2) == -expected_x2);
}

void CHECK_bwd_div(const Interval& y,
  const Interval& x1, const Interval& x2, const Interval& expected_x1, const Interval& expected_x2)
{
  Interval _x1, _x2;

  _x1 = x1; _x2 = x2; bwd_div(y,_x1,_x2);
  CHECK(Approx(_x1) == expected_x1);
  CHECK(Approx(_x2) == expected_x2);

  _x1 = -x1; _x2 = -x2; bwd_div(y,_x1,_x2);
  CHECK(Approx(_x1) == -expected_x1);
  CHECK(Approx(_x2) == -expected_x2);

  _x1 = -x1; _x2 = x2; bwd_div(-y,_x1,_x2);
  CHECK(Approx(_x1) == -expected_x1);
  CHECK(Approx(_x2) == expected_x2);

  _x1 = x1; _x2 = -x2; bwd_div(-y,_x1,_x2);
  CHECK(Approx(_x1) == expected_x1);
  CHECK(Approx(_x2) == -expected_x2);
}

void CHECK_bwd_max(const Interval& y,
  const Interval& x1, const Interval& x2, const Interval& expected_x1, const Interval& expected_x2)
{
  Interval _x1, _x2;

  _x1 = x1; _x2 = x2; bwd_max(y,_x1,_x2);
  CHECK(_x1 == expected_x1);
  CHECK(_x2 == expected_x2);

  _x1 = x1; _x2 = x2; bwd_max(y,_x2,_x1);
  CHECK(_x1 == expected_x1);
  CHECK(_x2 == expected_x2);

  _x1 = -x1; _x2 = -x2; bwd_min(-y,_x1,_x2);
  CHECK(_x1 == -expected_x1);
  CHECK(_x2 == -expected_x2);

  _x1 = -x1; _x2 = -x2; bwd_min(-y,_x2,_x1);
  CHECK(_x1 == -expected_x1);
  CHECK(_x2 == -expected_x2);
}

void CHECK_bwd_atan(const Interval& y, const Interval& x, const Interval& expected_x)
{
  Interval _x;

  _x = x; bwd_atan(y,_x);
  CHECK(_x == expected_x);

  _x = -x; bwd_atan(-y,_x);
  CHECK(_x == -expected_x);
}

void CHECK_bwd_add(const Interval& y,
  const Interval& x1, const Interval& x2, const Interval& expected_x1, const Interval& expected_x2)
{
  Interval _x1, _x2;

  _x1 = x1; _x2 = x2; bwd_add(y,_x1,_x2);
  CHECK(_x1 == expected_x1);
  CHECK(_x2 == expected_x2);

  _x1 = x1; _x2 = x2; bwd_add(y,_x2,_x1);
  CHECK(_x1 == expected_x1);
  CHECK(_x2 == expected_x2);

  _x1 = -x1; _x2 = -x2; bwd_add(-y,_x1,_x2);
  CHECK(_x1 == -expected_x1);
  CHECK(_x2 == -expected_x2);

  _x1 = -x1; _x2 = -x2; bwd_add(-y,_x2,_x1);
  CHECK(_x1 == -expected_x1);
  CHECK(_x2 == -expected_x2);
}

void CHECK_bwd_sub(const Interval& y,
  const Interval& x1, const Interval& x2, const Interval& expected_x1, const Interval& expected_x2)
{
  Interval _x1, _x2;

  _x1 = x1; _x2 = x2; bwd_sub(y,_x1,_x2);
  CHECK(_x1 == expected_x1);
  CHECK(_x2 == expected_x2);

  _x1 = -x1; _x2 = -x2; bwd_sub(-y,_x1,_x2);
  CHECK(_x1 == -expected_x1);
  CHECK(_x2 == -expected_x2);

  _x1 = x1; _x2 = x2; bwd_sub(-y,_x2,_x1);
  CHECK(_x1 == expected_x1);
  CHECK(_x2 == expected_x2);

  _x1 = -x1; _x2 = -x2; bwd_sub(y,_x2,_x1);
  CHECK(_x1 == -expected_x1);
  CHECK(_x2 == -expected_x2);
}

void CHECK_bwd_imod(double p,
  const Interval& x1, const Interval& x2, const Interval& expected_x1, const Interval& expected_x2)
{
  Interval _x1, _x2;
  _x1 = x1; _x2 = x2; bwd_imod(_x1,_x2,p);
  CHECK(Approx(_x1) == expected_x1);
  CHECK(Approx(_x2) == expected_x2);
}

TEST_CASE("Interval bwd operations")
{
  const double pi_lb = Interval::pi().lb();
  const double pi_ub = Interval::pi().ub();

  CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(0,pi_ub/2.0),          Interval(0.5,1.5));
  CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(0,5*pi_ub/2.0),        Interval(0.5,2*pi_ub+1.5));
  CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-2*pi_ub,pi_ub/2.0),   Interval(-2*pi_ub+0.5,1.5));
  CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-2*pi_ub,5*pi_ub/2.0), Interval(-2*pi_ub+0.5,2*pi_ub+1.5));
  CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-pi_lb-0.4,pi_ub/2.0), Interval(0.5,1.5));
  CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-pi_ub-0.5,pi_ub/2.0), Interval(-pi_ub-0.5,1.5));
  CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-3*pi_ub/2,pi_ub/2.0), Interval(-pi_ub-1.5,1.5));
  CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(0.5,pi_lb-1.6),        Interval(0.5,1.5));
  CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(0.5,pi_ub-1.5),        Interval(0.5,pi_ub-1.5));
  CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(0.5,pi_ub),            Interval(0.5,pi_ub-0.5));
  CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-pi_ub-0.5,pi_ub-1.5), Interval(-pi_ub-0.5,pi_ub-1.5));
  CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-3*pi_ub/2,pi_ub),     Interval(-pi_ub-1.5,pi_ub-0.5));
  CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-pi_ub/2,pi_ub/2.0),   Interval(0.5,1.5));
  CHECK_bwd_trigo(Interval(2,3),          Interval(-oo,oo),               Interval::empty());
  CHECK_bwd_trigo(Interval(sin(0.5),1.0), Interval(0,pi_ub/2.0),          Interval(0.5,pi_ub/2.0));
  CHECK_bwd_trigo(Interval(sin(0.5),1.0), Interval(0,5*pi_ub/2.0),        Interval(0.5,5*pi_ub/2.0));
  CHECK_bwd_trigo(Interval(sin(0.5),1.0), Interval(-2*pi_ub,pi_ub/2.0),   Interval(-2*pi_ub+0.5,pi_ub/2.0));
  CHECK_bwd_trigo(Interval(sin(0.5),1.0), Interval(-2*pi_ub,5*pi_ub/2.0), Interval(-2*pi_ub+0.5,5*pi_ub/2.0));
  CHECK_bwd_trigo(Interval(sin(0.5),1.0), Interval(-pi_lb-0.4,pi_ub/2.0), Interval(0.5,pi_ub/2.0));
  CHECK_bwd_trigo(Interval(sin(0.5),1.0), Interval(-pi_ub-0.5,pi_ub/2.0), Interval(-pi_ub-0.5,pi_ub/2.0));
  CHECK_bwd_trigo(Interval(sin(0.5),1.0), Interval(-3*pi_ub/2,pi_ub/2.0), Interval(-3*pi_ub/2,pi_ub/2.0));
  CHECK_bwd_trigo(Interval(sin(0.5),1.0), Interval(0.5,pi_ub),            Interval(0.5,pi_ub-0.5));
  CHECK_bwd_trigo(Interval(sin(0.5),1.0), Interval(-3*pi_ub/2,pi_ub),     Interval(-3*pi_ub/2,pi_ub-0.5));
  CHECK_bwd_trigo(Interval(1.0),          Interval(4.0,6.0),              Interval::empty());

  Interval x;

  x = Interval(0,4);    bwd_sqr(Interval(1,9),x);   CHECK(x == Interval(1,3));
  x = Interval(0,2);    bwd_sqr(Interval(1,9),x);   CHECK(x == Interval(1,2));
  x = Interval(-4,2);   bwd_sqr(Interval(1,9),x);   CHECK(x == Interval(-3,2));
  x = Interval(-4,-3);  bwd_sqr(Interval(1,9),x);   CHECK(x == Interval(-3,-3));
  x = Interval(-4,1);   bwd_sqr(Interval(-oo,9),x); CHECK(x == Interval(-3,1));
  x = Interval(-1,5);   bwd_sqr(Interval(4,9),x);   CHECK(x == Interval(2,3));
  x = Interval(-oo,oo); bwd_sqr(Interval(-4,-2),x); CHECK(x == Interval::empty());

  x = Interval(-oo,oo);     bwd_log(Interval(-oo,oo),x); CHECK(x == Interval(0,oo));
  x = Interval(-oo,oo);     bwd_log(Interval(-oo,0),x);  CHECK(x == Approx(Interval(0,1)));
  x = Interval(0,1);        bwd_log(Interval(0,oo),x);   CHECK(x == Approx(Interval(1)));
  x = Interval(-1,3);       bwd_log(Interval(0,1),x);    CHECK(x == Approx(Interval(1,::exp(1))));
  x = Interval(-1,3);       bwd_log(Interval(-oo,1),x);  CHECK(x == Approx(Interval(0,::exp(1))));
  x = Interval(-1,2);       bwd_log(Interval(-oo,1),x);  CHECK(x == Interval(0,2));
  x = Interval(3,4);        bwd_log(Interval(-oo,1),x);  CHECK(x == Interval::empty());
  x = Interval(-0.01,0.01); bwd_log(Interval(-1,1),x);   CHECK(x == Interval::empty());

  CHECK_bwd_pow(Interval(16,81),  Interval(1,4),    Interval(2,3), 4);
  CHECK_bwd_pow(Interval(16,81),  Interval(3,4),    Interval(3,3), 4);
  CHECK_bwd_pow(Interval(16,81),  Interval(-1,4),   Interval(2,3), 4);
  CHECK_bwd_pow(Interval(16,81),  Interval(-2,4),   Interval(-2,3), 4);
  CHECK_bwd_pow(Interval(16,81),  Interval(-5,4),   Interval(-3,3), 4);
  CHECK_bwd_pow(Interval(16,81),  Interval(1,1),    Interval::empty(), 4);
  CHECK_bwd_pow(Interval(16,81),  Interval(4,4),    Interval::empty(), 4);
  CHECK_bwd_pow(Interval(8,27),   Interval(1,4),    Interval(2,3), 3);
  CHECK_bwd_pow(Interval(8,27),   Interval(3,4),    Interval(3,3), 3);
  CHECK_bwd_pow(Interval(8,27),   Interval(-5,4),   Interval(2,3), 3);
  CHECK_bwd_pow(Interval(-8,27),  Interval(-5,4),   Interval(-2,3), 3);
  CHECK_bwd_pow(Interval(-27,27), Interval(-5,4),   Interval(-3,3), 3);
  CHECK_bwd_pow(Interval(8,27),   Interval(1,1),    Interval::empty(), 3);
  CHECK_bwd_pow(Interval(8,27),   Interval(4,4),    Interval::empty(), 3);
  CHECK_bwd_pow(Interval(-27,-8), Interval(-1,-1),  Interval::empty(), 3);
  CHECK_bwd_pow(Interval(-27,-8), Interval(-4,-4),  Interval::empty(), 3);
  CHECK_bwd_pow(Interval(0,1),    Interval(-10,10), Interval(-10,10), -2);

  CHECK_bwd_mul(Interval(1,2),Interval(0.1,2.0),Interval(-10,2),Interval(0.5,2.0),Interval(0.5,2));
  CHECK_bwd_mul(Interval::empty(),Interval(0.1,2.0),Interval(-10,2),Interval::empty(),Interval::empty());
  CHECK_bwd_mul(Interval(0,0),Interval(0.1,2.0),Interval(-10,2),Interval(0.1,2.0),Interval(0,0));
  CHECK_bwd_mul(Interval(0,0),Interval(-1,1),Interval(-1,1),Interval(-1,1),Interval(-1,1));
  CHECK_bwd_mul(Interval(1,1),Interval(0,10),Interval(0,10),Interval(0.1,10.0),Interval(0.1,10.0));

  CHECK_bwd_div(Interval(1,2), Interval(0,1), Interval(2,3), Interval::empty(), Interval::empty());
  CHECK_bwd_div(Interval(1,2), Interval(0,1), Interval(1,3), 1, 1);
  CHECK_bwd_div(Interval(1,2), Interval(1,3), Interval(0,1), Interval(1,2), Interval(0.5,1));
  CHECK_bwd_div(Interval(-1,1), Interval(-2,2), Interval(0,1), Interval(-1,1), Interval(0,1));
  CHECK_bwd_div(Interval(-1,1), Interval(-2,2), 0, 0, 0);
  CHECK_bwd_div(0, Interval(-2,2), Interval(-2,2), 0, Interval(-2,2));
  CHECK_bwd_div(Interval(0,oo), Interval(0,1), Interval(-1,0), 0, Interval(-1,0));
  CHECK_bwd_div(Interval(codac2::next_float(0.),oo), Interval(0,1), Interval(-1,0), 0, 0);
  CHECK_bwd_div(Interval(0,oo), Interval(codac2::next_float(0.),1), Interval(-1,0), Interval::empty(), Interval::empty());

  CHECK_bwd_max(Interval::empty(), Interval(-2,-1), Interval(-2,3), Interval::empty(), Interval::empty());
  CHECK_bwd_max(Interval(0,1), Interval(-2,-1), Interval(-2,3), Interval(-2,-1), Interval(0,1));
  CHECK_bwd_max(Interval(0,1), Interval(-2,-1), Interval(2,3), Interval::empty(), Interval::empty());
  CHECK_bwd_max(Interval(0,1), Interval(-2,0), Interval(-2,3), Interval(-2,0), Interval(-2,1));
  CHECK_bwd_max(Interval(0,1), Interval(-2,2), Interval(-2,3), Interval(-2,1), Interval(-2,1));

  Interval a, y;

  a = Interval(PI/6.,PI/3.); y = Interval(.5,10.); x = Interval(.5,2.);             
  bwd_atan2(a,y,x); CHECK(Approx(y) == Interval(.5,2.*sqrt(3.))); CHECK(Approx(x) == Interval(.5,2.));
  a = Interval(PI/6.,PI/3.); y = Interval(.5,2.); x = Interval(.5,10.);            
  bwd_atan2(a,y,x); CHECK(Approx(y) == Interval(.5,2.)); CHECK(Approx(x) == Interval(.5,2.*sqrt(3.)));
  a = Interval(-PI/4.,PI/4.); y = Interval(1.,2.); x = Interval(.5,2.);             
  bwd_atan2(a,y,x); CHECK(Approx(y) == Interval(1.,2.)); CHECK(Approx(x) == Interval(1.,2.));
  a = Interval(-PI/2.,0.); y = Interval(.5,2.); x = Interval(.5,10.);            
  bwd_atan2(a,y,x); CHECK(Approx(y) == Interval::empty()); CHECK(Approx(x) == Interval::empty());
  a = Interval(2.*PI,3.*PI); y = Interval(-.5,2.); x = Interval(.5,10.);            
  bwd_atan2(a,y,x); CHECK(Approx(y) == Interval::empty()); CHECK(Approx(x) == Interval::empty());
  a = Interval(2*PI/3.,5.*PI/6.); y = Interval(0.,100.); x = Interval(-20.,-sqrt(3.)/2.); 
  bwd_atan2(a,y,x); CHECK(Approx(y,1e-10) == Interval(.5,20.*sqrt(3.))); CHECK(Approx(x) == Interval(-20.,-sqrt(3.)/2));
  a = Interval(-3*PI/4.,-2*PI/3.); y = Interval(-sqrt(3.)/2.,2.); x = Interval(-sqrt(2.)/2.,0.);   
  bwd_atan2(a,y,x); CHECK(Approx(y) == Interval(-sqrt(3.)/2.,0.)); CHECK(Approx(x) == Interval(-sqrt(2.)/2.,0.));
  a = Interval(-3*PI/4.,-2*PI/3.); y = Interval(-sqrt(3.)/2.,2.); x = Interval(-1.,-.5);           
  bwd_atan2(a,y,x); CHECK(Approx(y) == Interval(-sqrt(3.)/2.,-.5)); CHECK(Approx(x) == Interval(-sqrt(3.)/2.,-.5));
  a = Interval(-3*PI/4.,-PI/4.); y = Interval(-5.,-.5); x = Interval(-oo,oo);            
  bwd_atan2(a,y,x); CHECK(Approx(y) == Interval(-5.,-.5)); CHECK(Approx(x) == Interval(-5.,5.));
  a = Interval(-PI/3.,PI/4.); y = Interval(-oo,oo); x = Interval(sqrt(3.)/2.);       
  bwd_atan2(a,y,x); CHECK(Approx(y) == Interval(-1.5,sqrt(3.)/2.)); CHECK(Approx(x) == Interval(sqrt(3.)/2.));
  a = Interval::half_pi(); y = Interval(1); x = 0.;                          
  bwd_atan2(a,y,x); CHECK(Approx(y) == 1.); CHECK(Approx(x) == 0.);

  x = Interval(0,15);   bwd_sqrt(Interval(1,3), x);   CHECK(x == Interval(1,9));
  x = Interval(0,2);    bwd_sqrt(Interval(1,3), x);   CHECK(x == Interval(1,2));
  x = Interval(-4,2);   bwd_sqrt(Interval(1,3), x);   CHECK(x == Interval(1,2));
  x = Interval(-4,-3);  bwd_sqrt(Interval(1,3), x);   CHECK(x == Interval::empty());
  x = Interval(-4,1);   bwd_sqrt(Interval(-oo,9), x); CHECK(x == Interval(0,1));
  x = Interval(-1,5);   bwd_sqrt(Interval(2,5), x);   CHECK(x == Interval(4,5));
  x = Interval(-oo,oo); bwd_sqrt(Interval(-4,-2), x); CHECK(x == Interval::empty());

  x = Interval(-1.,3.);       bwd_atan(Interval(0.,PI/6.),x);             CHECK(x == tan(Interval(0.,PI/6.)));
  x = Interval(0,5*PI/2.0); bwd_atan(Interval(-PI,1.5),x);              CHECK(x == Interval(0,5*PI/2.0));
  x = Interval(.2,.5);        bwd_atan(Interval(0.,PI/6.),x);             CHECK(x == Interval(.2,.5));
  x = Interval(-100,100);     bwd_atan(Interval(-PI/2-0.1,PI/2+0.1),x); CHECK(x == Interval(-100,100));
  x = Interval(-100,100);     bwd_atan(Interval(PI/2+0.1,PI),x);        CHECK(x == Interval::empty());
  x = Interval(-100,100);     bwd_atan(Interval(-PI,-PI/2-0.1),x);      CHECK(x == Interval::empty());

  x = Interval(-oo,oo); bwd_atan(Interval(-PI/4,PI/2.), x); CHECK(x.lb() == -1); CHECK(x.ub() > 1000);
  x = Interval(-oo,oo); bwd_atan(Interval(-PI/2,PI/4.), x); CHECK(x.ub() == +1); CHECK(x.lb() < -1000);

  CHECK_bwd_add(Interval(1,3),Interval(1,2),Interval(-10,5),Interval(1,2),Interval(-1,2));
  CHECK_bwd_add(Interval::empty(),Interval(0.1,2.0),Interval(-10,2),Interval::empty(),Interval::empty());
  CHECK_bwd_add(Interval(0,0),Interval(0,0),Interval(.1,5),Interval::empty(),Interval::empty());
  CHECK_bwd_add(Interval(0,0),Interval(-1,1),Interval(-1,1),Interval(-1,1),Interval(-1,1));
  CHECK_bwd_add(Interval(-1,1),Interval(1,2),Interval(-10,5),Interval(1,2),Interval(-3,0));

  CHECK_bwd_sub(Interval(1,3),Interval(1,2),Interval(-10,5),Interval(1,2),Interval(-2,1));
  CHECK_bwd_sub(Interval::empty(),Interval(0.1,2.0),Interval(-10,2),Interval::empty(),Interval::empty());
  CHECK_bwd_sub(Interval(0,0),Interval(0,0),Interval(.1,5),Interval::empty(),Interval::empty());
  CHECK_bwd_sub(Interval(0,0),Interval(-1,1),Interval(-1,1),Interval(-1,1),Interval(-1,1));
  CHECK_bwd_sub(Interval(-1,1),Interval(1,2),Interval(-10,5),Interval(1,2),Interval(0,3));

  CHECK_bwd_imod(3.,Interval(3.,5.),Interval(1.,2.),Interval(4.,5.),Interval(1.,2.));
  CHECK_bwd_imod(2.,Interval(7.,8.),Interval(.5,2.),Interval(7.,8.),Interval(1.,2.));
  CHECK_bwd_imod(2.,Interval(7.,8.),Interval(0.,2.),Interval(7.,8.),Interval(0.,2.));
  CHECK_bwd_imod(2.*PI,Interval(2.*PI,3.*PI),Interval(PI/6,PI/2.),Interval(13.*PI/6.,5.*PI/2.),Interval(PI/6,PI/2.));
  CHECK_bwd_imod(2.*PI,Interval(3.*PI,4.*PI),Interval(PI/3,PI/2.),Interval::empty(),Interval::empty());
  CHECK_bwd_imod(2.*PI,Interval(3.*PI,4.*PI),Interval(0.,PI/2.),Interval(4*PI),Interval(0.));
  CHECK_bwd_imod(2.*PI,Interval(2.*PI,4.*PI),Interval(-PI/6,PI/2.),Interval(2.*PI,4.*PI),Interval(-PI/6,PI/2.));
  CHECK_bwd_imod(2.*PI,Interval(7.*PI/4.,8.*PI/3),Interval(-PI/2,PI/2.),Interval(7.*PI/4.,5.*PI/2.),Interval(-PI/4,PI/2.));

  x = Interval(-oo,oo);        bwd_floor(Interval::empty(),x);   CHECK(x == Interval::empty());
  x = Interval(-oo,-0.000001); bwd_floor(Interval(-oo,-1),x);        CHECK(x == Interval(-oo,-0.000001));
  x = Interval(-oo, 0.000001); bwd_floor(Interval(-oo,-1),x);        CHECK(x == Interval(-oo,0));
  x = Interval(-oo, 0.000001); bwd_floor(Interval(-oo,-0.000001),x); CHECK(x == Interval(-oo,0));
  x = Interval(0.000001,oo);   bwd_floor(Interval(1,oo),x);          CHECK(x == Interval(1,oo));
  x = Interval(-0.000001,oo);  bwd_floor(Interval(1,oo),x);          CHECK(x == Interval(1,oo));
  x = Interval(-0.000001,oo);  bwd_floor(Interval(0.000001,oo),x);   CHECK(x == Interval(1,oo));
  x = Interval(0.01,2.99);     bwd_floor(Interval(1,2),x);           CHECK(x == Interval(1,2.99));
  x = Interval(2.99,3.01);     bwd_floor(Interval(1,2),x);           CHECK(x == Interval(2.99,3));
  x = Interval(3.01,3.99);     bwd_floor(Interval(1,2),x);           CHECK(x == Interval::empty());
  x = Interval(0.01,2.99);     bwd_floor(Interval(0.01,2.99),x);     CHECK(x == Interval(1,2.99));

  x = Interval(-oo,oo);        bwd_ceil(Interval::empty(),x);   CHECK(x == Interval::empty());
  x = Interval(-oo,-0.000001); bwd_ceil(Interval(-oo,-1),x);        CHECK(x == Interval(-oo,-1));
  x = Interval(-oo,0.000001);  bwd_ceil(Interval(-oo,-1),x);        CHECK(x == Interval(-oo,-1));
  x = Interval(-oo,0.000001);  bwd_ceil(Interval(-oo,-0.000001),x); CHECK(x == Interval(-oo,-1));
  x = Interval(0.000001,oo);   bwd_ceil(Interval(1,oo),x);          CHECK(x == Interval(0.000001,oo));
  x = Interval(-0.000001,oo);  bwd_ceil(Interval(1,oo),x);          CHECK(x == Interval(0,oo));
  x = Interval(-0.000001,oo);  bwd_ceil(Interval(0.000001,oo),x);   CHECK(x == Interval(0,oo));
  x = Interval(0.01,2.99);     bwd_ceil(Interval(1,2),x);           CHECK(x == Interval(0.01,2));
  x = Interval(-0.01,0.01);    bwd_ceil(Interval(1,2),x);           CHECK(x == Interval(0,0.01));
  x = Interval(-1.01,-0.01);   bwd_ceil(Interval(1,2),x);           CHECK(x == Interval::empty());
  x = Interval(0.01,2.99);     bwd_ceil(Interval(0.01,2.99),x);     CHECK(x == Interval(0.01,2));
  
  x = Interval(-1.57079632679489678, 1.1780972450961728626);
  CHECK(!(tan(x).is_empty()));
}

#include <codac2_IntervalMatrix.h>
#include <codac2_directed_ctc.h>

TEST_CASE("bwd mul operations")
{
  double delta = 0.1;
  IntervalMatrix A(2,2);
  Interval deltaM(-delta,delta);
  A(0,0) = 1+deltaM;
  A(0,1) = deltaM;
  A(1,0) = deltaM;
  A(1,1) = 1+deltaM;

  Vector b(2,1.0);

  IntervalVector x(2,Interval(-10,10));

  double _M[16]={1+delta,   0,       0,     delta,
             0,    1-delta,    0,    -delta,
             0,     -delta,    0,   1-delta,
             0,      delta, 1+delta,    0};
  Matrix M(4,4,_M);
  Matrix invM = M.inverse();

  Vector b2(4,1.0);
  Vector bounds = invM * b2;
  double _x2[2][2]={ {bounds[0],bounds[1]}, {bounds[2],bounds[3]} };
  IntervalVector x2(2,_x2);

  MulOp::bwd(b,A,x);

  cout << x << endl;
  cout << x2 << endl;
}