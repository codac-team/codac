#include <cstdio>
#include "catch_interval.hpp"

#include "ibex_Sep.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_QInter.h"
#include <codac_SepBox.h>
#include "codac_QInterProjF.h"


using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

TEST_CASE("QIntersection tests")
{
  SECTION("qinter_projf")
  {

    std::vector<IntervalVector> boxes = {
        IntervalVector({Interval(1, 5), Interval(1, 6)}),
        IntervalVector({Interval(3, 7), Interval(2, 5.5)}),
        IntervalVector({Interval(1.5, 2.5), Interval(4.5, 8)}),
        IntervalVector({Interval(2.5, 5), Interval(0, 4)}),
        IntervalVector({Interval(1.5, 8), Interval(1.5, 3)}),
    };

    ibex::Array<IntervalVector> array_boxes(boxes.size());
    for (size_t i = 0; i < boxes.size(); i++)
    {
      array_boxes.set_ref(i, boxes[i]);
    }
    {
      IntervalVector res = qinter_projf(array_boxes, 1);
      CHECK(res == IntervalVector({Interval(1, 8), Interval(0, 8)}));
    }

    {
      IntervalVector res = qinter_projf(array_boxes, 2);
      CHECK(res == IntervalVector({Interval(1.5, 7), Interval(1, 6)}));
    }

    {
      IntervalVector res = qinter_projf(array_boxes, 3);
      CHECK(res == IntervalVector({Interval(1.5, 5), Interval(1.5, 5.5)}));
    }

    {
      IntervalVector res = qinter_projf(array_boxes, 4);
      CHECK(res == IntervalVector({Interval(2.5, 5), Interval(2, 3)}));
    }
  }

  SECTION("qinter")
  {

    std::vector<IntervalVector> boxes = {
        IntervalVector({Interval(1, 5), Interval(1, 6)}),
        IntervalVector({Interval(3, 7), Interval(2, 5.5)}),
        IntervalVector({Interval(1.5, 2.5), Interval(4.5, 8)}),
        IntervalVector({Interval(2.5, 5), Interval(0, 4)}),
        IntervalVector({Interval(1.5, 8), Interval(1.5, 3)}),
    };

    ibex::Array<IntervalVector> array_boxes(boxes.size());
    for (size_t i = 0; i < boxes.size(); i++)
    {
      array_boxes.set_ref(i, boxes[i]);
    }
    {
      IntervalVector res = qinter(array_boxes, 1);
      CHECK(res == IntervalVector({Interval(1, 8), Interval(0, 8)}));
    }

    {
      IntervalVector res = qinter(array_boxes, 2);
      CHECK(res == IntervalVector({Interval(1.5, 7), Interval(1, 6)}));
    }

    {
      IntervalVector res = qinter(array_boxes, 3);
      CHECK(res == IntervalVector({Interval(2.5, 5), Interval(1.5, 4)}));
    }

    {
      IntervalVector res = qinter(array_boxes, 4);
      CHECK(res == IntervalVector({Interval(3, 5), Interval(2, 3)}));
    }
  }
}

TEST_CASE("SepQInterProjF")
{

  std::vector<SepBox> seps = {
      SepBox(IntervalVector({Interval(3, 7), Interval(2, 5.5)})),
      SepBox(IntervalVector({Interval(1, 5), Interval(1, 6)})),
      SepBox(IntervalVector({Interval(1.5, 2.5), Interval(4.5, 8)})),
      SepBox(IntervalVector({Interval(2.5, 5), Interval(0, 4)})),
      SepBox(IntervalVector({Interval(1.5, 8), Interval(1.5, 3)})),
  };

  ibex::Array<Sep> array_sep(seps.size());
  for (size_t i = 0; i < seps.size(); i++)
  {
    array_sep.set_ref(i, seps[i]);
  }

  SECTION("SepQInterProjF")
  {

    SepQInterProjF S_proj(array_sep, 4);
    // SepQInter S(array_sep, 4);

    // {
    IntervalVector X0 = IntervalVector({3, 4}).inflate(1);
    IntervalVector xin(X0), xout(X0);
    S_proj.separate(xin, xout);
    CHECK(xin.is_empty());
    CHECK(xout == X0);
  }
}

TEST_CASE("CtcQInterProjF")
{

  Function f("x", "y", "(x, y)");
  CtcFwdBwd ctc1(f, (IntervalVector({Interval(3, 7), Interval(2, 5.5)})));
  CtcFwdBwd ctc2(f, (IntervalVector({Interval(1, 5), Interval(1, 6)})));
  CtcFwdBwd ctc3(f, (IntervalVector({Interval(1.5, 2.5), Interval(4.5, 8)})));
  CtcFwdBwd ctc4(f, (IntervalVector({Interval(2.5, 5), Interval(0, 4)})));
  CtcFwdBwd ctc5(f, (IntervalVector({Interval(1.5, 8), Interval(1.5, 3)})));
  ibex::Array<Ctc> array_ctc(ctc1, ctc2, ctc3, ctc4, ctc5);

  // std::vector<CtcFwdBwd *> ctcs = {
  //     new CtcFwdBwd(f, IntervalVector({Interval(3, 7), Interval(2, 5.5)})),
  //     new CtcFwdBwd(f, IntervalVector({Interval(1, 5), Interval(1, 6)})),
  //     new CtcFwdBwd(f, IntervalVector({Interval(1.5, 2.5), Interval(4.5, 8)})),
  //     new CtcFwdBwd(f, IntervalVector({Interval(2.5, 5), Interval(0, 4)})),
  //     new CtcFwdBwd(f, IntervalVector({Interval(1.5, 8), Interval(1.5, 3)})),
  // };
  // ibex::Array<Ctc> array_ctc(ctcs.size());
  // for (int i = 0; i < ctcs.size(); i++)
  // {
  //   array_ctc.set_ref(i, *ctcs[i]);
  // }

  SECTION("CtcQInterProjF")
  {
    CtcQInterProjF C_proj(array_ctc, 2);
    IntervalVector X0 = IntervalVector({0, 1}).inflate(0.5);
    IntervalVector x(X0);
    C_proj.contract(x);
    CHECK(x.is_empty());
  }
}