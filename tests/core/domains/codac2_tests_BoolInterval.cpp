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

TEST_CASE("BoolInterval bitwise operators (&, |)")
{
  // Intersection (&)
  CHECK((BoolInterval::TRUE & BoolInterval::TRUE) == BoolInterval::TRUE);
  CHECK((BoolInterval::TRUE & BoolInterval::FALSE) == BoolInterval::EMPTY);
  CHECK((BoolInterval::TRUE & BoolInterval::UNKNOWN) == BoolInterval::TRUE);
  CHECK((BoolInterval::TRUE & BoolInterval::EMPTY) == BoolInterval::EMPTY);

  CHECK((BoolInterval::FALSE & BoolInterval::FALSE) == BoolInterval::FALSE);
  CHECK((BoolInterval::FALSE & BoolInterval::UNKNOWN) == BoolInterval::FALSE);
  CHECK((BoolInterval::FALSE & BoolInterval::EMPTY) == BoolInterval::EMPTY);

  CHECK((BoolInterval::EMPTY & BoolInterval::EMPTY) == BoolInterval::EMPTY);
  CHECK((BoolInterval::EMPTY & BoolInterval::UNKNOWN) == BoolInterval::EMPTY);

  CHECK((BoolInterval::UNKNOWN & BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);

  // Union (|)
  CHECK((BoolInterval::TRUE | BoolInterval::TRUE) == BoolInterval::TRUE);
  CHECK((BoolInterval::TRUE | BoolInterval::FALSE) == BoolInterval::UNKNOWN);
  CHECK((BoolInterval::TRUE | BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);
  CHECK((BoolInterval::TRUE | BoolInterval::EMPTY) == BoolInterval::TRUE);

  CHECK((BoolInterval::FALSE | BoolInterval::FALSE) == BoolInterval::FALSE);
  CHECK((BoolInterval::FALSE | BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);
  CHECK((BoolInterval::FALSE | BoolInterval::EMPTY) == BoolInterval::FALSE);

  CHECK((BoolInterval::EMPTY | BoolInterval::EMPTY) == BoolInterval::EMPTY);
  CHECK((BoolInterval::EMPTY | BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);

  CHECK((BoolInterval::UNKNOWN | BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);
}

TEST_CASE("BoolInterval logical operators (&&, ||)")
{
  // Logical AND (&&)
  CHECK((BoolInterval::TRUE && BoolInterval::TRUE) == BoolInterval::TRUE);
  CHECK((BoolInterval::TRUE && BoolInterval::FALSE) == BoolInterval::FALSE);
  CHECK((BoolInterval::TRUE && BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);
  CHECK((BoolInterval::TRUE && BoolInterval::EMPTY) == BoolInterval::EMPTY);

  CHECK((BoolInterval::FALSE && BoolInterval::TRUE) == BoolInterval::FALSE);
  CHECK((BoolInterval::FALSE && BoolInterval::FALSE) == BoolInterval::FALSE);
  CHECK((BoolInterval::FALSE && BoolInterval::UNKNOWN) == BoolInterval::FALSE);
  CHECK((BoolInterval::FALSE && BoolInterval::EMPTY) == BoolInterval::EMPTY);

  CHECK((BoolInterval::UNKNOWN && BoolInterval::TRUE) == BoolInterval::UNKNOWN);
  CHECK((BoolInterval::UNKNOWN && BoolInterval::FALSE) == BoolInterval::FALSE);
  CHECK((BoolInterval::UNKNOWN && BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);
  CHECK((BoolInterval::UNKNOWN && BoolInterval::EMPTY) == BoolInterval::EMPTY);

  CHECK((BoolInterval::EMPTY && BoolInterval::TRUE) == BoolInterval::EMPTY);
  CHECK((BoolInterval::EMPTY && BoolInterval::FALSE) == BoolInterval::EMPTY);
  CHECK((BoolInterval::EMPTY && BoolInterval::UNKNOWN) == BoolInterval::EMPTY);
  CHECK((BoolInterval::EMPTY && BoolInterval::EMPTY) == BoolInterval::EMPTY);

  // Logical OR (||)
  CHECK((BoolInterval::TRUE || BoolInterval::TRUE) == BoolInterval::TRUE);
  CHECK((BoolInterval::TRUE || BoolInterval::FALSE) == BoolInterval::TRUE);
  CHECK((BoolInterval::TRUE || BoolInterval::UNKNOWN) == BoolInterval::TRUE);
  CHECK((BoolInterval::TRUE || BoolInterval::EMPTY) == BoolInterval::EMPTY);

  CHECK((BoolInterval::FALSE || BoolInterval::TRUE) == BoolInterval::TRUE);
  CHECK((BoolInterval::FALSE || BoolInterval::FALSE) == BoolInterval::FALSE);
  CHECK((BoolInterval::FALSE || BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);
  CHECK((BoolInterval::FALSE || BoolInterval::EMPTY) == BoolInterval::EMPTY);

  CHECK((BoolInterval::UNKNOWN || BoolInterval::TRUE) == BoolInterval::TRUE);
  CHECK((BoolInterval::UNKNOWN || BoolInterval::FALSE) == BoolInterval::UNKNOWN);
  CHECK((BoolInterval::UNKNOWN || BoolInterval::UNKNOWN) == BoolInterval::UNKNOWN);
  CHECK((BoolInterval::UNKNOWN || BoolInterval::EMPTY) == BoolInterval::EMPTY);

  CHECK((BoolInterval::EMPTY || BoolInterval::TRUE) == BoolInterval::EMPTY);
  CHECK((BoolInterval::EMPTY || BoolInterval::FALSE) == BoolInterval::EMPTY);
  CHECK((BoolInterval::EMPTY || BoolInterval::UNKNOWN) == BoolInterval::EMPTY);
  CHECK((BoolInterval::EMPTY || BoolInterval::EMPTY) == BoolInterval::EMPTY);
}

TEST_CASE("BoolInterval complement (~)")
{
  CHECK(~BoolInterval::TRUE == BoolInterval::FALSE);
  CHECK(~BoolInterval::FALSE == BoolInterval::TRUE);
  CHECK(~BoolInterval::UNKNOWN == BoolInterval::UNKNOWN);
  CHECK(~BoolInterval::EMPTY == BoolInterval::EMPTY);
}