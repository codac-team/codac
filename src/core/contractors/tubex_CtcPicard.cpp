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
  CtcPicard::CtcPicard(float delta)
    : Ctc(), m_delta(delta)
  {

  }
  
  void CtcPicard::contract(const tubex::Fnc& f, Tube& x, TPropagation t_propa)
  {
    // todo: faster implementation in the scalar case?
    TubeVector x_vect(x);
    contract(f, x_vect, t_propa);
    x = x_vect[0];
  }

  void CtcPicard::contract(const tubex::Fnc& f, TubeVector& x, TPropagation t_propa)
  {
    // Vector implementation
    DimensionException::check(x, f);

    if(x.is_empty())
      return;

    if((t_propa & FORWARD) && (t_propa & BACKWARD))
    {
      // todo: select best way according to initial conditions
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

        for(int k = 0 ; k < nb_slices ; k++)
        {
          contract_kth_slices(f, x, k, FORWARD);

          // NB: all tube components share the same slicing
          // If the slice stays unbounded after the contraction step,
          // then it is sampled and contracted again.
          if(x(k).is_unbounded() && x[0].slice_domain(k).diam() > x.domain().diam() / 500.)
          {
            
            x.sample(x[0].slice_domain(k).mid()); // all the tubes components are sampled
            nb_slices ++;
            k --; // the first subslice will be computed
          }
        }
      }

      if(t_propa & BACKWARD)
      {
        for(int k = x.nb_slices() - 1 ; k >= 0 ; k--)
        {
          contract_kth_slices(f, x, k, BACKWARD);

          // NB: all tube components share the same slicing
          // If the slice stays unbounded after the contraction step,
          // then it is sampled and contracted again.
          if(x(k).is_unbounded() && x[0].slice_domain(k).diam() > x.domain().diam() / 500.)
          {
            x.sample(x[0].slice_domain(k).mid()); // all the tubes components are sampled
            k += 2; // the second subslice will be computed
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
  }

  int CtcPicard::picard_iterations() const
  {
    return m_picard_iterations;
  }

  void CtcPicard::contract_kth_slices(const tubex::Fnc& f,
                                      TubeVector& tube,
                                      int k,
                                      TPropagation t_propa)
  {
    // todo: check that !((t_propa & FORWARD) && (t_propa & BACKWARD))
    DimensionException::check(tube, f);
    // todo: check k

    if(tube.is_empty())
      return;

    IntervalVector f_eval = f.eval_vector(tube[0].slice_domain(k), tube); // computed only once
    guess_kth_slices_envelope(f, tube, k, t_propa);

    if((t_propa & FORWARD) && (t_propa & BACKWARD))
    {
      // todo: exception
      cout << "exception (todo)" << endl;
    }

    else if(t_propa & FORWARD)
      for(int i = 0 ; i < tube.size() ; i++)
      {
        Slice *slice = tube[i].get_slice(k);
        slice->set_output_gate(slice->output_gate()
          & (slice->input_gate() + slice->domain().diam() * f_eval[i]));
      }

    else if(t_propa & BACKWARD)
      for(int i = 0 ; i < tube.size() ; i++)
      {
        Slice *slice = tube[i].get_slice(k);
        slice->set_input_gate(slice->input_gate()
          & (slice->output_gate() - slice->domain().diam() * f_eval[i]));
      }
  }

  void CtcPicard::guess_kth_slices_envelope(const tubex::Fnc& f,
                                            TubeVector& tube,
                                            int k,
                                            TPropagation t_propa)
  {
    DimensionException::check(tube, f);
    // todo: check k

    if(tube.is_empty())
      return;
    
    float delta = m_delta;
    Interval h, t = tube[0].slice_domain(k);
    IntervalVector initial_x = tube(k), x0(tube.size()), xf(x0);

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

      for(int i = 0 ; i < tube.size() ; i++)
        tube[i].get_slice(k)->set_envelope(x_guess[i] & initial_x[i]);

      x_enclosure = x0 + h * f.eval_vector(t, tube);
      
      if(x_enclosure.is_unbounded() || x_enclosure.is_empty() || x_guess.is_empty())
      {
        for(int i = 0 ; i < tube.size() ; i++)
          tube[i].get_slice(k)->set_envelope(initial_x[i]); // coming back to the initial state
        // todo: do it only on the unbounded component?
        break;
      }
    } while(!x_enclosure.is_interior_subset(x_guess));

    // Restoring ending gate, contracted by setting the envelope
    for(int i = 0 ; i < tube.size() ; i++)
    {
      Slice *slice = tube[i].get_slice(k);
      if(t_propa & FORWARD)  slice->set_output_gate(xf[i]);
      if(t_propa & BACKWARD) slice->set_input_gate(xf[i]);
      // todo: ^ check this ^
    }
  }
}