/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcPolytope.h>
#include <codac2_pave.h>
#include <codac2_Figure3D.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcPolytope")
{
   std::vector<std::pair<Row,double>> facets 
       { { {1,1,1}, 3.0 },
         { {-1,1,1}, 3.0 },
         { {1,-1,1}, 3.0 },
         { {1,1,-1}, 3.0 },
         { {-1,-1,1}, 3.0 },
         { {-1,1,-1}, 3.0 },
         { {1,-1,-1}, 3.0 },
         { {-1,-1,-1}, 3.0 } };
  Polytope p({{-2,2.0},{-2.0,2.0},{-2.0,2.0}}, facets);
  CtcPolytope c(p);

  IntervalVector x(3);
  c.contract(x);
  CHECK(x == IntervalVector({{-2,2},{-2,2},{-2,2}}));

  x = IntervalVector({{1.02,2.0},{1.02,2.0},{1.02,2.0}}); // possible bug
  c.contract(x);
  CHECK(x.is_empty());

//  auto p_ctc  = pave({{-3,3},{-3,3},{-3,3}}, c, 0.2);
//  Figure3D fig_ctc("Paving contractor");
//  fig_ctc.draw_paving(p_ctc);
}

