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
  // todo: backward

  CtcPicard::CtcPicard(float delta, bool preserve_sampling)
    : m_delta(delta), m_preserve_sampling(preserve_sampling)
  {

  }
  
  bool CtcPicard::contract_fwd(const tubex::Fnc& f, TubeVector& x) const
  {
    // todo: DimensionException::check(x, f);
    return contract(f, x, true);
  }
  
  bool CtcPicard::contract_bwd(const tubex::Fnc& f, TubeVector& x) const
  {
    // todo: DimensionException::check(x, f);
    return contract(f, x, false);
  }
  
  bool CtcPicard::contract(const tubex::Fnc& f, TubeVector& x, bool fwd) const
  {
    // todo: DimensionException::check(x, f);
    bool ctc = false;
    TubeVector *x_ptr;

    // todo: if(m_preserve_sampling)
    // todo:   x_ptr = new TubeVector(x);
    // todo: 
    // todo: else
      x_ptr = &x;
    
    TubeSlice *slice_x;

    if(fwd) slice_x = x_ptr->getFirstSlice();
    else slice_x = x_ptr->getLastSlice();

    while(slice_x != NULL)
    {
      if(fwd) ctc |= contract_fwd(f, x, *slice_x);
      else ctc |= contract_bwd(f, x, *slice_x);

      bool unbounded_slice = slice_x->codomain().is_unbounded();

      if(unbounded_slice && slice_x->domain().diam() > x_ptr->domain().diam() / 5000.)
      {
        TubeSlice *prev_slice_x;
        if(fwd) prev_slice_x = slice_x->prevSlice();
        else prev_slice_x = slice_x->nextSlice();

        x_ptr->sample(slice_x->domain().mid());

        if(prev_slice_x == NULL)
        {
          if(fwd) slice_x = x_ptr->getFirstSlice();
          else slice_x = x_ptr->getLastSlice();
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

    if(m_preserve_sampling)
    {
      // todo: double lb = x.domain().lb(), ub = x.domain().ub();
      // todo: x.set(x.codomain() & x_ptr->codomain());
      // todo: x.set(x[lb] & (*x_ptr)[lb], lb);
      // todo: x.set(x[ub] & (*x_ptr)[ub], ub);
      // todo: delete x_ptr;
    }

    return ctc;
  }

  bool CtcPicard::contract_fwd(const tubex::Fnc& f, const TubeVector& tube, TubeSlice& slice) const
  {
    // todo: DimensionException::check(x, f);
    guessSliceEnvelope(f, tube, slice, true);
    slice.setOutputGate(slice.outputGate()
      & (slice.inputGate() + slice.domain().diam() * f.eval(slice.domain(), tube)));

    // todo: return ctc;
  }

  bool CtcPicard::contract_bwd(const tubex::Fnc& f, const TubeVector& tube, TubeSlice& slice) const
  {
    // todo: DimensionException::check(x, f);
    guessSliceEnvelope(f, tube, slice, false);
    slice.setInputGate(slice.inputGate()
      & (slice.outputGate() - slice.domain().diam() * f.eval(slice.domain(), tube)));

    // todo: return ctc;
  }

  int CtcPicard::picardIterations() const
  {
    return m_picard_iterations;
  }

  bool CtcPicard::guessSliceEnvelope(const tubex::Fnc& f,
                                     const TubeVector& tube,
                                     TubeSlice& slice,
                                     bool fwd) const
  {
    float delta = m_delta;
    Interval h, t = slice.domain();
    IntervalVector x(slice.codomain()), x0(tube.dim()), xf(x0), max_envelope = slice.codomain();

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
      x_guess = x_enclosure & max_envelope;

      for(int i = 0 ; i < x_guess.size() ; i++)
        x_guess[i] = x_guess[i].mid()
                   + delta * (x_guess[i] - x_guess[i].mid())
                   + Interval(-EPSILON,EPSILON); // in case of a degenerate box

      slice.setEnvelope(x_guess);
      x_enclosure = x0 + h * f.eval(t, tube);

      // eval(f, t, h, x_guess, x0);

      if(x_enclosure.is_unbounded())
      {
        slice.setEnvelope(x); // coming back to the initial state
        break;
      }

    } while(!x_enclosure.is_interior_subset(x_guess));

    // Restoring ending gate, contracted by setting the envelope
    if(fwd) slice.setOutputGate(xf);
    else slice.setInputGate(x0);
    //return ctc; // todo: return value
  }

  /*const IntervalVector CtcPicard::eval(const tubex::Fnc& f,
                                       const Interval& t,
                                       const Interval& h,
                                       const IntervalVector& x,
                                       const IntervalVector& x0,
                                       int order) const
  {
    switch(order)
    {
      case 1:
        return x0 + h * f.eval(t, x);

      //case 2: // not implemented yet for tubex::Function objects
      //  return x0
      //    + intv_h * f.eval_vector(x0)
      //    + pow(intv_h,2)/2. * f.jacobian(x) * f.eval_vector(x);

      default:
        throw Exception("CtcPicard::eval", "undefined evaluation order");
    }
  }*/
}