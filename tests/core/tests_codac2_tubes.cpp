#include "catch_interval.hpp"
#include "vibes.h"

// Using #define so that we can access protected methods
// of the class for tests purposes
#define protected public
#include "codac2_TDomain.h"

#include "codac_predef_values.h"
#include "codac2_Tube.h"
#include "codac2_CtcDiffInclusion.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac2;
using codac::oo;

Tube<IntervalVector> return_a_tube()
{
  return Tube<IntervalVector>(
    create_tdomain(Interval(0,2),0.5),
    IntervalVector(3,Interval(-1.5,1)));
}

TEST_CASE("Test codac2::tubes")
{
  SECTION("Test TDomain")
  {
    auto tdomain = create_tdomain(Interval(0,1), 0.5);
    CHECK(tdomain->nb_tslices() == 4);
    CHECK(tdomain->t0_tf() == Interval(0,1));
    
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
    Tube<IntervalVector> x(tdomain, 2); // adding a tubevector to the tdomain
    CHECK(tdomain->nb_tubes() == 1);

    { // new scope
      Tube<IntervalVector> v(tdomain, 3);
      CHECK(tdomain->nb_tubes() == 2);
    } // end of scope, auto removing the tube
    
    CHECK(tdomain->nb_tubes() == 1);
  }

  SECTION("Test degenerated TDomain")
  {
    auto tdomain = create_tdomain(Interval(1), 0.5);
    CHECK(tdomain->nb_tslices() == 2);
    CHECK(tdomain->t0_tf() == Interval(1));
    CHECK(tdomain->nb_tubes() == 0);
    
    const list<TSlice>& tslices = tdomain->tslices();
    vector<TSlice> vector_tslices{
      make_move_iterator(tslices.begin()),
      make_move_iterator(tslices.end()) };

    CHECK(vector_tslices.size() == 2);
    CHECK(vector_tslices[0].t0_tf() == Interval(-oo,1));
    CHECK(vector_tslices[1].t0_tf() == Interval(1,oo));
  }

  SECTION("Test TDomain with gates")
  {
    auto tdomain = create_tdomain(Interval(0,1), 0.5, true);
    CHECK(tdomain->nb_tslices() == 7);
    CHECK(tdomain->t0_tf() == Interval(0,1));
    CHECK(tdomain->nb_tubes() == 0);
    
    const list<TSlice>& tslices = tdomain->tslices();
    vector<TSlice> vector_tslices{
      make_move_iterator(tslices.begin()),
      make_move_iterator(tslices.end()) };

    CHECK(vector_tslices.size() == 7);
    CHECK(vector_tslices[0].t0_tf() == Interval(-oo,0));
    CHECK(vector_tslices[1].t0_tf() == Interval(0));
    CHECK(vector_tslices[2].t0_tf() == Interval(0,0.5));
    CHECK(vector_tslices[3].t0_tf() == Interval(0.5));
    CHECK(vector_tslices[4].t0_tf() == Interval(0.5,1));
    CHECK(vector_tslices[5].t0_tf() == Interval(1,1));
    CHECK(vector_tslices[6].t0_tf() == Interval(1,oo));

    CHECK(tdomain->iterator_tslice(-oo)->t0_tf() == Interval(-oo,0));
    CHECK(tdomain->iterator_tslice(-10.)->t0_tf() == Interval(-oo,0));
    CHECK(tdomain->iterator_tslice(0.)->t0_tf() == Interval(0));
    CHECK(tdomain->iterator_tslice(0.6)->t0_tf() == Interval(0.5,1));
    CHECK(tdomain->iterator_tslice(5540.2)->t0_tf() == Interval(1,oo));
    CHECK(tdomain->iterator_tslice(oo)->t0_tf() == Interval(1,oo));
  }

  SECTION("Test TDomain with sampling")
  {
    auto tdomain = create_tdomain(Interval(1), 0.5);
    CHECK(tdomain->nb_tslices() == 2);
    tdomain->sample(10.);
    CHECK(tdomain->nb_tslices() == 3);
    tdomain->sample(10.); // second sampling which creates a gate
    CHECK(tdomain->nb_tslices() == 4);
    tdomain->sample(10.); // no more action
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
    const list<TSlice>& tslices = tdomain->tslices();
    vector<TSlice> vector_tslices{
      make_move_iterator(tslices.begin()),
      make_move_iterator(tslices.end()) };
    CHECK(tdomain->nb_tslices() == 1);
    CHECK(vector_tslices.size() == 1);
    CHECK(vector_tslices[0].t0_tf() == Interval(-oo,oo));
  }

  SECTION("Test TDomain with sampling and values")
  {
    auto tdomain = create_tdomain();
    CHECK(tdomain->nb_tslices() == 1);
    Tube<IntervalVector> x(tdomain, 1);
    x.set(Interval(4));
    x(Interval(0,1)) = IntervalVector({{1,2}});
    x(Interval(1,2)) = IntervalVector({{5,6}});
    x(Interval(2,3)) = IntervalVector({{8,9}});
    CHECK(x.codomain() == IntervalVector({{1,9}}));
    CHECK(static_cast<IntervalVector>(x(0.5)) == IntervalVector({{1,2}}));
    CHECK(static_cast<IntervalVector>(x(1.5)) == IntervalVector({{5,6}}));
    CHECK(static_cast<IntervalVector>(x(2.5)) == IntervalVector({{8,9}}));
    
    const std::shared_ptr<Slice<IntervalVector>> s0 = x.first_slice_ptr();
    CHECK(s0->t0_tf() == Interval(-oo,0));
    CHECK(s0->codomain() == IntervalVector({{4}}));
    const std::shared_ptr<Slice<IntervalVector>> s1 = s0->next_slice_ptr();
    CHECK(s1->t0_tf() == Interval(0,1));
    CHECK(s1->codomain() == IntervalVector({{1,2}}));
    const std::shared_ptr<Slice<IntervalVector>> s2 = s1->next_slice_ptr();
    CHECK(s2->t0_tf() == Interval(1,2));
    CHECK(s2->codomain() == IntervalVector({{5,6}}));
    const std::shared_ptr<Slice<IntervalVector>> s3 = s2->next_slice_ptr();
    CHECK(s3->t0_tf() == Interval(2,3));
    CHECK(s3->codomain() == IntervalVector({{8,9}}));
    const std::shared_ptr<Slice<IntervalVector>> s4 = s3->next_slice_ptr();
    CHECK(s4->t0_tf() == Interval(3,oo));
    CHECK(s4->codomain() == IntervalVector({{4}}));

    CHECK(tdomain->nb_tslices() == 5);
    tdomain->sample(1.3);
    CHECK(tdomain->nb_tslices() == 6);
    CHECK(s2->t0_tf() == Interval(1,1.3));
    CHECK(s2->codomain() == IntervalVector({{5,6}}));
    const std::shared_ptr<Slice<IntervalVector>> s2bis = s2->next_slice_ptr();
    CHECK(s2bis->t0_tf() == Interval(1.3,2));
    CHECK(s2bis->codomain() == IntervalVector({{5,6}}));
    CHECK(s3->t0_tf() == Interval(2,3));
    CHECK(s3->codomain() == IntervalVector({{8,9}}));
  }

  SECTION("Test basic Tube<IntervalVector>")
  {
    auto tdomain = create_tdomain(Interval(0,1), 0.1, false);
    Tube<IntervalVector> x(tdomain, 3);

    CHECK(x.size() == 3);
    CHECK(x.tdomain() == tdomain);
    CHECK(x.t0_tf() == Interval(0,1));
    CHECK(x.nb_slices() == tdomain->nb_tslices());
    CHECK(x.nb_slices() == 12);
    CHECK(x.first_slice_ptr()->t0_tf() == Interval(-oo,0));
    CHECK(x.last_slice_ptr()->t0_tf() == Interval(1,oo));
    CHECK(x.codomain() == IntervalVector(3));
    x.set(IntervalVector(3, Interval(-10,10)));
    CHECK(x.codomain() == IntervalVector(3, Interval(-10,10)));

    // Tube<IntervalVector>Component
    CHECK(x[0].codomain() == Interval(-10,10));
    CHECK(x[0].t0_tf() == Interval(0,1));
    CHECK(x[0].tdomain() == tdomain);
    x[0].set(Interval(-20,20));
    CHECK(x[0].codomain() == Interval(-20,20));
    CHECK(x.codomain() == IntervalVector({Interval(-20,20),Interval(-10,10),Interval(-10,10)}));
    x[1] = x[0];
    CHECK(x[1].codomain() == Interval(-20,20));
    CHECK(x.codomain() == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));

    // Eval
    CHECK(tdomain->nb_tubes() == 1);
    CHECK(static_cast<IntervalVector>(x(Interval(-oo,oo))) == x.codomain());
    CHECK(static_cast<IntervalVector>(x(Interval(-1,1))) == x.codomain());
    CHECK(static_cast<IntervalVector>(x(-42.)) == x.codomain());

    // Eval: affectation at scalar t
    CHECK(tdomain->nb_tslices() == 12);
    x(-42.) = IntervalVector(3,Interval(2.,3.));
    CHECK(tdomain->nb_tslices() == 14);
    CHECK(static_cast<IntervalVector>(x(-42.)) == IntervalVector(3,Interval(2.,3.)));
    CHECK(static_cast<IntervalVector>(x(ibex::previous_float(-42.))) == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));
    CHECK(static_cast<IntervalVector>(x(ibex::next_float(-42.))) == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));

    // Eval: affectation at interval t
    CHECK(tdomain->nb_tslices() == 14);
    x(Interval(44,55)) = IntervalVector(3,Interval(9.,10.));
    CHECK(tdomain->nb_tslices() == 16);
    CHECK(static_cast<IntervalVector>(x(Interval(44,55))) == IntervalVector(3,Interval(9.,10.)));
    CHECK(static_cast<IntervalVector>(x(ibex::previous_float(44.))) == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));
    CHECK(static_cast<IntervalVector>(x(ibex::next_float(55.))) == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));

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
    Tube<IntervalVector> x(tdomain, 2);
    CHECK(x.nb_slices() == 12);
    CHECK(x.first_slice_ptr() == tdomain->iterator_tslice(-oo)->_slices.at(&x));
    CHECK(x.last_slice_ptr() == tdomain->iterator_tslice(oo)->_slices.at(&x));

    for(auto& s : x)
      s.set(IntervalVector(2,s.t0_tf()));

    vector<const Slice<IntervalVector>*> v;
    for(const auto& s : x)
      v.push_back(&s);

    CHECK(v[0]->t0_tf() == Interval(-oo,0));
    CHECK(v[0]->codomain() == IntervalVector(2,Interval(-oo,0)));
    CHECK(v[0]->input_gate() == IntervalVector(2,Interval(-oo,0)));
    CHECK(v[0]->output_gate() == IntervalVector(2,Interval(0)));

    CHECK(v[1]->t0_tf() == Interval(0,0.1));
    CHECK(v[1]->input_gate() == v[0]->output_gate());
    CHECK(v[1]->codomain() == IntervalVector(2,Interval(0,0.1)));
    CHECK(v[1]->input_gate() == IntervalVector(2,Interval(0)));
    CHECK(v[1]->output_gate() == IntervalVector(2,Interval(0.1)));

    CHECK(v[11]->t0_tf() == Interval(1,oo));
    CHECK(v[11]->input_gate() == v[10]->output_gate());
    CHECK(v[11]->codomain() == IntervalVector(2,Interval(1,oo)));
    CHECK(v[11]->input_gate() == IntervalVector(2,Interval(1)));
    CHECK(v[11]->output_gate() == IntervalVector(2,Interval(1,oo)));
  }

  SECTION("Test again 1")
  {
    auto tdomain = create_tdomain(Interval(0,10), 0.01, true); // last argument creates "gates" (degenerated slices at scalar timesteps)
    Tube<IntervalVector> x(tdomain,
      codac::TFunction("(sin(sqrt(t)+((t-5)^2)*[-0.01,0.01]) ; cos(t)+sin(t/0.2)*[-0.1,0.1])"));
    Tube<IntervalVector> u(tdomain, 2);

    CHECK(x.size() == 2);
    CHECK(u.size() == 2);

    codac::TFunction tf("x[2]", "u[2]", "(sin(x[1]) ; -sin(x[0]))");
    CtcDiffInclusion ctc_diffincl(tf);
    ctc_diffincl.contract(x,u);

    //vibes::beginDrawing();

    codac::TubeVector x_codac1 = x.to_codac1(); // may take time
    codac::Tube xi_codac1 = x[1].to_codac1(); // may take time

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
      Tube<IntervalVector> x(2, tdomain);

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
      CHECK(f.eval_vector(t,x,u) == ApproxIntvVector(IntervalVector({{7, 8.100000000000002},{10, 11.10000000000001}})));
    }
  }

  SECTION("Local TDomain, tube still valid after")
  {
    Tube<Interval> *x;

    {
      auto tdomain = create_tdomain(Interval(0,1), 0.1);
      x = new Tube<Interval>(tdomain);
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
}