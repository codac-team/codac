/** 
 *  Codac tests
 *  
 *  Most of these tests come from the IBEX library (Gilles Chabert)
 *  See more: https://ibex-lib.readthedocs.io
 *  They have been revised to fit with Codac (v2)
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Gilles Chabert, Simon Rohou
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

const double MAX_DOUBLE = std::numeric_limits<double>::max();

void CHECK_add(const Interval& x, const Interval& y, const Interval& expected)
{
  CHECK((x + y) == expected);
  CHECK((y + x) == expected); // symmetrical case
  CHECK((Interval(x) += y) == expected); // +=operator
  CHECK((Interval(y) += x) == expected); // +=operator in the other direction
  CHECK(((-x)-y) == -expected); // substraction
  CHECK(((-y)-x) == -expected); // symmetrical case
  CHECK((Interval(-x) -= y) == -expected); // -=operator
  CHECK((Interval(-y) -= x) == -expected); // -=operator in the other direction
}

void CHECK_add_scalar(const Interval& x, double y, const Interval& expected)
{
  CHECK((x+y) == expected);
  CHECK((y+x) == expected);
  CHECK((Interval(x) += y) == expected); // test the +=operator
  CHECK(((-x)-y) == -expected);
  CHECK(((-y)-x) == -expected);
  CHECK((Interval(-x) -= y) == -expected); // test the -=operator
}

void CHECK_mul(const Interval& x, const Interval& y, const Interval& expected)
{
  CHECK(x * y == expected);
  CHECK(y * x == expected); // symmetrical case
  CHECK((Interval(x) *= y) == expected); // *=operator
}

void CHECK_mul_scalar(const Interval& x, double y, const Interval& expected)
{
  CHECK(x * y == expected);
  CHECK(y * x == expected); // symmetrical case
  CHECK((Interval(x) *= y) == expected); // *=operator
}

void CHECK_div(const Interval& x, const Interval& y, const Interval& expected)
{
  CHECK(x / y == expected);
  CHECK((Interval(x) /= y) == expected); // /=operator
}

void CHECK_div_scalar(const Interval& x, double y, const Interval& expected)
{
  CHECK(x / y == expected);
  CHECK((Interval(x) /= y) == expected); // /=operator
}

void CHECK_trigo(const Interval& x, const Interval& expected)
{
  CHECK(Approx(sin(x), 1e-9) == expected);
  CHECK(Approx(sin(Interval::pi()-x), 1e-9) == expected);
  CHECK(Approx(sin(x+Interval::two_pi()), 1e-9) == expected);
  CHECK(Approx(sin(-x), 1e-9) == -expected);
  CHECK(Approx(cos(x-Interval::half_pi()), 1e-9) == expected);
  CHECK(Approx(cos(Interval::half_pi()-x), 1e-9) == expected);
  CHECK(Approx(cos(x+Interval::half_pi()), 1e-9) == -expected);
  CHECK(Approx(cos(x+Interval::two_pi()-Interval::half_pi()), 1e-9) == expected);
}

void CHECK_pow(const Interval& x, int p, const Interval& expected, double eps = 0.)
{
  CHECK(Approx(pow(x,p),eps) == expected);
  CHECK(Approx(pow(-x,p),eps) == ((p%2==0) ? expected : -expected));
}

TEST_CASE("Interval operations")
{
  CHECK(0 < codac2::next_float(0));
  CHECK(0 > codac2::previous_float(0));
  CHECK(1 < codac2::next_float(1));
  CHECK(1 > codac2::previous_float(1));
  CHECK(oo == codac2::next_float(oo));
  CHECK(-oo == codac2::previous_float(-oo));
  CHECK(-MAX_DOUBLE >= codac2::next_float(-oo));
  CHECK(MAX_DOUBLE <= codac2::previous_float(oo));
  CHECK(oo > MAX_DOUBLE);
  CHECK(-oo < -MAX_DOUBLE);
  CHECK(-Interval(0,1) == Interval(-1,0));
  CHECK(-Interval(-oo,oo) == Interval(-oo,oo));
  CHECK(-Interval(-oo,0) == Interval(0,oo));
  CHECK(-Interval(-oo,1) == Interval(-1,oo));

  CHECK_add(Interval::empty(), Interval(0,1), Interval::empty());
  CHECK_add(Interval(0,1), Interval::empty(), Interval::empty());
  CHECK_add(Interval(-oo,1), Interval(0,1), Interval(-oo, 2));
  CHECK_add(Interval(1,oo), Interval(0,1), Interval(1,oo));
  CHECK_add(Interval(-oo,oo), Interval(0,1), Interval(-oo,oo));
  CHECK_add(Interval(MAX_DOUBLE,oo), 1, Interval(MAX_DOUBLE,oo));
  CHECK_add(Interval(MAX_DOUBLE,oo), -1, Interval(codac2::previous_float(MAX_DOUBLE),oo));
  CHECK_add(Interval(MAX_DOUBLE,oo), Interval(MAX_DOUBLE,oo), Interval(MAX_DOUBLE,oo));
  CHECK_add(Interval(MAX_DOUBLE,oo), -oo, Interval::empty());
  CHECK_add(Interval(MAX_DOUBLE,oo), oo, Interval::empty());
  CHECK_add(Interval(-oo,-MAX_DOUBLE), 1, Interval(-oo,codac2::next_float(-MAX_DOUBLE)));
  CHECK_add(Interval(-oo,-MAX_DOUBLE), -1, Interval(-oo,-MAX_DOUBLE));
  CHECK_add(Interval(-oo,-MAX_DOUBLE), Interval(-oo,-MAX_DOUBLE), Interval(-oo,-MAX_DOUBLE));

  CHECK_add_scalar(Interval::empty(), oo, Interval::empty());
  CHECK_add_scalar(Interval::empty(), 0, Interval::empty());
  CHECK_add_scalar(Interval(0,1), 1, Interval(1,2));
  CHECK_add_scalar(Interval(0,1), -oo, Interval::empty());
  CHECK_add_scalar(Interval(0,1), oo, Interval::empty());
  CHECK_add_scalar(Interval(-oo,1), 1, Interval(-oo,2));

  CHECK_mul(Interval::empty(), Interval(0,1), Interval::empty());
  CHECK_mul(0, Interval(-oo,oo), 0);
  CHECK_mul(Interval(-1,1), Interval(-oo,0), Interval(-oo,oo));
  CHECK_mul(Interval(-oo,-1), Interval(-1,0), Interval(0,oo));
  CHECK_mul(Interval(-oo, 1), Interval(-1,0), Interval(-1,oo));
  CHECK_mul(Interval(0, 1), Interval(1,oo), Interval(0,oo));
  CHECK_mul(Interval(0, 1), Interval(-1,oo), Interval(-1,oo));
  CHECK_mul(Interval(-oo,-1), Interval(0,1), Interval(-oo,0));
  CHECK_mul(Interval(-oo, 1), Interval(0,1), Interval(-oo,1));
  CHECK_mul(Interval(0, 1), Interval(-oo,-1), Interval(-oo,0));
  CHECK_mul(Interval(0, 1), Interval(-oo,1), Interval(-oo,1));
  CHECK_mul(Interval(1,oo), Interval(0,1), Interval(0,oo));
  CHECK_mul(Interval(-1,oo), Interval(0,1), Interval(-1,oo));
  CHECK_mul(Interval(1,2), Interval(1,2), Interval(1,4));
  CHECK_mul(Interval(1,2), Interval(-2,3), Interval(-4,6));
  CHECK_mul(Interval(-1,1), Interval(-2,3), Interval(-3,3));
  CHECK_mul_scalar(Interval(1,2), -oo, Interval::empty());
  CHECK_mul_scalar(Interval(1,2), oo, Interval::empty());
  CHECK_mul_scalar(Interval(1,2), -1, Interval(-2,-1));

  CHECK_div(Interval::empty(), Interval(0,1), Interval::empty());
  CHECK_div(0, 0, Interval::empty());
  CHECK_div(Interval(1,2), 0, Interval::empty());
  CHECK_div(Interval(-oo,oo), 0, Interval::empty());
  CHECK_div(0, Interval(0,1), 0);
  CHECK_div(0, Interval(-oo,oo), 0);
  CHECK_div(Interval(6,12), Interval(2,3), Interval(2,6));
  CHECK_div(Interval(6,12), Interval(-3,-2), Interval(-6,-2));
  CHECK_div(Interval(6,12), Interval(-2,3), Interval(-oo,oo));
  CHECK_div(Interval(-oo,-1), Interval(-1,0), Interval(1,oo));
  CHECK_div(Interval(-oo,-1), Interval(0,1), Interval(-oo,-1));
  CHECK_div(Interval(1,oo), Interval(-1,0), Interval(-oo,-1));
  CHECK_div(Interval(1,oo), Interval(0,1), Interval(1,oo));
  CHECK_div(Interval(-1,1), Interval(-1,1), Interval(-oo,oo));
  CHECK_div_scalar(Interval(1,2), -oo, Interval::empty());
  CHECK_div_scalar(Interval(1,2), oo, Interval::empty());
  CHECK_div_scalar(Interval(1,2), -1, Interval(-2,-1));

  CHECK(sqrt(Interval(-oo,oo)) == Interval(0,oo));
  CHECK(sqrt(Interval(-oo,0)) == 0);
  CHECK(sqrt(Interval(-9,4)) == Interval(0,2));
  CHECK(sqrt(Interval(4,9)) == Interval(2,3));
  CHECK(sqrt(Interval(-9,-4)) == Interval::empty());
  CHECK(sqrt(Interval(-9,oo)) == Interval(0,oo));

  CHECK(log(Interval::empty()) == Interval::empty());
  CHECK(log(Interval(-oo,oo)) == Interval(-oo,oo));
  CHECK(log(Interval(0,oo)) == Interval(-oo,oo));
  CHECK(log(Interval(-oo,0)) ==  Interval::empty());
  CHECK(Approx(log(Interval(1,2))) == Interval(0,::log(2)));
  CHECK(Approx(log(Interval(-1,1))) == Interval(-oo,0));
  CHECK(Approx(log(Interval(0,1))) == Interval(-oo,0));
  CHECK(Approx(log(Interval(1,oo))) == Interval(0,oo));
  CHECK(log(Interval(0)) == Interval::empty());
  CHECK(log(Interval(-2,-1)) == Interval::empty());
  CHECK((log(Interval(0,codac2::next_float(0)))).ub() > -744.5);

  CHECK(exp(Interval::empty()) == Interval::empty());
  CHECK(exp(Interval(-oo,oo)) == Interval(0,oo));
  CHECK(Approx(exp(Interval(0,oo))) == Interval(1,oo));
  CHECK(Approx(exp(Interval(-oo,0))) == Interval(0,1));
  CHECK(Approx(exp(Interval(0,2))) == Interval(1,::exp(2)));
  CHECK(Approx(exp(Interval(-1,1))) == Interval(::exp(-1),::exp(1)));
  CHECK(exp(Interval(1.e100,1.e111)) == Interval(MAX_DOUBLE,oo));
  CHECK(exp(Interval(MAX_DOUBLE,oo)) == Interval(MAX_DOUBLE,oo));
  CHECK(Approx(exp(Interval(0,MAX_DOUBLE))) == Interval(1,oo));

  const double pi_lb = Interval::pi().lb();
  const double pi_ub = Interval::pi().ub();

  CHECK_trigo(Interval(-oo,oo), Interval(-1,1));
  CHECK_trigo(Interval::empty(), Interval::empty());
  CHECK_trigo(Interval(0,pi_ub/2.0), Interval(0,1));
  CHECK_trigo(Interval(0,pi_ub), Interval(0,1));
  CHECK_trigo(Interval(0,3*pi_ub/2.0), Interval(-1,1));
  CHECK_trigo(Interval(pi_lb,3*pi_ub/2.0), Interval(-1,0));
  CHECK_trigo(Interval(0.5,1.5), Interval(sin(0.5),sin(1.5)));
  CHECK_trigo(Interval(1.5,3), Interval(sin(3.0),1));
  CHECK_trigo(Interval(3,4), Interval(sin(4.0),sin(3.0)));
  CHECK_trigo(Interval(3,5), Interval(-1,sin(3.0)));
  CHECK_trigo(Interval(3,2*pi_ub+1.5), Interval(-1,sin(1.5)));
  CHECK_trigo(Interval(5,2*pi_ub+1.5), Interval(sin(5.0),sin(1.5)));
  CHECK_trigo(Interval(5,2*pi_ub+3), Interval(sin(5.0),1));

  CHECK(floor(Interval::empty()) == Interval::empty());
  CHECK(floor(Interval(-oo,-0.000001)) == Interval(-oo,-1));
  CHECK(floor(Interval(0.00000001,oo)) == Interval(0,oo));
  CHECK(floor(Interval(-oo,oo)) == Interval(-oo,oo));
  CHECK(floor(Interval(0.01,2.99)) == Interval(0,2));
  CHECK(floor(Interval(-0.01,2.99)) == Interval(-1,2));
  CHECK(floor(Interval(1.000000001,1.9999999999)) == 1.);
  CHECK(floor(Interval(1e8, MAX_DOUBLE)) == Interval(1e8,MAX_DOUBLE));

  CHECK(ceil(Interval::empty()) == Interval::empty());
  CHECK(ceil(Interval(-oo,-0.000001)) == Interval(-oo,0));
  CHECK(ceil(Interval(0.00000001,oo)) == Interval(1.0,oo));
  CHECK(ceil(Interval(-oo,oo)) == Interval(-oo,oo));
  CHECK(ceil(Interval(0.01,2.99)) == Interval(1,3));
  CHECK(ceil(Interval(-0.01,2.99)) == Interval(0,3));
  CHECK(ceil(Interval(1.000000001,1.9999999999)) == Interval(2,2));
  CHECK(ceil(Interval(1e8, MAX_DOUBLE)) == Interval(1e8,MAX_DOUBLE));

  CHECK(integer(Interval::empty()) == Interval::empty());
  CHECK(integer(Interval(-oo,-0.000001)) == Interval(-oo,-1));
  CHECK(integer(Interval(0.00000001,oo)) == Interval(1.0,oo));
  CHECK(integer(Interval(-oo,oo)) == Interval(-oo,oo));
  CHECK(integer(Interval(0.01,2.99)) == Interval(1,2));
  CHECK(integer(Interval(-0.01,2.99)) == Interval(0,2));
  CHECK(integer(Interval(1.000000001,1.9999999999)) == Interval::empty());
  CHECK(integer(Interval(1e8, MAX_DOUBLE)) == Interval(1e8,MAX_DOUBLE));

  CHECK(tan(Interval::pi()/2.0) == Interval(-oo,oo));
  {
    Interval x = tan(-Interval::pi());
    CHECK(x.diam() < 1e-8);
    CHECK(x.contains(0.));
  }

  CHECK(Approx(tan((3*Interval::pi()/4.0)|(5*Interval::pi()/4.0))) == Interval(-1,1));
  CHECK(tan(Interval(-oo,oo)) == Interval(-oo,oo));
  CHECK(Approx(tan((-Interval::pi()/4.0)|(Interval::pi()/4.0))) == Interval(-1,1));

  {
    // tan(pi/4,pi/2)=[1,+oo)
    Interval x = Interval(pi_lb/4.0,(1-1e-10)*pi_lb/2.0);
    Interval y = tan(x);
    CHECK(Approx(Interval(y.lb())) == Interval(1.0));
    CHECK(y.ub() > 1000);
  }
  {
    // tan(-pi/2,pi/4)=(-oo,1]
    Interval y = tan(Interval(-(1-1e-10)*pi_lb/2.0,pi_lb/4.0));
    CHECK(y.lb() < -1000);
    CHECK(y.ub() == 1.0);
  }

  CHECK(atan2(Interval(-oo,oo),Interval::empty()) == Interval::empty());
  CHECK(Approx(atan2(Interval(1),1)) == Interval::pi()/4.0);
  CHECK(Approx(atan2(Interval(-1),-1)) == -3*Interval::pi()/4.0);
  CHECK(Approx(atan2(Interval(-1),1)) == -Interval::pi()/4.0);
  CHECK(Approx(atan2(Interval(1),-1)) == 3*Interval::pi()/4.0);
  CHECK(Approx(atan2(Interval(0,oo),Interval(0,oo))) == Interval(0,1)*Interval::half_pi());
  CHECK(Approx(atan2(Interval(-oo,codac2::previous_float(0.0)),Interval(-oo,0))) == -(Interval::half_pi() | Interval::pi()));
  CHECK(Approx(atan2(Interval(-oo,0),Interval(0,oo))) == Interval(-1,0)*Interval::half_pi());
  CHECK(Approx(atan2(Interval(0,oo),Interval(-oo,0))) == (Interval::half_pi() | Interval::pi()));
  CHECK(Approx(atan2(Interval(1,oo),Interval(-1,1))) == (Interval::pi()/4.0 | 3*Interval::pi()/4.0));
  CHECK(Approx(atan2(Interval(-oo,-1),Interval(-1,1))) == -(Interval::pi()/4.0 | 3*Interval::pi()/4.0));
  CHECK(atan2(Interval(-1,1),Interval(1,oo)) == Interval(-1,1)*Interval::pi()/4.0);
  CHECK(atan2(Interval(-1,1),Interval(-oo,-1)) == Interval(-1,1)*Interval::pi());
  CHECK(atan2(Interval(-oo,oo),Interval(-oo,oo)) == Interval(-1,1)*Interval::pi());
  CHECK(atan2(Interval(0),0) == Interval::empty());

  CHECK_pow(Interval(-oo,oo), 4, Interval(0,oo));
  CHECK_pow(Interval::empty(), 4, Interval::empty());
  CHECK_pow(Interval(2,3), 4, Interval(16,81));
  CHECK_pow(Interval(-2,3), 4, Interval(0,81));
  CHECK_pow(Interval(-3,2), 4, Interval(0,81));
  CHECK_pow(Interval(2,oo), 4, Interval(16,oo));
  CHECK_pow(Interval(-oo,oo), 3, Interval(-oo,oo));
  CHECK_pow(Interval::empty(), 3, Interval::empty());
  CHECK_pow(Interval(2,3), 3, Interval(8,27));
  CHECK_pow(Interval(-2,3), 3, Interval(-8,27));
  CHECK_pow(Interval(-3,2), 3, Interval(-27,8));
  CHECK_pow(Interval(2,oo), 3, Interval(8,oo));
  CHECK_pow(Interval(-10,10), -2, Interval(1.0/100,oo), std::numeric_limits<double>::epsilon());

  CHECK(root(Interval(0,1),-1) == Interval(1.0,oo));
  CHECK(Approx(root(Interval(-27,-8),3)) == Interval(-3,-2));
  CHECK(root(Interval(-4,1),2) == Interval(0,1));
  CHECK(Approx(root(Interval(-8,1),3)) == Interval(-2,1));
}