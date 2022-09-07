#include <cstdio>
#include "catch_interval.hpp"
#include "codac_SepTransform.h"
#include "ibex_Sep.h"
#include "codac_SepPolygon.h"
#include "codac_CtcSegment.h"

#include <algorithm> // std::reverse

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

TEST_CASE("CtcSegment")
{
  SECTION("test_CtcSegment01")
  {
    CtcSegment c(0, 0, 10, 20);
    IntervalVector box({Interval(-5, 50), Interval(-5, 50)});
    IntervalVector resbox({Interval(0, 10), Interval(0, 20)});

    c.contract(box);
    CHECK(box == resbox);
  }

  SECTION("test_CtcSegment02")
  {
    CtcSegment c(0, 0, 10, 20);
    IntervalVector box({Interval(10, 50), Interval(20, 50)});
    IntervalVector resbox({Interval(10, 10), Interval(20, 20)});

    c.contract(box);
    CHECK(box == resbox);
  }

  SECTION("test_contract_degenerate")
  {
    CtcSegment c(5, 5, 5, 5);

    IntervalVector box({Interval(-5, 50), Interval(-5, 50)});
    IntervalVector resbox({Interval(5, 5), Interval(5, 5)});

    c.contract(box);
    CHECK(box == resbox);
  }

  SECTION("test_contract_degenerate_x")
  {
    CtcSegment c(5, 5, 10, 5);

    IntervalVector box({Interval(-50, 50), Interval(-50, 50)});
    IntervalVector resbox({Interval(5, 10), Interval(5, 5)});

    c.contract(box);
    CHECK(box == resbox);
  }

  SECTION("test_contract_degenerate_y")
  {
    CtcSegment c(-5, -5, -5, 15);

    IntervalVector box({Interval(-50, 50), Interval(-50, 50)});
    IntervalVector resbox({Interval(-5, -5), Interval(-5, 15)});

    c.contract(box);
    CHECK(box == resbox);
  }

  SECTION("test_contract_empty")
  {
    CtcSegment c(0, 0, 10, 20);
    IntervalVector box({Interval(-5, -2), Interval(-5, 50)});
    IntervalVector resbox = IntervalVector::empty(2);

    c.contract(box);
    CHECK(box == resbox);
  }

  SECTION("test_call_with_empty_box")
  {
    CtcSegment c(0, 0, 10, 20);
    IntervalVector box = IntervalVector(2, Interval::ALL_REALS);
    IntervalVector resbox({Interval(0, 10), Interval(0, 20)});

    c.contract(box);
    CHECK(box == resbox);
  }
}

TEST_CASE("PdcPointInPoly")
{
  SECTION("test normal vertices")
  {
    std::vector<std::vector<double>> poly1_vertices = {
        {6, -6},
        {7, 9},
        {0, 5},
        {-9, 8},
        {-8, -9}};

    PdcInPolygon pdc(poly1_vertices);
    IntervalVector X0 = IntervalVector({Interval(-10, 10), Interval(-10, 10)});

    CHECK(pdc.test(IntervalVector({0, 0})) == ibex::YES);
    CHECK(pdc.test(IntervalVector({8, 0})) == ibex::NO);
  }

  SECTION("test reverse vertices ")
  {

    // Same result as before. (Is it normal ?)
    std::vector<std::vector<double>> vertices = {
        {6, -6},
        {7, 9},
        {0, 5},
        {-9, 8},
        {-8, -9}};

    std::reverse(vertices.begin(), vertices.end());
    PdcInPolygon pdc(vertices);
    // IntervalVector X0 = IntervalVector({Interval(-10, 10), Interval(-10, 10)});

    CHECK(pdc.test(IntervalVector({0, 0})) == ibex::YES);
    CHECK(pdc.test(IntervalVector({8, 0})) == ibex::NO);
  }
}

TEST_CASE("SepPolygon")
{

  SECTION("polygone with hole")
  {
    std::vector<std::vector<std::vector<double>>> vertices = {
        // external border
        {{6, -6}, {7, 9}},
        {{7, 9}, {0, 5}},
        {{0, 5}, {-9, 8}},
        {{-9, 8}, {-8, -9}},
        {{-8, -9}, {6, -6}},
        // hole
        {{-2, 3}, {3.5, 2}},
        {{3.5, 2}, {1.5, 0.5}},
        {{1.5, 0.5}, {3, -4}},
        {{3, -4}, {-3, -3}},
        {{-3, -3}, {-2, 3}}
    };

    SepPolygon sep(vertices);
    // check a box inside the hole
    {
      IntervalVector X0 = IntervalVector({Interval(0), Interval(0)}).inflate(0.5);
      IntervalVector xin(X0), xout(X0);
      sep.separate(xin, xout);
      CHECK(xin == X0);
      CHECK(xout.is_empty());
    }

    // check a box inside the polygone
    {
      IntervalVector X0 = IntervalVector({Interval(5), Interval(-5)}).inflate(0.5);
      IntervalVector xin(X0), xout(X0);
      sep.separate(xin, xout);
      CHECK(xin.is_empty());
      CHECK(xout == X0);
    }

    // check a box outside the polygon
    {
      IntervalVector X0 = IntervalVector({Interval(-1), Interval(8)}).inflate(0.5);
      IntervalVector xin(X0), xout(X0);
      sep.separate(xin, xout);
      CHECK(xin == X0);
      CHECK(xout.is_empty());
    }
    

    // IntervalVector X0 = IntervalVector({Interval(-10, 10), Interval(-10, 10)});
    // SIVIA(X0, sep, 1, true, "SepPolygon_vec_vec_vec_double");
  }

  SECTION("contructor close polygone build with edge")
  {

    std::vector<std::vector<double>> vertices = {
        {6, -6},
        {7, 9},
        {0, 5},
        {-9, 8},
        {-8, -9}};

    SepPolygon sep(vertices);

        // check a box inside the polygone
    {
      IntervalVector X0 = IntervalVector({Interval(5), Interval(-5)}).inflate(0.5);
      IntervalVector xin(X0), xout(X0);
      sep.separate(xin, xout);
      CHECK(xin.is_empty());
      CHECK(xout == X0);
    }

    // check a box outside the polygon
    {
      IntervalVector X0 = IntervalVector({Interval(-1), Interval(8)}).inflate(0.5);
      IntervalVector xin(X0), xout(X0);
      sep.separate(xin, xout);
      CHECK(xin == X0);
      CHECK(xout.is_empty());
    }
  }
}
