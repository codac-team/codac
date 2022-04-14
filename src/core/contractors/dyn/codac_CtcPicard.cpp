/** 
 *  CtcPicard class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_CtcPicard.h"
#include "codac_DomainsTypeException.h"

using namespace std;
using namespace ibex;

#define EPSILON std::numeric_limits<float>::epsilon()

namespace codac
{
  CtcPicard::CtcPicard(const Function& f, float delta)
    : DynCtc(true), m_f_ptr(new TFunction(f)), m_f(*m_f_ptr), m_delta(delta)
  {
    assert(f.nb_var() == f.image_dim());
    assert(delta > 0.);
  }

  CtcPicard::CtcPicard(const TFnc& f, float delta)
    : DynCtc(true), m_f(f), m_delta(delta)
  {
    assert(f.nb_var() == f.image_dim());
    assert(delta > 0.);
  }

  CtcPicard::~CtcPicard()
  {
    if(m_f_ptr)
      delete m_f_ptr;
  }

  // Static members for contractor signature (mainly used for CN Exceptions)
  const string CtcPicard::m_ctc_name = "CtcPicard";
  vector<string> CtcPicard::m_str_expected_doms(
  {
    "Tube",
    "TubeVector",
  });
  
  void CtcPicard::contract(vector<Domain*>& v_domains)
  {
    if(v_domains.size() != 1)
      throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);

    if(v_domains[0]->type() == Domain::Type::T_TUBE)
      contract(v_domains[0]->tube(), TimePropag::FORWARD | TimePropag::BACKWARD);

    else if(v_domains[0]->type() == Domain::Type::T_TUBE_VECTOR)
      contract(v_domains[0]->tube_vector(), TimePropag::FORWARD | TimePropag::BACKWARD);

    else
      throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);
  }
  
  void CtcPicard::contract(Tube& x, TimePropag t_propa)
  {
    assert(m_f.nb_var() == 1 && "scalar case");
    // todo: faster implementation in the scalar case?
    TubeVector x_vect(1, x);
    contract(x_vect, t_propa);
    x &= x_vect[0];
  }

  bool is_unbounded(const IntervalVector& x)
  {
    if(x.is_unbounded())
      return true;
    for(int i = 0 ; i < x.size() ; i++)
      if(x[i] == Interval(-99999.,99999.)) // todo: remove this (improvements to be done in CN)
        return true;
    return false;
  }

  void CtcPicard::contract(TubeVector& x, TimePropag t_propa)
  {
    assert(m_f.nb_var() == x.size());

    if(x.is_empty())
      return;

    if((t_propa & TimePropag::FORWARD) && (t_propa & TimePropag::BACKWARD))
    {
      // todo: select best way according to initial conditions
      contract(x, TimePropag::FORWARD);
      contract(x, TimePropag::BACKWARD);
    }

    else
    {
      TubeVector *first_slicing = nullptr;
      if(m_preserve_slicing)
        first_slicing = new TubeVector(x);

      if(t_propa & TimePropag::FORWARD)
      {
        int nb_slices = x.nb_slices();

        for(int k = 0 ; k < nb_slices ; k++)
        {
          if(!is_unbounded(x(k)))
            continue;

          contract_kth_slices(x, k, TimePropag::FORWARD);

          // NB: all tube components share the same slicing
          // If the slice stays unbounded after the contraction step,
          // then it is sampled and contracted again.
          if(is_unbounded(x(k)) && x[0].slice_tdomain(k).diam() > x.tdomain().diam() / 500.)
          {
            
            x.sample(x[0].slice_tdomain(k).mid()); // all the components of the tube are sampled,
            // and sampling time is selected according to the first slice of one of the components,
            // for instance the first one x[0]
            nb_slices ++;
            k --; // the first subslice will be computed
          }
        }
      }

      if(t_propa & TimePropag::BACKWARD)
      {
        for(int k = x.nb_slices() - 1 ; k >= 0 ; k--)
        {
          if(!is_unbounded(x(k)))
            continue;

          contract_kth_slices(x, k, TimePropag::BACKWARD);

          // NB: all tube components share the same slicing
          // If the slice stays unbounded after the contraction step,
          // then it is sampled and contracted again.
          if(is_unbounded(x(k)) && x[0].slice_tdomain(k).diam() > x.tdomain().diam() / 500.)
          {
            x.sample(x[0].slice_tdomain(k).mid()); // all the components of the tube are sampled,
            // and sampling time is selected according to the first slice of one of the components,
            // for instance the first one x[0]
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

  void CtcPicard::contract_kth_slices(TubeVector& x, int k, TimePropag t_propa)
  {
    assert(m_f.nb_var() == x.size());
    assert(!((t_propa & TimePropag::FORWARD) && (t_propa & TimePropag::BACKWARD)) && "forward/backward case not implemented yet");
    assert(k >= 0 && k < x.nb_slices());

    if(x.is_empty())
      return;

    guess_kth_slices_envelope(x, k, t_propa);
    IntervalVector f_eval = m_f.eval_vector(k, x); // computed only once

    if(t_propa & TimePropag::FORWARD)
      for(int i = 0 ; i < x.size() ; i++)
      {
        Slice *s = x[i].slice(k);
        s->set_output_gate(s->output_gate()
          & (s->input_gate() + s->tdomain().diam() * f_eval[i]));
      }

    else if(t_propa & TimePropag::BACKWARD)
      for(int i = 0 ; i < x.size() ; i++)
      {
        Slice *s = x[i].slice(k);
        s->set_input_gate(s->input_gate()
          & (s->output_gate() - s->tdomain().diam() * f_eval[i]));
      }
  }

  void CtcPicard::guess_kth_slices_envelope(TubeVector& x, int k, TimePropag t_propa)
  {
    assert(m_f.nb_var() == x.size());
    assert(!((t_propa & TimePropag::FORWARD) && (t_propa & TimePropag::BACKWARD)) && "forward/backward case not implemented yet");
    assert(k >= 0 && k < x.nb_slices());

    if(x.is_empty())
      return;
    
    float delta = m_delta;
    Interval h, t = x[0].slice_tdomain(k);
    IntervalVector initial_x = x(k), x0(x.size()), xf(x0);

    if(t_propa & TimePropag::FORWARD)
    {
      x0 = x(t.lb());
      xf = x(t.ub());
      h = Interval(0., t.diam());
    }

    else if(t_propa & TimePropag::BACKWARD)
    {
      x0 = x(t.ub());
      xf = x(t.lb());
      h = Interval(-t.diam(), 0.);
    }

    IntervalVector x_guess(x.size()), x_enclosure = x0;
    m_picard_iterations = 0;

    do
    {
      m_picard_iterations++;
      x_guess = x_enclosure;

      for(int i = 0 ; i < x_guess.size() ; i++)
        x_guess[i] = x_guess[i].mid()
                   + delta * (x_guess[i] - x_guess[i].mid())
                   + Interval(-EPSILON,EPSILON); // in case of a degenerate box

      if(m_f.is_intertemporal())
      {
        // Update needed for further computations
        // that may be related to this slice k
        for(int i = 0 ; i < x.size() ; i++)
          x[i].slice(k)->set_envelope(x_guess[i] & initial_x[i]);
        x_enclosure = x0 + h * m_f.eval_vector(k, x);
      }

      else // faster evaluation without tube update
      {
        IntervalVector input_box(x.size() + 1);
        input_box[0] = t;
        input_box.put(1, x_guess & initial_x); // todo: perform the intersection before?
        x_enclosure = x0 + h * m_f.eval_vector(input_box);
      }

      if(is_unbounded(x_enclosure) || x_enclosure.is_empty() || x_guess.is_empty())
      {
        if(m_f.is_intertemporal())
          for(int i = 0 ; i < x.size() ; i++)
            x[i].slice(k)->set_envelope(initial_x[i]); // coming back to the initial state
        break;
      }
    } while(!x_enclosure.is_interior_subset(x_guess));

    // Setting tube's values
    if(!(is_unbounded(x_enclosure) || x_enclosure.is_empty() || x_guess.is_empty()))
      for(int i = 0 ; i < x.size() ; i++)
        x[i].slice(k)->set_envelope(initial_x[i] & x_enclosure[i]);

    if(m_f.is_intertemporal())
    {
      // Restoring ending gate, contracted by setting the envelope
      for(int i = 0 ; i < x.size() ; i++)
      {
        Slice *s = x[i].slice(k);
        if(t_propa & TimePropag::FORWARD)  s->set_output_gate(xf[i]);
        if(t_propa & TimePropag::BACKWARD) s->set_input_gate(xf[i]);
        // todo: ^ check this ^
      }
    }
  }
}
