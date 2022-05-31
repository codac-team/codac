#include "catch_interval.hpp"
#include "vibes.h"

// Using #define so that we can access protected methods
// of the class for tests purposes
#define protected public
#include "codac2_TDomain.h"

#include "codac_predef_values.h"
#include "codac2_TubeVector.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac2;
using codac::oo;

TEST_CASE("Test codac2::tubes")
{
  SECTION("Test TDomain")
  {
    TDomain tdomain(Interval(0,1), 0.5);
    CHECK(tdomain.nb_tslices() == 4);
    CHECK(tdomain.t0_tf() == Interval(0,1));
    
    const list<TSlice>& tslices = tdomain.tslices();
    vector<TSlice> vector_tslices{
      make_move_iterator(tslices.begin()),
      make_move_iterator(tslices.end()) };

    CHECK(vector_tslices.size() == 4);
    CHECK(vector_tslices[0].t0_tf() == Interval(-oo,0));
    CHECK(vector_tslices[1].t0_tf() == Interval(0,0.5));
    CHECK(vector_tslices[2].t0_tf() == Interval(0.5,1));
    CHECK(vector_tslices[3].t0_tf() == Interval(1,oo));

    CHECK(tdomain.iterator_tslice(-10.)->t0_tf() == Interval(-oo,0));
    CHECK(tdomain.iterator_tslice(-120.)->t0_tf() == Interval(-oo,0));
    CHECK(tdomain.iterator_tslice(0.2)->t0_tf() == Interval(0,0.5));
    CHECK(tdomain.iterator_tslice(5540.2)->t0_tf() == Interval(1,oo));

    CHECK(tdomain.nb_tubes() == 0);
    TubeVector x(2, tdomain); // adding a tubevector to the tdomain
    CHECK(tdomain.nb_tubes() == 1);

    { // new scope
      TubeVector v(3, tdomain);
      CHECK(tdomain.nb_tubes() == 2);
    } // end of scope, auto removing the tube
    
    CHECK(tdomain.nb_tubes() == 1);
  }

  SECTION("Test degenerated TDomain")
  {
    TDomain tdomain(Interval(1), 0.5);
    CHECK(tdomain.nb_tslices() == 2);
    CHECK(tdomain.t0_tf() == Interval(1));
    CHECK(tdomain.nb_tubes() == 0);
    
    const list<TSlice>& tslices = tdomain.tslices();
    vector<TSlice> vector_tslices{
      make_move_iterator(tslices.begin()),
      make_move_iterator(tslices.end()) };

    CHECK(vector_tslices.size() == 2);
    CHECK(vector_tslices[0].t0_tf() == Interval(-oo,1));
    CHECK(vector_tslices[1].t0_tf() == Interval(1,oo));
  }

  SECTION("Test TDomain with gates")
  {
    TDomain tdomain(Interval(0,1), 0.5, true);
    CHECK(tdomain.nb_tslices() == 7);
    CHECK(tdomain.t0_tf() == Interval(0,1));
    CHECK(tdomain.nb_tubes() == 0);
    
    const list<TSlice>& tslices = tdomain.tslices();
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

    CHECK(tdomain.iterator_tslice(-oo)->t0_tf() == Interval(-oo,0));
    CHECK(tdomain.iterator_tslice(-10.)->t0_tf() == Interval(-oo,0));
    CHECK(tdomain.iterator_tslice(0.)->t0_tf() == Interval(0));
    CHECK(tdomain.iterator_tslice(0.6)->t0_tf() == Interval(0.5,1));
    CHECK(tdomain.iterator_tslice(5540.2)->t0_tf() == Interval(1,oo));
    CHECK(tdomain.iterator_tslice(oo)->t0_tf() == Interval(1,oo));
  }

  SECTION("Test TDomain with sampling")
  {
    TDomain tdomain(Interval(1), 0.5);
    CHECK(tdomain.nb_tslices() == 2);
    tdomain.sample(10.);
    CHECK(tdomain.nb_tslices() == 3);
    tdomain.sample(10.); // second sampling which creates a gate
    CHECK(tdomain.nb_tslices() == 4);
    tdomain.sample(10.); // no more action
    CHECK(tdomain.nb_tslices() == 4);
    
    const list<TSlice>& tslices = tdomain.tslices();
    vector<TSlice> vector_tslices{
      make_move_iterator(tslices.begin()),
      make_move_iterator(tslices.end()) };

    CHECK(vector_tslices.size() == 4);
    CHECK(vector_tslices[0].t0_tf() == Interval(-oo,1));
    CHECK(vector_tslices[1].t0_tf() == Interval(1,10));
    CHECK(vector_tslices[2].t0_tf() == Interval(10));
    CHECK(vector_tslices[3].t0_tf() == Interval(10,oo));
  }

  SECTION("Test basic TubeVector")
  {
    TDomain tdomain(Interval(0,1), 0.1, false);
    TubeVector x(3, tdomain);

    CHECK(x.size() == 3);
    CHECK(&x.tdomain() == &tdomain);
    CHECK(x.t0_tf() == Interval(0,1));
    CHECK(x.nb_slices() == tdomain.nb_tslices());
    CHECK(x.nb_slices() == 12);
    CHECK(x.first_slice().t0_tf() == Interval(-oo,0));
    CHECK(x.last_slice().t0_tf() == Interval(1,oo));
    CHECK(x.codomain() == IntervalVector(3));
    x.set(IntervalVector(3, Interval(-10,10)));
    CHECK(x.codomain() == IntervalVector(3, Interval(-10,10)));

    // TubeVectorComponent
    CHECK(x[0].codomain() == Interval(-10,10));
    CHECK(x[0].t0_tf() == Interval(0,1));
    CHECK(&x[0].tdomain() == &tdomain);
    x[0].set(Interval(-20,20));
    CHECK(x[0].codomain() == Interval(-20,20));
    CHECK(x.codomain() == IntervalVector({Interval(-20,20),Interval(-10,10),Interval(-10,10)}));
    x[1] = x[0];
    CHECK(x[1].codomain() == Interval(-20,20));
    CHECK(x.codomain() == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));

    // Eval
    CHECK(tdomain.nb_tubes() == 1);
    CHECK(static_cast<IntervalVector>(x(Interval(-oo,oo))) == x.codomain());
    CHECK(static_cast<IntervalVector>(x(Interval(-1,1))) == x.codomain());
    CHECK(static_cast<IntervalVector>(x(-42.)) == x.codomain());

    // Eval: affectation at scalar t
    CHECK(tdomain.nb_tslices() == 12);
    x(-42.) = IntervalVector(3,Interval(2.,3.));
    CHECK(tdomain.nb_tslices() == 14);
    CHECK(static_cast<IntervalVector>(x(-42.)) == IntervalVector(3,Interval(2.,3.)));
    CHECK(static_cast<IntervalVector>(x(ibex::previous_float(-42.))) == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));
    CHECK(static_cast<IntervalVector>(x(ibex::next_float(-42.))) == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));

    // Eval: affectation at interval t
    CHECK(tdomain.nb_tslices() == 14);
    x(Interval(44,55)) = IntervalVector(3,Interval(9.,10.));
    CHECK(tdomain.nb_tslices() == 16);
    CHECK(static_cast<IntervalVector>(x(Interval(44,55))) == IntervalVector(3,Interval(9.,10.)));
    CHECK(static_cast<IntervalVector>(x(ibex::previous_float(44.))) == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));
    CHECK(static_cast<IntervalVector>(x(ibex::next_float(55.))) == IntervalVector({Interval(-20,20),Interval(-20,20),Interval(-10,10)}));

    // Iterators tests
    {
      SliceVector* s_ = &x.first_slice();
      for(auto& s : x)
      {
        CHECK(&s == s_);
        s_ = s_->next_slice();
      }
    }

    // Iterators tests (const)
    {
      const TubeVector y(x); // copy constructor
      const SliceVector* s_ = &x.first_slice();
      for(const auto& s : x)
      {
        CHECK(&s == s_);
        s_ = s_->next_slice();
      }
    }
  }

  SECTION("Test SliceVector")
  {
    TDomain tdomain(Interval(0,1), 0.1);
    TubeVector x(2, tdomain);
    CHECK(x.nb_slices() == 12);
    CHECK(&x.first_slice() == &tdomain.iterator_tslice(-oo)->_slices.at(&x));
    CHECK(&x.last_slice() == &tdomain.iterator_tslice(oo)->_slices.at(&x));

    for(auto& s : x)
      s.set(IntervalVector(2,s.t0_tf()));

    vector<const SliceVector*> v;
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
}