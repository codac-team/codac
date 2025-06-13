/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <limits>
#include <catch2/catch_test_macros.hpp>
#include <codac2_Interval.h>
#include <codac2_IntervalVector.h>
#include <codac2_TDomain.h>
#include <codac2_SlicedTube.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

TEST_CASE("TDomain")
{
  SECTION("Test TDomain")
  {
    auto tdomain = create_tdomain();
    tdomain->sample(Interval(0,1), 0.5);
    CHECK(tdomain->nb_tslices() == 4);
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    
    vector<TSlice> vector_tslices = tdomain->tslices_vector();

    CHECK(vector_tslices.size() == 4);
    CHECK(vector_tslices[0] == Interval(-oo,0));
    CHECK(vector_tslices[1] == Interval(0,0.5));
    CHECK(vector_tslices[2] == Interval(0.5,1));
    CHECK(vector_tslices[3] == Interval(1,oo));

    CHECK(*tdomain->tslice(-10.) == Interval(-oo,0));
    CHECK(*tdomain->tslice(-120.) == Interval(-oo,0));
    CHECK(*tdomain->tslice(0.2) == Interval(0,0.5));
    CHECK(*tdomain->tslice(5540.2) == Interval(1,oo));

    CHECK(tdomain->nb_tubes() == 0);
    SlicedTube x(tdomain, IntervalVector(2));
    CHECK(tdomain->nb_tubes() == 1);

    { // new scope
      SlicedTube v(tdomain, IntervalVector(3));
      CHECK(tdomain->nb_tubes() == 2);
    } // end of scope, auto removing the tube
    
    CHECK(tdomain->nb_tubes() == 1);
  }

  SECTION("Test degenerated TDomain")
  {
    auto tdomain = create_tdomain(Interval(1));
    CHECK(tdomain->nb_tslices() == 1);
    CHECK(tdomain->t0_tf() == Interval(1));
    CHECK(tdomain->nb_tubes() == 0);
    
    vector<TSlice> vector_tslices = tdomain->tslices_vector();
      
    CHECK(vector_tslices.size() == 1);
    CHECK(vector_tslices[0] == Interval(1));
  }

  SECTION("Test TDomain with gates")
  {
    auto tdomain = create_tdomain(Interval(0,1), 0.5, true);
    CHECK(tdomain->nb_tslices() == 5);
    CHECK(tdomain->t0_tf() == Interval(0,1));
    CHECK(tdomain->nb_tubes() == 0);
    
    vector<TSlice> vector_tslices = tdomain->tslices_vector();

    CHECK(vector_tslices.size() == 5);
    CHECK(vector_tslices[0] == Interval(0));
    CHECK(vector_tslices[1] == Interval(0,0.5));
    CHECK(vector_tslices[2] == Interval(0.5));
    CHECK(vector_tslices[3] == Interval(0.5,1));
    CHECK(vector_tslices[4] == Interval(1,1));

    CHECK(*tdomain->tslice(0.) == Interval(0));
    CHECK(*tdomain->tslice(0.1) == Interval(0,0.5));
    CHECK(*tdomain->tslice(0.5) == Interval(0.5));
    CHECK(*tdomain->tslice(0.6) == Interval(0.5,1));
    CHECK(*tdomain->tslice(1.) == Interval(1));
  }

  SECTION("Test TDomain with sampling")
  {
    auto tdomain = create_tdomain();
    tdomain->sample(1.);
    CHECK(tdomain->nb_tslices() == 2);
    tdomain->sample(10.,false); // no gate
    CHECK(tdomain->nb_tslices() == 3);
    tdomain->sample(10.,true); // second sampling with gate
    CHECK(tdomain->nb_tslices() == 4);
    tdomain->sample(10.,true); // no more action
    CHECK(tdomain->nb_tslices() == 4);
    
    vector<TSlice> vector_tslices = tdomain->tslices_vector();

    CHECK(vector_tslices.size() == 4);
    CHECK(vector_tslices[0] == Interval(-oo,1));
    CHECK(vector_tslices[1] == Interval(1,10));
    CHECK(vector_tslices[2] == Interval(10));
    CHECK(vector_tslices[3] == Interval(10,oo));
  }

  SECTION("Test unbounded TDomain")
  {
    auto tdomain = create_tdomain();
    CHECK(tdomain->nb_tslices() == 1);
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    
    vector<TSlice> vector_tslices = tdomain->tslices_vector();

    CHECK(vector_tslices.size() == 1);
    CHECK(vector_tslices[0] == Interval(-oo,oo));
  }
}