/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcLazy.h>
#include <codac2_Approx.h>
#include <codac2_arithmetic.h>

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

TEST_CASE("CtcLazy")
{
  CtcCustom ctc_custom;
  IntervalVector x({{-1000,1000},{-1000,1000}});
  ctc_custom.contract(x);
  CHECK(Approx(x) == IntervalVector({{-500,500},{-500,500}}));
  ctc_custom.contract(x);
  CHECK(Approx(x) == IntervalVector({{-250,250},{-250,250}}));
  ctc_custom.contract(x);
  CHECK(Approx(x) == IntervalVector({{-125,125},{-125,125}}));

  CtcLazy ctc_lazy(ctc_custom);
  
  x = IntervalVector({{-1000,1000},{-1000,1000}});
  ctc_lazy.contract(x);
  CHECK(x == IntervalVector({{-250,250},{-250,250}}));
  
  x = IntervalVector({{-500,500},{-500,500}});
  ctc_lazy.contract(x);
  CHECK(x == IntervalVector({{-125,125},{-125,125}}));
}