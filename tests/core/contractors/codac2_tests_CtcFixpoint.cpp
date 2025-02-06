/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcFixpoint.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

class CtcCustom : public Ctc<CtcCustom,IntervalVector>
{
  public:

    CtcCustom()
      : Ctc<CtcCustom,IntervalVector>(2)
    { }

    void contract(IntervalVector& x) const
    {
      if(x[0].rad() > 1.)
        x = IntervalVector(x.mid()).inflate(0.5*x.rad());
    }
};

TEST_CASE("CtcFixpoint")
{
  CtcCustom ctc_custom;
  IntervalVector x({{-1000,1000},{-1000,1000}});
  ctc_custom.contract(x);
  CHECK(Approx(x) == IntervalVector({{-500,500},{-500,500}}));
  ctc_custom.contract(x);
  CHECK(Approx(x) == IntervalVector({{-250,250},{-250,250}}));
  ctc_custom.contract(x);
  CHECK(Approx(x) == IntervalVector({{-125,125},{-125,125}}));

  {
    CtcFixpoint ctc_fixed(ctc_custom, 0.);
    
    x = IntervalVector({{-1000,1000},{-1000,1000}});
    ctc_fixed.contract(x);
    CHECK(Approx(x,1e-1) == IntervalVector({{-1,1},{-1,1}}));
    x = IntervalVector({{-500,500},{-500,500}});
    ctc_fixed.contract(x);
    CHECK(Approx(x,1e-1) == IntervalVector({{-1,1},{-1,1}}));
  }

  {
    CtcFixpoint ctc_fixed(ctc_custom, 0.8);
    
    x = IntervalVector({{-1000,1000},{-1000,1000}});
    ctc_fixed.contract(x);
    CHECK(Approx(x) == IntervalVector({{-500,500},{-500,500}}));
    ctc_fixed.contract(x);
    CHECK(Approx(x) == IntervalVector({{-250,250},{-250,250}}));
  }

  {
    CtcFixpoint ctc_fixed(ctc_custom, 1.);
    
    x = IntervalVector({{-1000,1000},{-1000,1000}});
    ctc_fixed.contract(x);
    CHECK(Approx(x) == IntervalVector({{-500,500},{-500,500}}));
    ctc_fixed.contract(x);
    CHECK(Approx(x) == IntervalVector({{-250,250},{-250,250}}));
  }
}