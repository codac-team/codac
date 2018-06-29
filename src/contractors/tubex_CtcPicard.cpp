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
  
  bool CtcPicard::contract_fwd(const Function& f, TubeVector& x)
  {
    DimensionException::check(x, f);
    return contract(f, x, true);
  }
  
  bool CtcPicard::contract_bwd(const Function& f, TubeVector& x)
  {
    DimensionException::check(x, f);
    return contract(f, x, false);
  }
  
  bool CtcPicard::contract(const Function& f, TubeVector& x, bool fwd)
  {
    DimensionException::check(x, f);
    bool ctc = false;
    TubeVector *x_ptr;

    if(m_preserve_sampling)
      x_ptr = new TubeVector(x);

    else
      x_ptr = &x;
    
    TubeSlice *slice_x;

    if(fwd) slice_x = x_ptr->getFirstSlice();
    else slice_x = x_ptr->getLastSlice();

    while(slice_x != NULL)
    {
      if(fwd) ctc |= contract_fwd(f, *slice_x);
      else ctc |= contract_bwd(f, *slice_x);

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
      double lb = x.domain().lb(), ub = x.domain().ub();
      x.set(x.codomain() & x_ptr->codomain());
      x.set(x[lb] & (*x_ptr)[lb], lb);
      x.set(x[ub] & (*x_ptr)[ub], ub);
      delete x_ptr;
    }

    return ctc;
  }

  bool CtcPicard::contract_fwd(const Function& f, TubeSlice& x)
  {
    DimensionException::check(x, f);
    double h = x.domain().diam();
    IntervalVector intv_x = x.codomain(), intv_x0 = x.inputGate();
    bool ctc = contract(f, intv_x, intv_x0, h);
    x.setEnvelope(intv_x);
    x.setOutputGate(x.outputGate() & (intv_x0 + h * f.eval_vector(intv_x)));
    return ctc;
  }

  bool CtcPicard::contract_bwd(const Function& f, TubeSlice& x)
  {
    DimensionException::check(x, f);
    double h = x.domain().diam();
    IntervalVector intv_x = x.codomain(), intv_xf = x.outputGate();
    bool ctc = contract(f, intv_x, intv_xf, -h);
    x.setEnvelope(intv_x);
    x.setInputGate(x.inputGate() & (intv_xf - h * f.eval_vector(intv_x)));
    return ctc;
  }

  int CtcPicard::picardIterations() const
  {
    return m_picard_iterations;
  }

  bool CtcPicard::contract(const Function& f,
                           IntervalVector& x,
                           const IntervalVector& x0,
                           double h)
  {
    float delta = m_delta;
    IntervalVector x_guess = x0, x_enclosure = x0;
    m_picard_iterations = 0;

    do
    {
      m_picard_iterations++;
      x_guess = x_enclosure;

      for(int i = 0 ; i < x_guess.size() ; i++)
        x_guess[i] = x_guess[i].mid()
                   + delta * (x_guess[i] - x_guess[i].mid())
                   + Interval(-EPSILON,EPSILON); // in case of a degenerate box

      x_enclosure = eval(1, f, x_guess, x0, h);// & eval(2, f, x_guess, x0, h);

      if(x_enclosure.is_unbounded())
        return false;

    } while(!x_enclosure.is_interior_subset(x_guess));

    bool ctc = x != x_enclosure;
    x &= x_enclosure;
    return ctc;
  }

  const IntervalVector CtcPicard::eval(int order,
                                       const Function& f,
                                       const IntervalVector& x,
                                       const IntervalVector& x0,
                                       double h)
  {
    Interval intv_h = (Interval(h) | 0.);

    switch(order)
    {
      case 1:
        return x0 + intv_h * f.eval_vector(x);

      case 2:
        return x0
          + intv_h * f.eval_vector(x0)
          + pow(intv_h,2)/2. * f.jacobian(x) * f.eval_vector(x);

      default:
        throw Exception("CtcPicard::eval", "undefined evaluation order");
    }
  }
}