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

TEST_CASE("Serialization")
{
  Vector v({2,3,6});

  {
    std::ofstream out("data.cdc", std::ios::binary);
    serialize(out, v);
    out.close();
  }

  {
    Vector v_deserialized;
    std::ifstream in("data.cdc", std::ios::binary);
    deserialize(in, v_deserialized);
    CHECK(v == v_deserialized);
    in.close();
  }
}