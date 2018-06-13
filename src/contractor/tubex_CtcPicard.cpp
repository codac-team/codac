/* ============================================================================
 *  tubex-lib - CtcHC4 class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_CtcPicard.h"
#include "tubex_StructureException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // todo: backward

  CtcPicard::CtcPicard(float delta) : m_delta(delta)
  {

  }

  bool CtcPicard::contract(const Function& f, Tube& x)
  {
    bool ctc = false;
    TubeSlice *slice_x = x.getFirstSlice();

    while(slice_x != NULL)
    {
      ctc |= contract(f, *slice_x);
      if(slice_x->codomain().is_unbounded()) // Picard failed
      {
        TubeSlice *prev_slice_x = slice_x->prevSlice();
        x.sample(slice_x->domain().mid());

        if(prev_slice_x == NULL)
          slice_x = x.getFirstSlice();

        else
          slice_x = prev_slice_x->nextSlice();
        continue;
      }

      slice_x = slice_x->nextSlice();
    }

    return ctc;
  }

  bool CtcPicard::contract(const Function& f, Tube& x, Tube& xdot)
  {
    StructureException::check(x, xdot);
    bool ctc = false;
    TubeSlice *slice_x = x.getFirstSlice(), *slice_xdot = xdot.getFirstSlice();

    while(slice_x != NULL)
    {
      ctc |= contract(f, *slice_x, *slice_xdot);
      if(slice_x->codomain().is_unbounded()) // Picard failed
      {
        TubeSlice *prev_slice_x = slice_x->prevSlice();
        TubeSlice *prev_slice_xdot = slice_xdot->prevSlice();

        x.sample(slice_x->domain().mid());
        xdot.sample(slice_x->domain().mid());

        if(prev_slice_x == NULL)
        {
          slice_x = x.getFirstSlice();
          slice_xdot = xdot.getFirstSlice();
        }

        else
        {
          slice_x = prev_slice_x->nextSlice();
          slice_xdot = prev_slice_xdot->nextSlice();
        }

        continue;
      }

      slice_x = slice_x->nextSlice();
      slice_xdot = slice_xdot->nextSlice();
    }

    return ctc;
  }

  bool CtcPicard::contract(const Function& f, TubeSlice& x)
  {
    TubeSlice xdot_unbounded(x);
    xdot_unbounded.set(Interval::ALL_REALS);
    return contract(f, x, xdot_unbounded);
  }

  bool CtcPicard::contract(const Function& f, TubeSlice& x, const TubeSlice& xdot)
  {
    bool ctc = false;
    Interval intv_x = x.codomain();
    ctc |= contract(f, intv_x, x.inputGate(), Interval(0., x.domain().diam()), xdot.codomain());
    x.setEnvelope(intv_x);
    intv_x = x.outputGate();
    ctc |= contract(f, intv_x, x.inputGate(), Interval(x.domain().diam()), xdot.codomain());
    x.setOutputGate(intv_x);
    return ctc;
  }

  bool CtcPicard::contract(const Function& f, Interval& x, const Interval& x0, const Interval& h, const Interval& xdot)
  {
    float delta = m_delta;
    float epsilon = std::numeric_limits<float>::epsilon();
    Interval guess = x0, new_x = x;

    do
    {
      guess = guess.mid() + delta * (guess - guess.mid()) + Interval(-epsilon,epsilon);
      IntervalVector box(1, guess);
      new_x = x0 + h * (f.eval(box) & xdot);
      if(new_x.is_unbounded())
        return false;
    } while(!new_x.is_strict_interior_subset(guess));

    bool ctc = x != new_x;
    x = new_x;
    return ctc;
  }
}