/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_ibex.h>

using namespace std;
using codac2::oo;

TEST_CASE("IBEX")
{
  // [Interval] Codac -> IBEX - Non const ref
  {
    codac2::Interval codac_x(1,2);
    CHECK((codac2::to_ibex(codac_x) &= ibex::Interval(0,1)) == ibex::Interval(1.));
    CHECK(codac_x == codac2::Interval(1.));
  }

  // [Interval] Codac -> IBEX - Const ref
  {
    CHECK(codac2::to_ibex(codac2::Interval(1,2)) == ibex::Interval(1,2));
    CHECK(codac2::to_ibex(codac2::Interval(-oo,oo)) == ibex::Interval());
    CHECK(codac2::to_ibex(codac2::Interval::empty()) == ibex::Interval::empty_set());
  }

  // [Interval] IBEX -> Codac
  {
    CHECK(codac2::to_codac(ibex::Interval(1,2)) == codac2::Interval(1,2));
    CHECK(codac2::to_codac(ibex::Interval()) == codac2::Interval(-oo,oo));
    CHECK(codac2::to_codac(ibex::Interval::empty_set()) == codac2::Interval::empty());
  }

  // [Vector] Codac -> IBEX
  {
    CHECK(codac2::to_ibex(codac2::Vector(2)) == ibex::Vector(2));
    CHECK(codac2::to_ibex(codac2::Vector({1,2,3})) == ibex::Vector({1,2,3}));
  }

  // [Vector] IBEX -> Codac
  {
    CHECK(codac2::to_codac(ibex::Vector(2)) == codac2::Vector(2));
    CHECK(codac2::to_codac(ibex::Vector({1,2,3})) == codac2::Vector({1,2,3}));
  }

  // [IntervalVector] Codac -> IBEX
  {
    CHECK(codac2::to_ibex(codac2::IntervalVector(2)) == ibex::IntervalVector(2));
    CHECK(codac2::to_ibex(codac2::IntervalVector({{1,2},{2,3},{-oo,oo}}))
      == ibex::IntervalVector({{1,2},{2,3},ibex::Interval()}));
  }

  // [IntervalVector] IBEX -> Codac
  {
    CHECK(codac2::to_codac(ibex::IntervalVector(2)) == codac2::IntervalVector(2));
    CHECK(codac2::to_codac(ibex::IntervalVector({{1,2},{2,3},ibex::Interval()}))
      == codac2::IntervalVector({{1,2},{2,3},{-oo,oo}}));
  }

  // [Matrix] Codac -> IBEX
  {
    CHECK(codac2::to_ibex(codac2::Matrix(2,3)) == ibex::Matrix(2,3));
    CHECK(codac2::to_ibex(codac2::Matrix({{1,2,3},{4,5,6}}))
      == ibex::Matrix({{1,2,3},{4,5,6}}));
  }

  // [Matrix] IBEX -> Codac
  {
    CHECK(codac2::to_codac(ibex::Matrix(2,3)) == codac2::Matrix(2,3));
    CHECK(codac2::to_codac(ibex::Matrix({{1,2,3},{4,5,6}}))
      == codac2::Matrix({{1,2,3},{4,5,6}}));
  }

  // [IntervalMatrix] Codac -> IBEX
  {
    CHECK(codac2::to_ibex(codac2::IntervalMatrix(2,3)) == ibex::IntervalMatrix(2,3));
    CHECK(codac2::to_ibex(codac2::IntervalMatrix({{{-oo,1},{-oo,2},{-oo,3}},{{-oo,4},{-oo,5},{-oo,6}}}))
      == ibex::IntervalMatrix({{{-oo,1},{-oo,2},{-oo,3}},{{-oo,4},{-oo,5},{-oo,6}}}));
  }

  // [IntervalMatrix] IBEX -> Codac
  {
    CHECK(codac2::to_codac(ibex::IntervalMatrix(2,3)) == codac2::IntervalMatrix(2,3));
    CHECK(codac2::to_codac(ibex::IntervalMatrix({{{-oo,1},{-oo,2},{-oo,3}},{{-oo,4},{-oo,5},{-oo,6}}}))
      == codac2::IntervalMatrix({{{-oo,1},{-oo,2},{-oo,3}},{{-oo,4},{-oo,5},{-oo,6}}}));
  }
}