/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Vector.h>
#include <codac2_Matrix.h>

using namespace std;
using namespace codac2;

TEST_CASE("Matrix")
{
  Matrix x({
  	{ -1, 3  },
  	{ -6, -9 }
  });

  CHECK(abs(x) == Matrix({
  	{ 1, 3 },
  	{ 6, 9 }
  }));

  Matrix y({
    { -1.2, 3.9  },
    { -6.2, -9.0 }
  });

  CHECK(floor(y) == Matrix({
    { -2, 3  },
    { -7, -9 }
  }));

  CHECK(ceil(y) == Matrix({
    { -1, 4  },
    { -6, -9 }
  }));

  CHECK(round(y) == Matrix({
    { -1, 4  },
    { -6, -9 }
  }));
}