/* ============================================================================
 *  tubex-lib - CtcEval class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include <list>
#include "tubex_CtcEval.h"
#include "tubex_CtcDeriv.h"
#include "tubex_SlicingException.h"
#include "tubex_EmptyException.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcEval::CtcEval(bool preserve_slicing, bool enable_propagation)
    : Ctc(preserve_slicing), m_propagation_enabled(enable_propagation)
  {

  }

  bool CtcEval::contract(double t, Interval& z, Tube& y, Tube& w) const
  {
    IntervalVector z_(1, z);
    bool contraction = contract(t, z_, y, w);
    z &= z_[0];
    return contraction;
  }

  bool CtcEval::contract(double t, IntervalVector& z, TubeVector& y, TubeVector& w) const
  {
    SlicingException::check(y, w);
    DimensionException::check(y, z);
    DimensionException::check(y, w);
    SlicingException::check(y, w);

    if(z.is_empty() || y.is_empty())
      return false;

    IntervalVector z_ = z;
    TubeVector y_ = y;

    z &= y.interpol(t, w);
    y.set(z, t);
    w.sample(t); // w is also sampled to stay compliant with y
    y_.sample(t); // the same for future comparison

    if(m_propagation_enabled)
    {
      CtcDeriv ctc_deriv;
      ctc_deriv.contract(y, w);
    }

    if(z.is_empty() || y.is_empty())
    {
      z.set_empty();
      y.set_empty();
      return true;
    }

    return z != z_ || y.is_strict_subset(y_);
  }

  bool CtcEval::contract(Interval& t, Interval& z, Tube& y, Tube& w) const
  {
    IntervalVector z_(1, z);
    bool contraction = contract(t, z_, y, w);
    z &= z_[0];
    return contraction;
  }

  bool CtcEval::contract(Interval& t, IntervalVector& z, TubeVector& y, TubeVector& w) const
  {
    SlicingException::check(y, w);
    DimensionException::check(y, z);
    DimensionException::check(y, w);

    if(t.is_empty() || z.is_empty() || y.is_empty())
      return false;

    Interval t_ = t;
    IntervalVector z_ = z;
    TubeVector y_ = y;
    
    if(t.is_degenerated())
      return contract(t.lb(), z, y, w);

    t &= y.domain();
    t &= y.invert(z, w ,t);

    if(!t.is_empty())
    {
      if(t.is_degenerated())
        return contract(t.lb(), z, y, w);
      
      z &= y.interpol(t, w);

      if(!z.is_empty())
      {
        y.set(y.interpol(t.lb(), w), t.lb()); w.sample(t.lb()); y_.sample(t.lb());
        y.set(y.interpol(t.ub(), w), t.ub()); w.sample(t.ub()); y_.sample(t.ub());
        // Note: w is also sampled to stay compliant with y,
        // the same for future comparison

        CtcDeriv ctc_deriv;
        IntervalVector front_gate(y.dim());
        list<IntervalVector> l_gates;
        TubeSlice *slice_y;
        TubeSlice *slice_w;

        // 1. Forward propagation

          slice_y = y.get_slice(t.lb());
          slice_w = w.get_slice(t.lb());

          front_gate = slice_y->input_gate() & z;
            // Mathematically, front_gate should not be empty at this point.
            // Due to numerical approximations, the computation of t by the invert method
            // provides a wider enclosure t'. The evaluation of y[t'.lb()] may not
            // intersect z and so the front_gate becomes empty.
            // An epsilon inflation could be used to overcome this problem. Or:
            for(int i = 0 ; i < y.dim() ; i++)
              if(front_gate[i].is_empty())
              {
                if(slice_y->input_gate()[i].ub() < z[i].lb()) front_gate[i] = z[i].lb();
                else front_gate[i] = z[i].ub();
              }

          l_gates.push_front(front_gate);

          while(slice_y != NULL && slice_y->domain().lb() < t.ub())
          {
            // Forward propagation of the evaluation
            front_gate += slice_y->domain().diam() * slice_w->codomain(); // projection
            front_gate |= z; // evaluation
            front_gate &= slice_y->output_gate(); // contraction

            // Storing temporarily fwd propagation 
            l_gates.push_front(front_gate);

            // Iteration
            slice_y = slice_y->next_slice();
            slice_w = slice_w->next_slice();
          }

        // 2. Backward propagation

          slice_y = y.get_slice(previous_float(t.ub()));
          slice_w = w.get_slice(previous_float(t.ub()));

          front_gate = slice_y->output_gate() & z;
            // Overcoming numerical approximations, same remark as before:
            for(int i = 0 ; i < y.dim() ; i++)
              if(front_gate[i].is_empty())
              {
                if(slice_y->output_gate()[i].ub() < z[i].lb()) front_gate[i] = z[i].lb();
                else front_gate[i] = z[i].ub();
              }

          slice_y->set_output_gate(l_gates.front() | front_gate);

          while(slice_y != NULL && slice_y->domain().lb() >= t.lb())
          {
            // Backward propagation of the evaluation
            front_gate -= slice_y->domain().diam() * slice_w->codomain(); // projection
            front_gate |= z; // evaluation
            front_gate &= slice_y->input_gate(); // contraction

            // Updating tube
            l_gates.pop_front();
            slice_y->set_input_gate(l_gates.front() | front_gate);
            ctc_deriv.contract_gates(*slice_y, *slice_w);

            // Iteration
            slice_y = slice_y->prev_slice();
            slice_w = slice_w->prev_slice();
          }

        // 3. Envelopes contraction

          if(m_propagation_enabled)
            ctc_deriv.contract(y, w);

        // 4. Evaluation contraction

          t &= y.invert(z, w ,t);
          if(!t.is_empty())
            z &= y.interpol(t, w);
      }
    }

    if(t.is_empty() || z.is_empty() || y.is_empty())
    {
      t.set_empty();
      z.set_empty();
      y.set_empty();
      return true;
    }

    return t != t_ || z != z_ || y.is_strict_subset(y_);
  }
}