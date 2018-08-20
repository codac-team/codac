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
  
  bool CtcPicard::contract(const tubex::Fnc& f, Tube& x, TPropagation t_propa) const
  {
    // todo: faster implementation in the scalar case?
    TubeVector x_vect(x);
    bool result = contract(f, x_vect, t_propa);
    x = x_vect[0];
    return result;
  }

  bool CtcPicard::contract(const tubex::Fnc& f, TubeVector& x, TPropagation t_propa) const
  {
    // Vector implementation:

    DimensionException::check(x, f);

    if((t_propa & FORWARD) && (t_propa & BACKWARD))
    {
      // todo: select best way
      contract(f, x, FORWARD);
      contract(f, x, BACKWARD);
    }

    else
    {
      TubeVector *first_slicing;
      if(m_preserve_slicing)
        first_slicing = new TubeVector(x);

      if(t_propa & FORWARD)
      {
        int nb_slices = x.nb_slices();

        for(int i = 0 ; i < nb_slices ; i++)
        {
          contract_ith_slices(f, x, i, t_propa);

          if(x(i).is_unbounded() && x[0].slice_domain(i).diam() > x.domain().diam() / 5000.)
          {
            // NB: all tube components share the same slicing
            x.sample(x[0].slice_domain(i).mid()); // all of them are sampled
            nb_slices ++;
            i--; // the first subslice will be again computed
          }
        }
      }

      else if(t_propa & BACKWARD)
      {
        for(int i = x.nb_slices() - 1 ; i >= 0 ; i--)
        {
          contract_ith_slices(f, x, i, t_propa);

          if(x(i).is_unbounded() && x[0].slice_domain(i).diam() > x.domain().diam() / 5000.)
          {
            // NB: all tube components share the same slicing
            x.sample(x[0].slice_domain(i).mid()); // all of them are sampled
            i ++; // the second subslice will be again computed
          }
        }
      }

      if(m_preserve_slicing)
      {
        first_slicing->set_empty();
        *first_slicing |= x;
        x = *first_slicing;
        delete first_slicing;
      }
    }

    // todo: return value
  }

  void CtcPicard::contract_ith_slices(const tubex::Fnc& f, TubeVector& tube, int slice_id, TPropagation t_propa) const
  {
    // todo: check that !((t_propa & FORWARD) && (t_propa & BACKWARD))
    DimensionException::check(tube, f);
    // todo: check slice_id

    IntervalVector f_eval = f.eval_vector(tube[0].slice_domain(slice_id), tube); // computed only once

    guess_slice_envelope(f, tube, slice_id, t_propa);

    if((t_propa & FORWARD) && (t_propa & BACKWARD))
    {
      // todo: exception
      cout << "exception (todo)" << endl;
    }

    else if(t_propa & FORWARD)
      for(int i = 0 ; i < tube.size() ; i++)
      {
        Slice *slice = tube[i].get_slice(slice_id);
        slice->set_output_gate(slice->output_gate()
          & (slice->input_gate() + slice->domain().diam() * f_eval[i]));
      }

    else if(t_propa & BACKWARD)
      for(int i = 0 ; i < tube.size() ; i++)
      {
        Slice *slice = tube[i].get_slice(slice_id);
        slice->set_input_gate(slice->input_gate()
          & (slice->output_gate() - slice->domain().diam() * f_eval[i]));
      }
  }

  int CtcPicard::picard_iterations() const
  {
    return m_picard_iterations;
  }

  void CtcPicard::guess_slice_envelope(const tubex::Fnc& f,
                                       TubeVector& tube,
                                       int slice_id,
                                       TPropagation t_propa) const
  {
    DimensionException::check(tube, f);
    // todo: check slice_id

    float delta = m_delta;
    Interval h, t = tube[0].slice_domain(slice_id);
    IntervalVector initial_x = tube(slice_id), x0(tube.size()), xf(x0);

    if((t_propa & FORWARD) && (t_propa & BACKWARD))
    {
      // todo: exception
      cout << "exception (todo)" << endl;
    }

    else if(t_propa & FORWARD)
    {
      x0 = tube(t.lb());
      xf = tube(t.ub());
      h = Interval(0., t.diam());
    }

    else if(t_propa & BACKWARD)
    {
      x0 = tube(t.ub());
      xf = tube(t.lb());
      h = Interval(-t.diam(), 0.);
    }

    IntervalVector x_guess(tube.size()), x_enclosure = x0;
    m_picard_iterations = 0;

    do
    {
      m_picard_iterations++;
      x_guess = x_enclosure;

      for(int i = 0 ; i < x_guess.size() ; i++)
        x_guess[i] = x_guess[i].mid()
                   + delta * (x_guess[i] - x_guess[i].mid())
                   + Interval(-EPSILON,EPSILON); // in case of a degenerate box

      x_guess &= initial_x;

      for(int i = 0 ; i < tube.size() ; i++)
        tube[i].get_slice(slice_id)->set_envelope(x_guess[i]);

      x_enclosure = x0 + h * f.eval_vector(t, tube);

      if(x_enclosure.is_unbounded())
      {
        for(int i = 0 ; i < tube.size() ; i++)
          tube[i].get_slice(slice_id)->set_envelope(initial_x[i]); // coming back to the initial state
        // todo: do it only on the unbounded component?
        break;
      }

    } while(!x_enclosure.is_interior_subset(x_guess));

    // Restoring ending gate, contracted by setting the envelope
    for(int i = 0 ; i < tube.size() ; i++)
    {
      Slice *slice = tube[i].get_slice(slice_id);
      if(t_propa & FORWARD)  slice->set_output_gate(xf[i]);
      if(t_propa & BACKWARD) slice->set_input_gate(xf[i]);
      // todo: ^ check this ^
    }
  }
}