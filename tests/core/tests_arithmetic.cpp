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
    // CHECK(ApproxIntv(pow(x,7.2).codomain()) == pow(a,7.2)); // todo: not working with GAOL

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

    //const TubeVector operator+(const TubeVector& x, const ibex::IntervalVector& y);
    z = x + iy;
    CHECK(z[0].codomain() == a0+b0); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b2); 

    //const TubeVector operator+(const ibex::IntervalVector& x, const TubeVector& y);
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

    //const TubeVector operator-(const TubeVector& x, const ibex::IntervalVector& y);
    z = x - iy;
    CHECK(z[0].codomain() == a0-b0); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b2); 

    //const TubeVector operator-(const ibex::IntervalVector& x, const TubeVector& y);
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

    //const TubeVector operator*(const ibex::Interval& x, const TubeVector& y);
    z = Interval(2) * y;
    CHECK(z[0].codomain() == 2*b0); CHECK(z[1].codomain() == 2*b1); CHECK(z[2].codomain() == 2*b2); 

    //const TubeVector operator*(const Tube& x, const ibex::IntervalVector& y);
    z = x[1] * iy;
    CHECK(z[0].codomain() == a1*b0); CHECK(z[1].codomain() == a1*b1); CHECK(z[2].codomain() == a1*b2); 

    //const TubeVector operator*(const Trajectory& x, const TubeVector& y);
    z = trajx[1] * y;
    CHECK(z[0].codomain() == a1*b0); CHECK(z[1].codomain() == a1*b1); CHECK(z[2].codomain() == a1*b2); 

    //const TubeVector operator/(const TubeVector& x, const Tube& y);
    z = x / y[0];
    CHECK(z[0].codomain() == a0/b0); CHECK(z[1].codomain() == a1/b0); CHECK(z[2].codomain() == a2/b0); 

    //const TubeVector operator/(const TubeVector& x, const ibex::Interval& y);
    z = x / iy[0];
    CHECK(z[0].codomain() == a0/b0); CHECK(z[1].codomain() == a1/b0); CHECK(z[2].codomain() == a2/b0); 

    //const TubeVector operator/(const ibex::IntervalVector& x, const Tube& y);
    z = ix / y[0];
    CHECK(z[0].codomain() == a0/b0); CHECK(z[1].codomain() == a1/b0); CHECK(z[2].codomain() == a2/b0); 

    //const TubeVector operator/(const TubeVector& x, const Trajectory& y);
    z = x / trajy[0];
    CHECK(z[0].codomain() == a0/b0); CHECK(z[1].codomain() == a1/b0); CHECK(z[2].codomain() == a2/b0); 

    //const TubeVector operator|(const TubeVector& x, const TubeVector& y);
    z = x | y;
    CHECK(z[0].codomain() == (Interval(a0) | b0)); CHECK(z[1].codomain() == (Interval(a1) | b1)); CHECK(z[2].codomain() == (Interval(a2) | b2)); 

    //const TubeVector operator|(const TubeVector& x, const ibex::IntervalVector& y);
    z = x | iy;
    CHECK(z[0].codomain() == (Interval(a0) | b0)); CHECK(z[1].codomain() == (Interval(a1) | b1)); CHECK(z[2].codomain() == (Interval(a2) | b2)); 

    //const TubeVector operator|(const ibex::IntervalVector& x, const TubeVector& y);
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

    //const TubeVector operator&(const TubeVector& x, const ibex::IntervalVector& y);
    z = x & iy;
    CHECK(z[0].codomain() == Interval::EMPTY_SET); CHECK(z[1].codomain() == a1); CHECK(z[2].codomain() == a2); 

    //const TubeVector operator&(const ibex::IntervalVector& x, const TubeVector& y);
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

    //const TubeVector& operator+=(const ibex::Interval& x);
    z = x; z+=iy[1];
    CHECK(z[0].codomain() == a0+b1); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b1); 

    //const TubeVector& operator+=(const Trajectory& x);
    z = x; z+=trajy[1];
    CHECK(z[0].codomain() == a0+b1); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b1); 

    //const TubeVector& operator+=(const Tube& x);
    z = x; z+=y[1];
    CHECK(z[0].codomain() == a0+b1); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b1); 

    //const TubeVector& operator+=(const ibex::IntervalVector& x);
    z = x; z+=iy;
    CHECK(z[0].codomain() == a0+b0); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b2); 

    //const TubeVector& operator+=(const TrajectoryVector& x);
    z = x; z+=trajy;
    CHECK(z[0].codomain() == a0+b0); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b2); 

    //const TubeVector& operator+=(const TubeVector& x);
    z = x; z+=y;
    CHECK(z[0].codomain() == a0+b0); CHECK(z[1].codomain() == a1+b1); CHECK(z[2].codomain() == a2+b2); 

    //const TubeVector& operator-=(const ibex::Interval& x);
    z = x; z-=iy[1];
    CHECK(z[0].codomain() == a0-b1); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b1); 

    //const TubeVector& operator-=(const Trajectory& x);
    z = x; z-=trajy[1];
    CHECK(z[0].codomain() == a0-b1); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b1); 

    //const TubeVector& operator-=(const Tube& x);
    z = x; z-=y[1];
    CHECK(z[0].codomain() == a0-b1); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b1); 

    //const TubeVector& operator-=(const ibex::IntervalVector& x);
    z = x; z-=iy;
    CHECK(z[0].codomain() == a0-b0); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b2); 

    //const TubeVector& operator-=(const TrajectoryVector& x);
    z = x; z-=trajy;
    CHECK(z[0].codomain() == a0-b0); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b2); 

    //const TubeVector& operator-=(const TubeVector& x);
    z = x; z-=y;
    CHECK(z[0].codomain() == a0-b0); CHECK(z[1].codomain() == a1-b1); CHECK(z[2].codomain() == a2-b2); 

    //const TubeVector& operator*=(const ibex::Interval& x);
    z = x; z*=iy[1];
    CHECK(z[0].codomain() == a0*b1); CHECK(z[1].codomain() == a1*b1); CHECK(z[2].codomain() == a2*b1); 

    //const TubeVector& operator*=(const Trajectory& x);
    z = x; z*=trajy[1];
    CHECK(z[0].codomain() == a0*b1); CHECK(z[1].codomain() == a1*b1); CHECK(z[2].codomain() == a2*b1); 

    //const TubeVector& operator*=(const Tube& x);
    z = x; z*=y[1];
    CHECK(z[0].codomain() == a0*b1); CHECK(z[1].codomain() == a1*b1); CHECK(z[2].codomain() == a2*b1); 

    //const TubeVector& operator/=(const ibex::Interval& x);
    z = x; z/=iy[1];
    CHECK(ApproxIntv(z[0].codomain()) == a0/b1); CHECK(ApproxIntv(z[1].codomain()) == a1/b1); CHECK(ApproxIntv(z[2].codomain()) == a2/b1); 

    //const TubeVector& operator/=(const Trajectory& x);
    z = x; z/=trajy[1];
    CHECK(ApproxIntv(z[0].codomain()) == a0/b1); CHECK(ApproxIntv(z[1].codomain()) == a1/b1); CHECK(ApproxIntv(z[2].codomain()) == a2/b1); 

    //const TubeVector& operator/=(const Tube& x);
    z = x; z/=y[1];
    CHECK(ApproxIntv(z[0].codomain()) == a0/b1); CHECK(ApproxIntv(z[1].codomain()) == a1/b1); CHECK(ApproxIntv(z[2].codomain()) == a2/b1); 

    //const TubeVector& operator|=(const ibex::IntervalVector& x);
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

    //const TubeVector& operator&=(const ibex::IntervalVector& x);
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