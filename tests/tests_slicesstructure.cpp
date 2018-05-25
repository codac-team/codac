#include "tests.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("input2index / index2input")
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

  SECTION("index2input")
  {
    Tube tube = tubeTest1();

    CHECK(tube.index2input(0) == 0.0);
    CHECK(tube.input2index(1) == 1.0);
    CHECK(tube.input2index(2) == 2.0);
    for(int i = 0 ; i < 46 ; i++)
      CHECK(i == tube.input2index(tube.index2input(i)));
  }
}

TEST_CASE("Tube slices structure")
{
  SECTION("TubeSlice class")
  {
    TubeSlice tubeslice(Interval(0.,1.));
    CHECK(tubeslice.isSlice());
    CHECK(tubeslice.nbSlices() == 1);
  }

  SECTION("TubeNode class")
  {
    TubeNode tubetree_a(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tubetree_a.isSlice() == false);
    CHECK(tubetree_a.getFirstTubeComponent() != NULL);
    CHECK(tubetree_a.getFirstTubeComponent()->isSlice());
    CHECK(tubetree_a.getFirstTubeComponent()->codomain() == Interval(-1.,1.));
    CHECK(tubetree_a.getSecondTubeComponent() == NULL);
    CHECK(tubetree_a.nbSlices() == 1);
    CHECK(tubetree_a.getFirstTubeComponent()->nbSlices() == 1);
    CHECK(tubetree_a.tubeReference() == &tubetree_a);

    TubeNode tubetree_b(tubetree_a);
    CHECK(tubetree_b.isSlice() == false);
    CHECK(tubetree_b.getFirstTubeComponent() != NULL);
    CHECK(tubetree_b.getFirstTubeComponent()->isSlice());
    CHECK(tubetree_b.getFirstTubeComponent()->codomain() == Interval(-1.,1.));
    CHECK(tubetree_b.getSecondTubeComponent() == NULL);
    CHECK(tubetree_b.nbSlices() == 1);
    CHECK(tubetree_b.getFirstTubeComponent()->nbSlices() == 1);
    CHECK(tubetree_b.tubeReference() == &tubetree_b);

    TubeNode tubetree_c = tubetree_a;
    CHECK(tubetree_c.isSlice() == false);
    CHECK(tubetree_c.getFirstTubeComponent() != NULL);
    CHECK(tubetree_c.getFirstTubeComponent()->isSlice());
    CHECK(tubetree_c.getFirstTubeComponent()->codomain() == Interval(-1.,1.));
    CHECK(tubetree_c.getSecondTubeComponent() == NULL);
    CHECK(tubetree_c.nbSlices() == 1);
    CHECK(tubetree_c.getFirstTubeComponent()->nbSlices() == 1);
    CHECK(tubetree_c.tubeReference() == &tubetree_c);
  }

  SECTION("Sample method")
  {
    TubeNode tubetree_a(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tubetree_a.nbSlices() == 1);
    CHECK(tubetree_a.getFirstTubeComponent()->domain() == Interval(0.,1.));
    CHECK(tubetree_a.getSecondTubeComponent() == NULL);
    CHECK(tubetree_a.tubeReference() == &tubetree_a);

    CHECK_THROWS(tubetree_a.sample(-1.0););

    tubetree_a.sample(0.); // the following has no effect
    CHECK(tubetree_a.nbSlices() == 1);
    tubetree_a.sample(1.); // the following has no effect
    CHECK(tubetree_a.nbSlices() == 1);

    tubetree_a.sample(0.6);

    {
      TubeNode *tubetree_b1 = (TubeNode*)tubetree_a.getFirstTubeComponent();
      TubeNode *tubetree_b2 = (TubeNode*)tubetree_a.getSecondTubeComponent();

      CHECK(tubetree_a.nbSlices() == 2);
      CHECK(tubetree_b1->nbSlices() == 1);
      CHECK(tubetree_b2->nbSlices() == 1);
      CHECK(tubetree_b1->domain() == Interval(0.,0.6));
      CHECK(tubetree_b2->domain() == Interval(0.6,1.));
      CHECK(tubetree_b1->tubeReference() == &tubetree_a);
      CHECK(tubetree_b2->tubeReference() == &tubetree_a);
      CHECK_THROWS(tubetree_b1->sample(0.7););
    }

    tubetree_a.sample(0.7);

    {
      TubeNode *tubetree_b1 = (TubeNode*)tubetree_a.getFirstTubeComponent();
      TubeNode *tubetree_b2 = (TubeNode*)tubetree_a.getSecondTubeComponent();
      TubeNode *tubetree_c1 = (TubeNode*)tubetree_b2->getFirstTubeComponent();
      TubeNode *tubetree_c2 = (TubeNode*)tubetree_b2->getSecondTubeComponent();

      CHECK(tubetree_a.nbSlices() == 3);
      CHECK(tubetree_b1->nbSlices() == 1);
      CHECK(tubetree_b1->domain() == Interval(0.,0.6));
      CHECK(tubetree_b2->nbSlices() == 2);
      CHECK(tubetree_b2->domain() == Interval(0.6,1.));
      CHECK(tubetree_b1->tubeReference() == &tubetree_a);
      CHECK(tubetree_b2->tubeReference() == &tubetree_a);
      CHECK(tubetree_c1->nbSlices() == 1);
      CHECK(tubetree_c2->nbSlices() == 1);
      CHECK(tubetree_c1->domain() == Interval(0.6,0.7));
      CHECK(tubetree_c2->domain() == Interval(0.7,1.0));
      CHECK(tubetree_c1->tubeReference() == &tubetree_a);
      CHECK(tubetree_c2->tubeReference() == &tubetree_a);
    }

    tubetree_a.sample(0.7); // the following has no more effect
    tubetree_a.sample(0.7);
    CHECK(tubetree_a.nbSlices() == 3);
    tubetree_a.sample(0.62);

    {
      TubeNode *tubetree_b1 = (TubeNode*)tubetree_a.getFirstTubeComponent();
      TubeNode *tubetree_b2 = (TubeNode*)tubetree_a.getSecondTubeComponent();
      TubeNode *tubetree_c1 = (TubeNode*)tubetree_b2->getFirstTubeComponent();
      TubeNode *tubetree_c2 = (TubeNode*)tubetree_b2->getSecondTubeComponent();
      TubeNode *tubetree_d1 = (TubeNode*)tubetree_c1->getFirstTubeComponent();
      TubeNode *tubetree_d2 = (TubeNode*)tubetree_c1->getSecondTubeComponent();

      CHECK(tubetree_a.nbSlices() == 4);
      CHECK(tubetree_b1->nbSlices() == 1);
      CHECK(tubetree_b1->domain() == Interval(0.,0.6));
      CHECK(tubetree_b2->nbSlices() == 3);
      CHECK(tubetree_b2->domain() == Interval(0.6,1.));
      CHECK(tubetree_b1->tubeReference() == &tubetree_a);
      CHECK(tubetree_b2->tubeReference() == &tubetree_a);
      CHECK(tubetree_c1->nbSlices() == 2);
      CHECK(tubetree_c2->nbSlices() == 1);
      CHECK(tubetree_c1->domain() == Interval(0.6,0.7));
      CHECK(tubetree_c2->domain() == Interval(0.7,1.0));
      CHECK(tubetree_c1->tubeReference() == &tubetree_a);
      CHECK(tubetree_c2->tubeReference() == &tubetree_a);
      CHECK(tubetree_d1->nbSlices() == 1);
      CHECK(tubetree_d2->nbSlices() == 1);
      CHECK(tubetree_d1->domain() == Interval(0.6,0.62));
      CHECK(tubetree_d2->domain() == Interval(0.62,0.7));
      CHECK(tubetree_d1->tubeReference() == &tubetree_a);
      CHECK(tubetree_d2->tubeReference() == &tubetree_a);
    }

    tubetree_a.sample(0.1);
    tubetree_a.sample(0.1); // the following has no more effect

    {
      TubeNode *tubetree_b1 = (TubeNode*)tubetree_a.getFirstTubeComponent();
      TubeNode *tubetree_b2 = (TubeNode*)tubetree_a.getSecondTubeComponent();
      TubeNode *tubetree_e1 = (TubeNode*)tubetree_b1->getFirstTubeComponent();
      TubeNode *tubetree_e2 = (TubeNode*)tubetree_b1->getSecondTubeComponent();

      CHECK(tubetree_a.nbSlices() == 5);
      CHECK(tubetree_b1->nbSlices() == 2);
      CHECK(tubetree_b1->domain() == Interval(0.,0.6));
      CHECK(tubetree_b2->nbSlices() == 3);
      CHECK(tubetree_b2->domain() == Interval(0.6,1.));
      CHECK(tubetree_e1->nbSlices() == 1);
      CHECK(tubetree_e2->nbSlices() == 1);
      CHECK(tubetree_e1->domain() == Interval(0.0,0.1));
      CHECK(tubetree_e2->domain() == Interval(0.1,0.6));
      CHECK(tubetree_e1->tubeReference() == &tubetree_a);
      CHECK(tubetree_e2->tubeReference() == &tubetree_a);
    }

    tubetree_a.sample(0.); // the following has no effect
    CHECK(tubetree_a.nbSlices() == 5);
    tubetree_a.sample(1.); // the following has no effect
    CHECK(tubetree_a.nbSlices() == 5);
  }

  SECTION("Sample method (bis)")
  {
    Tube tube(Interval(0.,6.), 2.);
    CHECK(tube.nbSlices() == 3);

    tube.set(Interval(-3.,2.), 0);
    tube.set(Interval(-1.,3.), 1);
    tube.set(Interval(-3.,2.), 2);
    tube.set(Interval(-3.,1.), 0.); // gate
    tube.set(Interval(0.,1.), 2.); // gate
    tube.set(Interval(-1.,1.), 4.); // gate
    tube.set(Interval(-1.,0.), 6.); // gate

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
    TubeNode tubetree(Interval(0.,1.), Interval(-1.,1.));
    tubetree.sample(0.6);
    tubetree.sample(0.7);
    tubetree.sample(0.62);
    tubetree.sample(0.1);

    // input2index
    CHECK_THROWS(tubetree.input2index(-0.1));
    CHECK(tubetree.input2index(0.) == 0);
    CHECK(tubetree.input2index(0.01) == 0);
    CHECK(tubetree.input2index(0.6) == 2);
    CHECK(tubetree.input2index(0.61) == 2);
    CHECK(tubetree.input2index(1.0) == 4);
    CHECK_THROWS(tubetree.input2index(1.01));

    // idnex2input
    CHECK_THROWS(tubetree.index2input(-1));
    CHECK(tubetree.index2input(0) == 0.);
    CHECK(tubetree.index2input(1) == 0.1);
    CHECK(tubetree.index2input(4) == 0.7);
    CHECK_THROWS(tubetree.input2index(5));
  }

  SECTION("Getting slices")
  {
    TubeNode tubetree(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tubetree.getSlice(0) == tubetree.getFirstTubeComponent());
    CHECK_THROWS(tubetree.getSlice(1));
    tubetree.sample(0.6);
    tubetree.sample(0.7);
    tubetree.sample(0.62);
    tubetree.sample(0.1);
    CHECK(tubetree.nbSlices() == 5);

    // By indexes
    CHECK_THROWS(tubetree.getSlice(-1));
    CHECK(tubetree.getSlice(0)->domain() == Interval(0.0,0.1));
    CHECK(tubetree.getSlice(1)->domain() == Interval(0.1,0.6));
    CHECK(tubetree.getSlice(2)->domain() == Interval(0.6,0.62));
    CHECK(tubetree.getSlice(3)->domain() == Interval(0.62,0.7));
    CHECK(tubetree.getSlice(4)->domain() == Interval(0.7,1.0));

    // By times
    CHECK_THROWS(tubetree.getSlice(-0.001));
    CHECK_THROWS(tubetree.getSlice(1.001));
    CHECK(tubetree.getSlice(0.)->domain() == Interval(0.0,0.1));
    CHECK(tubetree.getSlice(0.03)->domain() == Interval(0.0,0.1));
    CHECK(tubetree.getSlice(0.2)->domain() == Interval(0.1,0.6));
    CHECK(tubetree.getSlice(0.6)->domain() == Interval(0.6,0.62));
    CHECK(tubetree.getSlice(0.62)->domain() == Interval(0.62,0.7));
    CHECK(tubetree.getSlice(1.0)->domain() == Interval(0.7,1.0));

    // Copy: constructor
    TubeNode tubetree_b(tubetree);
    CHECK(tubetree_b.getSlice(0.)->domain() == Interval(0.0,0.1));
    CHECK(tubetree_b.getSlice(0.03)->domain() == Interval(0.0,0.1));
    CHECK(tubetree_b.getSlice(0.2)->domain() == Interval(0.1,0.6));
    CHECK(tubetree_b.getSlice(0.6)->domain() == Interval(0.6,0.62));
    CHECK(tubetree_b.getSlice(0.62)->domain() == Interval(0.62,0.7));
    CHECK(tubetree_b.getSlice(1.0)->domain() == Interval(0.7,1.0));
    
    // Copy: operator
    TubeNode tubetree_c = tubetree;
    CHECK(tubetree_c.getSlice(0.)->domain() == Interval(0.0,0.1));
    CHECK(tubetree_c.getSlice(0.03)->domain() == Interval(0.0,0.1));
    CHECK(tubetree_c.getSlice(0.2)->domain() == Interval(0.1,0.6));
    CHECK(tubetree_c.getSlice(0.6)->domain() == Interval(0.6,0.62));
    CHECK(tubetree_c.getSlice(0.62)->domain() == Interval(0.62,0.7));
    CHECK(tubetree_c.getSlice(1.0)->domain() == Interval(0.7,1.0));

    // First and last slices
    CHECK(tubetree.getSlice(0) == tubetree.getFirstSlice());
    CHECK(tubetree.getSlice(0) == tubetree.getFirstTubeComponent()->getFirstSlice());
    CHECK(tubetree.getSlice(1) == tubetree.getFirstTubeComponent()->getLastSlice());
    CHECK(tubetree.getSlice(4) == tubetree.getLastSlice());

    // Checking root references
    CHECK(tubetree.getSlice(0)->tubeReference() == &tubetree);
    CHECK(tubetree.getSlice(1)->tubeReference() == &tubetree);
    CHECK(tubetree.getSlice(2)->tubeReference() == &tubetree);
    CHECK(tubetree.getSlice(3)->tubeReference() == &tubetree);
    CHECK(tubetree.getSlice(4)->tubeReference() == &tubetree);

    // getSlices
    vector<const TubeSlice*> v_slices;
    tubetree.getSlices(v_slices);
    CHECK(v_slices.size() == 5);
    for(int i = 0 ; i < 5 ; i++)
      CHECK(v_slices[i] == tubetree.getSlice(i));
    CHECK(tubetree.sliceBox(0.1)[0] == Interval(0.1,0.6));
    CHECK(tubetree.sliceBox(0.1)[1] == Interval(-1.,1.));
    CHECK(tubetree.sliceDomain(0.1) == Interval(0.1,0.6));
    CHECK(tubetree.sliceBox(1)[0] == Interval(0.1,0.6));
    CHECK(tubetree.sliceBox(1)[1] == Interval(-1.,1.));
    CHECK(tubetree.sliceDomain(1) == Interval(0.1,0.6));
    CHECK(tubetree.sliceBox(4)[0] == Interval(0.7,1.0));
    CHECK(tubetree.sliceBox(4)[1] == Interval(-1.,1.));
    CHECK(tubetree.sliceDomain(4) == Interval(0.7,1.0));
    CHECK_THROWS(tubetree.sliceBox(5));

    // getTubeComponents
    vector<const TubeComponent*> v_nodes;
    tubetree.getTubeComponents(v_nodes);
    CHECK(v_nodes.size() == 9);
    CHECK(v_nodes[0] == &tubetree);
    CHECK(v_nodes[1]->domain() == Interval(0.,0.6));
    CHECK(v_nodes[2]->domain() == Interval(0.,0.1));

    // prev/next slices
    CHECK(tubetree.getSlice(0)->prevSlice() == NULL);
    for(int i = 0 ; i < tubetree.nbSlices()-1 ; i++)
    {      
      CHECK(tubetree.getSlice(i)->nextSlice() == tubetree.getSlice(i+1));
      CHECK(tubetree.getSlice(i+1)->prevSlice() == tubetree.getSlice(i));
    }
    CHECK(tubetree.getSlice(4)->nextSlice() == NULL);

    TubeNode tubetree_simple(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tubetree_simple.getSlice(0)->prevSlice() == NULL);
    CHECK(tubetree_simple.getSlice(0)->nextSlice() == NULL);
    tubetree_simple.sample(0.3);
    CHECK(tubetree_simple.getSlice(0)->nextSlice() == tubetree_simple.getSlice(1));
    CHECK(tubetree_simple.getSlice(1)->prevSlice() == tubetree_simple.getSlice(0));
  }

  SECTION("Gates")
  {
    TubeNode tubetree_a(Interval(0.,1.), Interval(-1.,1.));
    tubetree_a.set(Interval(0.), 0.);
    tubetree_a.set(Interval(0.2,0.3), 1.);
    tubetree_a.sample(0.6, Interval(0.0,0.5));

    CHECK(tubetree_a.getSlice(0)->inputGate() == 0.);
    CHECK(tubetree_a.getSlice(0)->outputGate() == Interval(0.,0.5));
    CHECK(tubetree_a.getSlice(1)->inputGate() == Interval(0.,0.5));
    CHECK(tubetree_a.getSlice(1)->outputGate() == Interval(0.2,0.3));

    tubetree_a.sample(0.7, Interval(0.2,0.3));
    tubetree_a.sample(0.62, Interval(-6.,6.)); // too large gate
    tubetree_a.sample(0.1, Interval(-1.0,-0.8));

    CHECK(tubetree_a.getSlice(0)->inputGate() == 0.);
    CHECK(tubetree_a.getSlice(0)->outputGate() == Interval(-1.,-0.8));
    CHECK(tubetree_a.getSlice(1)->inputGate() == Interval(-1.,-0.8));
    CHECK(tubetree_a.getSlice(1)->outputGate() == Interval(0.,0.5));
    CHECK(tubetree_a.getSlice(2)->inputGate() == Interval(0.,0.5));
    CHECK(tubetree_a.getSlice(2)->outputGate() == Interval(-1.,1.));
    CHECK(tubetree_a.getSlice(3)->inputGate() == Interval(-1.,1.));
    CHECK(tubetree_a.getSlice(3)->outputGate() == Interval(0.2,0.3));
    CHECK(tubetree_a.getSlice(4)->inputGate() == Interval(0.2,0.3));
    CHECK(tubetree_a.getSlice(4)->outputGate() == Interval(0.2,0.3));

    TubeNode tubetree_b(tubetree_a);
    CHECK(tubetree_b.getSlice(0)->inputGate() == 0.);
    CHECK(tubetree_b.getSlice(0)->outputGate() == Interval(-1.,-0.8));
    CHECK(tubetree_b.getSlice(1)->inputGate() == Interval(-1.,-0.8));
    CHECK(tubetree_b.getSlice(1)->outputGate() == Interval(0.,0.5));
    CHECK(tubetree_b.getSlice(2)->inputGate() == Interval(0.,0.5));
    CHECK(tubetree_b.getSlice(2)->outputGate() == Interval(-1.,1.));
    CHECK(tubetree_b.getSlice(3)->inputGate() == Interval(-1.,1.));
    CHECK(tubetree_b.getSlice(3)->outputGate() == Interval(0.2,0.3));
    CHECK(tubetree_b.getSlice(4)->inputGate() == Interval(0.2,0.3));
    CHECK(tubetree_b.getSlice(4)->outputGate() == Interval(0.2,0.3));

    TubeNode tubetree_c = tubetree_a;
    CHECK(tubetree_c.getSlice(0)->inputGate() == 0.);
    CHECK(tubetree_c.getSlice(0)->outputGate() == Interval(-1.,-0.8));
    CHECK(tubetree_c.getSlice(1)->inputGate() == Interval(-1.,-0.8));
    CHECK(tubetree_c.getSlice(1)->outputGate() == Interval(0.,0.5));
    CHECK(tubetree_c.getSlice(2)->inputGate() == Interval(0.,0.5));
    CHECK(tubetree_c.getSlice(2)->outputGate() == Interval(-1.,1.));
    CHECK(tubetree_c.getSlice(3)->inputGate() == Interval(-1.,1.));
    CHECK(tubetree_c.getSlice(3)->outputGate() == Interval(0.2,0.3));
    CHECK(tubetree_c.getSlice(4)->inputGate() == Interval(0.2,0.3));
    CHECK(tubetree_c.getSlice(4)->outputGate() == Interval(0.2,0.3));

    tubetree_a.sample(0.67); // no gate specification
    CHECK(tubetree_a.getSlice(0.67)->inputGate() == Interval(-1.,1));
    tubetree_a.sample(0.68, Interval::ALL_REALS);
    CHECK(tubetree_a.getSlice(0.68)->inputGate() == Interval(-1.,1));
    tubetree_a.sample(0.69, Interval::EMPTY_SET);
    CHECK(tubetree_a.getSlice(0.69)->inputGate() == Interval::EMPTY_SET);

    tubetree_a.getSlice(0.69)->setInputGate(Interval(5.));
    CHECK(tubetree_a.getSlice(0.69)->inputGate().is_empty());
    CHECK(tubetree_a[0.69].is_empty());

    tubetree_a.set(Interval(0.1), 0.);
    CHECK(tubetree_a.getSlice(0)->inputGate() == Interval(0.1));
    CHECK(tubetree_a[0.] == Interval(0.1));

    tubetree_a.set(Interval(0.3), 1.);
    CHECK(tubetree_a.getSlice(tubetree_a.nbSlices()-1)->outputGate() == Interval(0.3));
    CHECK(tubetree_a[1.] == Interval(0.3));
  }
}
