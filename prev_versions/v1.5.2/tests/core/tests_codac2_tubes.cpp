#include "catch_interval.hpp"
#include "vibes.h"

// Using #define so that we can access protected methods
// of the class for tests purposes
#define protected public
#include "codac2_TDomain.h"

//#include "codac_predef_values.h"
#include "codac2_Tube.h"
#include "codac2_CtcDiffInclusion.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace codac2;

Tube<IntervalVector> return_a_tube()
{
  return Tube<IntervalVector>(
    create_tdomain(Interval(0,2),0.5),
    IntervalVector(3,Interval(-1.5,1)));
}

TEST_CASE("Test tubes")
{
  SECTION("Test TDomain")
  {
    auto tdomain = create_tdomain();
    tdomain->sample(Interval(0,1), 0.5);
    CHECK(tdomain->nb_tslices() == 4);
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    
    const list<TSlice>& tslices = tdomain->tslices();
    vector<TSlice> vector_tslices{
      make_move_iterator(tslices.begin()),
      make_move_iterator(tslices.end()) };

    CHECK(vector_tslices.size() == 4);
    CHECK(vector_tslices[0].t0_tf() == Interval(-oo,0));
    CHECK(vector_tslices[1].t0_tf() == Interval(0,0.5));
    CHECK(vector_tslices[2].t0_tf() == Interval(0.5,1));
    CHECK(vector_tslices[3].t0_tf() == Interval(1,oo));

    CHECK(tdomain->iterator_tslice(-10.)->t0_tf() == Interval(-oo,0));
    CHECK(tdomain->iterator_tslice(-120.)->t0_tf() == Interval(-oo,0));
    CHECK(tdomain->iterator_tslice(0.2)->t0_tf() == Interval(0,0.5));
    CHECK(tdomain->iterator_tslice(5540.2)->t0_tf() == Interval(1,oo));

    CHECK(tdomain->nb_tubes() == 0);
    Tube<IntervalVector> x(tdomain, IntervalVector(2)); // adding a tubevector to the tdomain
    CHECK(tdomain->nb_tubes() == 1);

    { // new scope
      Tube<IntervalVector> v(tdomain, IntervalVector(3));
      CHECK(tdomain->nb_tubes() == 2);
    } // end of scope, auto removing the tube
    
    CHECK(tdomain->nb_tubes() == 1);
  }

  // should assert SECTION("Test degenerated TDomain")
  // should assert {
  // should assert   auto tdomain = create_tdomain(Interval(1), 0.5);
  // should assert   CHECK(tdomain->nb_tslices() == 2);
  // should assert   CHECK(tdomain->t0_tf() == Interval(1));
  // should assert   CHECK(tdomain->nb_tubes() == 0);
  // should assert   
  // should assert   const list<TSlice>& tslices = tdomain->tslices();
  // should assert   vector<TSlice> vector_tslices{
  // should assert     make_move_iterator(tslices.begin()),
  // should assert     make_move_iterator(tslices.end()) };
  // should assert     
  // should assert   CHECK(vector_tslices.size() == 2);
  // should assert   CHECK(vector_tslices[0].t0_tf() == Interval(-oo,1));
  // should assert   CHECK(vector_tslices[1].t0_tf() == Interval(1,oo));
  // should assert }

  SECTION("Test TDomain with gates")
  {
    auto tdomain = create_tdomain(Interval(0,1), 0.5, true);
    CHECK(tdomain->nb_tslices() == 5);
    CHECK(tdomain->t0_tf() == Interval(0,1));
    CHECK(tdomain->nb_tubes() == 0);
    
    const list<TSlice>& tslices = tdomain->tslices();
    vector<TSlice> vector_tslices{
      make_move_iterator(tslices.begin()),
      make_move_iterator(tslices.end()) };

    CHECK(vector_tslices.size() == 5);
    CHECK(vector_tslices[0].t0_tf() == Interval(0));
    CHECK(vector_tslices[1].t0_tf() == Interval(0,0.5));
    CHECK(vector_tslices[2].t0_tf() == Interval(0.5));
    CHECK(vector_tslices[3].t0_tf() == Interval(0.5,1));
    CHECK(vector_tslices[4].t0_tf() == Interval(1,1));

    CHECK(tdomain->iterator_tslice(0.)->t0_tf() == Interval(0));
    CHECK(tdomain->iterator_tslice(0.1)->t0_tf() == Interval(0,0.5));
    CHECK(tdomain->iterator_tslice(0.5)->t0_tf() == Interval(0.5));
    CHECK(tdomain->iterator_tslice(0.6)->t0_tf() == Interval(0.5,1));
    CHECK(tdomain->iterator_tslice(1.)->t0_tf() == Interval(1));
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
    
    const list<TSlice>& tslices = tdomain->tslices();
    vector<TSlice> vector_tslices{
      make_move_iterator(tslices.begin()),
      make_move_iterator(tslices.end()) };

    CHECK(vector_tslices.size() == 4);
    CHECK(vector_tslices[0].t0_tf() == Interval(-oo,1));
    CHECK(vector_tslices[1].t0_tf() == Interval(1,10));
    CHECK(vector_tslices[2].t0_tf() == Interval(10));
    CHECK(vector_tslices[3].t0_tf() == Interval(10,oo));
  }

  SECTION("Test unbounded TDomain")
  {
    auto tdomain = create_tdomain();
    CHECK(tdomain->nb_tslices() == 1);
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    const list<TSlice>& tslices = tdomain->tslices();
    vector<TSlice> vector_tslices{
      make_move_iterator(tslices.begin()),
      make_move_iterator(tslices.end()) };
    CHECK(vector_tslices.size() == 1);
    CHECK(vector_tslices[0].t0_tf() == Interval(-oo,oo));
  }

  SECTION("Test TDomain with sampling and values")
  {
    auto tdomain = create_tdomain();
    CHECK(tdomain->nb_tslices() == 1);
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    Tube<IntervalVector> x(tdomain, IntervalVector(1));
    //x.set(Interval(4));
    x(Interval(0,1)) = IntervalVector({{1,5}});
    x(Interval(1,2)) = IntervalVector({{2,8}});
    x(Interval(2,3)) = IntervalVector({{6,9}});
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

    CHECK(tdomain->iterator_tslice(-1.)->t0_tf() == Interval(-oo,0));
    CHECK(tdomain->iterator_tslice(0.)->t0_tf() == Interval(0,1));
    CHECK(tdomain->iterator_tslice(0.01)->t0_tf() == Interval(0,1));
    CHECK(tdomain->iterator_tslice(1)->t0_tf() == Interval(1,2));
    CHECK(tdomain->iterator_tslice(2)->t0_tf() == Interval(2,3));
    CHECK(tdomain->iterator_tslice(ibex::previous_float(3.))->t0_tf() == Interval(2,3));
    CHECK(tdomain->iterator_tslice(3)->t0_tf() == Interval(3,oo));
    CHECK(tdomain->iterator_tslice(ibex::next_float(3.))->t0_tf() == Interval(3,oo));

    CHECK(tdomain->nb_tslices() == 5); // with [-oo,0] and [3,oo]
    CHECK(static_cast<IntervalVector>(x(Interval(0,3))) == IntervalVector({{1,9}}));
    CHECK(static_cast<IntervalVector>(x(-1)) == IntervalVector(1));
    CHECK(static_cast<IntervalVector>(x(0.5)) == IntervalVector({{1,5}}));
    CHECK(static_cast<IntervalVector>(x(1.5)) == IntervalVector({{2,8}}));
    CHECK(static_cast<IntervalVector>(x(2.5)) == IntervalVector({{6,9}}));
    // No gates: testing values between slices
    CHECK(static_cast<IntervalVector>(x(1.)) == IntervalVector({{2,5}}));
    CHECK(static_cast<IntervalVector>(x(2.)) == IntervalVector({{6,8}}));
    CHECK(static_cast<IntervalVector>(x(3.)) == IntervalVector({{6,9}}));
    CHECK(static_cast<IntervalVector>(x(999.)) == IntervalVector(1));
    
    const std::shared_ptr<Slice<IntervalVector>> s0 = x.first_slice_ptr();
    CHECK(s0->t0_tf() == Interval(-oo,0));
    CHECK(s0->codomain() == IntervalVector({{-oo,oo}}));
    const std::shared_ptr<Slice<IntervalVector>> s1 = s0->next_slice_ptr();
    CHECK(s1->t0_tf() == Interval(0,1));
    CHECK(s1->codomain() == IntervalVector({{1,5}}));
    const std::shared_ptr<Slice<IntervalVector>> s2 = s1->next_slice_ptr();
    CHECK(s2->t0_tf() == Interval(1,2));
    CHECK(s2->codomain() == IntervalVector({{2,8}}));
    const std::shared_ptr<Slice<IntervalVector>> s3 = s2->next_slice_ptr();
    CHECK(s3->t0_tf() == Interval(2,3));
    CHECK(s3->codomain() == IntervalVector({{6,9}}));
    const std::shared_ptr<Slice<IntervalVector>> s4 = s3->next_slice_ptr();
    CHECK(s4->t0_tf() == Interval(3,oo));
    CHECK(s4->codomain() == IntervalVector({{-oo,oo}}));

    CHECK(tdomain->nb_tslices() == 5);
    tdomain->sample(1.3);
    CHECK(tdomain->nb_tslices() == 6);
    CHECK(s2->t0_tf() == Interval(1,1.3));
    CHECK(s2->codomain() == IntervalVector({{2,8}}));
    const std::shared_ptr<Slice<IntervalVector>> s2bis = s2->next_slice_ptr();
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
    std::list<TSlice>::iterator it = tdomain->sample(10., true);
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    CHECK(tdomain->nb_tslices() == 5);
    CHECK(it->t0_tf() == Interval(10.));
    it = tdomain->sample(15., false);
    CHECK(tdomain->t0_tf() == Interval(-oo,oo));
    CHECK(tdomain->nb_tslices() == 6);
    CHECK(it->t0_tf() == Interval(15.,oo));
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
    CHECK(tdomain->nb_tslices() == 2); //
    tdomain->sample(1., false);
    CHECK(tdomain->t0_tf() == Interval(0,1));
    CHECK(tdomain->nb_tslices() == 2);
    tdomain->sample(1., true);
    CHECK(tdomain->t0_tf() == Interval(0,1));
    CHECK(tdomain->nb_tslices() == 3);
    std::list<TSlice>::iterator it = tdomain->sample(10., true);
    CHECK(tdomain->t0_tf() == Interval(0,10));
    CHECK(tdomain->nb_tslices() == 5);
    CHECK(it->t0_tf() == Interval(10.));
    it = tdomain->sample(15., false);
    CHECK(tdomain->t0_tf() == Interval(0,15));
    CHECK(tdomain->nb_tslices() == 6);
    CHECK(it->t0_tf() == Interval(10,15));
  }

  SECTION("Test basic Tube<IntervalVector>")
  {
    auto tdomain = create_tdomain(Interval(0,1), 0.1, false);
    Tube<IntervalVector> x(tdomain, IntervalVector(3));

    CHECK(x.size() == 3);
    CHECK(x.tdomain() == tdomain);
    CHECK(x.t0_tf() == Interval(0,1));
    CHECK(x.nb_slices() == tdomain->nb_tslices());
    CHECK(x.nb_slices() == 10);
    CHECK(x.first_slice_ptr()->t0_tf() == Interval(0,0.1));
    CHECK(ApproxIntv(x.last_slice_ptr()->t0_tf()) == Interval(0.9,1));
    CHECK(x.codomain() == IntervalVector(3));
    x.set(IntervalVector(3, Interval(-10,10)));
    CHECK(x.codomain() == IntervalVector(3, Interval(-10,10)));

    // Tube<IntervalVector>Component
    // removed: CHECK(x[0].codomain() == Interval(-10,10));
    // removed: CHECK(x[0].t0_tf() == Interval(0,1));
    // removed: CHECK(x[0].tdomain() == tdomain);
    // removed: x[0].set(Interval(-20,20));
    // removed: CHECK(x[0].codomain() == Interval(-20,20));
    // removed: CHECK(x.codomain() == IntervalVector({Interval(-20,20),Interval(-10,10),Interval(-10,10)}));
    // removed: x[1] = x[0];
    // removed: CHECK(x[1].codomain() == Interval(-20,20));
    // removed: CHECK(x.codomain() == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));

    // Eval
    CHECK(tdomain->nb_tubes() == 1);
    CHECK(x(Interval(-oo,oo)) == IntervalVector(3));
    CHECK(x(Interval(-1,1)) == IntervalVector(3));
    CHECK(x(tdomain->t0_tf()) == x.codomain());
    CHECK(x(-42.) == IntervalVector(3));

    // Eval: affectation at scalar t
    CHECK(tdomain->nb_tslices() == 10);
    x(-42.) = IntervalVector(3,Interval(2.,3.));
    CHECK(tdomain->nb_tslices() == 12);

    // Checking structure
    vector<const Slice<IntervalVector>*> v;
    for(const auto& s : x)
      v.push_back(&s);
    CHECK(v[0]->t0_tf() == Interval(-42.));
    CHECK(v[0]->codomain() == IntervalVector(3,Interval(2.,3.)));
    CHECK(v[1]->t0_tf() == Interval(-42.,0.));
    CHECK(v[1]->codomain() == IntervalVector(3));
    CHECK(v[2]->t0_tf() == Interval(0.,0.1));
    CHECK(v[2]->codomain() == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));
    CHECK(v[3]->t0_tf() == Interval(0.1,0.2));
    CHECK(v[3]->codomain() == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));

    CHECK(static_cast<IntervalVector>(x(-42.)) == IntervalVector(3,Interval(2.,3.)));
    CHECK(static_cast<IntervalVector>(x(ibex::previous_float(-42.))) == IntervalVector(3));
    CHECK(static_cast<IntervalVector>(x(ibex::next_float(-42.))) == IntervalVector(3));

    // Eval: affectation at interval t
    CHECK(x.codomain() == IntervalVector(3));
    CHECK(tdomain->nb_tslices() == 12);
    x(Interval(44,55)) = IntervalVector(3,Interval(9.,10.));
    CHECK(tdomain->nb_tslices() == 14);

    v.clear();
    for(const auto& s : x)
      v.push_back(&s);
    CHECK(ApproxIntv(v[11]->t0_tf()) == Interval(0.9,1));
    CHECK(v[11]->codomain() == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));
    CHECK(v[12]->t0_tf() == Interval(1,44));
    CHECK(v[12]->codomain() == IntervalVector(3));
    CHECK(v[13]->t0_tf() == Interval(44,55));
    CHECK(v[13]->codomain() == IntervalVector(3,Interval(9.,10.)));

    CHECK(static_cast<IntervalVector>(x(Interval(44,55))) == IntervalVector(3,Interval(9.,10.)));
    CHECK(static_cast<IntervalVector>(x(ibex::previous_float(44.))) == IntervalVector(3));
    CHECK(static_cast<IntervalVector>(x(ibex::next_float(55.))) == IntervalVector(3));

    // Iterators tests
    {
      shared_ptr<Slice<IntervalVector>> s_ = x.first_slice_ptr();
      for(auto& s : x)
      {
        CHECK(&s == &(*s_));
        s_ = s_->next_slice_ptr();
      }
    }

    // Iterators tests (const)
    {
      const Tube<IntervalVector> y(x); // copy constructor
      shared_ptr<Slice<IntervalVector>> s_ = x.first_slice_ptr();
      for(const auto& s : x)
      {
        CHECK(&s == &(*s_));
        s_ = s_->next_slice_ptr();
      }
    }
  }

  SECTION("Test SliceVector")
  {
    auto tdomain = create_tdomain(Interval(0,1), 0.1);
    Tube<IntervalVector> x(tdomain, IntervalVector(2));
    CHECK(x.nb_slices() == 10);
    CHECK(tdomain->iterator_tslice(-oo) == tdomain->_tslices.end());
    CHECK(tdomain->iterator_tslice(oo) == tdomain->_tslices.end());
    CHECK(x.first_slice_ptr() == tdomain->iterator_tslice(0.)->_slices.at(&x));
    CHECK(x.last_slice_ptr() == tdomain->iterator_tslice(1.)->_slices.at(&x));

    for(auto& s : x)
      s.set(IntervalVector(2,s.t0_tf()));

    vector<const Slice<IntervalVector>*> v;
    for(const auto& s : x)
      v.push_back(&s);

    CHECK(v[0]->t0_tf() == Interval(0,0.1));
    CHECK(v[0]->input_gate() == IntervalVector(2,Interval(0.,0.1))); // nothing before
    CHECK(v[0]->codomain() == IntervalVector(2,Interval(0,0.1)));
    CHECK(v[0]->output_gate() == IntervalVector(2,Interval(0.1)));

    CHECK(ApproxIntv(v[9]->t0_tf()) == Interval(0.9,1.));
    CHECK(v[9]->input_gate() == v[8]->output_gate());
    CHECK(ApproxIntvVector(v[9]->codomain()) == IntervalVector(2,Interval(0.9,1.)));
    CHECK(ApproxIntvVector(v[9]->input_gate()) == IntervalVector(2,Interval(0.9)));
    CHECK(ApproxIntvVector(v[9]->output_gate()) == IntervalVector(2,Interval(0.9,1.))); // nothing after
  }

  SECTION("Test again 1")
  {
    auto tdomain = create_tdomain(Interval(1,10), 0.01, true); // last argument creates "gates" (degenerated slices at scalar timesteps)
    Tube<IntervalVector> x(tdomain,
      codac::TFunction("(sin(sqrt(t)+((t-5)^2)*[-0.01,0.01]) ; cos(t)+sin(t/0.2)*[-0.1,0.1])"));
    Tube<IntervalVector> u(tdomain, IntervalVector(2));

    CHECK(x.size() == 2);
    CHECK(u.size() == 2);

    //codac::TFunction tf("x[2]", "u[2]", "(sin(x[1]) ; -sin(x[0]))");
    //CtcDiffInclusion ctc_diffincl(tf);
    //ctc_diffincl.contract(x,u);

    //vibes::beginDrawing();

    //codac::TubeVector x_codac1 = to_codac1(x); // may take time
    //codac::Tube xi_codac1 = to_codac1(x)[1]; // may take time

    //codac::VIBesFigTube fig("Tube");
    //fig.set_properties(100, 100, 600, 300);
    //fig.add_tube(&xi_codac1, "x");
    //fig.show(true);

    //vibes::endDrawing();
  }

  SECTION("Test again 2")
  {
    /*{
      auto tdomain = create_tdomain(Interval(0,10), 1., false);
      Tube<IntervalVector> x(tdomain, IntervalVector(2));

      for(auto& sx : x)
      {
        if(sx.t0_tf().contains(5.2))
        {
          cout << "sample" << endl;
          tdomain->sample(5.2);
        }
        cout << sx << endl;
      }
    }*/

    {
      codac::TFunction f("x1", "x2", "u1", "u2", "(t+u1+x1;t+u2+x2)");
      IntervalVector x({{2,3},{5,6}});
      IntervalVector u({{0,0.1},{0,0.1}});
      Interval t(5.);
      //cout << f.eval_vector(t,x,u) << endl;
      CHECK(f.eval_vector(t,to_codac1(x),to_codac1(u)) == ApproxIntvVector(IntervalVector({{7, 8.100000000000002},{10, 11.10000000000001}})));
    }
  }

  SECTION("Local TDomain, tube still valid after")
  {
    Tube<Interval> *x;

    {
      auto tdomain = create_tdomain(Interval(0,1), 0.1);
      x = new Tube<Interval>(tdomain, Interval());
      CHECK(x->tdomain() == tdomain);
    }

    CHECK(x->tdomain()->t0_tf() == Interval(0,1)); // should not segfault
    delete x;
  }

  SECTION("Function returning a tube")
  {
    Tube<IntervalVector> x = return_a_tube();
    CHECK(x.tdomain()->t0_tf() == Interval(0,2));
    CHECK(x.size() == 3);
    CHECK(x.codomain()[1] == Interval(-1.5,1));
  }

  SECTION("Reverse iterator")
  {
    auto tdomain = create_tdomain(Interval(0,1), 0.5);
    Tube<Interval> x(tdomain, Interval());

    auto it1 = x.begin();
    CHECK(it1->t0_tf() == Interval(0,0.5)); ++it1;
    CHECK(it1->t0_tf() == Interval(0.5,1)); ++it1;
    CHECK(it1 == x.end());

    auto it2 = x.rbegin();
    CHECK(it2->t0_tf() == Interval(0.5,1)); ++it2;
    CHECK(it2->t0_tf() == Interval(0,0.5)); ++it2;
    CHECK(it2 == x.rend());
  }

  SECTION("Conversion of tubes: codac1-codac2")
  {
    codac::Tube codac1(Interval(0,10),1.,Interval(-3,6));
    auto tdomain = create_tdomain(Interval(0,10),1.,true); // with gates
    Tube<Interval> codac2(tdomain, Interval(-3,6));
    CHECK(to_codac1(codac2) == codac1);
    CHECK(to_codac2(codac1) == codac2);
    CHECK(to_codac1(to_codac2(codac1)) == codac1);
    CHECK(to_codac2(to_codac1(codac2)) == codac2);

    codac::TubeVector codac1_vector(Interval(0,10),1.,codac::IntervalVector({{-1,2},{6,8}}));
    Tube<IntervalVector> codac2_vector(tdomain, IntervalVector({{-1,2},{6,8}}));
    codac1_vector.set({{1.2,1.3},{6.8,6.9}}, 0.58);
    codac2_vector.set({{1.2,1.3},{6.8,6.9}}, 0.58);
    codac1_vector.set({{1.6},{7.2}}, 10.);
    codac2_vector.set({{1.6},{7.2}}, 10.);
    CHECK(tdomain->t0_tf().ub() == 10.);
    CHECK(codac1_vector(0.58) == codac::IntervalVector({{1.2,1.3},{6.8,6.9}}));
    CHECK(IntervalVector(codac2_vector(0.58)) == IntervalVector({{1.2,1.3},{6.8,6.9}}));

    CHECK(to_codac1(codac2_vector) == codac1_vector);
    CHECK(to_codac2(codac1_vector) == codac2_vector);
    CHECK(to_codac1(to_codac2(codac1_vector)) == codac1_vector);
    CHECK(to_codac2(to_codac1(codac2_vector)) == codac2_vector);

    codac::TubeVector to_codac1_codac2_vector = to_codac1(codac2_vector);
    Tube<IntervalVector> to_codac2_codac1_vector = to_codac2(codac1_vector);
    // t=0.58
    CHECK(to_codac1_codac2_vector(ibex::previous_float(0.58)) == codac::IntervalVector({{-1,2},{6,8}}));
    CHECK(to_codac1_codac2_vector(0.58) == codac::IntervalVector({{1.2,1.3},{6.8,6.9}}));
    CHECK(to_codac1_codac2_vector(ibex::next_float(0.58)) == codac::IntervalVector({{-1,2},{6,8}}));
    CHECK(IntervalVector(to_codac2_codac1_vector(ibex::previous_float(0.58))) == IntervalVector({{-1,2},{6,8}}));
    CHECK(IntervalVector(to_codac2_codac1_vector(0.58)) == IntervalVector({{1.2,1.3},{6.8,6.9}}));
    CHECK(IntervalVector(to_codac2_codac1_vector(ibex::next_float(0.58))) == IntervalVector({{-1,2},{6,8}}));
    // t=10
    CHECK(to_codac1_codac2_vector(ibex::previous_float(10.)) == codac::IntervalVector({{-1,2},{6,8}}));
    CHECK(to_codac1_codac2_vector(10.) == codac::IntervalVector({{1.6},{7.2}}));
    CHECK(to_codac1_codac2_vector(ibex::next_float(10.)) == codac::IntervalVector(2));
    CHECK(IntervalVector(to_codac2_codac1_vector(ibex::previous_float(10.))) == IntervalVector({{-1,2},{6,8}}));
    CHECK(IntervalVector(to_codac2_codac1_vector(10.)) == IntervalVector({{1.6},{7.2}}));
    CHECK(IntervalVector(to_codac2_codac1_vector(ibex::next_float(10.))) == IntervalVector(2));
  }
  
  SECTION("Testing setting values")
  {
    auto tdomain = create_tdomain(Interval(0,10),1.,true); // with gates
    Tube<Interval> x(tdomain, Interval(-10,10));
    CHECK(x.codomain() == Interval(-10,10));

    std::list<TSlice>::iterator it = tdomain->tslices().begin();
    CHECK(it->t0_tf() == Interval(0));
    CHECK(x(it).codomain() == Interval(-10,10));
    it++;
    CHECK(it->t0_tf() == Interval(0,1));
    CHECK(x(it).codomain() == Interval(-10,10));
    it++;
    CHECK(it->t0_tf() == Interval(1,1));
    CHECK(x(it).codomain() == Interval(-10,10));
    it = tdomain->tslices().begin();
    it++;
    x(it).set(Interval(2,8));
    CHECK(x(it).codomain() == Interval(2,8));
    CHECK(x(std::prev(it)).codomain() == Interval(2,8));
    CHECK(x(std::next(it)).codomain() == Interval(2,8));
    CHECK(x(std::next(std::next(it))).codomain() == Interval(-10,10));
    CHECK(x.codomain() == Interval(-10,10));
    it++; it++; it++; it++; it++;
    CHECK(it->t0_tf() == Interval(3));
    x(it).set(Interval(3,5));
    CHECK(x(it).codomain() == Interval(3,5));
    CHECK(x(std::prev(it)).codomain() == Interval(-10,10));
    CHECK(x(std::next(it)).codomain() == Interval(-10,10));
    CHECK(x(std::next(std::next(it))).codomain() == Interval(-10,10));
    CHECK(x.codomain() == Interval(-10,10));
  }

  SECTION("Testing validity of copy of tubes")
  {
    auto tdomain = create_tdomain(Interval(0,5), 0.01, true);

    Tube<Interval> y(tdomain, Interval(2.)), x1(tdomain, Interval(-1,1)), x2(tdomain, Interval(1));
    Tube<Interval> cx1(x1), cx2(x2); // copy

    for(std::list<TSlice>::iterator it = cx1.tdomain()->tslices().begin();
      it != cx1.tdomain()->tslices().end(); ++it)
    {
      Interval ix1 = cx1(it).codomain(), ix2 = cx2(it).codomain();
      ibex::bwd_add(y(it).codomain(), ix1, ix2);
      cx1(it).set(ix1);
      cx2(it).set(ix2);
    }

    CHECK(cx1.codomain() == Interval(1));
    CHECK(cx2.codomain() == Interval(1));
    CHECK(y.codomain() == Interval(2));
  }

  SECTION("Tube not empty if built from a Function")
  {
    auto tdomain = create_tdomain(Interval(0,5), 0.01, true);
    Tube<Interval> aa1(tdomain, TFunction("5*sin(2*t)+t"));
    CHECK(!aa1.is_empty());
  }

  SECTION("Testing tube evaluation")
  {
    auto tdomain = create_tdomain(Interval(0,5), 0.1, true);
    Tube<Interval> a(tdomain, TFunction("10*cos(t)+t"));
    codac::Tube a_codac1 = to_codac1(a);
    //vibes::beginDrawing();
    //codac::VIBesFigTube fig("Tube");
    //fig.set_properties(100, 100, 600, 300);
    //fig.add_tube(&a_codac1, "a_codac1", "blue[#7EC8FF88]");
    //fig.draw_box({{1,2},a.eval(Interval(1,2))});
    //fig.draw_box({{1,2},a_codac1(Interval(1,2))}, "red");
    //fig.show();
    //vibes::endDrawing();

    CHECK(ApproxIntv(tdomain->iterator_tslice(2.)->t0_tf()) == Interval(1.900000000000001, 2.000000000000002));
    CHECK(ApproxIntv(a.eval(Interval(1,2))) == Interval(-2.26146836547144, 7.216099682706644));
  }
}