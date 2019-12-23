#include "tests.h"
#include "tubex_tube_arithmetic.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Arithmetic")
{
  // todo: test tubes with different slicing

  SECTION("Tests scalar")
  {
    Interval domain(0.,10.);
    double dt = 0.1;
    double a = -0.8, b = 3.;

    Tube x(domain, dt, a), y(x, b);
    CHECK(x.codomain() == a);
    CHECK(y.codomain() == b);

    //const Tube cos(const Tube& x);
    CHECK(ApproxIntv(cos(x).codomain()) == cos(a));

    //const Tube sin(const Tube& x);
    CHECK(ApproxIntv(sin(x).codomain()) == sin(a));

    //const Tube abs(const Tube& x);
    CHECK(ApproxIntv(abs(x).codomain()) == fabs(a));

    //const Tube sqr(const Tube& x);
    CHECK(ApproxIntv(sqr(x).codomain()) == pow(a,2));

    //const Tube sqrt(const Tube& x);
    CHECK(ApproxIntv(sqrt(x).codomain()) == Interval::EMPTY_SET);
    a = 0.2;
    x.set(a);
    CHECK(ApproxIntv(sqrt(x).codomain()) == sqrt(a));

    //const Tube exp(const Tube& x);
    CHECK(ApproxIntv(exp(x).codomain()) == exp(a));

    //const Tube log(const Tube& x);
    CHECK(ApproxIntv(log(x).codomain()) == log(a));

    //const Tube tan(const Tube& x);
    CHECK(ApproxIntv(tan(x).codomain()) == tan(a));

    //const Tube acos(const Tube& x);
    CHECK(ApproxIntv(acos(x).codomain()) == acos(a));

    //const Tube asin(const Tube& x);
    CHECK(ApproxIntv(asin(x).codomain()) == asin(a));

    //const Tube atan(const Tube& x);
    CHECK(ApproxIntv(atan(x).codomain()) == atan(a));

    //const Tube cosh(const Tube& x);
    CHECK(ApproxIntv(cosh(x).codomain()) == cosh(a));

    //const Tube sinh(const Tube& x);
    CHECK(ApproxIntv(sinh(x).codomain()) == sinh(a));

    //const Tube tanh(const Tube& x);
    CHECK(ApproxIntv(tanh(x).codomain()) == tanh(a));

    //const Tube acosh(const Tube& x);
    CHECK(ApproxIntv(acosh(x).codomain()) == acosh(a));

    //const Tube asinh(const Tube& x);
    CHECK(ApproxIntv(asinh(x).codomain()) == asinh(a));

    //const Tube atanh(const Tube& x);
    CHECK(ApproxIntv(atanh(x).codomain()) == atanh(a));

    //const Tube atan2(const Tube& y, const Tube& x);
    CHECK(ApproxIntv(atan2(y,x).codomain()) == atan2(b,a));

    //const Tube atan2(const Tube& y, const ibex::Interval& x);
    CHECK(ApproxIntv(atan2(y,Interval(a)).codomain()) == atan2(b,a));

    //const Tube atan2(const ibex::Interval& y, const Tube& x);
    CHECK(ApproxIntv(atan2(Interval(b),x).codomain()) == atan2(b,a));

    //const Tube pow(const Tube& x, int p);
    CHECK(ApproxIntv(pow(x,3).codomain()) == pow(a,3));

    //const Tube pow(const Tube& x, double p);
    CHECK(ApproxIntv(pow(x,7.2).codomain()) == pow(a,7.2));

    //const Tube pow(const Tube& x, const ibex::Interval& p);
    CHECK(ApproxIntv(pow(x,Interval(4.,5.2)).codomain()) == Interval(pow(a,5.2),pow(a,4.)));

    //const Tube root(const Tube& x, int p);
    CHECK(ApproxIntv(root(x,6).codomain()) == root(a,6));

    //const Tube operator+(const Tube& x);
    CHECK(+x == x);
    CHECK((+x).codomain() == a);

    //const Tube operator+(const Tube& x, const Tube& y);
    CHECK(ApproxIntv((x+y).codomain()) == a+b);

    //const Tube operator+(const Tube& x, const ibex::Interval& y);
    CHECK(ApproxIntv((x+Interval(b)).codomain()) == a+b);

    //const Tube operator+(const ibex::Interval& x, const Tube& y);
    CHECK(ApproxIntv((Interval(a)+y).codomain()) == a+b);

    Trajectory trajx, trajy;
    trajx.set(a, domain.lb()); trajx.set(a, domain.mid()); trajx.set(a, domain.ub());
    trajy.set(b, domain.lb()); trajy.set(b, domain.mid()); trajy.set(b, domain.ub());

    //const Tube operator+(const Tube& x, const Trajectory& y);
    CHECK(ApproxIntv((x+trajy).codomain()) == a+b);
    CHECK(ApproxIntv((y+trajx).codomain()) == a+b);

    //const Tube operator+(const Trajectory& x, const Tube& y);
    CHECK(ApproxIntv((trajy+x).codomain()) == a+b);
    CHECK(ApproxIntv((trajx+y).codomain()) == a+b);

    //const Tube operator-(const Tube& x);
    CHECK((-x).codomain() == -a);

    //const Tube operator-(const Tube& x, const Tube& y);
    CHECK(ApproxIntv((x-y).codomain()) == a-b);

    //const Tube operator-(const Tube& x, const ibex::Interval& y);
    CHECK(ApproxIntv((x-Interval(b)).codomain()) == a-b);

    //const Tube operator-(const ibex::Interval& x, const Tube& y);
    CHECK(ApproxIntv((Interval(a)-y).codomain()) == a-b);

    //const Tube operator-(const Tube& x, const Trajectory& y);
    CHECK(ApproxIntv((x-trajy).codomain()) == a-b);

    //const Tube operator-(const Trajectory& x, const Tube& y);
    CHECK(ApproxIntv((trajx-y).codomain()) == a-b);
    CHECK(ApproxIntv((trajy-x).codomain()) == b-a);

    //const Tube operator*(const Tube& x, const Tube& y);
    CHECK(ApproxIntv((x*y).codomain()) == a*b);

    //const Tube operator*(const Tube& x, const ibex::Interval& y);
    CHECK(ApproxIntv((x*Interval(b)).codomain()) == a*b);

    //const Tube operator*(const ibex::Interval& x, const Tube& y);
    CHECK(ApproxIntv((Interval(a)*y).codomain()) == a*b);

    //const Tube operator*(const Tube& x, const Trajectory& y);
    CHECK(ApproxIntv((x*trajy).codomain()) == a*b);

    //const Tube operator*(const Trajectory& x, const Tube& y);
    CHECK(ApproxIntv((trajx*y).codomain()) == a*b);

    //const Tube operator/(const Tube& x, const Tube& y);
    CHECK(ApproxIntv((x/y).codomain()) == a/b);

    //const Tube operator/(const Tube& x, const ibex::Interval& y);
    CHECK(ApproxIntv((x/Interval(b)).codomain()) == a/b);

    //const Tube operator/(const ibex::Interval& x, const Tube& y);
    CHECK(ApproxIntv((Interval(a)/y).codomain()) == a/b);

    //const Tube operator/(const Tube& x, const Trajectory& y);
    CHECK(ApproxIntv((x/trajy).codomain()) == a/b);

    //const Tube operator/(const Trajectory& x, const Tube& y);
    CHECK(ApproxIntv((trajx/y).codomain()) == a/b);

    //const Tube operator|(const Tube& x, const Tube& y);
    CHECK(ApproxIntv((x|y).codomain()) == Interval(a,b));

    //const Tube operator|(const Tube& x, const ibex::Interval& y);
    CHECK(ApproxIntv((x|Interval(b)).codomain()) == Interval(a,b));

    //const Tube operator|(const ibex::Interval& x, const Tube& y);
    CHECK(ApproxIntv((Interval(a)|y).codomain()) == Interval(a,b));

    //const Tube operator|(const Tube& x, const Trajectory& y);
    CHECK(ApproxIntv((x|trajy).codomain()) == Interval(a,b));

    //const Tube operator|(const Trajectory& x, const Tube& y);
    CHECK(ApproxIntv((trajx|y).codomain()) == Interval(a,b));

    x.set(Interval(-10.,10.));

    //const Tube operator&(const Tube& x, const Tube& y);
    CHECK(ApproxIntv((x&y).codomain()) == b);

    //const Tube operator&(const Tube& x, const ibex::Interval& y);
    CHECK(ApproxIntv((x&Interval(b)).codomain()) == b);

    //const Tube operator&(const ibex::Interval& x, const Tube& y);
    CHECK(ApproxIntv((Interval(a)&x).codomain()) == a);

    //const Tube operator&(const Tube& x, const Trajectory& y);
    CHECK(ApproxIntv((x&trajy).codomain()) == b);

    //const Tube operator&(const Trajectory& x, const Tube& y);
    CHECK(ApproxIntv((trajx&x).codomain()) == a);

    x.set(a);
    Tube z(x);

    //const Tube& operator+=(const ibex::Interval& x);
    z = x; z += Interval(b);
    CHECK(ApproxIntv(z.codomain()) == a+b);

    //const Tube& operator+=(const Trajectory& x);
    z = x; z += trajy;
    CHECK(ApproxIntv(z.codomain()) == a+b);

    //const Tube& operator+=(const Tube& x);
    z = x; z += y;
    CHECK(ApproxIntv(z.codomain()) == a+b);

    //const Tube& operator-=(const ibex::Interval& x);
    z = x; z -= Interval(b);
    CHECK(ApproxIntv(z.codomain()) == a-b);

    //const Tube& operator-=(const Trajectory& x);
    z = x; z -= trajy;
    CHECK(ApproxIntv(z.codomain()) == a-b);

    //const Tube& operator-=(const Tube& x);
    z = x; z -= y;
    CHECK(ApproxIntv(z.codomain()) == a-b);

    //const Tube& operator*=(const ibex::Interval& x);
    z = x; z *= Interval(b);
    CHECK(ApproxIntv(z.codomain()) == a*b);

    //const Tube& operator*=(const Trajectory& x);
    z = x; z *= trajy;
    CHECK(ApproxIntv(z.codomain()) == a*b);

    //const Tube& operator*=(const Tube& x);
    z = x; z *= y;
    CHECK(ApproxIntv(z.codomain()) == a*b);

    //const Tube& operator/=(const ibex::Interval& x);
    z = x; z /= Interval(b);
    CHECK(ApproxIntv(z.codomain()) == a/b);

    //const Tube& operator/=(const Trajectory& x);
    z = x; z /= trajy;
    CHECK(ApproxIntv(z.codomain()) == a/b);

    //const Tube& operator/=(const Tube& x);
    z = x; z /= y;
    CHECK(ApproxIntv(z.codomain()) == a/b);

    //const Tube& operator|=(const ibex::Interval& x);
    z = x; z |= Interval(b);
    CHECK(ApproxIntv(z.codomain()) == Interval(a,b));

    //const Tube& operator|=(const Trajectory& x);
    z = x; z |= trajy;
    CHECK(ApproxIntv(z.codomain()) == Interval(a,b));

    //const Tube& operator|=(const Tube& x);
    z = x; z |= y;
    CHECK(ApproxIntv(z.codomain()) == Interval(a,b));

    //const Tube& operator&=(const ibex::Interval& x);
    z.set(Interval(-10.,10.)); z &= Interval(b);
    CHECK(ApproxIntv(z.codomain()) == b);

    //const Tube& operator&=(const Trajectory& x);
    z.set(Interval(-10.,10.)); z &= trajy;
    CHECK(ApproxIntv(z.codomain()) == b);

    //const Tube& operator&=(const Tube& x);
    z.set(Interval(-10.,10.)); z &= y;
    CHECK(ApproxIntv(z.codomain()) == b);

  }

  SECTION("Tests vector")
  {
    Interval domain(0.,10.);
    double dt = 0.1;

    TubeVector x(domain, dt, 3), y(x), z(x);
    x[0].set(1.); x[1].set(2.); x[2].set(3.);
    y[0].set(0.); y[1].set(10.); y[2].set(20.);

    CHECK(x.size() == 3); CHECK(y.size() == 3); CHECK(z.size() == 3);

    // x @= y

      x += Interval(2.);
      CHECK(x[0].codomain() == 3.);
      CHECK(x[1].codomain() == 4.);
      CHECK(x[2].codomain() == 5.);

      x -= Interval(4.);
      CHECK(x[0].codomain() == -1.);
      CHECK(x[1].codomain() == 0.);
      CHECK(x[2].codomain() == 1.);

      x *= Interval(-2.);
      CHECK(x[0].codomain() == 2.);
      CHECK(x[1].codomain() == 0.);
      CHECK(x[2].codomain() == -2.);

      x /= Interval(4.);
      CHECK(x[0].codomain() == 0.5);
      CHECK(x[1].codomain() == 0.);
      CHECK(x[2].codomain() == -0.5);

    // x = x @ y

      x[0].set(1.); x[1].set(2.); x[2].set(3.);

      x = x + IntervalVector(3, 2.);
      CHECK(x[0].codomain() == 3.);
      CHECK(x[1].codomain() == 4.);
      CHECK(x[2].codomain() == 5.);

      x = x - IntervalVector(3, 4.);
      CHECK(x[0].codomain() == -1.);
      CHECK(x[1].codomain() == 0.);
      CHECK(x[2].codomain() == 1.);

      x = Interval(-2.) * x;
      CHECK(x[0].codomain() == 2.);
      CHECK(x[1].codomain() == 0.);
      CHECK(x[2].codomain() == -2.);

      x = x / Interval(4.);
      CHECK(x[0].codomain() == 0.5);
      CHECK(x[1].codomain() == 0.);
      CHECK(x[2].codomain() == -0.5);


    //const TubeVector operator+(const TubeVector& x);
    z = +x;
    //CHECK(z[0].codomain() == 1.); CHECK(z[1].codomain() == 2.); CHECK(z[2].codomain() == 3.); 

    //const TubeVector operator+(const TubeVector& x, const TubeVector& y);
    z = x + y;
    //CHECK(z[0].codomain() == 1.); CHECK(z[1].codomain() == 12.); CHECK(z[2].codomain() == 23.); 



    //const TubeVector operator+(const TubeVector& x, const ibex::IntervalVector& y);
    //const TubeVector operator+(const ibex::IntervalVector& x, const TubeVector& y);
    //const TubeVector operator+(const TubeVector& x, const TrajectoryVector& y);
    //const TubeVector operator+(const TrajectoryVector& x, const TubeVector& y);

    //const TubeVector operator-(const TubeVector& x);
    //const TubeVector operator-(const TubeVector& x, const TubeVector& y);
    //const TubeVector operator-(const TubeVector& x, const ibex::IntervalVector& y);
    //const TubeVector operator-(const ibex::IntervalVector& x, const TubeVector& y);
    //const TubeVector operator-(const TubeVector& x, const TrajectoryVector& y);
    //const TubeVector operator-(const TrajectoryVector& x, const TubeVector& y);

    //const TubeVector operator*(const Tube& x, const TubeVector& y);
    //const TubeVector operator*(const ibex::Interval& x, const TubeVector& y);
    //const TubeVector operator*(const Tube& x, const ibex::IntervalVector& y);
    //const TubeVector operator*(const Trajectory& x, const TubeVector& y);

    //const TubeVector operator/(const TubeVector& x, const Tube& y);
    //const TubeVector operator/(const TubeVector& x, const ibex::Interval& y);
    //const TubeVector operator/(const ibex::IntervalVector& x, const Tube& y);
    //const TubeVector operator/(const TubeVector& x, const Trajectory& y);

    //const TubeVector operator|(const TubeVector& x, const TubeVector& y);
    //const TubeVector operator|(const TubeVector& x, const ibex::IntervalVector& y);
    //const TubeVector operator|(const ibex::IntervalVector& x, const TubeVector& y);
    //const TubeVector operator|(const TubeVector& x, const TrajectoryVector& y);
    //const TubeVector operator|(const TrajectoryVector& x, const TubeVector& y);

    //const TubeVector operator&(const TubeVector& x, const TubeVector& y);
    //const TubeVector operator&(const TubeVector& x, const ibex::IntervalVector& y);
    //const TubeVector operator&(const ibex::IntervalVector& x, const TubeVector& y);
    //const TubeVector operator&(const TubeVector& x, const TrajectoryVector& y);
    //const TubeVector operator&(const TrajectoryVector& x, const TubeVector& y);

    //const TubeVector& operator+=(const ibex::Interval& x);
    //const TubeVector& operator+=(const Trajectory& x);
    //const TubeVector& operator+=(const Tube& x);
    //const TubeVector& operator+=(const ibex::IntervalVector& x);
    //const TubeVector& operator+=(const TrajectoryVector& x);
    //const TubeVector& operator+=(const TubeVector& x);
    //const TubeVector& operator-=(const ibex::Interval& x);
    //const TubeVector& operator-=(const Trajectory& x);
    //const TubeVector& operator-=(const Tube& x);
    //const TubeVector& operator-=(const ibex::IntervalVector& x);
    //const TubeVector& operator-=(const TrajectoryVector& x);
    //const TubeVector& operator-=(const TubeVector& x);
    //const TubeVector& operator*=(const ibex::Interval& x);
    //const TubeVector& operator*=(const Trajectory& x);
    //const TubeVector& operator*=(const Tube& x);
    //const TubeVector& operator/=(const ibex::Interval& x);
    //const TubeVector& operator/=(const Trajectory& x);
    //const TubeVector& operator/=(const Tube& x);
    //const TubeVector& operator|=(const ibex::IntervalVector& x);
    //const TubeVector& operator|=(const TrajectoryVector& x);
    //const TubeVector& operator|=(const TubeVector& x);
    //const TubeVector& operator&=(const ibex::IntervalVector& x);
    //const TubeVector& operator&=(const TrajectoryVector& x);
    //const TubeVector& operator&=(const TubeVector& x);
  }
}