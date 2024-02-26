#include "catch_interval.hpp"
#include "codac_tube_arithmetic.h"
#include "codac_traj_arithmetic.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

TEST_CASE("Arithmetic on tubes")
{
  // todo: test tubes with different slicing

  SECTION("Tests scalar tube")
  {
    Interval domain(0.,10.);
    double dt = 0.1;
    double a = -0.8, b = 3.;

    Tube x(domain, dt, a);
    Tube y(x); y.set(b);
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

    //const Tube atan2(const Tube& y, const Interval& x);
    CHECK(ApproxIntv(atan2(y,Interval(a)).codomain()) == atan2(b,a));

    //const Tube atan2(const Interval& y, const Tube& x);
    CHECK(ApproxIntv(atan2(Interval(b),x).codomain()) == atan2(b,a));

    //const Tube pow(const Tube& x, int p);
    CHECK(ApproxIntv(pow(x,3).codomain()) == pow(a,3));

    //const Tube pow(const Tube& x, double p);
    // CHECK(ApproxIntv(pow(x,7.2).codomain()) == pow(a,7.2)); // todo: not working with GAOL

    //const Tube pow(const Tube& x, const Interval& p);
    CHECK(ApproxIntv(pow(x,Interval(4.,5.2)).codomain()) == Interval(pow(a,5.2),pow(a,4.)));

    //const Tube root(const Tube& x, int p);
    CHECK(ApproxIntv(root(x,6).codomain()) == root(a,6));

    //const Tube operator+(const Tube& x);
    CHECK(+x == x);
    CHECK((+x).codomain() == a);

    //const Tube operator+(const Tube& x, const Tube& y);
    CHECK(ApproxIntv((x+y).codomain()) == a+b);

    //const Tube operator+(const Tube& x, const Interval& y);
    CHECK(ApproxIntv((x+Interval(b)).codomain()) == a+b);

    //const Tube operator+(const Interval& x, const Tube& y);
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

    //const Tube operator-(const Tube& x, const Interval& y);
    CHECK(ApproxIntv((x-Interval(b)).codomain()) == a-b);

    //const Tube operator-(const Interval& x, const Tube& y);
    CHECK(ApproxIntv((Interval(a)-y).codomain()) == a-b);

    //const Tube operator-(const Tube& x, const Trajectory& y);
    CHECK(ApproxIntv((x-trajy).codomain()) == a-b);

    //const Tube operator-(const Trajectory& x, const Tube& y);
    CHECK(ApproxIntv((trajx-y).codomain()) == a-b);
    CHECK(ApproxIntv((trajy-x).codomain()) == b-a);

    //const Tube operator*(const Tube& x, const Tube& y);
    CHECK(ApproxIntv((x*y).codomain()) == a*b);

    //const Tube operator*(const Tube& x, const Interval& y);
    CHECK(ApproxIntv((x*Interval(b)).codomain()) == a*b);

    //const Tube operator*(const Interval& x, const Tube& y);
    CHECK(ApproxIntv((Interval(a)*y).codomain()) == a*b);

    //const Tube operator*(const Tube& x, const Trajectory& y);
    CHECK(ApproxIntv((x*trajy).codomain()) == a*b);

    //const Tube operator*(const Trajectory& x, const Tube& y);
    CHECK(ApproxIntv((trajx*y).codomain()) == a*b);

    //const Tube operator/(const Tube& x, const Tube& y);
    CHECK(ApproxIntv((x/y).codomain()) == a/b);

    //const Tube operator/(const Tube& x, const Interval& y);
    CHECK(ApproxIntv((x/Interval(b)).codomain()) == a/b);

    //const Tube operator/(const Interval& x, const Tube& y);
    CHECK(ApproxIntv((Interval(a)/y).codomain()) == a/b);

    //const Tube operator/(const Tube& x, const Trajectory& y);
    CHECK(ApproxIntv((x/trajy).codomain()) == a/b);

    //const Tube operator/(const Trajectory& x, const Tube& y);
    CHECK(ApproxIntv((trajx/y).codomain()) == a/b);

    //const Tube operator|(const Tube& x, const Tube& y);
    CHECK(ApproxIntv((x|y).codomain()) == Interval(a,b));

    //const Tube operator|(const Tube& x, const Interval& y);
    CHECK(ApproxIntv((x|Interval(b)).codomain()) == Interval(a,b));

    //const Tube operator|(const Interval& x, const Tube& y);
    CHECK(ApproxIntv((Interval(a)|y).codomain()) == Interval(a,b));

    //const Tube operator|(const Tube& x, const Trajectory& y);
    CHECK(ApproxIntv((x|trajy).codomain()) == Interval(a,b));

    //const Tube operator|(const Trajectory& x, const Tube& y);
    CHECK(ApproxIntv((trajx|y).codomain()) == Interval(a,b));

    x.set(Interval(-10.,10.));

    //const Tube operator&(const Tube& x, const Tube& y);
    CHECK(ApproxIntv((x&y).codomain()) == b);

    //const Tube operator&(const Tube& x, const Interval& y);
    CHECK(ApproxIntv((x&Interval(b)).codomain()) == b);

    //const Tube operator&(const Interval& x, const Tube& y);
    CHECK(ApproxIntv((Interval(a)&x).codomain()) == a);

    //const Tube operator&(const Tube& x, const Trajectory& y);
    CHECK(ApproxIntv((x&trajy).codomain()) == b);

    //const Tube operator&(const Trajectory& x, const Tube& y);
    CHECK(ApproxIntv((trajx&x).codomain()) == a);

    x.set(a);
    Tube z(x);

    //const Tube& operator+=(const Interval& x);
    z = x; z += Interval(b);
    CHECK(ApproxIntv(z.codomain()) == a+b);

    //const Tube& operator+=(const Trajectory& x);
    z = x; z += trajy;
    CHECK(ApproxIntv(z.codomain()) == a+b);

    //const Tube& operator+=(const Tube& x);
    z = x; z += y;
    CHECK(ApproxIntv(z.codomain()) == a+b);

    //const Tube& operator-=(const Interval& x);
    z = x; z -= Interval(b);
    CHECK(ApproxIntv(z.codomain()) == a-b);

    //const Tube& operator-=(const Trajectory& x);
    z = x; z -= trajy;
    CHECK(ApproxIntv(z.codomain()) == a-b);

    //const Tube& operator-=(const Tube& x);
    z = x; z -= y;
    CHECK(ApproxIntv(z.codomain()) == a-b);

    //const Tube& operator*=(const Interval& x);
    z = x; z *= Interval(b);
    CHECK(ApproxIntv(z.codomain()) == a*b);

    //const Tube& operator*=(const Trajectory& x);
    z = x; z *= trajy;
    CHECK(ApproxIntv(z.codomain()) == a*b);

    //const Tube& operator*=(const Tube& x);
    z = x; z *= y;
    CHECK(ApproxIntv(z.codomain()) == a*b);

    //const Tube& operator/=(const Interval& x);
    z = x; z /= Interval(b);
    CHECK(ApproxIntv(z.codomain()) == a/b);

    //const Tube& operator/=(const Trajectory& x);
    z = x; z /= trajy;
    CHECK(ApproxIntv(z.codomain()) == a/b);

    //const Tube& operator/=(const Tube& x);
    z = x; z /= y;
    CHECK(ApproxIntv(z.codomain()) == a/b);

    //const Tube& operator|=(const Interval& x);
    z = x; z |= Interval(b);
    CHECK(ApproxIntv(z.codomain()) == Interval(a,b));

    //const Tube& operator|=(const Trajectory& x);
    z = x; z |= trajy;
    CHECK(ApproxIntv(z.codomain()) == Interval(a,b));

    //const Tube& operator|=(const Tube& x);
    z = x; z |= y;
    CHECK(ApproxIntv(z.codomain()) == Interval(a,b));

    //const Tube& operator&=(const Interval& x);
    z.set(Interval(-10.,10.)); z &= Interval(b);
    CHECK(ApproxIntv(z.codomain()) == b);

    //const Tube& operator&=(const Trajectory& x);
    z.set(Interval(-10.,10.)); z &= trajy;
    CHECK(ApproxIntv(z.codomain()) == b);

    //const Tube& operator&=(const Tube& x);
    z.set(Interval(-10.,10.)); z &= y;
    CHECK(ApproxIntv(z.codomain()) == b);
  }

  SECTION("Tests vector tube")
  {
    Interval domain(0.,10.);
    double dt = 0.1;

    TubeVector x(domain, dt, 3), y(x), z(x);
    TrajectoryVector trajx(3), trajy(3);
    IntervalVector ix(3), iy(3);

    double a0 = 1., a1 = 2., a2 = 3.;
    double b0 = 0., b1 = 10., b2 = 20.;

    ix[0] = a0; ix[1] = a1; ix[2] = a2;
    iy[0] = b0; iy[1] = b1; iy[2] = b2;

    x.set(ix);
    y.set(iy);

    trajx.set(ix.mid(), domain.lb()); trajx.set(ix.mid(), domain.mid()); trajx.set(ix.mid(), domain.ub());
    trajy.set(iy.mid(), domain.lb()); trajy.set(iy.mid(), domain.mid()); trajy.set(iy.mid(), domain.ub());

    CHECK(x.size() == 3); CHECK(y.size() == 3); CHECK(z.size() == 3);


    //const TubeVector operator+(const TubeVector& x);
    z = +x;
    CHECK(z[0].codomain() == a0); CHECK(z[1].codomain() == a1); CHECK(z[2].codomain() == a2); 

    //const TubeVector operator+(const TubeVector& x, const TubeVector& y);
    z = x + y;
    CHECK(z[0].codomain() == a0+b0); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b2); 

    //const TubeVector operator+(const TubeVector& x, const IntervalVector& y);
    z = x + iy;
    CHECK(z[0].codomain() == a0+b0); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b2); 

    //const TubeVector operator+(const IntervalVector& x, const TubeVector& y);
    z = ix + y;
    CHECK(z[0].codomain() == a0+b0); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b2); 

    //const TubeVector operator+(const TubeVector& x, const TrajectoryVector& y);
    z = x + trajy;
    CHECK(z[0].codomain() == a0+b0); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b2); 

    //const TubeVector operator+(const TrajectoryVector& x, const TubeVector& y);
    z = trajx + y;
    CHECK(z[0].codomain() == a0+b0); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b2); 

    //const TubeVector operator-(const TubeVector& x);
    z = -x;
    CHECK(z[0].codomain() == -a0); CHECK(z[1].codomain() == -a1); CHECK(z[2].codomain() == -a2); 

    //const TubeVector operator-(const TubeVector& x, const TubeVector& y);
    z = x - y;
    CHECK(z[0].codomain() == a0-b0); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b2); 

    //const TubeVector operator-(const TubeVector& x, const IntervalVector& y);
    z = x - iy;
    CHECK(z[0].codomain() == a0-b0); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b2); 

    //const TubeVector operator-(const IntervalVector& x, const TubeVector& y);
    z = ix - y;
    CHECK(z[0].codomain() == a0-b0); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b2); 

    //const TubeVector operator-(const TubeVector& x, const TrajectoryVector& y);
    z = x - trajy;
    CHECK(z[0].codomain() == a0-b0); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b2); 

    //const TubeVector operator-(const TrajectoryVector& x, const TubeVector& y);
    z = trajx - y;
    CHECK(z[0].codomain() == a0-b0); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b2); 

    //const TubeVector operator*(const Tube& x, const TubeVector& y);
    z = x[1] * y;
    CHECK(z[0].codomain() == a1*b0); CHECK(z[1].codomain() == a1*b1); CHECK(z[2].codomain() == a1*b2); 

    //const TubeVector operator*(const Interval& x, const TubeVector& y);
    z = Interval(2) * y;
    CHECK(z[0].codomain() == 2*b0); CHECK(z[1].codomain() == 2*b1); CHECK(z[2].codomain() == 2*b2); 

    //const TubeVector operator*(const Tube& x, const IntervalVector& y);
    z = x[1] * iy;
    CHECK(z[0].codomain() == a1*b0); CHECK(z[1].codomain() == a1*b1); CHECK(z[2].codomain() == a1*b2); 

    //const TubeVector operator*(const Trajectory& x, const TubeVector& y);
    z = trajx[1] * y;
    CHECK(z[0].codomain() == a1*b0); CHECK(z[1].codomain() == a1*b1); CHECK(z[2].codomain() == a1*b2); 

    //const TubeVector operator/(const TubeVector& x, const Tube& y);
    z = x / y[0];
    CHECK(z[0].codomain() == a0/b0); CHECK(z[1].codomain() == a1/b0); CHECK(z[2].codomain() == a2/b0); 

    //const TubeVector operator/(const TubeVector& x, const Interval& y);
    z = x / iy[0];
    CHECK(z[0].codomain() == a0/b0); CHECK(z[1].codomain() == a1/b0); CHECK(z[2].codomain() == a2/b0); 

    //const TubeVector operator/(const IntervalVector& x, const Tube& y);
    z = ix / y[0];
    CHECK(z[0].codomain() == a0/b0); CHECK(z[1].codomain() == a1/b0); CHECK(z[2].codomain() == a2/b0); 

    //const TubeVector operator/(const TubeVector& x, const Trajectory& y);
    z = x / trajy[0];
    CHECK(z[0].codomain() == a0/b0); CHECK(z[1].codomain() == a1/b0); CHECK(z[2].codomain() == a2/b0); 

    //const TubeVector operator|(const TubeVector& x, const TubeVector& y);
    z = x | y;
    CHECK(z[0].codomain() == (Interval(a0) | b0)); CHECK(z[1].codomain() == (Interval(a1) | b1)); CHECK(z[2].codomain() == (Interval(a2) | b2)); 

    //const TubeVector operator|(const TubeVector& x, const IntervalVector& y);
    z = x | iy;
    CHECK(z[0].codomain() == (Interval(a0) | b0)); CHECK(z[1].codomain() == (Interval(a1) | b1)); CHECK(z[2].codomain() == (Interval(a2) | b2)); 

    //const TubeVector operator|(const IntervalVector& x, const TubeVector& y);
    z = ix | y;
    CHECK(z[0].codomain() == (Interval(a0) | b0)); CHECK(z[1].codomain() == (Interval(a1) | b1)); CHECK(z[2].codomain() == (Interval(a2) | b2)); 

    //const TubeVector operator|(const TubeVector& x, const TrajectoryVector& y);
    z = x | trajy;
    CHECK(z[0].codomain() == (Interval(a0) | b0)); CHECK(z[1].codomain() == (Interval(a1) | b1)); CHECK(z[2].codomain() == (Interval(a2) | b2)); 

    //const TubeVector operator|(const TrajectoryVector& x, const TubeVector& y);
    z = trajx | y;
    CHECK(z[0].codomain() == (Interval(a0) | b0)); CHECK(z[1].codomain() == (Interval(a1) | b1)); CHECK(z[2].codomain() == (Interval(a2) | b2)); 

    iy |= IntervalVector(3, -1);
    y.set(iy);

    //const TubeVector operator&(const TubeVector& x, const TubeVector& y);
    z = x & y;
    CHECK(z[0].codomain() == Interval::EMPTY_SET); CHECK(z[1].codomain() == a1); CHECK(z[2].codomain() == a2); 

    //const TubeVector operator&(const TubeVector& x, const IntervalVector& y);
    z = x & iy;
    CHECK(z[0].codomain() == Interval::EMPTY_SET); CHECK(z[1].codomain() == a1); CHECK(z[2].codomain() == a2); 

    //const TubeVector operator&(const IntervalVector& x, const TubeVector& y);
    z = ix & y;
    CHECK(z[0].codomain() == Interval::EMPTY_SET); CHECK(z[1].codomain() == a1); CHECK(z[2].codomain() == a2); 

    //const TubeVector operator&(const TubeVector& x, const TrajectoryVector& y);
    z = y & trajx;
    CHECK(z[0].codomain() == Interval::EMPTY_SET); CHECK(z[1].codomain() == a1); CHECK(z[2].codomain() == a2); 

    //const TubeVector operator&(const TrajectoryVector& x, const TubeVector& y);
    z = trajx & y;
    CHECK(z[0].codomain() == Interval::EMPTY_SET); CHECK(z[1].codomain() == a1); CHECK(z[2].codomain() == a2); 

    iy[0] = b0; iy[1] = b1; iy[2] = b2;
    y.set(iy);

    //const TubeVector& operator+=(const Interval& x);
    z = x; z+=iy[1];
    CHECK(z[0].codomain() == a0+b1); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b1); 

    //const TubeVector& operator+=(const Trajectory& x);
    z = x; z+=trajy[1];
    CHECK(z[0].codomain() == a0+b1); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b1); 

    //const TubeVector& operator+=(const Tube& x);
    z = x; z+=y[1];
    CHECK(z[0].codomain() == a0+b1); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b1); 

    //const TubeVector& operator+=(const IntervalVector& x);
    z = x; z+=iy;
    CHECK(z[0].codomain() == a0+b0); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b2); 

    //const TubeVector& operator+=(const TrajectoryVector& x);
    z = x; z+=trajy;
    CHECK(z[0].codomain() == a0+b0); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b2); 

    //const TubeVector& operator+=(const TubeVector& x);
    z = x; z+=y;
    CHECK(z[0].codomain() == a0+b0); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b2); 

    //const TubeVector& operator-=(const Interval& x);
    z = x; z-=iy[1];
    CHECK(z[0].codomain() == a0-b1); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b1); 

    //const TubeVector& operator-=(const Trajectory& x);
    z = x; z-=trajy[1];
    CHECK(z[0].codomain() == a0-b1); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b1); 

    //const TubeVector& operator-=(const Tube& x);
    z = x; z-=y[1];
    CHECK(z[0].codomain() == a0-b1); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b1); 

    //const TubeVector& operator-=(const IntervalVector& x);
    z = x; z-=iy;
    CHECK(z[0].codomain() == a0-b0); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b2); 

    //const TubeVector& operator-=(const TrajectoryVector& x);
    z = x; z-=trajy;
    CHECK(z[0].codomain() == a0-b0); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b2); 

    //const TubeVector& operator-=(const TubeVector& x);
    z = x; z-=y;
    CHECK(z[0].codomain() == a0-b0); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b2); 

    //const TubeVector& operator*=(const Interval& x);
    z = x; z*=iy[1];
    CHECK(z[0].codomain() == a0*b1); CHECK(z[1].codomain() == a1*b1); CHECK(z[2].codomain() == a2*b1); 

    //const TubeVector& operator*=(const Trajectory& x);
    z = x; z*=trajy[1];
    CHECK(z[0].codomain() == a0*b1); CHECK(z[1].codomain() == a1*b1); CHECK(z[2].codomain() == a2*b1); 

    //const TubeVector& operator*=(const Tube& x);
    z = x; z*=y[1];
    CHECK(z[0].codomain() == a0*b1); CHECK(z[1].codomain() == a1*b1); CHECK(z[2].codomain() == a2*b1); 

    //const TubeVector& operator/=(const Interval& x);
    z = x; z/=iy[1];
    CHECK(ApproxIntv(z[0].codomain()) == a0/b1); CHECK(ApproxIntv(z[1].codomain()) == a1/b1); CHECK(ApproxIntv(z[2].codomain()) == a2/b1); 

    //const TubeVector& operator/=(const Trajectory& x);
    z = x; z/=trajy[1];
    CHECK(ApproxIntv(z[0].codomain()) == a0/b1); CHECK(ApproxIntv(z[1].codomain()) == a1/b1); CHECK(ApproxIntv(z[2].codomain()) == a2/b1); 

    //const TubeVector& operator/=(const Tube& x);
    z = x; z/=y[1];
    CHECK(ApproxIntv(z[0].codomain()) == a0/b1); CHECK(ApproxIntv(z[1].codomain()) == a1/b1); CHECK(ApproxIntv(z[2].codomain()) == a2/b1); 

    //const TubeVector& operator|=(const IntervalVector& x);
    z = x; z|=iy;
    CHECK(ApproxIntv(z[0].codomain()) == (Interval(a0) | b0)); CHECK(ApproxIntv(z[1].codomain()) == (Interval(a1) | b1)); CHECK(ApproxIntv(z[2].codomain()) == (Interval(a2) | b2)); 

    //const TubeVector& operator|=(const TrajectoryVector& x);
    z = x; z|=trajy;
    CHECK(ApproxIntv(z[0].codomain()) == (Interval(a0) | b0)); CHECK(ApproxIntv(z[1].codomain()) == (Interval(a1) | b1)); CHECK(ApproxIntv(z[2].codomain()) == (Interval(a2) | b2)); 

    //const TubeVector& operator|=(const TubeVector& x);
    z = x; z|=y;
    CHECK(ApproxIntv(z[0].codomain()) == (Interval(a0) | b0)); CHECK(ApproxIntv(z[1].codomain()) == (Interval(a1) | b1)); CHECK(ApproxIntv(z[2].codomain()) == (Interval(a2) | b2)); 

    iy |= IntervalVector(3, -1);
    y.set(iy);

    //const TubeVector& operator&=(const IntervalVector& x);
    z = x; z&=iy;
    CHECK(ApproxIntv(z[0].codomain()) == Interval::EMPTY_SET); CHECK(ApproxIntv(z[1].codomain()) == a1); CHECK(ApproxIntv(z[2].codomain()) == a2); 

    //const TubeVector& operator&=(const TrajectoryVector& x);
    z = x; y&=trajx;
    CHECK(ApproxIntv(z[0].codomain()) == a0); CHECK(ApproxIntv(z[1].codomain()) == a1); CHECK(ApproxIntv(z[2].codomain()) == a2); 

    //const TubeVector& operator&=(const TubeVector& x);
    z = x; z&=y;
    CHECK(ApproxIntv(z[0].codomain()) == Interval::EMPTY_SET); CHECK(ApproxIntv(z[1].codomain()) == a1); CHECK(ApproxIntv(z[2].codomain()) == a2); 

  }
}


