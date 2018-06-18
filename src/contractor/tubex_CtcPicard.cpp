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
  
  bool CtcPicard::contract(const Function& f, vector<Tube*>& x)
  {
    bool ctc = false;
    vector<TubeSlice*> v_slice_x;
    for(int i = 0 ; i < x.size() ; i++)
      v_slice_x.push_back(x[i]->getFirstSlice());

    while(v_slice_x[0] != NULL)
    {
      ctc |= contract(f, v_slice_x);

      bool unbounded_slice = false;
      for(int i = 0 ; i < v_slice_x.size() ; i++)
        unbounded_slice |= v_slice_x[i]->codomain().is_unbounded();

      if(unbounded_slice && v_slice_x[0]->domain().diam() > x[0]->domain().diam() / 500.) // Picard failed
      {
        for(int i = 0 ; i < v_slice_x.size() ; i++)
        {
          TubeSlice *prev_slice_x = v_slice_x[i]->prevSlice();
          if(i == 0) cout << "sampling at " << v_slice_x[i]->domain().diam() << endl;
          x[i]->sample(v_slice_x[i]->domain().mid());

          if(prev_slice_x == NULL)
            v_slice_x[i] = x[i]->getFirstSlice();

          else
            v_slice_x[i] = prev_slice_x->nextSlice();
        }
        continue;
      }

      for(int i = 0 ; i < v_slice_x.size() ; i++)
        v_slice_x[i] = v_slice_x[i]->nextSlice();
    }

    return ctc;
  }

  bool CtcPicard::contract(const Function& f, TubeSlice& x)
  {
    bool ctc = false;
    Interval intv_x = x.codomain();
    ctc |= contract(f, intv_x, x.inputGate(), Interval(0., x.domain().diam()));
    x.setEnvelope(intv_x);
    intv_x = x.outputGate();
    ctc |= contract(f, intv_x, x.inputGate(), Interval(x.domain().diam()));
    x.setOutputGate(intv_x);
    return ctc;
  }

  bool CtcPicard::contract(const Function& f, vector<TubeSlice*>& x)
  {
    bool ctc = false;
    IntervalVector intv_x(x.size()), intv_x0(x.size()), intv_xf(x.size());

    for(int i = 0 ; i < x.size() ; i++)
    {
      intv_x[i] = x[i]->codomain();
      intv_x0[i] = x[i]->inputGate();
    }

    ctc |= contract(f, intv_x, intv_x0, Interval(0., x[0]->domain().diam()));
    for(int i = 0 ; i < x.size() ; i++)
      x[i]->setEnvelope(intv_x[i]);

    for(int i = 0 ; i < x.size() ; i++)
      x[i]->setOutputGate(x[i]->inputGate()
                        + x[i]->domain().diam() * f.eval_vector(intv_x)[i]);

    return ctc;
  }

  bool CtcPicard::contract(const Function& f, Interval& x, const Interval& x0, const Interval& h)
  {
    float delta = m_delta;
    Interval guess = x0, new_x = x;

    do
    {
      guess = guess.mid() + delta * (guess - guess.mid()) + Interval(-EPSILON,EPSILON);
      IntervalVector box(1, guess);
      new_x = x0 + h * f.eval(box);
      if(new_x.is_unbounded())
        return false;
    } while(!new_x.is_strict_interior_subset(guess));

    bool ctc = x != new_x;
    x = new_x;
    return ctc;
  }

  bool CtcPicard::contract(const Function& f, IntervalVector& x, const IntervalVector& x0, const Interval& h)
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
    } while(!x_enclosure.is_strict_interior_subset(x_guess));

    bool ctc = x != x_enclosure;
    x = x_enclosure;
    return ctc;
  }
}