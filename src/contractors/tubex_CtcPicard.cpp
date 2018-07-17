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
  CtcPicard::CtcPicard(bool preserve_slicing, float delta)
    : Ctc(preserve_slicing), m_delta(delta)
  {

  }
  
  bool CtcPicard::contract(const tubex::Fnc& f, TubeVector& x, TPropagation t_propa) const
  {
    DimensionException::check(x, f);
    TubeVector first_slicing(x);

    if((t_propa & FORWARD) && (t_propa & BACKWARD))
    {
      // todo: select best way
      contract(f, x, FORWARD);
      contract(f, x, BACKWARD);
    }

    else if(t_propa & FORWARD)
    {
      TubeSlice *slice_x = x.get_first_slice();

      while(slice_x != NULL)
      {
        contract(f, x, *slice_x, t_propa);
        bool unbounded_slice = slice_x->codomain().is_unbounded();

        if(unbounded_slice && slice_x->domain().diam() > x.domain().diam() / 5000.)
        {
          TubeSlice *prev_slice_x = slice_x->prev_slice();
          x.sample(slice_x->domain().mid());

          if(prev_slice_x == NULL) slice_x = x.get_first_slice();
          else slice_x = prev_slice_x->next_slice();
          continue;
        }

        slice_x = slice_x->next_slice();
      }
    }

    else if(t_propa & BACKWARD)
    {
      TubeSlice *slice_x = x.get_last_slice();

      while(slice_x != NULL)
      {
        contract(f, x, *slice_x, t_propa);
        bool unbounded_slice = slice_x->codomain().is_unbounded();

        if(unbounded_slice && slice_x->domain().diam() > x.domain().diam() / 5000.)
        {
          TubeSlice *prev_slice_x = slice_x->next_slice();
          x.sample(slice_x->domain().mid());

          if(prev_slice_x == NULL) slice_x = x.get_last_slice();
          else slice_x = prev_slice_x->prev_slice();
          continue;
        }

        slice_x = slice_x->prev_slice();
      }
    }

    if(m_preserve_slicing)
    {
      first_slicing.set_empty();
      first_slicing |= x;
      x = first_slicing;
    }

    // todo: return value
  }

  void CtcPicard::contract(const tubex::Fnc& f, const TubeVector& tube, TubeSlice& slice, TPropagation t_propa) const
  {
    // todo: check that !((t_propa & FORWARD) && (t_propa & BACKWARD))
    DimensionException::check(tube, f);
    DimensionException::check(tube, slice);

    guess_slice_envelope(f, tube, slice, t_propa);

    if((t_propa & FORWARD) && (t_propa & BACKWARD))
    {
      // todo: exception
      cout << "exception (todo)" << endl;
    }

    else if(t_propa & FORWARD)
      slice.set_output_gate(slice.output_gate()
        & (slice.input_gate() + slice.domain().diam() * f.eval(slice.domain(), tube)));

    else if(t_propa & BACKWARD)
      slice.set_input_gate(slice.input_gate()
        & (slice.output_gate() - slice.domain().diam() * f.eval(slice.domain(), tube)));
  }

  int CtcPicard::picard_iterations() const
  {
    return m_picard_iterations;
  }

  void CtcPicard::guess_slice_envelope(const tubex::Fnc& f,
                                       const TubeVector& tube,
                                       TubeSlice& slice,
                                       TPropagation t_propa) const
  {
    DimensionException::check(tube, f);
    DimensionException::check(tube, slice);

    float delta = m_delta;
    Interval h, t = slice.domain();
    IntervalVector initial_x(slice.codomain()), x0(tube.dim()), xf(x0);

    if((t_propa & FORWARD) && (t_propa & BACKWARD))
    {
      // todo: exception
      cout << "exception (todo)" << endl;
    }

    else if(t_propa & FORWARD)
    {
      x0 = slice.input_gate();
      xf = slice.output_gate();
      h = Interval(0., t.diam());
    }

    else if(t_propa & BACKWARD)
    {
      x0 = slice.output_gate();
      xf = slice.input_gate();
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

      slice.set_envelope(x_guess & initial_x);
      x_enclosure = x0 + h * f.eval(t, tube);

      if(x_enclosure.is_unbounded())
      {
        slice.set_envelope(initial_x); // coming back to the initial state
        break;
      }

    } while(!x_enclosure.is_interior_subset(x_guess));

    // Restoring ending gate, contracted by setting the envelope
    if(t_propa & FORWARD)  slice.set_output_gate(xf);
    if(t_propa & BACKWARD) slice.set_input_gate(xf);
  }
}