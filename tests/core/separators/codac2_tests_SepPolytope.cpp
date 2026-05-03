/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_SepPolytope.h>
#include <codac2_CtcPolytope.h>
#include <codac2_pave.h>
#include <codac2_Figure3D.h>

using namespace std;
using namespace codac2;

TEST_CASE("SepPolytope")
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
  SepPolytope s(p);

  IntervalVector x(3);
  auto xs = s.separate(x);
  CHECK(xs.outer == IntervalVector({{-2,2},{-2,2},{-2,2}}));
  CHECK(xs.inner == IntervalVector(3));

  x = IntervalVector({{1.02,2.0},{1.02,2.0},{1.02,2.0}}); 
  xs = s.separate(x);
  CHECK(xs.inner == x);
  CHECK(xs.outer.is_empty());

}