TEST_CASE("Arithmetic on trajs")
{
  SECTION("Tests scalar traj")
  {
    Interval domain(0.,10.);
    double x = 0.8, y = 3.;

    Trajectory trajx;
    Trajectory trajy, trajz;
    trajx.set(x, domain.lb()); trajx.set(x, domain.mid()); trajx.set(x, domain.ub());
    trajy.set(y, domain.lb()); trajy.set(y, domain.mid()); trajy.set(y, domain.ub());

    CHECK(trajx.codomain() == x);

    //const Trajectory cos(const Trajectory& x);
    CHECK(ApproxIntv(cos(trajx).codomain()) == cos(Interval(x)));
    
    //const Trajectory sin(const Trajectory& x);
    CHECK(ApproxIntv(sin(trajx).codomain()) == sin(Interval(x)));
    
    //const Trajectory abs(const Trajectory& x);
    CHECK(ApproxIntv(abs(trajx).codomain()) == abs(Interval(x)));
    
    //const Trajectory sqr(const Trajectory& x);
    CHECK(ApproxIntv(sqr(trajx).codomain()) == pow(Interval(x),2));
    
    //const Trajectory sqrt(const Trajectory& x);
    CHECK(ApproxIntv(sqrt(trajx).codomain()) == sqrt(Interval(x)));
    
    //const Trajectory exp(const Trajectory& x);
    CHECK(ApproxIntv(exp(trajx).codomain()) == exp(Interval(x)));
    
    //const Trajectory log(const Trajectory& x);
    CHECK(ApproxIntv(log(trajx).codomain()) == log(Interval(x)));
    
    //const Trajectory tan(const Trajectory& x);
    CHECK(ApproxIntv(tan(trajx).codomain()) == tan(Interval(x)));
    
    //const Trajectory acos(const Trajectory& x);
    CHECK(ApproxIntv(acos(trajx).codomain()) == acos(Interval(x)));
    
    //const Trajectory asin(const Trajectory& x);
    CHECK(ApproxIntv(asin(trajx).codomain()) == asin(Interval(x)));
    
    //const Trajectory atan(const Trajectory& x);
    CHECK(ApproxIntv(atan(trajx).codomain()) == atan(Interval(x)));
    
    //const Trajectory cosh(const Trajectory& x);
    CHECK(ApproxIntv(cosh(trajx).codomain()) == cosh(Interval(x)));
    
    //const Trajectory sinh(const Trajectory& x);
    CHECK(ApproxIntv(sinh(trajx).codomain()) == sinh(Interval(x)));
    
    //const Trajectory tanh(const Trajectory& x);
    CHECK(ApproxIntv(tanh(trajx).codomain()) == tanh(Interval(x)));
    
    //const Trajectory acosh(const Trajectory& x);
    CHECK(ApproxIntv(acosh(trajx).codomain()) == acosh(Interval(x)));
    
    //const Trajectory asinh(const Trajectory& x);
    CHECK(ApproxIntv(asinh(trajx).codomain()) == asinh(Interval(x)));
    
    //const Trajectory atanh(const Trajectory& x);
    CHECK(ApproxIntv(atanh(trajx).codomain()) == atanh(Interval(x)));
    
    //const Trajectory atan2(const Trajectory& y, const Trajectory& x);
    CHECK(ApproxIntv(atan2(trajx,trajy).codomain()) == Interval(atan2(x,y)));
    
    //const Trajectory atan2(const Trajectory& y, double x);
    CHECK(ApproxIntv(atan2(trajx,y).codomain()) == atan2(Interval(x),Interval(y)));
    
    //const Trajectory atan2(double y, const Trajectory& x);
    CHECK(ApproxIntv(atan2(x,trajy).codomain()) == atan2(Interval(x),Interval(y)));
    
    //const Trajectory pow(const Trajectory& x, int p);
    CHECK(ApproxIntv(pow(trajx,3).codomain()) == pow(Interval(x),3));
    
    //const Trajectory pow(const Trajectory& x, double p);
    //CHECK(ApproxIntv(pow(trajx,7.2).codomain()) == pow(Interval(x),7.2)); // todo: not working with GAOL
    
    //const Trajectory root(const Trajectory& x, int p);
    CHECK(ApproxIntv(root(trajx,6).codomain()) == root(Interval(x),6));
    
    //const Trajectory operator+(const Trajectory& x);
    CHECK(ApproxIntv((+trajy).codomain()) == Interval(+y));
    
    //const Trajectory operator+(const Trajectory& x, const Trajectory& y);
    CHECK(ApproxIntv((trajx+trajy).codomain()) == Interval(x+y));
    
    //const Trajectory operator+(const Trajectory& x, double y);
    CHECK(ApproxIntv((trajx+y).codomain()) == Interval(x+y));
    
    //const Trajectory operator+(double x, const Trajectory& y);
    CHECK(ApproxIntv((x+trajy).codomain()) == Interval(x+y));
    
    //const Trajectory operator-(const Trajectory& x);
    CHECK(ApproxIntv((-trajx).codomain()) == Interval(-x));
    
    //const Trajectory operator-(const Trajectory& x, const Trajectory& y);
    CHECK(ApproxIntv((trajx-trajy).codomain()) == Interval(x-y));
    
    //const Trajectory operator-(const Trajectory& x, double y);
    CHECK(ApproxIntv((trajx-y).codomain()) == Interval(x-y));
    
    //const Trajectory operator-(double x, const Trajectory& y);
    CHECK(ApproxIntv((x-trajy).codomain()) == Interval(x-y));
    
    //const Trajectory operator*(const Trajectory& x, const Trajectory& y);
    CHECK(ApproxIntv((trajx*trajy).codomain()) == Interval(x*y));
    
    //const Trajectory operator*(const Trajectory& x, double y);
    CHECK(ApproxIntv((trajx*y).codomain()) == Interval(x*y));
    
    //const Trajectory operator*(double x, const Trajectory& y);
    CHECK(ApproxIntv((x*trajy).codomain()) == Interval(x*y));
    
    //const Trajectory operator/(const Trajectory& x, const Trajectory& y);
    CHECK(ApproxIntv((trajx/trajy).codomain()) == Interval(x/y));
    
    //const Trajectory operator/(const Trajectory& x, double y);
    CHECK(ApproxIntv((trajx/y).codomain()) == Interval(x/y));
    
    //const Trajectory operator/(double x, const Trajectory& y);
    CHECK(ApproxIntv((x/trajy).codomain()) == Interval(x/y));

    //const Trajectory& operator+=(double x);
    trajz = trajx; trajz += y;
    CHECK(ApproxIntv(trajz.codomain()) == Interval(x+y));
    
    //const Trajectory& operator+=(const Trajectory& x);
    trajz = trajx; trajz += trajy;
    CHECK(ApproxIntv(trajz.codomain()) == Interval(x+y));
    
    //const Trajectory& operator-=(double x);
    trajz = trajx; trajz -= y;
    CHECK(ApproxIntv(trajz.codomain()) == Interval(x-y));
    
    //const Trajectory& operator-=(const Trajectory& x);
    trajz = trajx; trajz -= trajy;
    CHECK(ApproxIntv(trajz.codomain()) == Interval(x-y));
    
    //const Trajectory& operator*=(double x);
    trajz = trajx; trajz *= y;
    CHECK(ApproxIntv(trajz.codomain()) == Interval(x*y));
    
    //const Trajectory& operator*=(const Trajectory& x);
    trajz = trajx; trajz *= trajy;
    CHECK(ApproxIntv(trajz.codomain()) == Interval(x*y));
    
    //const Trajectory& operator/=(double x);
    trajz = trajx; trajz /= y;
    CHECK(ApproxIntv(trajz.codomain()) == Interval(x/y));
    
    //const Trajectory& operator/=(const Trajectory& x);
    trajz = trajx; trajz /= trajy;
    CHECK(ApproxIntv(trajz.codomain()) == Interval(x/y));
  }

  SECTION("Tests vector traj")
  {
    Interval domain(0.,10.);
    TrajectoryVector trajx(3), trajy(3), trajz(3);

    Vector vx(3), vy(3);
    vx[0] = 1.; vx[1] = 2.; vx[2] = 3.;
    vy[0] = 10.; vy[1] = 20.; vy[2] = 30.;

    trajx.set(vx, domain.lb()); trajx.set(vx, domain.mid()); trajx.set(vx, domain.ub());
    trajy.set(vy, domain.lb()); trajy.set(vy, domain.mid()); trajy.set(vy, domain.ub());

    CHECK(trajx.size() == 3); CHECK(trajy.size() == 3);

    //const TrajectoryVector operator+(const TrajectoryVector& x);
    CHECK(ApproxIntvVector((+trajx).codomain()) == IntervalVector(vx));
    
    //const TrajectoryVector operator+(const TrajectoryVector& x, const TrajectoryVector& y);
    CHECK(ApproxIntvVector((trajx+trajy).codomain()) == IntervalVector(vx+vy));
    
    //const TrajectoryVector operator+(const TrajectoryVector& x, const Vector& y);
    CHECK(ApproxIntvVector((trajx+vy).codomain()) == IntervalVector(vx+vy));
    
    //const TrajectoryVector operator+(const Vector& x, const TrajectoryVector& y);
    CHECK(ApproxIntvVector((vx+trajy).codomain()) == IntervalVector(vx+vy));
    
    //const TrajectoryVector operator-(const TrajectoryVector& x);
    CHECK(ApproxIntvVector((-trajx).codomain()) == IntervalVector(-vx));
    
    //const TrajectoryVector operator-(const TrajectoryVector& x, const TrajectoryVector& y);
    CHECK(ApproxIntvVector((trajx-trajy).codomain()) == IntervalVector(vx-vy));
    
    //const TrajectoryVector operator-(const TrajectoryVector& x, const Vector& y);
    CHECK(ApproxIntvVector((trajx-vy).codomain()) == IntervalVector(vx-vy));
    
    //const TrajectoryVector operator-(const Vector& x, const TrajectoryVector& y);
    CHECK(ApproxIntvVector((vx-trajy).codomain()) == IntervalVector(vx-vy));
    
    //const TrajectoryVector operator*(const Trajectory& x, const TrajectoryVector& y);
    CHECK(ApproxIntvVector((trajx[1]*trajy).codomain()) == IntervalVector(vx[1]*vy));
    
    //const TrajectoryVector operator*(double x, const TrajectoryVector& y);
    CHECK(ApproxIntvVector((vx[1]*trajy).codomain()) == vx[1]*IntervalVector(vy));
    
    //const TrajectoryVector operator*(const Trajectory& x, const Vector& y);
    CHECK(ApproxIntvVector((trajx[1]*vy).codomain()) == IntervalVector(vx[1]*vy));
    
    //const TrajectoryVector operator*(const Trajectory& x, const TrajectoryVector& y);
    CHECK(ApproxIntvVector((trajx[1]*trajy).codomain()) == IntervalVector(vx[1]*vy));
    
    //const TrajectoryVector operator/(const TrajectoryVector& x, double y);
    CHECK(ApproxIntvVector((trajx/vy[1]).codomain()) == IntervalVector((1./vy[1])*vx));
    
    //const TrajectoryVector operator/(const TrajectoryVector& x, const Trajectory& y);
    CHECK(ApproxIntvVector((trajx/trajy[1]).codomain()) == IntervalVector((1./vy[1])*vx));
    
    //const TrajectoryVector operator/(const Vector& x, const Trajectory& y);
    CHECK(ApproxIntvVector((vx/trajy[1]).codomain()) == IntervalVector((1./vy[1])*vx));
    
    //const Trajectory& operator+=(double x);
    trajz = trajx; trajz += vy[1];
    CHECK(ApproxIntvVector(trajz.codomain()) == IntervalVector(vx+Vector(3,vy[1])));
    
    //const Trajectory& operator+=(const Trajectory& x);
    trajz = trajx; trajz += trajy[1];
    CHECK(ApproxIntvVector(trajz.codomain()) == IntervalVector(vx+Vector(3,vy[1])));

    //const TrajectoryVector& operator+=(const Vector& x);
    trajz = trajx; trajz += vy;
    CHECK(ApproxIntvVector(trajz.codomain()) == IntervalVector(vx+vy));

    //const TrajectoryVector& operator+=(const TrajectoryVector& x);
    trajz = trajx; trajz += trajy;
    CHECK(ApproxIntvVector(trajz.codomain()) == IntervalVector(vx+vy));
    
    //const Trajectory& operator-=(double x);
    trajz = trajx; trajz -= vy[1];
    CHECK(ApproxIntvVector(trajz.codomain()) == IntervalVector(vx-Vector(3,vy[1])));
    
    //const Trajectory& operator-=(const Trajectory& x);
    trajz = trajx; trajz -= trajy[1];
    CHECK(ApproxIntvVector(trajz.codomain()) == IntervalVector(vx-Vector(3,vy[1])));

    //const TrajectoryVector& operator-=(const Vector& x);
    trajz = trajx; trajz -= vy;
    CHECK(ApproxIntvVector(trajz.codomain()) == IntervalVector(vx-vy));

    //const TrajectoryVector& operator-=(const TrajectoryVector& x);
    trajz = trajx; trajz -= trajy;
    CHECK(ApproxIntvVector(trajz.codomain()) == IntervalVector(vx-vy));

    //const Trajectory& operator*=(double x);
    trajz = trajx; trajz *= vy[1];
    CHECK(ApproxIntvVector(trajz.codomain()) == IntervalVector(vy[1]*vx));
    
    //const Trajectory& operator*=(const Trajectory& x);
    trajz = trajx; trajz *= trajy[1];
    CHECK(ApproxIntvVector(trajz.codomain()) == IntervalVector(vy[1]*vx));

    //const Trajectory& operator/=(double x);
    trajz = trajx; trajz /= vy[1];
    CHECK(ApproxIntvVector(trajz.codomain()) == IntervalVector((1./vy[1])*vx));
    
    //const Trajectory& operator/=(const Trajectory& x);
    trajz = trajx; trajz /= trajy[1];
    CHECK(ApproxIntvVector(trajz.codomain()) == IntervalVector((1./vy[1])*vx));
  }
  
  SECTION("Tests vector traj")
  {
    TubeVector tube(Interval(0,10), 1., 3);
    tube[0].set(Interval(2,3));
    tube[1].set(Interval(1,5));
    tube[2] = max(tube[0],tube[1]);
    CHECK(tube[2].codomain() == Interval(2,5));
  }
}