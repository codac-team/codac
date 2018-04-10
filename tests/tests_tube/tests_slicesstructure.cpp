#include "../catch/catch.hpp"
#include "../catch/catch_interval.hpp"
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

  SECTION("TubeTree class")
  {
    TubeTree tubetree_a(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tubetree_a.isSlice() == false);
    CHECK(tubetree_a.getFirstTubeNode() != NULL);
    CHECK(tubetree_a.getFirstTubeNode()->isSlice());
    CHECK(tubetree_a.getFirstTubeNode()->codomain() == Interval(-1.,1.));
    CHECK(tubetree_a.getSecondTubeNode() == NULL);
    CHECK(tubetree_a.nbSlices() == 1);
    CHECK(tubetree_a.getFirstTubeNode()->nbSlices() == 1);

    TubeTree tubetree_b(tubetree_a);
    CHECK(tubetree_b.isSlice() == false);
    CHECK(tubetree_b.getFirstTubeNode() != NULL);
    CHECK(tubetree_b.getFirstTubeNode()->isSlice());
    CHECK(tubetree_b.getFirstTubeNode()->codomain() == Interval(-1.,1.));
    CHECK(tubetree_b.getSecondTubeNode() == NULL);
    CHECK(tubetree_b.nbSlices() == 1);
    CHECK(tubetree_b.getFirstTubeNode()->nbSlices() == 1);

    TubeTree tubetree_c = tubetree_a;
    CHECK(tubetree_c.isSlice() == false);
    CHECK(tubetree_c.getFirstTubeNode() != NULL);
    CHECK(tubetree_c.getFirstTubeNode()->isSlice());
    CHECK(tubetree_c.getFirstTubeNode()->codomain() == Interval(-1.,1.));
    CHECK(tubetree_c.getSecondTubeNode() == NULL);
    CHECK(tubetree_c.nbSlices() == 1);
    CHECK(tubetree_c.getFirstTubeNode()->nbSlices() == 1);
  }

  SECTION("Sample method")
  {
    TubeTree tubetree_a(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tubetree_a.nbSlices() == 1);
    CHECK(tubetree_a.getFirstTubeNode()->domain() == Interval(0.,1.));
    CHECK(tubetree_a.getSecondTubeNode() == NULL);

    CHECK_THROWS(tubetree_a.sample(-1.0););

    tubetree_a.sample(0.); // the following has no effect
    CHECK(tubetree_a.nbSlices() == 1);
    tubetree_a.sample(1.); // the following has no effect
    CHECK(tubetree_a.nbSlices() == 1);

    tubetree_a.sample(0.6);

    {
      TubeTree *tubetree_b1 = (TubeTree*)tubetree_a.getFirstTubeNode();
      TubeTree *tubetree_b2 = (TubeTree*)tubetree_a.getSecondTubeNode();

      CHECK(tubetree_a.nbSlices() == 2);
      CHECK(tubetree_b1->nbSlices() == 1);
      CHECK(tubetree_b2->nbSlices() == 1);
      CHECK(tubetree_b1->domain() == Interval(0.,0.6));
      CHECK(tubetree_b2->domain() == Interval(0.6,1.));
      CHECK_THROWS(tubetree_b1->sample(0.7););
    }

    tubetree_a.sample(0.7);

    {
      TubeTree *tubetree_b1 = (TubeTree*)tubetree_a.getFirstTubeNode();
      TubeTree *tubetree_b2 = (TubeTree*)tubetree_a.getSecondTubeNode();
      TubeTree *tubetree_c1 = (TubeTree*)tubetree_b2->getFirstTubeNode();
      TubeTree *tubetree_c2 = (TubeTree*)tubetree_b2->getSecondTubeNode();

      CHECK(tubetree_a.nbSlices() == 3);
      CHECK(tubetree_b1->nbSlices() == 1);
      CHECK(tubetree_b1->domain() == Interval(0.,0.6));
      CHECK(tubetree_b2->nbSlices() == 2);
      CHECK(tubetree_b2->domain() == Interval(0.6,1.));
      CHECK(tubetree_c1->nbSlices() == 1);
      CHECK(tubetree_c2->nbSlices() == 1);
      CHECK(tubetree_c1->domain() == Interval(0.6,0.7));
      CHECK(tubetree_c2->domain() == Interval(0.7,1.0));
    }

    tubetree_a.sample(0.7); // the following has no more effect
    tubetree_a.sample(0.7);
    CHECK(tubetree_a.nbSlices() == 3);
    tubetree_a.sample(0.62);

    {
      TubeTree *tubetree_b1 = (TubeTree*)tubetree_a.getFirstTubeNode();
      TubeTree *tubetree_b2 = (TubeTree*)tubetree_a.getSecondTubeNode();
      TubeTree *tubetree_c1 = (TubeTree*)tubetree_b2->getFirstTubeNode();
      TubeTree *tubetree_c2 = (TubeTree*)tubetree_b2->getSecondTubeNode();
      TubeTree *tubetree_d1 = (TubeTree*)tubetree_c1->getFirstTubeNode();
      TubeTree *tubetree_d2 = (TubeTree*)tubetree_c1->getSecondTubeNode();

      CHECK(tubetree_a.nbSlices() == 4);
      CHECK(tubetree_b1->nbSlices() == 1);
      CHECK(tubetree_b1->domain() == Interval(0.,0.6));
      CHECK(tubetree_b2->nbSlices() == 3);
      CHECK(tubetree_b2->domain() == Interval(0.6,1.));
      CHECK(tubetree_c1->nbSlices() == 2);
      CHECK(tubetree_c2->nbSlices() == 1);
      CHECK(tubetree_c1->domain() == Interval(0.6,0.7));
      CHECK(tubetree_c2->domain() == Interval(0.7,1.0));
      CHECK(tubetree_d1->nbSlices() == 1);
      CHECK(tubetree_d2->nbSlices() == 1);
      CHECK(tubetree_d1->domain() == Interval(0.6,0.62));
      CHECK(tubetree_d2->domain() == Interval(0.62,0.7));
    }

    tubetree_a.sample(0.1);
    tubetree_a.sample(0.1); // the following has no more effect

    {
      TubeTree *tubetree_b1 = (TubeTree*)tubetree_a.getFirstTubeNode();
      TubeTree *tubetree_b2 = (TubeTree*)tubetree_a.getSecondTubeNode();
      TubeTree *tubetree_e1 = (TubeTree*)tubetree_b1->getFirstTubeNode();
      TubeTree *tubetree_e2 = (TubeTree*)tubetree_b1->getSecondTubeNode();

      CHECK(tubetree_a.nbSlices() == 5);
      CHECK(tubetree_b1->nbSlices() == 2);
      CHECK(tubetree_b1->domain() == Interval(0.,0.6));
      CHECK(tubetree_b2->nbSlices() == 3);
      CHECK(tubetree_b2->domain() == Interval(0.6,1.));
      CHECK(tubetree_e1->nbSlices() == 1);
      CHECK(tubetree_e2->nbSlices() == 1);
      CHECK(tubetree_e1->domain() == Interval(0.0,0.1));
      CHECK(tubetree_e2->domain() == Interval(0.1,0.6));
    }

    tubetree_a.sample(0.); // the following has no effect
    CHECK(tubetree_a.nbSlices() == 5);
    tubetree_a.sample(1.); // the following has no effect
    CHECK(tubetree_a.nbSlices() == 5);
  }

  SECTION("input2index and reverse operation")
  {
    TubeTree tubetree(Interval(0.,1.), Interval(-1.,1.));
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
    TubeTree tubetree(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tubetree.getSlice(0) == tubetree.getFirstTubeNode());
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
    TubeTree tubetree_b(tubetree);
    CHECK(tubetree_b.getSlice(0.)->domain() == Interval(0.0,0.1));
    CHECK(tubetree_b.getSlice(0.03)->domain() == Interval(0.0,0.1));
    CHECK(tubetree_b.getSlice(0.2)->domain() == Interval(0.1,0.6));
    CHECK(tubetree_b.getSlice(0.6)->domain() == Interval(0.6,0.62));
    CHECK(tubetree_b.getSlice(0.62)->domain() == Interval(0.62,0.7));
    CHECK(tubetree_b.getSlice(1.0)->domain() == Interval(0.7,1.0));
    
    // Copy: operator
    TubeTree tubetree_c = tubetree;
    CHECK(tubetree_c.getSlice(0.)->domain() == Interval(0.0,0.1));
    CHECK(tubetree_c.getSlice(0.03)->domain() == Interval(0.0,0.1));
    CHECK(tubetree_c.getSlice(0.2)->domain() == Interval(0.1,0.6));
    CHECK(tubetree_c.getSlice(0.6)->domain() == Interval(0.6,0.62));
    CHECK(tubetree_c.getSlice(0.62)->domain() == Interval(0.62,0.7));
    CHECK(tubetree_c.getSlice(1.0)->domain() == Interval(0.7,1.0));

    // First and last slices
    CHECK(tubetree.getSlice(0) == tubetree.getFirstSlice());
    CHECK(tubetree.getSlice(0) == tubetree.getFirstTubeNode()->getFirstSlice());
    CHECK(tubetree.getSlice(1) == tubetree.getFirstTubeNode()->getLastSlice());
    CHECK(tubetree.getSlice(4) == tubetree.getLastSlice());

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

    // getTubeNodes
    vector<const TubeNode*> v_nodes;
    tubetree.getTubeNodes(v_nodes);
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

    TubeTree tubetree_simple(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tubetree_simple.getSlice(0)->prevSlice() == NULL);
    CHECK(tubetree_simple.getSlice(0)->nextSlice() == NULL);
    tubetree_simple.sample(0.3);
    CHECK(tubetree_simple.getSlice(0)->nextSlice() == tubetree_simple.getSlice(1));
    CHECK(tubetree_simple.getSlice(1)->prevSlice() == tubetree_simple.getSlice(0));
  }

  SECTION("Gates")
  {
    TubeTree tubetree_a(Interval(0.,1.), Interval(-1.,1.));
    tubetree_a.sample(0.6, Interval(0.0,0.5));
    tubetree_a.sample(0.7, Interval(0.2,0.3));
    tubetree_a.sample(0.62, Interval(-6.,6.)); // too large gate
    tubetree_a.sample(0.1, Interval(-1.0,-0.8));
    CHECK(tubetree_a.getSlice(0)->inputGate() == Interval(-1.,1.));
    CHECK(tubetree_a.getSlice(0)->outputGate() == Interval(-1.,-0.8));
    CHECK(tubetree_a.getSlice(1)->inputGate() == Interval(-1.,-0.8));
    CHECK(tubetree_a.getSlice(1)->outputGate() == Interval(0.,0.5));
    CHECK(tubetree_a.getSlice(2)->inputGate() == Interval(0.,0.5));
    CHECK(tubetree_a.getSlice(2)->outputGate() == Interval(-1.,1.));
    CHECK(tubetree_a.getSlice(3)->inputGate() == Interval(-1.,1.));
    CHECK(tubetree_a.getSlice(3)->outputGate() == Interval(0.2,0.3));
    CHECK(tubetree_a.getSlice(4)->inputGate() == Interval(0.2,0.3));
    CHECK(tubetree_a.getSlice(4)->outputGate() == Interval(-1.,1));

    TubeTree tubetree_b(tubetree_a);
    CHECK(tubetree_b.getSlice(0)->inputGate() == Interval(-1.,1.));
    CHECK(tubetree_b.getSlice(0)->outputGate() == Interval(-1.,-0.8));
    CHECK(tubetree_b.getSlice(1)->inputGate() == Interval(-1.,-0.8));
    CHECK(tubetree_b.getSlice(1)->outputGate() == Interval(0.,0.5));
    CHECK(tubetree_b.getSlice(2)->inputGate() == Interval(0.,0.5));
    CHECK(tubetree_b.getSlice(2)->outputGate() == Interval(-1.,1.));
    CHECK(tubetree_b.getSlice(3)->inputGate() == Interval(-1.,1.));
    CHECK(tubetree_b.getSlice(3)->outputGate() == Interval(0.2,0.3));
    CHECK(tubetree_b.getSlice(4)->inputGate() == Interval(0.2,0.3));
    CHECK(tubetree_b.getSlice(4)->outputGate() == Interval(-1.,1));

    TubeTree tubetree_c = tubetree_a;
    CHECK(tubetree_c.getSlice(0)->inputGate() == Interval(-1.,1.));
    CHECK(tubetree_c.getSlice(0)->outputGate() == Interval(-1.,-0.8));
    CHECK(tubetree_c.getSlice(1)->inputGate() == Interval(-1.,-0.8));
    CHECK(tubetree_c.getSlice(1)->outputGate() == Interval(0.,0.5));
    CHECK(tubetree_c.getSlice(2)->inputGate() == Interval(0.,0.5));
    CHECK(tubetree_c.getSlice(2)->outputGate() == Interval(-1.,1.));
    CHECK(tubetree_c.getSlice(3)->inputGate() == Interval(-1.,1.));
    CHECK(tubetree_c.getSlice(3)->outputGate() == Interval(0.2,0.3));
    CHECK(tubetree_c.getSlice(4)->inputGate() == Interval(0.2,0.3));
    CHECK(tubetree_c.getSlice(4)->outputGate() == Interval(-1.,1));

    tubetree_a.sample(0.67); // no gate specification
    CHECK(tubetree_a.getSlice(0.67)->inputGate() == Interval(-1.,1));
    tubetree_a.sample(0.68, Interval::ALL_REALS);
    CHECK(tubetree_a.getSlice(0.68)->inputGate() == Interval(-1.,1));
    tubetree_a.sample(0.69, Interval::EMPTY_SET);
    CHECK(tubetree_a.getSlice(0.69)->inputGate() == Interval::EMPTY_SET);

    tubetree_a.getSlice(0.69)->setInputGate(Interval(5.));
    CHECK(tubetree_a.getSlice(0.69)->inputGate() == Interval(5.));
    CHECK(tubetree_a[0.69] == Interval(5.));

    tubetree_a.set(Interval(0.), 0.);
    CHECK(tubetree_a.getSlice(0)->inputGate() == Interval(0.));
    CHECK(tubetree_a[0.] == Interval(0.));

    tubetree_a.set(Interval(0.3), 1.);
    CHECK(tubetree_a.getSlice(tubetree_a.nbSlices()-1)->outputGate() == Interval(0.3));
    CHECK(tubetree_a[1.] == Interval(0.3));
  }
}
