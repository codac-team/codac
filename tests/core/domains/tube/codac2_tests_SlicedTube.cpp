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
#include <codac2_SlicedTube.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

SlicedTube<IntervalVector> return_a_tube()
{
  return SlicedTube(
    create_tdomain(Interval(0,2),0.5),
    IntervalVector::constant(3,Interval(-1.5,1)));
}

TEST_CASE("SlicedTube")
{
  SECTION("Test TDomain with sampling and values")
  {
    auto tdomain = create_tdomain();
    CHECK(tdomain->nb_tslices() == 1);
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    SlicedTube x(tdomain, IntervalVector(1));
    x.set(IntervalVector({{1,5}}), {0,1});
    x.set(IntervalVector({{2,8}}), {1,2});
    x.set(IntervalVector({{6,9}}), {2,3});

    // Checking structure
    vector<const Slice<IntervalVector>*> v;
    for(const auto& s : x)
      v.push_back(&s);
    CHECK(v[0]->t0_tf() == Interval(-oo,0));
    CHECK(v[0]->codomain() == IntervalVector({{-oo,oo}}));
    CHECK(v[1]->t0_tf() == Interval(0,1));
    CHECK(v[1]->codomain() == IntervalVector({{1,5}}));
    CHECK(v[2]->t0_tf() == Interval(1,2));
    CHECK(v[2]->codomain() == IntervalVector({{2,8}}));
    CHECK(v[3]->t0_tf() == Interval(2,3));
    CHECK(v[3]->codomain() == IntervalVector({{6,9}}));
    CHECK(v[4]->t0_tf() == Interval(3,oo));
    CHECK(v[4]->codomain() == IntervalVector({{-oo,oo}}));

    CHECK(*tdomain->tslice(-1.) == Interval(-oo,0));
    CHECK(*tdomain->tslice(0.) == Interval(0,1));
    CHECK(*tdomain->tslice(0.01) == Interval(0,1));
    CHECK(*tdomain->tslice(1) == Interval(1,2));
    CHECK(*tdomain->tslice(2) == Interval(2,3));
    CHECK(*tdomain->tslice(previous_float(3.)) == Interval(2,3));
    CHECK(*tdomain->tslice(3) == Interval(3,oo));
    CHECK(*tdomain->tslice(next_float(3.)) == Interval(3,oo));

    CHECK(tdomain->nb_tslices() == 5); // with [-oo,0] and [3,oo]
    CHECK(x(Interval(0,3)) == IntervalVector({{1,9}}));
    CHECK(x(-1) == IntervalVector(1));
    CHECK(x(0.5) == IntervalVector({{1,5}}));
    CHECK(x(1.5) == IntervalVector({{2,8}}));
    CHECK(x(2.5) == IntervalVector({{6,9}}));
    // No gates: testing values between slices
    CHECK(x(1.) == IntervalVector({{2,5}}));
    CHECK(x(2.) == IntervalVector({{6,8}}));
    CHECK(x(3.) == IntervalVector({{6,9}}));
    CHECK(x(999.) == IntervalVector(1));
    
    auto s0 = x.first_slice();
    CHECK(s0->t0_tf() == Interval(-oo,0));
    CHECK(s0->codomain() == IntervalVector({{-oo,oo}}));
    auto s1 = s0->next_slice();
    CHECK(s1->t0_tf() == Interval(0,1));
    CHECK(s1->codomain() == IntervalVector({{1,5}}));
    auto s2 = s1->next_slice();
    CHECK(s2->t0_tf() == Interval(1,2));
    CHECK(s2->codomain() == IntervalVector({{2,8}}));
    auto s3 = s2->next_slice();
    CHECK(s3->t0_tf() == Interval(2,3));
    CHECK(s3->codomain() == IntervalVector({{6,9}}));
    auto s4 = s3->next_slice();
    CHECK(s4->t0_tf() == Interval(3,oo));
    CHECK(s4->codomain() == IntervalVector({{-oo,oo}}));

    CHECK(tdomain->nb_tslices() == 5);
    tdomain->sample(1.3);
    CHECK(tdomain->nb_tslices() == 6);
    CHECK(s2->t0_tf() == Interval(1,1.3));
    CHECK(s2->codomain() == IntervalVector({{2,8}}));
    auto s2bis = s2->next_slice();
    CHECK(s2bis->t0_tf() == Interval(1.3,2));
    CHECK(s2bis->codomain() == IntervalVector({{2,8}}));
    CHECK(s3->t0_tf() == Interval(2,3));
    CHECK(s3->codomain() == IntervalVector({{6,9}}));
  }

  SECTION("Sampling inside tdomain")
  {
    auto tdomain = create_tdomain();
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    CHECK(tdomain->nb_tslices() == 1);
    tdomain->sample(1., false);
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    CHECK(tdomain->nb_tslices() == 2);
    tdomain->sample(1., false);
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    CHECK(tdomain->nb_tslices() == 2);
    tdomain->sample(1., true);
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    CHECK(tdomain->nb_tslices() == 3);
    auto it = tdomain->sample(10., true);
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    CHECK(tdomain->nb_tslices() == 5);
    CHECK(*it == Interval(10.));
    it = tdomain->sample(15., false);
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    CHECK(tdomain->nb_tslices() == 6);
    CHECK(*it == Interval(15.,oo));
  }

  SECTION("Sampling outside tdomain")
  {
    auto tdomain = create_tdomain(Interval(0,0.5));
    CHECK(tdomain->t0_tf() == Interval(0,0.5));
    CHECK(tdomain->nb_tslices() == 1);
    tdomain->sample(1., false);
    CHECK(tdomain->t0_tf() == Interval(0,1));
    CHECK(tdomain->nb_tslices() == 2);
    tdomain->sample(1., false);
    CHECK(tdomain->t0_tf() == Interval(0,1));
    CHECK(tdomain->nb_tslices() == 2);
    tdomain->sample(1., false);
    CHECK(tdomain->t0_tf() == Interval(0,1));
    CHECK(tdomain->nb_tslices() == 2);
    tdomain->sample(1., true);
    CHECK(tdomain->t0_tf() == Interval(0,1));
    CHECK(tdomain->nb_tslices() == 3);
    auto it = tdomain->sample(10., true);
    CHECK(tdomain->t0_tf() == Interval(0,10));
    CHECK(tdomain->nb_tslices() == 5);
    CHECK(*it == Interval(10.));
    it = tdomain->sample(15., false);
    CHECK(tdomain->t0_tf() == Interval(0,15));
    CHECK(tdomain->nb_tslices() == 6);
    CHECK(*it == Interval(10,15));
  }

  SECTION("Test basic SlicedTube")
  {
    auto tdomain = create_tdomain(Interval(0,1), 0.1, false);
    SlicedTube x(tdomain, IntervalVector(3));

    CHECK(x.size() == 3);
    CHECK(x.tdomain() == tdomain);
    CHECK(x.t0_tf() == Interval(0,1));
    CHECK(x.nb_slices() == tdomain->nb_tslices());
    CHECK(x.nb_slices() == 10);
    CHECK(x.first_slice()->t0_tf() == Interval(0,0.1));
    CHECK(Approx(x.last_slice()->t0_tf()) == Interval(0.9,1));
    CHECK(x.codomain() == IntervalVector(3));
    x.set(IntervalVector::constant(3, Interval(-10,10)));
    CHECK(x.codomain() == IntervalVector::constant(3, Interval(-10,10)));

    // Eval
    CHECK(tdomain->nb_tubes() == 1);
    CHECK(x(Interval(-oo,oo)) == IntervalVector(3));
    CHECK(x(Interval(-1,1)) == IntervalVector(3));
    CHECK(x(tdomain->t0_tf()) == x.codomain());
    CHECK(x(-42.) == IntervalVector(3));

    // Eval: affectation at scalar t
    CHECK(tdomain->nb_tslices() == 10);
    x.set(IntervalVector::constant(3,Interval(2.,3.)), -42.);
    CHECK(tdomain->nb_tslices() == 12);

    // Checking structure
    vector<const Slice<IntervalVector>*> v;
    for(const auto& s : x)
      v.push_back(&s);
    CHECK(v[0]->t0_tf() == Interval(-42.));
    CHECK(v[0]->codomain() == IntervalVector::constant(3,Interval(2.,3.)));
    CHECK(v[1]->t0_tf() == Interval(-42.,0.));
    CHECK(v[1]->codomain() == IntervalVector(3));
    CHECK(v[2]->t0_tf() == Interval(0.,0.1));
    CHECK(v[2]->codomain() == IntervalVector::constant(3,Interval(-10,10)));
    CHECK(v[3]->t0_tf() == Interval(0.1,0.2));
    CHECK(v[3]->codomain() == IntervalVector::constant(3,Interval(-10,10)));

    CHECK(x(-42.) == IntervalVector::constant(3,Interval(2.,3.)));
    CHECK(x(previous_float(-42.)) == IntervalVector(3));
    CHECK(x(next_float(-42.)) == IntervalVector(3));

    // Eval: affectation at interval t
    CHECK(x.codomain() == IntervalVector(3));
    CHECK(tdomain->nb_tslices() == 12);
    x.set(IntervalVector::constant(3,Interval(9.,10.)), Interval(44,55));
    CHECK(tdomain->nb_tslices() == 14);

    v.clear();
    for(const auto& s : x)
      v.push_back(&s);
    CHECK(Approx(v[11]->t0_tf()) == Interval(0.9,1));
    CHECK(v[11]->codomain() == IntervalVector::constant(3,Interval(-10,10)));
    CHECK(v[12]->t0_tf() == Interval(1,44));
    CHECK(v[12]->codomain() == IntervalVector(3));
    CHECK(v[13]->t0_tf() == Interval(44,55));
    CHECK(v[13]->codomain() == IntervalVector::constant(3,Interval(9.,10.)));

    CHECK(x(Interval(44,55)) == IntervalVector::constant(3,Interval(9.,10.)));
    CHECK(x(previous_float(44.)) == IntervalVector(3));
    CHECK(x(next_float(55.)) == IntervalVector(3));

    // Iterators tests
    {
      auto s_ = x.first_slice();
      for(auto& s : x)
      {
        CHECK(&s == &(*s_));
        s_ = s_->next_slice();
      }
    }

    // Iterators tests (const)
    {
      auto y(x); // copy constructor
      auto s_ = x.first_slice();
      for(const auto& s : x)
      {
        CHECK(&s == &(*s_));
        s_ = s_->next_slice();
      }
    }
  }

  SECTION("Test Slice<T>")
  {
    auto tdomain = create_tdomain(Interval(0,1), 0.1);
    SlicedTube x(tdomain, IntervalVector(2));
    CHECK(x.nb_slices() == 10);
    CHECK(tdomain->tslice(-oo) == tdomain->end());
    CHECK(tdomain->tslice(oo) == tdomain->end());
    CHECK(x.first_slice() == tdomain->tslice(0.)->slices().at(&x));
    CHECK(x.last_slice() == tdomain->tslice(1.)->slices().at(&x));

    for(auto& s : x)
      s.set(IntervalVector::constant(2,s.t0_tf()));

    vector<const Slice<IntervalVector>*> v;
    for(const auto& s : x)
      v.push_back(&s);

    CHECK(v[0]->t0_tf() == Interval(0,0.1));
    CHECK(v[0]->input_gate() == IntervalVector::constant(2,Interval(0.,0.1))); // nothing before
    CHECK(v[0]->codomain() == IntervalVector::constant(2,Interval(0,0.1)));
    CHECK(v[0]->output_gate() == IntervalVector::constant(2,Interval(0.1)));

    CHECK(Approx(v[9]->t0_tf()) == Interval(0.9,1.));
    CHECK(v[9]->input_gate() == v[8]->output_gate());
    CHECK(Approx(v[9]->codomain()) == IntervalVector::constant(2,Interval(0.9,1.)));
    CHECK(Approx(v[9]->input_gate()) == IntervalVector::constant(2,Interval(0.9)));
    CHECK(Approx(v[9]->output_gate()) == IntervalVector::constant(2,Interval(0.9,1.))); // nothing after
  }

  SECTION("Test again 1")
  {
    auto tdomain = create_tdomain(Interval(1,10), 0.01, true); // last argument creates "gates" (degenerated slices at scalar timesteps)
    ScalarVar t;
    SlicedTube x(tdomain, AnalyticFunction(
      {t},
      {
        sin(sqrt(t)+((t-5)^2)*Interval(-0.01,0.01)),
        cos(t)+sin(t/0.2)*Interval(-0.1,0.1)
      }));
    SlicedTube u(tdomain, IntervalVector(2));
    CHECK(x.size() == 2);
    CHECK(Approx(x.codomain(),1e-4) == IntervalVector({{-0.267392,1},{-1.06721,1.06721}}));
  }

  SECTION("Local TDomain, tube still valid after")
  {
    SlicedTube<Interval> *x;

    {
      auto tdomain = create_tdomain(Interval(0,1), 0.1);
      x = new SlicedTube<Interval>(tdomain, Interval());
      CHECK(x->tdomain() == tdomain);
    }

    CHECK(x->tdomain()->t0_tf() == Interval(0,1)); // should not segfault
    delete x;
  }

  SECTION("Function returning a tube")
  {
    SlicedTube x = return_a_tube();
    CHECK(x.tdomain()->t0_tf() == Interval(0,2));
    CHECK(x.size() == 3);
    CHECK(x.codomain()[1] == Interval(-1.5,1));
  }

  SECTION("Reverse iterator")
  {
    auto tdomain = create_tdomain(Interval(0,1), 0.5);
    SlicedTube x(tdomain, Interval());

    auto it1 = x.begin();
    CHECK(it1->t0_tf() == Interval(0,0.5)); ++it1;
    CHECK(it1->t0_tf() == Interval(0.5,1)); ++it1;
    CHECK(it1 == x.end());

    auto it2 = x.rbegin();
    CHECK(it2->t0_tf() == Interval(0.5,1)); ++it2;
    CHECK(it2->t0_tf() == Interval(0,0.5)); ++it2;
    CHECK(it2 == x.rend());
  }

  SECTION("Testing setting values")
  {
    auto tdomain = create_tdomain(Interval(0,10),1.,true); // with gates
    SlicedTube x(tdomain, Interval(-10,10));
    CHECK(x.codomain() == Interval(-10,10));

    auto it = tdomain->begin();
    CHECK(*it == Interval(0));
    CHECK(x(it)->codomain() == Interval(-10,10));
    it++;
    CHECK(*it == Interval(0,1));
    CHECK(x(it)->codomain() == Interval(-10,10));
    it++;
    CHECK(*it == Interval(1,1));
    CHECK(x(it)->codomain() == Interval(-10,10));
    it = tdomain->begin();
    it++;
    x(it)->set(Interval(2,8));
    CHECK(x(it)->codomain() == Interval(2,8));
    CHECK(x(std::prev(it))->codomain() == Interval(2,8));
    CHECK(x(std::next(it))->codomain() == Interval(2,8));
    CHECK(x(std::next(std::next(it)))->codomain() == Interval(-10,10));
    CHECK(x.codomain() == Interval(-10,10));
    it++; it++; it++; it++; it++;
    CHECK(*it == Interval(3));
    x(it)->set(Interval(3,5));
    CHECK(x(it)->codomain() == Interval(3,5));
    CHECK(x(std::prev(it))->codomain() == Interval(-10,10));
    CHECK(x(std::next(it))->codomain() == Interval(-10,10));
    CHECK(x(std::next(std::next(it)))->codomain() == Interval(-10,10));
    CHECK(x.codomain() == Interval(-10,10));
  }

  SECTION("Testing validity of copy of tubes")
  {
    auto tdomain = create_tdomain(Interval(0,5), 0.01, true);

    SlicedTube y(tdomain, Interval(2.)), x1(tdomain, Interval(-1,1)), x2(tdomain, Interval(1));
    SlicedTube cx1(x1), cx2(x2); // copy

    for(auto it = cx1.tdomain()->begin();
      it != cx1.tdomain()->end(); ++it)
    {
      Interval ix1 = cx1(it)->codomain(), ix2 = cx2(it)->codomain();
      AddOp::bwd(y(it)->codomain(), ix1, ix2);
      cx1(it)->set(ix1);
      cx2(it)->set(ix2);
    }

    CHECK(cx1.codomain() == Interval(1));
    CHECK(cx2.codomain() == Interval(1));
    CHECK(y.codomain() == Interval(2));
  }

  SECTION("Tube not empty if built from a AnalyticFunction")
  {
    auto tdomain = create_tdomain(Interval(0,5), 0.01, true);
    ScalarVar t;
    AnalyticFunction f({t}, 5*sin(2*t)+t);
    SlicedTube aa1(tdomain, f);
    CHECK(!aa1.is_empty());
  }

  SECTION("Testing tube evaluation")
  {
    auto tdomain = create_tdomain(Interval(0,5), 0.1, true);
    ScalarVar t;
    AnalyticFunction f({t}, 10*cos(t)+t);
    SlicedTube a(tdomain, f);
    CHECK(Approx((Interval)*tdomain->tslice(2.)) == Interval(1.900000000000001, 2.000000000000002));
    CHECK(Approx(a(Interval(1,2)),1e-4) == Interval(-2.17496, 7.13757));
  }

  SECTION("Testing specific detected bug from sampling")
  {
    auto tdomain = create_tdomain({0.,46.}, 0.5);
    SlicedTube x(tdomain, Interval());
    list<TSlice>::iterator it = tdomain->sample(46, false);
    CHECK(*it == Interval(45.5,46));
    x.set({-1,3}, {30,31});
    CHECK(x({30,31}) == Interval(-1,3));
    x.set({-1,3}, {45,46});
    CHECK(x({45,46}) == Interval(-1,3));
    CHECK(x({45.5,46}) == Interval(-1,3));
    CHECK(x(45.8) == Interval(-1,3));
    CHECK(x(45.2) == Interval(-1,3));
  }
}