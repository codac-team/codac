/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_BoolInterval.h>

using namespace std;
using namespace codac2;

TEST_CASE("BoolInterval")
{
  CHECK((BoolInterval::YES & BoolInterval::YES) == BoolInterval::YES);
  CHECK((BoolInterval::YES & BoolInterval::NO) == BoolInterval::EMPTY);
  CHECK((BoolInterval::YES & BoolInterval::UNKNOWN) == BoolInterval::YES);
  CHECK((BoolInterval::YES & BoolInterval::EMPTY) == BoolInterval::EMPTY);
  CHECK((BoolInterval::NO & BoolInterval::NO) == BoolInterval::NO);
  CHECK((BoolInterval::NO & BoolInterval::UNKNOWN) == BoolInterval::NO);
  CHECK((BoolInterval::NO & BoolInterval::EMPTY) == BoolInterval::EMPTY);
  CHECK((BoolInterval::EMPTY & BoolInterval::EMPTY) == BoolInterval::EMPTY);
  CHECK((BoolInterval::EMPTY & BoolInterval::UNKNOWN) == BoolInterval::EMPTY);
  CHECK((BoolInterval::UNKNOWN & BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);

  CHECK((BoolInterval::YES | BoolInterval::YES) == BoolInterval::YES);
  CHECK((BoolInterval::YES | BoolInterval::NO) == BoolInterval::UNKNOWN);
  CHECK((BoolInterval::YES | BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);
  CHECK((BoolInterval::YES | BoolInterval::EMPTY) == BoolInterval::YES);
  CHECK((BoolInterval::NO | BoolInterval::NO) == BoolInterval::NO);
  CHECK((BoolInterval::NO | BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);
  CHECK((BoolInterval::NO | BoolInterval::EMPTY) == BoolInterval::NO);
  CHECK((BoolInterval::EMPTY | BoolInterval::EMPTY) == BoolInterval::EMPTY);
  CHECK((BoolInterval::EMPTY | BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);
  CHECK((BoolInterval::UNKNOWN | BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);
}