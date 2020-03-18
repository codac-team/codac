/** 
 *  CtcEval class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include "tubex_CtcEval.h"
#include "tubex_CtcDeriv.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcEval::CtcEval() : Ctc()
  {

  }

  void CtcEval::contract(vector<DomainParams>& v_domains)
  {
    assert(v_domains[0].ad.m_i != NULL);

    if(v_domains.size() == 4) // full constraint with derivative
    {
      v_domains[3].rel = DomainRelation::IN; // derivative cannot be contracted

      // Scalar case:
      if(v_domains[1].ad->type() == DomainType::INTERVAL && v_domains[2].ad->type() == DomainType::TUBE && v_domains[3].ad->type() == DomainType::TUBE)
        contract(v_domains[0].ad->m_i, v_domains[1].ad->m_i, v_domains[2].ad->m_t, v_domains[3].ad->m_t);

      // Vector case:
      else if(v_domains[1].ad->type() == DomainType::INTERVAL_VECTOR && v_domains[2].ad->type() == DomainType::TUBE_VECTOR && v_domains[3].ad->type() == DomainType::TUBE_VECTOR)
        contract(v_domains[0].ad->m_i, v_domains[1].ad->m_iv, v_domains[2].ad->m_tv, v_domains[3].ad->m_tv);

      else
        assert(false && "unhandled case");
    }

    else if(v_domains.size() == 3) // simple evaluation without tube contraction
    {
      v_domains[2].rel = DomainRelation::IN; // tube cannot be contracted without derivative

      // Scalar case:
      if(v_domains[1].ad->type() == DomainType::INTERVAL && v_domains[2].ad->type() == DomainType::TUBE)
        contract(v_domains[0].ad->m_i, v_domains[1].ad->m_i, v_domains[2].ad->m_t);

      // Vector case:
      else if(v_domains[1].ad->type() == DomainType::INTERVAL_VECTOR && v_domains[2].ad->type() == DomainType::TUBE_VECTOR)
        contract(v_domains[0].ad->m_i, v_domains[1].ad->m_iv, v_domains[2].ad->m_tv);

      else
        assert(false && "unhandled case");
    }
  
    else
      assert(false && "unhandled case");
  }

  void CtcEval::enable_temporal_propagation(bool enable_propagation)
  {
    m_propagation_enabled = enable_propagation;
  }

  void CtcEval::contract(double t, Interval& z, Tube& y, Tube& w)
  {
    assert(y.domain().contains(t));
    assert(y.domain() == w.domain());
    assert(Tube::same_slicing(y, w));

    if(z.is_empty() || y.is_empty())
    {
      z.set_empty();
      y.set_empty();
      return;
    }

    bool merge_after_ctc = m_preserve_slicing && !y.gate_exists(t);

    z &= y.interpol(t, w);
    y.set(z, t);
    w.sample(t); // w is also sampled to stay compliant with y
    assert(Tube::same_slicing(y, w));

    if(m_propagation_enabled)
    {
      CtcDeriv ctc_deriv;
      ctc_deriv.set_restricted_domain(m_restricted_domain);
      ctc_deriv.set_fast_mode(m_fast_mode);
      ctc_deriv.contract(y, w);
    }

    else if(merge_after_ctc)
    {
      // The gate will be lost during the final operation for
      // preserving the slicing. So we need to propagate localy
      // the information on nearby slices, to keep the information,
      // and then merge them.

      // 1. Contration

        CtcDeriv ctc_deriv;
        ctc_deriv.contract(*y.slice(t)->prev_slice(), *w.slice(t)->prev_slice());
        ctc_deriv.contract(*y.slice(t), *w.slice(t));

      // 2. Merge

        y.remove_gate(t);
        w.remove_gate(t);

        y.delete_synthesis_tree(); // todo: update tree if created, instead of delete
        w.delete_synthesis_tree(); // todo: update tree if created, instead of delete
    }

    if(z.is_empty() || y.is_empty())
    {
      z.set_empty();
      y.set_empty();
      return;
    }
  }

  void CtcEval::contract(Interval& t, Interval& z, Tube& y, Tube& w)
  {
    assert(y.domain() == w.domain());
    assert(Tube::same_slicing(y, w));
    double volume = y.volume() + w.volume(); // for last assert
    
    if(t.is_degenerated())
      return contract(t.lb(), z, y, w);

    if(t.is_empty() || z.is_empty() || y.is_empty())
    {
      t.set_empty();
      z.set_empty();
      y.set_empty();
      return;
    }

    y &= Interval(-BOUNDED_INFINITY,BOUNDED_INFINITY); // todo: remove this

    t &= y.domain();
    t &= y.invert(z, w ,t);

    if(!t.is_empty())
    {
      if(t.is_degenerated())
        return contract(t.lb(), z, y, w);
      
      z &= y.interpol(t, w);

      if(!z.is_empty())
      {
        vector<double> v_gates_to_remove;
        if(m_preserve_slicing)
        {
          if(!y.gate_exists(t.lb())) // will exist then
            v_gates_to_remove.push_back(t.lb());
          if(!y.gate_exists(t.ub())) // will exist then
            v_gates_to_remove.push_back(t.ub());
        }

        y.set(y.interpol(t.lb(), w), t.lb()); w.sample(t.lb());
        y.set(y.interpol(t.ub(), w), t.ub()); w.sample(t.ub());
          
        // Note: w is also sampled to stay compliant with y.

        CtcDeriv ctc_deriv;
        ctc_deriv.set_restricted_domain(m_restricted_domain);
        ctc_deriv.set_fast_mode(m_fast_mode);

        Interval front_gate(y.size());
        list<Interval> l_gates;
        Slice *s_y;
        Slice *s_w;

        // 1. Forward propagation

          s_y = y.slice(t.lb());
          s_w = w.slice(t.lb());

          front_gate = s_y->input_gate() & z;
            // Mathematically, front_gate should not be empty at this point.
            // Due to numerical approximations, the computation of t by the invert method
            // provides a wider enclosure t'. The evaluation of y[t'.lb()] may not
            // intersect z and so the front_gate becomes empty.
            // An epsilon inflation could be used to overcome this problem. Or:
            if(front_gate.is_empty())
            {
              if(s_y->input_gate().ub() < z.lb()) front_gate = z.lb();
              else front_gate = z.ub();
            }

          l_gates.push_front(front_gate);

          while(s_y != NULL && s_y->domain().lb() < t.ub())
          {
            // Forward propagation of the evaluation
            front_gate += s_y->domain().diam() * s_w->codomain(); // projection
            front_gate |= z; // evaluation
            front_gate &= s_y->output_gate(); // contraction

            // Storing temporarily fwd propagation 
            l_gates.push_front(front_gate);

            // Iteration
            s_y = s_y->next_slice();
            s_w = s_w->next_slice();
          }

        // 2. Backward propagation

          s_y = y.slice(ibex::previous_float(t.ub()));
          s_w = w.slice(ibex::previous_float(t.ub()));

          front_gate = s_y->output_gate() & z;
            // Overcoming numerical approximations, same remark as before:
            if(front_gate.is_empty())
            {
              if(s_y->output_gate().ub() < z.lb()) front_gate = z.lb();
              else front_gate = z.ub();
            }

          s_y->set_output_gate(l_gates.front() | front_gate);

          while(s_y != NULL && s_y->domain().lb() >= t.lb())
          {
            // Backward propagation of the evaluation
            front_gate -= s_y->domain().diam() * s_w->codomain(); // projection
            front_gate |= z; // evaluation
            front_gate &= s_y->input_gate(); // contraction

            // Updating tube
            l_gates.pop_front();
            s_y->set_input_gate(l_gates.front() | front_gate);
            ctc_deriv.contract_gates(*s_y, *s_w);

            // Iteration
            s_y = s_y->prev_slice();
            s_w = s_w->prev_slice();
          }

        // 3. Envelopes contraction

          if(m_propagation_enabled)
            ctc_deriv.contract(y, w);

        // 4. Evaluation contraction

          t &= y.invert(z, w ,t);
          if(!t.is_empty())
            z &= y.interpol(t, w);

        // 5. If requested, preserving the initial slicing

          for(int i = 0 ; i < v_gates_to_remove.size() ; i++)
          {
            // The gate will be lost during the final operation for
            // preserving the slicing. So we need to propagate localy
            // the information on nearby slices, to keep the information,
            // and then merge them.

            Slice *s_y = y.slice(v_gates_to_remove[i]);
            Slice *s_w = w.slice(v_gates_to_remove[i]);

            // 1. Contration

              CtcDeriv ctc_deriv;
              ctc_deriv.contract(*s_y, *s_w);
              // If the number of slices has not changed, the next slice should exist:
              assert(s_y->next_slice() != NULL && s_w->next_slice() != NULL);
              ctc_deriv.contract(*s_y->next_slice(), *s_w->next_slice());

            // 2. Merge

              y.remove_gate(v_gates_to_remove[i]);
              w.remove_gate(v_gates_to_remove[i]);

              y.delete_synthesis_tree(); // todo: update tree if created, instead of delete
              w.delete_synthesis_tree(); // todo: update tree if created, instead of delete
          }
      }

      // todo: remove this (or use Polygons with truncature)

        for(Slice *s = y.first_slice() ; s != NULL ; s = s->next_slice())
        {
          Interval envelope = s->codomain();
          if(envelope.ub() == BOUNDED_INFINITY) envelope = Interval(envelope.lb(),POS_INFINITY);
          if(envelope.lb() == -BOUNDED_INFINITY) envelope |= Interval(NEG_INFINITY,envelope.ub());
          s->set_envelope(envelope);

          Interval ingate = s->input_gate();
          if(ingate.ub() == BOUNDED_INFINITY) ingate = Interval(ingate.lb(),POS_INFINITY);
          if(ingate.lb() == -BOUNDED_INFINITY) ingate = Interval(NEG_INFINITY,ingate.ub());
          s->set_input_gate(ingate);

          Interval outgate = s->output_gate();
          if(outgate.ub() == BOUNDED_INFINITY) outgate = Interval(outgate.lb(),POS_INFINITY);
          if(outgate.lb() == -BOUNDED_INFINITY) outgate = Interval(NEG_INFINITY,outgate.ub());
          s->set_output_gate(outgate);
        }
    }

    if(t.is_empty() || z.is_empty() || y.is_empty())
    {
      t.set_empty();
      z.set_empty();
      y.set_empty();
    }

    assert(volume >= y.volume() + w.volume() && "contraction rule not respected");
  }

  void CtcEval::contract(double t, ibex::IntervalVector& z, TubeVector& y, TubeVector& w)
  {
    assert(y.domain().contains(t));
    assert(y.domain() == w.domain());
    assert(TubeVector::same_slicing(y, w));

    for(int i = 0 ; i < y.size() ; i++)
      contract(t, z[i], y[i], w[i]);
  }

  void CtcEval::contract(const Interval& t, const Interval& z, Tube& y, Tube& w)
  {
    assert(y.domain() == w.domain());
    assert(Tube::same_slicing(y, w));

    Interval _t(t), _z(z);
    contract(_t, _z, y, w);
  }

  void CtcEval::contract(ibex::Interval& t, ibex::IntervalVector& z, TubeVector& y, TubeVector& w)
  {
    assert(y.size() == z.size());
    assert(y.size() == w.size());
    assert(y.domain() == w.domain());
    assert(TubeVector::same_slicing(y, w));

    Interval t_result = Interval::EMPTY_SET;

    for(int i = 0 ; i < y.size() ; i++)
    {
      Interval _t(t);
      contract(_t, z[i], y[i], w[i]);
      t_result |= _t;
    }

    t &= t_result;
  }

  void CtcEval::contract(const Interval& t, const IntervalVector& z, TubeVector& y, TubeVector& w)
  {
    assert(y.size() == z.size());
    assert(y.size() == w.size());
    assert(y.domain() == w.domain());
    assert(TubeVector::same_slicing(y, w));

    Interval _t(t); IntervalVector _z(z);
    contract(_t, _z, y, w);
  }

  void CtcEval::contract(Interval& t, Interval& z, const Tube& y)
  {
    t &= y.invert(z);
    z &= y(t);
  }

  void CtcEval::contract(Interval& t, IntervalVector& z, const TubeVector& y)
  {
    assert(y.size() == z.size());
    t &= y.invert(z);
    z &= y(t);
  }
}