/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_linear_ctc.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;


TEST_CASE("Linear contractors")
{
  IntervalMatrix A {
    {{4,5},{-1,1},{1.5,2.5}},
    {{-0.5,0.5},{-7,-5},{1,2}},
    {{-1.5,-0.5},{-0.7,-0.5},{2,3}}
  };
  IntervalVector x {{-oo,oo},{-oo,oo},{-oo,oo}};
  IntervalVector b {{3,4},{0,2},{3,4}};

  // Numerical values come from the book:
  //   Applied Interval Analysis
  //   Luc Jaulin, Michel Kieffer, Olivier Didrit, Éric Walter
  //   2001, Springer London
  //   doi: https://doi.org/10.1007/978-1-4471-0249-6

  // CtcGaussElim

    CtcGaussElim ctc_ge;
    ctc_ge.contract(A,x,b);
    CHECK(Approx(x) == IntervalVector({
      {-1.819278874340436,1.168720384766974},
      {-0.4140698379116945,1.725229626734416},
      {0.7002320584443487,3.42075434825093}
    }));

  // CtcGaussSeidel

    CtcGaussSeidel ctc_gs;
    x = {{-10,10},{-10,10},{-10,10}};
    for(int k = 1 ; k <= 100 ; k++)
      ctc_gs.contract(A,x,b);
    CHECK(Approx(x) == IntervalVector({
      {-2.084512144759555,1.63672286912456},
      {-0.7369829550424718,1.74356809144887},
      {-0.3213286433350618,3.837791484351055}
    }));

  // CtcGaussSeidel with preconditioning

    CtcLinearPrecond ctc_gsp(ctc_gs);
    x = {{-10,10},{-10,10},{-10,10}};
    for(int k = 1 ; k <= 100 ; k++)
      ctc_gsp.contract(A,x,b);
    CHECK(Approx(x) == IntervalVector({
      {-1.106970597649814,1.383669626776028},
      {-0.8113846871807701,1.332001097010008},
      {0.5757415290046493,2.984085335831012}
    }));

  // CtcGaussElim with preconditioning

    CtcLinearPrecond ctc_gep(ctc_ge);
    x = {{-oo,oo},{-oo,oo},{-oo,oo}};
    ctc_gep.contract(A,x,b);
    CHECK(Approx(x) == IntervalVector({
      {-1.106970597649814,1.383669626776028},
      {-0.7852408901116992,1.332001097010009},
      {0.7583501672635362,2.984085335831012}
    }));
}