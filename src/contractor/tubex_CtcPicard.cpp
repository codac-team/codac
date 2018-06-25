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
  
  bool CtcPicard::contract(const Function& f, TubeVector& x, bool preserve_sampling)
  {
    bool ctc = false;
    TubeVector *x_ptr;

    if(preserve_sampling)
      x_ptr = new TubeVector(x);

    else
      x_ptr = &x;
    
    TubeSlice *slice_x = x_ptr->getFirstSlice();

    while(slice_x != NULL)
    {
      ctc |= contract_fwd(f, *slice_x);
      //ctc |= contract_bwd(f, *slice_x);

      bool unbounded_slice = slice_x->codomain().is_unbounded();

      if(unbounded_slice && slice_x->domain().diam() > x_ptr->domain().diam() / 5000.)
      {
        TubeSlice *prev_slice_x = slice_x->prevSlice();
        cout << "sampling " << slice_x->domain().mid() << endl;
        x_ptr->sample(slice_x->domain().mid());

        if(prev_slice_x == NULL)
          slice_x = x_ptr->getFirstSlice();

        else
          slice_x = prev_slice_x->nextSlice();

        continue;
      }

      slice_x = slice_x->nextSlice();
    }

    if(preserve_sampling)
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
    IntervalVector intv_x = x.codomain(), intv_x0 = x.inputGate(), intv_xf = x.outputGate();
    bool ctc = contract(f, intv_x, intv_x0, Interval(0., x.domain().diam()));
    x.setEnvelope(intv_x);
    x.setOutputGate(x.inputGate() + x.domain().diam() * f.eval_vector(intv_x));
    return ctc;
  }

  bool CtcPicard::contract_bwd(const Function& f, TubeSlice& x)
  {
    IntervalVector intv_x = x.codomain(), intv_x0 = x.inputGate(), intv_xf = x.outputGate();
    bool ctc = contract(f, intv_x, intv_xf, -Interval(0., x.domain().diam()));
    x.setEnvelope(intv_x);
    x.setInputGate(x.outputGate() - x.domain().diam() * f.eval_vector(intv_x));
    return ctc;
  }

  int CtcPicard::picardIterations() const
  {
    return m_picard_iterations;
  }

  bool CtcPicard::contract(const Function& f,
                           IntervalVector& x, const IntervalVector& x0,
                           const Interval& h)
  {
    float delta = m_delta;
    IntervalVector x_guess = x0, x_enclosure(x.size());
    m_picard_iterations = 0;

    do
    {
      m_picard_iterations++;

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