#include <cstdio>
#include "catch_interval.hpp"
#include "codac_SepTransform.h"
#include "ibex_Sep.h"
#include "codac_SepPolygon.h"
#include "codac_CtcSegment.h"
#include "codac_SepFixPoint.h"
#include "codac_SepCtcPairProj.h"
#include "codac_SepProj.h"
#include "codac_sivia.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

class FakeSep : public ibex::Sep
  {
    public:

      /**
       * \brief fake separator used for testing the fix point
       *
       */
      FakeSep() : Sep(3), count(0) {};

      /**
       * \brief \f$\mathcal{S}\big([\mathbf{x}_{\textrm{in}}],[\mathbf{x}_{\textrm{out}}]\big)\f$
       *
       * \param x_in the n-dimensional box \f$[\mathbf{x}_{\textrm{in}}]\f$ to be inner-contracted
       * \param x_out the n-dimensional box \f$[\mathbf{x}_{\textrm{out}}]\f$ to be outer-contracted
       */
      virtual void separate(IntervalVector& x_in, IntervalVector& x_out){

          
          IntervalVector x = x_in & x_out;
          IntervalVector xpos = x[0] & Interval::POS_REALS;
          IntervalVector xneg = x[0] & Interval::NEG_REALS;

          x_in[0] &=  Interval::NEG_REALS | IntervalVector(xpos.mid())[0]; //| IntervalVector([0]).inflate(1e-10)
          x_out[0] &= Interval::POS_REALS | IntervalVector(xneg.mid())[0]; //| IntervalVector([0]).inflate(1e-10)
          count++;
      }

        size_t count;
  };
  


TEST_CASE("FixPoint tests")
{
    FakeSep S;
    SepFixPoint sep(S);
    IntervalVector x_in(3, Interval(-10, 10)), x_out(3, Interval(-10, 10));


    sep.separate(x_in, x_out);
    //cout << "FakeSep called " << S.count << " time\n";
    CHECK(x_out[0].contains({0}));
    CHECK(x_in[0].contains({0}));
    CHECK((x_in[0] | x_out[0]) == Interval(-10, 10));
    CHECK((x_in[0] & x_out[0]) == Interval(0));
    CHECK(x_out[0] == Interval(0, 10));
    CHECK(x_in[0] == Interval(-10, 0));
    CHECK(S.count > 1000);
}

TEST_CASE("SepProj")
{
    Function f("x", "y", "z", "x^2 + y^2 + 2*x*y + z^2 + 3*z*x -9");
    IntervalVector yinit(1, Interval(1, 2));
    IntervalVector X0(2, Interval(-30, 30));

    SECTION("SEPPROJ"){
      SepFwdBwd sepfb(f, ibex::LEQ);
      SepProj sep(sepfb, yinit, 0.01);
      SepFixPoint S(sep);
      // cout << "SIVIA SepProj\n";
      // SIVIA(X0, sep, 0.1, true, "SepProj");
      SECTION("Test Inner separation") {
        X0 = IntervalVector({-2, 1.5}).inflate(0.5);
        IntervalVector xin(X0), xout(X0);
        S.separate(xin, xout);
        CHECK(xin.is_empty());
        CHECK(xout == X0);
      }

      SECTION("Test Outer separation") {
        X0 = IntervalVector({5, -7}).inflate(0.5);
        IntervalVector xin(X0), xout(X0);
        S.separate(xin, xout);
        CHECK(xout.is_empty());
        CHECK(xin == X0);
      }
    }

    SECTION("SepCtcPairProj"){
// SepFixPoint S(sep);

    CtcFwdBwd sep_in(f, ibex::GEQ);
    CtcFwdBwd sep_out(f, ibex::LEQ);
    SepCtcPairProj sep(sep_in, sep_out , yinit, 0.01);
    SepFixPoint S(sep);
    SECTION("Test Inner separation") {
        X0 = IntervalVector({-2, 1.5}).inflate(0.5);
        IntervalVector xin(X0), xout(X0);
        S.separate(xin, xout);
        CHECK(xin.is_empty());
        CHECK(xout == X0);
      }

      SECTION("Test Outer separation") {
        X0 = IntervalVector({5, -7}).inflate(0.5);
        IntervalVector xin(X0), xout(X0);
        S.separate(xin, xout);
        CHECK(xout.is_empty());
        CHECK(xin == X0);
      }
    // cout << "SIVIA SepCtcPairProj\n";
    // SIVIA(X0, sep, 0.1, true, "SepCtcPairProj");
    }
}
