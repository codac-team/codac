/** 
 *  CtcEval class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include "codac_CtcEval.h"
#include "codac_CtcDeriv.h"
#include "codac_Domain.h"
#include "codac_DomainsTypeException.h"
#include "codac_DomainsSizeException.h"

using namespace std;
using namespace ibex;

namespace codac
{
  CtcEval::CtcEval()
    : DynCtc(true) // inter-temporal as [t] may involve several times
  {

  }

  // Static members for contractor signature (mainly used for CN Exceptions)
  const string CtcEval::m_ctc_name = "CtcEval";
  vector<string> CtcEval::m_str_expected_doms(
  {
    "Interval, Interval, Tube[, Tube]",
    "Interval, IntervalVector, TubeVector[, TubeVector]"
  });

  void CtcEval::contract(vector<Domain*>& v_domains)
  {
    if(v_domains[0]->type() != Domain::Type::T_INTERVAL)
      throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);

    if(v_domains.size() == 4) // full constraint with derivative
    {
      // Scalar case:
      if(v_domains[1]->type() == Domain::Type::T_INTERVAL && v_domains[2]->type() == Domain::Type::T_TUBE && v_domains[3]->type() == Domain::Type::T_TUBE)
        contract(v_domains[0]->interval(), v_domains[1]->interval(), v_domains[2]->tube(), v_domains[3]->tube());

      // Vector case:
      else if(v_domains[1]->type() == Domain::Type::T_INTERVAL_VECTOR && v_domains[2]->type() == Domain::Type::T_TUBE_VECTOR && v_domains[3]->type() == Domain::Type::T_TUBE_VECTOR)
        contract(v_domains[0]->interval(), v_domains[1]->interval_vector(), v_domains[2]->tube_vector(), v_domains[3]->tube_vector());

      else
        throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);
    }

    else if(v_domains.size() == 3) // simple evaluation without tube contraction
    {
      // Scalar case:
      if(v_domains[1]->type() == Domain::Type::T_INTERVAL && v_domains[2]->type() == Domain::Type::T_TUBE)
        contract(v_domains[0]->interval(), v_domains[1]->interval(), v_domains[2]->tube());

      // Vector case:
      else if(v_domains[1]->type() == Domain::Type::T_INTERVAL_VECTOR && v_domains[2]->type() == Domain::Type::T_TUBE_VECTOR)
        contract(v_domains[0]->interval(), v_domains[1]->interval_vector(), v_domains[2]->tube_vector());

      else
        throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);
    }
  
    else
      throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);
  }

  void CtcEval::enable_time_propag(bool enable_propagation)
  {
    m_propagation_enabled = enable_propagation;
  }

  void CtcEval::contract(double t, Interval& z, Tube& y, Tube& w)
  {
    assert(!std::isnan(t));
    assert(y.tdomain().contains(t));
    assert(y.tdomain() == w.tdomain());
    assert(Tube::same_slicing(y, w));

    if(z.is_empty() || y.is_empty() || w.is_empty())
    {
      z.set_empty();
      y.set_empty();
      w.set_empty();
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
      ctc_deriv.restrict_tdomain(m_restricted_tdomain);
      ctc_deriv.set_fast_mode(m_fast_mode);
      ctc_deriv.contract(y, w);
    }

    else if(merge_after_ctc)
    {
      // The gate will be lost during the final operation for
      // preserving the slicing. So we need to propagate locally
      // the information on nearby slices, to keep the information,
      // and then merge them.

      // 1. Contraction

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
    assert(y.tdomain() == w.tdomain());
    assert(Tube::same_slicing(y, w));
    #ifndef NDEBUG
      double volume = y.volume() + w.volume(); // for last assert
    #endif

    if(t.is_empty() || z.is_empty() || y.is_empty())
    {
      // todo: add w.is_empty() in the above conditions ^
      t.set_empty();
      z.set_empty();
      y.set_empty();
      w.set_empty();
      return;
    }

    if(t.is_degenerated())
      return contract(t.lb(), z, y, w);
    
    y &= Interval(-BOUNDED_INFINITY,BOUNDED_INFINITY); // todo: remove this
    w &= Interval(-BOUNDED_INFINITY,BOUNDED_INFINITY); // todo: remove this

    t &= y.tdomain();
    t &= y.invert(z, w, t);

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
        ctc_deriv.restrict_tdomain(m_restricted_tdomain);
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

          while(s_y && s_y->tdomain().lb() < t.ub())
          {
            // Forward propagation of the evaluation
            front_gate += s_y->tdomain().diam() * s_w->codomain(); // projection
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

          while(s_y && s_y->tdomain().lb() >= t.lb())
          {
            // Backward propagation of the evaluation
            front_gate -= s_y->tdomain().diam() * s_w->codomain(); // projection
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

          t &= y.invert(z, w, t);
          if(!t.is_empty())
            z &= y.interpol(t, w);

        // 5. If requested, preserving the initial slicing

          for(size_t i = 0 ; i < v_gates_to_remove.size() ; i++)
          {
            // The gate will be lost during the final operation for
            // preserving the slicing. So we need to propagate locally
            // the information on nearby slices, to keep the information,
            // and then merge them.

            Slice *s_y = y.slice(v_gates_to_remove[i]);
            Slice *s_w = w.slice(v_gates_to_remove[i]);

            // 1. Contraction

              CtcDeriv ctc_deriv;
              ctc_deriv.contract(*s_y, *s_w);
              if(s_y->next_slice() && s_w->next_slice())
                ctc_deriv.contract(*s_y->next_slice(), *s_w->next_slice());

            // 2. Merge

              y.remove_gate(v_gates_to_remove[i]);
              w.remove_gate(v_gates_to_remove[i]);

              y.delete_synthesis_tree(); // todo: update tree if created, instead of delete
              w.delete_synthesis_tree(); // todo: update tree if created, instead of delete
          }
      }

      // todo: remove this (or use Polygons with truncation)

        for(Slice *s = y.first_slice() ; s ; s = s->next_slice())
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

  void CtcEval::contract(double t, IntervalVector& z, TubeVector& y, TubeVector& w)
  {
    assert(!std::isnan(t));
    Interval intv_t(t);
    contract(intv_t, z, y, w);
  }

  void CtcEval::contract(Interval& t, IntervalVector& z, TubeVector& y, TubeVector& w)
  {
    assert(y.size() == z.size());
    assert(y.size() == w.size());
    assert(y.tdomain() == w.tdomain());
    assert(TubeVector::same_slicing(y, w));

    if(y.size() != z.size() || y.size() != w.size())
      throw DomainsSizeException(m_ctc_name);

    if(t.is_empty() || z.is_empty() || y.is_empty() || w.is_empty())
    {
      t.set_empty();
      z.set_empty();
      y.set_empty();
      w.set_empty();
      return;
    }

    vector<Interval> v_t;
    y.invert(z, v_t, t); // todo: invert with derivative? 
    Interval t_ = Interval::EMPTY_SET;
    for(const auto& ti : v_t)
    {
      z &= y(ti);
      t_ |= ti;
    }
    t &= t_;

    for(int i = 0 ; i < y.size() ; i++)
      contract(t, z[i], y[i], w[i]);
  }
  
  void CtcEval::contract(Interval& t, Interval& z, const Tube& y)
  {
    if(t.is_empty() || z.is_empty() || y.is_empty())
    {
      t.set_empty();
      z.set_empty();
      return;
    }

    vector<Interval> v_t;
    y.invert(z, v_t, t);
    Interval t_ = Interval::EMPTY_SET;
    for(const auto& ti : v_t)
    {
      z &= y(ti);
      t_ |= ti;
    }
    t &= t_;
  }

  void CtcEval::contract(Interval& t, IntervalVector& z, const TubeVector& y)
  {
    assert(y.size() == z.size());

    if(y.size() != z.size())
      throw DomainsSizeException(m_ctc_name);

    if(t.is_empty() || z.is_empty() || y.is_empty())
    {
      t.set_empty();
      z.set_empty();
      return;
    }

    vector<Interval> v_t;
    y.invert(z, v_t, t);
    Interval t_ = Interval::EMPTY_SET;
    for(const auto& ti : v_t)
    {
      z &= y(ti);
      t_ |= ti;
    }
    t &= t_;
  }
}