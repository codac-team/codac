/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_IntervalMatrix.h>
#include <codac2_trunc.h>

using namespace std;
using namespace codac2;

TEST_CASE("trunc")
{
  double l = next_float(-oo), u = prev_float(oo);
  CHECK(codac2::trunc(0) == 0);
  CHECK(codac2::trunc(-oo) == l);
  CHECK(codac2::trunc(oo) == u);
  CHECK(untrunc(l) == -oo);
  CHECK(untrunc(u) == oo);
  CHECK(untrunc(0) == 0);
  
  // trunc matrix types

  CHECK(trunc(IntervalMatrix({
      {{-1,1},{-2,2}},
      {{-3,3},{-4,4}}
    })) == IntervalMatrix({
      {{-1,1},{-2,2}},
      {{-3,3},{-4,4}}
    }));
  
  CHECK(trunc(IntervalMatrix({
      {{-1,oo},{-2,2}},
      {{-3,3},{-4,4}}
    })) == IntervalMatrix({
      {{-1,u},{-2,2}},
      {{-3,3},{-4,4}}
    }));
  
  CHECK(trunc(IntervalMatrix({
      {{-oo,oo},{-2,2}},
      {{-3,3},{-4,4}}
    })) == IntervalMatrix({
      {{l,u},{-2,2}},
      {{-3,3},{-4,4}}
    }));
  
  CHECK(trunc(IntervalMatrix({
      {{-oo,oo},{-oo,oo}},
      {{-oo,oo},{-oo,oo}}
    })) == IntervalMatrix({
      {{l,u},{l,u}},
      {{l,u},{l,u}}
    }));
  
  // untrunc matrix types

  CHECK(untrunc(IntervalMatrix({
      {{-1,1},{-2,2}},
      {{-3,3},{-4,4}}
    })) == IntervalMatrix({
      {{-1,1},{-2,2}},
      {{-3,3},{-4,4}}
    }));
  
  CHECK(untrunc(IntervalMatrix({
      {{-1,u},{-2,2}},
      {{-3,3},{-4,4}}
    })) == IntervalMatrix({
      {{-1,oo},{-2,2}},
      {{-3,3},{-4,4}}
    }));
  
  CHECK(untrunc(IntervalMatrix({
      {{l,u},{-2,2}},
      {{-3,3},{-4,4}}
    })) == IntervalMatrix({
      {{-oo,oo},{-2,2}},
      {{-3,3},{-4,4}}
    }));
  
  CHECK(untrunc(IntervalMatrix({
      {{l,u},{l,u}},
      {{l,u},{l,u}}
    })) == IntervalMatrix({
      {{-oo,oo},{-oo,oo}},
      {{-oo,oo},{-oo,oo}}
    }));
}