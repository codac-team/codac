/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_serialization.h>

using namespace std;
using namespace codac2;

TEST_CASE("tools - manual")
{
  {
    // [tools-1-beg]
    IntervalVector x1({{0,1},{-oo,0},Interval::pi()});
    // x1 = [ [0, 1] ; [-inf, 0] ; [3.14159, 3.1416] ]

    // Serializing the interval vector
    std::ofstream out("data.cdc", std::ios::binary);
    serialize(out, x1);
    out.close();

    // Deserializing the interval vector (possibly in another project)
    IntervalVector x2;
    std::ifstream in("data.cdc", std::ios::binary);
    deserialize(in, x2);
    in.close();

    // x1 == x2
    // [tools-1-end]
    CHECK(x1 == x2);
  }
}