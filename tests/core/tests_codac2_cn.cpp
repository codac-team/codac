#include "catch_interval.hpp"
#include "vibes.h"

#include "codac2_ContractorNetwork.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace codac2;

TEST_CASE("Tests for CN")
{
  SECTION("PropagationSpan for two Intervals")
  {
    // Full contraction case
    {
      Interval a(2,4), b(3,5);
      auto span = propagspan_from_contraction(a,b);
      CHECK(span != nullptr);
      auto span_cast = std::dynamic_pointer_cast<PropagationSpan<Interval>>(span);
      CHECK(span_cast);
      CHECK(span_cast->_x == &b);
      CHECK(!span_cast->intersects(std::make_shared<PropagationSpan<Interval>>(&a)));
      CHECK(span_cast->intersects(std::make_shared<PropagationSpan<Interval>>(&b)));
    }

    // No contraction case
    {
      Interval a(2,4), b(2,4);
      auto span = propagspan_from_contraction(a,b);
      CHECK(span == nullptr);
    }
  }

  SECTION("PropagationSpan for two Interval variables")
  {
    // Full contraction case
    {
      Var<Interval> a(2,4), b(3,5);
      auto span = propagspan_from_contraction(a,b);
      CHECK(span != nullptr);
      auto span_cast = std::dynamic_pointer_cast<PropagationSpan<Interval>>(span);
      CHECK(span_cast);
      CHECK(span_cast->_x == &b);
      CHECK(!span_cast->intersects(std::make_shared<PropagationSpan<Interval>>(&a)));
      CHECK(span_cast->intersects(std::make_shared<PropagationSpan<Interval>>(&b)));
    }

    // No contraction case
    {
      Var<Interval> a(2,4), b(2,4);
      auto span = propagspan_from_contraction(a,b);
      CHECK(span == nullptr);
    }
  }

  SECTION("PropagationSpan for two IntervalVectors")
  {
    // Full contraction case
    {
      IntervalVector a({{2,4},{12,14}}), b({{3,5},{13,15}});
      auto span = propagspan_from_contraction(a,b);
      CHECK(span != nullptr);
      auto span_cast = std::dynamic_pointer_cast<PropagationSpan<IntervalVector>>(span);
      CHECK(span_cast);
      CHECK(span_cast->_x == &b);
      CHECK(span_cast->_indices == vector<size_t>({0,1}));
      CHECK(!span_cast->intersects(std::make_shared<PropagationSpan<IntervalVector>>(&a)));
      CHECK(span_cast->intersects(std::make_shared<PropagationSpan<IntervalVector>>(&b)));
    }

    // Partial contraction case
    {
      IntervalVector a({{2,4},{12,14}}), b({{2,4},{13,15}});
      auto span = propagspan_from_contraction(a,b);
      CHECK(span != nullptr);
      auto span_cast = std::dynamic_pointer_cast<PropagationSpan<IntervalVector>>(span);
      CHECK(span_cast);
      CHECK(span_cast->_x == &b);
      CHECK(span_cast->_indices == vector<size_t>({1}));
      CHECK(!span_cast->intersects(std::make_shared<PropagationSpan<IntervalVector>>(&a)));
      CHECK(span_cast->intersects(std::make_shared<PropagationSpan<IntervalVector>>(&b)));
    }

    // No contraction case
    {
      IntervalVector a({{2,4},{12,14}}), b({{2,4},{12,14}});
      auto span = propagspan_from_contraction(a,b);
      CHECK(span == nullptr);
    }
  }

  SECTION("PropagationSpan for two IntervalVector variables")
  {
    // Full contraction case
    {
      Var<IntervalVector> a({{2,4},{12,14}}), b({{3,5},{13,15}});
      auto span = propagspan_from_contraction(a,b);
      CHECK(span != nullptr);
      auto span_cast = std::dynamic_pointer_cast<PropagationSpan<IntervalVector>>(span);
      CHECK(span_cast);
      CHECK(span_cast->_x == &b);
      CHECK(span_cast->_indices == vector<size_t>({0,1}));
      CHECK(!span_cast->intersects(std::make_shared<PropagationSpan<IntervalVector>>(&a)));
      CHECK(span_cast->intersects(std::make_shared<PropagationSpan<IntervalVector>>(&b)));
    }

    // Partial contraction case
    {
      Var<IntervalVector> a({{2,4},{12,14}}), b({{2,4},{13,15}});
      auto span = propagspan_from_contraction(a,b);
      CHECK(span != nullptr);
      auto span_cast = std::dynamic_pointer_cast<PropagationSpan<IntervalVector>>(span);
      CHECK(span_cast);
      CHECK(span_cast->_x == &b);
      CHECK(span_cast->_indices == vector<size_t>({1}));
      CHECK(!span_cast->intersects(std::make_shared<PropagationSpan<IntervalVector>>(&a)));
      CHECK(span_cast->intersects(std::make_shared<PropagationSpan<IntervalVector>>(&b)));
    }

    // No contraction case
    {
      Var<IntervalVector> a({{2,4},{12,14}}), b({{2,4},{12,14}});
      auto span = propagspan_from_contraction(a,b);
      CHECK(span == nullptr);
    }
  }
}