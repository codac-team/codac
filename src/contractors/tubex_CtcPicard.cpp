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
#include "tubex_DimensionException.h"

using namespace std;
using namespace ibex;

#define EPSILON std::numeric_limits<float>::epsilon()

namespace tubex
{
  CtcPicard::CtcPicard(float delta) : m_delta(delta)
  {

  }
  
  void CtcPicard::contract_fwd(const tubex::Fnc& f, TubeVector& x) const
  {
    DimensionException::check(x, f);
    contract(f, x, true);
  }
  
  void CtcPicard::contract_bwd(const tubex::Fnc& f, TubeVector& x) const
  {
    DimensionException::check(x, f);
    contract(f, x, false);
  }
  
  bool CtcPicard::contract(const tubex::Fnc& f, TubeVector& x, bool fwd) const
  {
    DimensionException::check(x, f);
    TubeSlice *slice_x;

    if(fwd) slice_x = x.getFirstSlice();
    else slice_x = x.getLastSlice();

    while(slice_x != NULL)
    {
      if(fwd) contract_fwd(f, x, *slice_x);
      else contract_bwd(f, x, *slice_x);

      bool unbounded_slice = slice_x->codomain().is_unbounded();

      if(unbounded_slice && slice_x->domain().diam() > x.domain().diam() / 5000.)
      {
        TubeSlice *prev_slice_x;
        if(fwd) prev_slice_x = slice_x->prevSlice();
        else prev_slice_x = slice_x->nextSlice();

        x.sample(slice_x->domain().mid());

        if(prev_slice_x == NULL)
        {
          if(fwd) slice_x = x.getFirstSlice();
          else slice_x = x.getLastSlice();
        }

        else
        {
          if(fwd) slice_x = prev_slice_x->nextSlice();
          else slice_x = prev_slice_x->prevSlice();
        }

        continue;
      }

      if(fwd) slice_x = slice_x->nextSlice();
      else slice_x = slice_x->prevSlice();
    }

    // todo: return value
  }

  void CtcPicard::contract_fwd(const tubex::Fnc& f, const TubeVector& tube, TubeSlice& slice) const
  {
    DimensionException::check(tube, f);
    DimensionException::check(tube, slice);

    guessSliceEnvelope(f, tube, slice, true);
    slice.setOutputGate(slice.outputGate()
      & (slice.inputGate() + slice.domain().diam() * f.eval(slice.domain(), tube)));
  }

  void CtcPicard::contract_bwd(const tubex::Fnc& f, const TubeVector& tube, TubeSlice& slice) const
  {
    DimensionException::check(tube, f);
    DimensionException::check(tube, slice);

    guessSliceEnvelope(f, tube, slice, false);
    slice.setInputGate(slice.inputGate()
      & (slice.outputGate() - slice.domain().diam() * f.eval(slice.domain(), tube)));
  }

  int CtcPicard::picardIterations() const
  {
    return m_picard_iterations;
  }

  void CtcPicard::guessSliceEnvelope(const tubex::Fnc& f,
                                     const TubeVector& tube,
                                     TubeSlice& slice,
                                     bool fwd) const
  {
    DimensionException::check(tube, f);
    DimensionException::check(tube, slice);

    float delta = m_delta;
    Interval h, t = slice.domain();
    IntervalVector initial_x(slice.codomain()), x0(tube.dim()), xf(x0);

    if(fwd)
    {
      x0 = slice.inputGate();
      xf = slice.outputGate();
      h = Interval(0., t.diam());
    }

    else
    {
      x0 = slice.outputGate();
      xf = slice.inputGate();
      h = Interval(-t.diam(), 0.);
    }

    IntervalVector x_guess(tube.dim()), x_enclosure = x0;
    m_picard_iterations = 0;

    do
    {
      m_picard_iterations++;
      x_guess = x_enclosure;

      for(int i = 0 ; i < x_guess.size() ; i++)
        x_guess[i] = x_guess[i].mid()
                   + delta * (x_guess[i] - x_guess[i].mid())
                   + Interval(-EPSILON,EPSILON); // in case of a degenerate box

      slice.setEnvelope(x_guess & initial_x);
      x_enclosure = x0 + h * f.eval(t, tube);

      if(x_enclosure.is_unbounded())
      {
        slice.setEnvelope(initial_x); // coming back to the initial state
        break;
      }

    } while(!x_enclosure.is_interior_subset(x_guess));

    // Restoring ending gate, contracted by setting the envelope
    if(fwd) slice.setOutputGate(xf);
    else slice.setInputGate(xf);
  }
}