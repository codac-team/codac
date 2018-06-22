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

#define EPSILON std::numeric_limits<float>::epsilon()

namespace tubex
{
  // todo: backward

  CtcPicard::CtcPicard(float delta) : m_delta(delta)
  {

  }
  
  bool CtcPicard::contract(const Function& f, TubeVector& x)
  {
    bool ctc = false;
    TubeSlice* slice_x = x.getFirstSlice();

    while(slice_x != NULL)
    {
      ctc |= contract_fwd(f, *slice_x);
      ctc |= contract_bwd(f, *slice_x);

      bool unbounded_slice = slice_x->codomain().is_unbounded();

      if(unbounded_slice && slice_x->domain().diam() > x.domain().diam() / 5000.)
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

  bool CtcPicard::contract_fwd(const Function& f, TubeSlice& x)
  {
    // todo: optimal approach with polygons?

    IntervalVector intv_x = x.codomain(), intv_x0 = x.inputGate(), intv_xf = x.outputGate();
    bool ctc = contract(f, intv_x, intv_x0, Interval(0., x.domain().diam()));
    x.setEnvelope(intv_x);
    x.setOutputGate(x.inputGate() + x.domain().diam() * f.eval_vector(intv_x));
    return ctc;
  }

  bool CtcPicard::contract_bwd(const Function& f, TubeSlice& x)
  {
    // todo: optimal approach with polygons?

    IntervalVector intv_x = x.codomain(), intv_x0 = x.inputGate(), intv_xf = x.outputGate();
    bool ctc = contract(f, intv_x, intv_xf, -Interval(0., x.domain().diam()));
    x.setEnvelope(intv_x);
    x.setOutputGate(x.outputGate() - x.domain().diam() * f.eval_vector(intv_x));
    return ctc;
  }

  bool CtcPicard::contract(const Function& f,
                           IntervalVector& x, const IntervalVector& x0,
                           const Interval& h)
  {
    float delta = m_delta;
    IntervalVector x_guess = x0, x_enclosure(x.size());

    do
    {
      for(int i = 0 ; i < x_guess.size() ; i++)
        x_guess[i] = x_guess[i].mid()
                   + delta * (x_guess[i] - x_guess[i].mid())
                   + Interval(-EPSILON,EPSILON); // in case of degenerate box

      x_enclosure = x0 + h * f.eval_vector(x_guess);

      if(x_enclosure.is_unbounded())
        return false;

    } while(!x_enclosure.is_strict_interior_subset(x_guess));

    bool ctc = x != x_enclosure;
    x = x_enclosure;
    return ctc;
  }
}