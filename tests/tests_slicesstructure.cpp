#include "tests.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("input2index")
{
  SECTION("input2index")
  {
    Tube tube = tubeTest1();

    CHECK(tube.input2index(0.0) == 0);
    CHECK(tube.input2index(0.1) == 0);
    CHECK(tube.input2index(0.5) == 0);
    CHECK(tube.input2index(0.6) == 0);
    CHECK(tube.input2index(0.9) == 0);
    CHECK(tube.input2index(ibex::previous_float(1.0)) == 0);
    CHECK(tube.input2index(1.0) == 1);
    CHECK(tube.input2index(46.0) == 45);
    CHECK(tube.input2index(tube.domain().ub()) == 45);
    CHECK(tube.nbSlices() == 46);
  }

  /* DEPRECATED
  SECTION("index2input")
  {
    Tube tube = tubeTest1();

    CHECK(tube.index2input(0) == 0.0);
    CHECK(tube.input2index(1) == 1.0);
    CHECK(tube.input2index(2) == 2.0);
    for(int i = 0 ; i < 46 ; i++)
      CHECK(i == tube.input2index(tube.index2input(i)));
  }*/
}

TEST_CASE("Tube slices structure")
{
  SECTION("TubeSlice class")
  {
    TubeSlice tubeslice(Interval(0.,1.));
    CHECK(tubeslice.isSlice());
    CHECK(tubeslice.nbSlices() == 1);
  }

  SECTION("Tube class (one slice)")
  {
    Tube tube_a(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tube_a.domain() == Interval(0.,1.));
    CHECK(tube_a.codomain() == Interval(-1.,1.));
    CHECK(tube_a.nbSlices() == 1);
    CHECK(tube_a.getSlice(0)->tubeReference() == &tube_a);

    Tube tube_b(tube_a);
    CHECK(tube_b.domain() == Interval(0.,1.));
    CHECK(tube_b.codomain() == Interval(-1.,1.));
    CHECK(tube_b.nbSlices() == 1);
    CHECK(tube_b.getSlice(0) != NULL);
    CHECK(tube_b.getSlice(0)->tubeReference() == &tube_b);

    Tube tube_c = tube_a;
    CHECK(tube_c.domain() == Interval(0.,1.));
    CHECK(tube_c.codomain() == Interval(-1.,1.));
    CHECK(tube_c.nbSlices() == 1);
    CHECK(tube_c.getSlice(0)->tubeReference() == &tube_c);
  }

  SECTION("Tube class (several slices)")
  {
    Tube tube_a(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tube_a.nbSlices() == 1);
    CHECK(tube_a.domain() == Interval(0.,1.));
    CHECK(tube_a.getSlice(0)->tubeReference() == &tube_a);

    CHECK_THROWS(tube_a.sample(-1.0););

    tube_a.sample(0.); // the following has no effect
    CHECK(tube_a.nbSlices() == 1);
    tube_a.sample(1.); // the following has no effect
    CHECK(tube_a.nbSlices() == 1);

    tube_a.sample(0.6);

    {
      TubeComponent *subtube_b1 = ((TubeNode*)tube_a.getTubeComponent())->getFirstTubeComponent();
      TubeComponent *subtube_b2 = ((TubeNode*)tube_a.getTubeComponent())->getSecondTubeComponent();

      CHECK(tube_a.nbSlices() == 2);
      CHECK(subtube_b1->nbSlices() == 1);
      CHECK(subtube_b2->nbSlices() == 1);
      CHECK(subtube_b1->domain() == Interval(0.,0.6));
      CHECK(subtube_b2->domain() == Interval(0.6,1.));
      CHECK(subtube_b1->tubeReference() == &tube_a);
      CHECK(subtube_b2->tubeReference() == &tube_a);
    }

    tube_a.sample(0.7);

    {
      TubeComponent *subtube_b1 = ((TubeNode*)tube_a.getTubeComponent())->getFirstTubeComponent();
      TubeComponent *subtube_b2 = ((TubeNode*)tube_a.getTubeComponent())->getSecondTubeComponent();
      TubeComponent *subtube_c1 = ((TubeNode*)subtube_b2)->getFirstTubeComponent();
      TubeComponent *subtube_c2 = ((TubeNode*)subtube_b2)->getSecondTubeComponent();

      CHECK(tube_a.nbSlices() == 3);
      CHECK(subtube_b1->nbSlices() == 1);
      CHECK(subtube_b1->domain() == Interval(0.,0.6));
      CHECK(subtube_b2->nbSlices() == 2);
      CHECK(subtube_b2->domain() == Interval(0.6,1.));
      CHECK(subtube_b1->tubeReference() == &tube_a);
      CHECK(subtube_b2->tubeReference() == &tube_a);
      CHECK(subtube_c1->nbSlices() == 1);
      CHECK(subtube_c2->nbSlices() == 1);
      CHECK(subtube_c1->domain() == Interval(0.6,0.7));
      CHECK(subtube_c2->domain() == Interval(0.7,1.0));
      CHECK(subtube_c1->tubeReference() == &tube_a);
      CHECK(subtube_c2->tubeReference() == &tube_a);
    }

    tube_a.sample(0.7); // the following has no more effect
    tube_a.sample(0.7);
    CHECK(tube_a.nbSlices() == 3);
    tube_a.sample(0.62);

    {
      TubeComponent *subtube_b1 = ((TubeNode*)tube_a.getTubeComponent())->getFirstTubeComponent();
      TubeComponent *subtube_b2 = ((TubeNode*)tube_a.getTubeComponent())->getSecondTubeComponent();
      TubeComponent *subtube_c1 = ((TubeNode*)subtube_b2)->getFirstTubeComponent();
      TubeComponent *subtube_c2 = ((TubeNode*)subtube_b2)->getSecondTubeComponent();
      TubeComponent *subtube_d1 = ((TubeNode*)subtube_c1)->getFirstTubeComponent();
      TubeComponent *subtube_d2 = ((TubeNode*)subtube_c1)->getSecondTubeComponent();

      CHECK(tube_a.nbSlices() == 4);
      CHECK(subtube_b1->nbSlices() == 1);
      CHECK(subtube_b1->domain() == Interval(0.,0.6));
      CHECK(subtube_b2->nbSlices() == 3);
      CHECK(subtube_b2->domain() == Interval(0.6,1.));
      CHECK(subtube_b1->tubeReference() == &tube_a);
      CHECK(subtube_b2->tubeReference() == &tube_a);
      CHECK(subtube_c1->nbSlices() == 2);
      CHECK(subtube_c2->nbSlices() == 1);
      CHECK(subtube_c1->domain() == Interval(0.6,0.7));
      CHECK(subtube_c2->domain() == Interval(0.7,1.0));
      CHECK(subtube_c1->tubeReference() == &tube_a);
      CHECK(subtube_c2->tubeReference() == &tube_a);
      CHECK(subtube_d1->nbSlices() == 1);
      CHECK(subtube_d2->nbSlices() == 1);
      CHECK(subtube_d1->domain() == Interval(0.6,0.62));
      CHECK(subtube_d2->domain() == Interval(0.62,0.7));
      CHECK(subtube_d1->tubeReference() == &tube_a);
      CHECK(subtube_d2->tubeReference() == &tube_a);
    }

    tube_a.sample(0.1);
    tube_a.sample(0.1); // the following has no more effect

    {
      TubeComponent *subtube_b1 = ((TubeNode*)tube_a.getTubeComponent())->getFirstTubeComponent();
      TubeComponent *subtube_b2 = ((TubeNode*)tube_a.getTubeComponent())->getSecondTubeComponent();
      TubeComponent *subtube_e1 = ((TubeNode*)subtube_b1)->getFirstTubeComponent();
      TubeComponent *subtube_e2 = ((TubeNode*)subtube_b1)->getSecondTubeComponent();

      CHECK(tube_a.nbSlices() == 5);
      CHECK(subtube_b1->nbSlices() == 2);
      CHECK(subtube_b1->domain() == Interval(0.,0.6));
      CHECK(subtube_b2->nbSlices() == 3);
      CHECK(subtube_b2->domain() == Interval(0.6,1.));
      CHECK(subtube_e1->nbSlices() == 1);
      CHECK(subtube_e2->nbSlices() == 1);
      CHECK(subtube_e1->domain() == Interval(0.0,0.1));
      CHECK(subtube_e2->domain() == Interval(0.1,0.6));
      CHECK(subtube_e1->tubeReference() == &tube_a);
      CHECK(subtube_e2->tubeReference() == &tube_a);
    }

    tube_a.sample(0.); // the following has no effect
    CHECK(tube_a.nbSlices() == 5);
    tube_a.sample(1.); // the following has no effect
    CHECK(tube_a.nbSlices() == 5);
  }

  SECTION("Sample method (bis)")
  {
    Tube tube(Interval(0.,6.), 2.);
    CHECK(tube.nbSlices() == 3);

    tube.set(Interval(-3.,2.), 0);
    tube.set(Interval(-1.,3.), 1);
    tube.set(Interval(-3.,2.), 2);
    CHECK(tube.nbSlices() == 3);
    tube.set(Interval(-3.,1.), 0.); // gate
    CHECK(tube.nbSlices() == 3);
    tube.set(Interval(0.,1.), 2.); // gate
    CHECK(tube.nbSlices() == 3);
    tube.set(Interval(-1.,1.), 4.); // gate
    CHECK(tube.nbSlices() == 3);
    tube.set(Interval(-1.,0.), 6.); // gate
    CHECK(tube.nbSlices() == 3);
    CHECK(tube[0] == Interval(-3.,2.));
    CHECK(tube[1] == Interval(-1.,3.));
    CHECK(tube[2] == Interval(-3.,2.));
    CHECK(tube[0.] == Interval(-3.,1.)); // gate
    CHECK(tube[2.] == Interval(0.,1.)); // gate
    CHECK(tube[4.] == Interval(-1.,1.)); // gate
    CHECK(tube[6.] == Interval(-1.,0.)); // gate

    tube.sample(1.);
    tube.sample(5.);

    CHECK(tube.nbSlices() == 5);
    CHECK(tube[0] == Interval(-3.,2.));
    CHECK(tube[1] == Interval(-3.,2.));
    CHECK(tube[2] == Interval(-1.,3.));
    CHECK(tube[3] == Interval(-3.,2.));
    CHECK(tube[4] == Interval(-3.,2.));
    CHECK(tube[0.] == Interval(-3.,1.)); // gate
    CHECK(tube[1.] == Interval(-3.,2.)); // gate
    CHECK(tube[2.] == Interval(0.,1.)); // gate
    CHECK(tube[3.] == Interval(-1.,3.));
    CHECK(tube[4.] == Interval(-1.,1.)); // gate
    CHECK(tube[5.] == Interval(-3.,2.)); // gate
    CHECK(tube[6.] == Interval(-1.,0.)); // gate
  }

  SECTION("input2index and reverse operation")
  {
    Tube tube(Interval(0.,1.), Interval(-1.,1.));
    tube.sample(0.6);
    tube.sample(0.7);
    tube.sample(0.62);
    tube.sample(0.1);

    // input2index
    CHECK_THROWS(tube.input2index(-0.1));
    CHECK(tube.input2index(0.) == 0);
    CHECK(tube.input2index(0.01) == 0);
    CHECK(tube.input2index(0.6) == 2);
    CHECK(tube.input2index(0.61) == 2);
    CHECK(tube.input2index(1.0) == 4);
    CHECK_THROWS(tube.input2index(1.01));

    /* DEPRECATED
    // index2input
    CHECK_THROWS(tube.index2input(-1));
    CHECK(tube.index2input(0) == 0.);
    CHECK(tube.index2input(1) == 0.1);
    CHECK(tube.index2input(4) == 0.7);
    CHECK_THROWS(tube.input2index(5));
    */
  }

  SECTION("Getting slices")
  {
    Tube tube(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tube.getSlice(0) == ((TubeSlice*)tube.getTubeComponent()));
    CHECK_THROWS(tube.getSlice(1));
    tube.sample(0.6);
    tube.sample(0.7);
    tube.sample(0.62);
    tube.sample(0.1);
    CHECK(tube.nbSlices() == 5);

    // By indexes
    CHECK_THROWS(tube.getSlice(-1));
    CHECK(tube.getSlice(0)->domain() == Interval(0.0,0.1));
    CHECK(tube.getSlice(1)->domain() == Interval(0.1,0.6));
    CHECK(tube.getSlice(2)->domain() == Interval(0.6,0.62));
    CHECK(tube.getSlice(3)->domain() == Interval(0.62,0.7));
    CHECK(tube.getSlice(4)->domain() == Interval(0.7,1.0));

    // By times
    CHECK_THROWS(tube.getSlice(-0.001));
    CHECK_THROWS(tube.getSlice(1.001));
    CHECK(tube.getSlice(0.)->domain() == Interval(0.0,0.1));
    CHECK(tube.getSlice(0.03)->domain() == Interval(0.0,0.1));
    CHECK(tube.getSlice(0.2)->domain() == Interval(0.1,0.6));
    CHECK(tube.getSlice(0.6)->domain() == Interval(0.6,0.62));
    CHECK(tube.getSlice(0.62)->domain() == Interval(0.62,0.7));
    CHECK(tube.getSlice(1.0)->domain() == Interval(0.7,1.0));

    // Copy: constructor
    Tube tube_b(tube);
    CHECK(tube_b.getSlice(0.)->domain() == Interval(0.0,0.1));
    CHECK(tube_b.getSlice(0.03)->domain() == Interval(0.0,0.1));
    CHECK(tube_b.getSlice(0.2)->domain() == Interval(0.1,0.6));
    CHECK(tube_b.getSlice(0.6)->domain() == Interval(0.6,0.62));
    CHECK(tube_b.getSlice(0.62)->domain() == Interval(0.62,0.7));
    CHECK(tube_b.getSlice(1.0)->domain() == Interval(0.7,1.0));
    
    // Copy: operator
    Tube tube_c = tube;
    CHECK(tube_c.getSlice(0.)->domain() == Interval(0.0,0.1));
    CHECK(tube_c.getSlice(0.03)->domain() == Interval(0.0,0.1));
    CHECK(tube_c.getSlice(0.2)->domain() == Interval(0.1,0.6));
    CHECK(tube_c.getSlice(0.6)->domain() == Interval(0.6,0.62));
    CHECK(tube_c.getSlice(0.62)->domain() == Interval(0.62,0.7));
    CHECK(tube_c.getSlice(1.0)->domain() == Interval(0.7,1.0));

    // First and last slices
    CHECK(tube.getSlice(0) == tube.getFirstSlice());
    CHECK(tube.getSlice(0) == ((TubeNode*)tube.getTubeComponent())->getFirstTubeComponent()->getFirstSlice());
    CHECK(tube.getSlice(1) == ((TubeNode*)tube.getTubeComponent())->getFirstTubeComponent()->getLastSlice());
    CHECK(tube.getSlice(4) == tube.getLastSlice());

    // Checking root references
    CHECK(tube.getSlice(0)->tubeReference() == &tube);
    CHECK(tube.getSlice(1)->tubeReference() == &tube);
    CHECK(tube.getSlice(2)->tubeReference() == &tube);
    CHECK(tube.getSlice(3)->tubeReference() == &tube);
    CHECK(tube.getSlice(4)->tubeReference() == &tube);

    // getSlices
    vector<const TubeSlice*> v_slices;
    tube.getSlices(v_slices);
    CHECK(v_slices.size() == 5);
    for(int i = 0 ; i < 5 ; i++)
      CHECK(v_slices[i] == tube.getSlice(i));
    CHECK(tube.getSlice(0.1)->box()[0] == Interval(0.1,0.6));
    CHECK(tube.getSlice(0.1)->box()[1] == Interval(-1.,1.));
    CHECK(tube.getSlice(0.1)->domain() == Interval(0.1,0.6));
    CHECK(tube.getSlice(1)->box()[0] == Interval(0.1,0.6));
    CHECK(tube.getSlice(1)->box()[1] == Interval(-1.,1.));
    CHECK(tube.getSlice(1)->domain() == Interval(0.1,0.6));
    CHECK(tube.getSlice(4)->box()[0] == Interval(0.7,1.0));
    CHECK(tube.getSlice(4)->box()[1] == Interval(-1.,1.));
    CHECK(tube.getSlice(4)->domain() == Interval(0.7,1.0));
    CHECK_THROWS(tube.getSlice(5)->box());

    // prev/next slices
    CHECK(tube.getSlice(0)->prevSlice() == NULL);
    for(int i = 0 ; i < tube.nbSlices()-1 ; i++)
    {      
      CHECK(tube.getSlice(i)->nextSlice() == tube.getSlice(i+1));
      CHECK(tube.getSlice(i+1)->prevSlice() == tube.getSlice(i));
    }
    CHECK(tube.getSlice(4)->nextSlice() == NULL);

    Tube tube_simple(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tube_simple.getSlice(0)->prevSlice() == NULL);
    CHECK(tube_simple.getSlice(0)->nextSlice() == NULL);
    tube_simple.sample(0.3);
    CHECK(tube_simple.getSlice(0)->nextSlice() == tube_simple.getSlice(1));
    CHECK(tube_simple.getSlice(1)->prevSlice() == tube_simple.getSlice(0));
  }

  SECTION("Gates")
  {
    Tube tube_a(Interval(0.,1.), Interval(-1.,1.));
    tube_a.set(Interval(0.), 0.);
    tube_a.set(Interval(0.2,0.3), 1.);
    tube_a.sample(0.6, Interval(0.0,0.5));

    CHECK(tube_a.getSlice(0)->inputGate() == 0.);
    CHECK(tube_a.getSlice(0)->outputGate() == Interval(0.,0.5));
    CHECK(tube_a.getSlice(1)->inputGate() == Interval(0.,0.5));
    CHECK(tube_a.getSlice(1)->outputGate() == Interval(0.2,0.3));

    tube_a.sample(0.7, Interval(0.2,0.3));
    tube_a.sample(0.62, Interval(-6.,6.)); // too large gate
    tube_a.sample(0.1, Interval(-1.0,-0.8));

    CHECK(tube_a.getSlice(0)->inputGate() == 0.);
    CHECK(tube_a.getSlice(0)->outputGate() == Interval(-1.,-0.8));
    CHECK(tube_a.getSlice(1)->inputGate() == Interval(-1.,-0.8));
    CHECK(tube_a.getSlice(1)->outputGate() == Interval(0.,0.5));
    CHECK(tube_a.getSlice(2)->inputGate() == Interval(0.,0.5));
    CHECK(tube_a.getSlice(2)->outputGate() == Interval(-1.,1.));
    CHECK(tube_a.getSlice(3)->inputGate() == Interval(-1.,1.));
    CHECK(tube_a.getSlice(3)->outputGate() == Interval(0.2,0.3));
    CHECK(tube_a.getSlice(4)->inputGate() == Interval(0.2,0.3));
    CHECK(tube_a.getSlice(4)->outputGate() == Interval(0.2,0.3));

    Tube tube_b(tube_a);
    CHECK(tube_b.getSlice(0)->inputGate() == 0.);
    CHECK(tube_b.getSlice(0)->outputGate() == Interval(-1.,-0.8));
    CHECK(tube_b.getSlice(1)->inputGate() == Interval(-1.,-0.8));
    CHECK(tube_b.getSlice(1)->outputGate() == Interval(0.,0.5));
    CHECK(tube_b.getSlice(2)->inputGate() == Interval(0.,0.5));
    CHECK(tube_b.getSlice(2)->outputGate() == Interval(-1.,1.));
    CHECK(tube_b.getSlice(3)->inputGate() == Interval(-1.,1.));
    CHECK(tube_b.getSlice(3)->outputGate() == Interval(0.2,0.3));
    CHECK(tube_b.getSlice(4)->inputGate() == Interval(0.2,0.3));
    CHECK(tube_b.getSlice(4)->outputGate() == Interval(0.2,0.3));

    Tube tube_c = tube_a;
    CHECK(tube_c.getSlice(0)->inputGate() == 0.);
    CHECK(tube_c.getSlice(0)->outputGate() == Interval(-1.,-0.8));
    CHECK(tube_c.getSlice(1)->inputGate() == Interval(-1.,-0.8));
    CHECK(tube_c.getSlice(1)->outputGate() == Interval(0.,0.5));
    CHECK(tube_c.getSlice(2)->inputGate() == Interval(0.,0.5));
    CHECK(tube_c.getSlice(2)->outputGate() == Interval(-1.,1.));
    CHECK(tube_c.getSlice(3)->inputGate() == Interval(-1.,1.));
    CHECK(tube_c.getSlice(3)->outputGate() == Interval(0.2,0.3));
    CHECK(tube_c.getSlice(4)->inputGate() == Interval(0.2,0.3));
    CHECK(tube_c.getSlice(4)->outputGate() == Interval(0.2,0.3));

    tube_a.sample(0.67); // no gate specification
    CHECK(tube_a.getSlice(0.67)->inputGate() == Interval(-1.,1));
    tube_a.sample(0.68, Interval::ALL_REALS);
    CHECK(tube_a.getSlice(0.68)->inputGate() == Interval(-1.,1));
    tube_a.sample(0.69, Interval::EMPTY_SET);
    CHECK(tube_a.getSlice(0.69)->inputGate() == Interval::EMPTY_SET);

    tube_a.getSlice(0.69)->setInputGate(Interval(5.));
    CHECK(tube_a.getSlice(0.69)->inputGate().is_empty());
    CHECK(tube_a[0.69].is_empty());

    tube_a.set(Interval(0.1), 0.);
    CHECK(tube_a.getSlice(0)->inputGate() == Interval(0.1));
    CHECK(tube_a[0.] == Interval(0.1));

    tube_a.set(Interval(0.3), 1.);
    CHECK(tube_a.getSlice(tube_a.nbSlices()-1)->outputGate() == Interval(0.3));
    CHECK(tube_a[1.] == Interval(0.3));
  }
}
