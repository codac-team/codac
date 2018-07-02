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

#include "tubex_CtcEval.h"
#include "tubex_CtcDeriv.h"
#include "tubex_StructureException.h"
#include "tubex_EmptyException.h"
#include "tubex_DomainException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcEval::CtcEval(bool enable_propagation, bool enable_slicing)
    : m_propagation_enabled(enable_propagation), m_slicing_enabled(enable_slicing)
  {

  }

  bool CtcEval::contract(double t, Interval& z, Tube& y, Tube& w) const
  {
    StructureException::check(y, w);
    DomainException::check(y, t);
    EmptyException::check(w); // todo: remove this

    bool contraction = false;

    z &= y.interpol(t, w);
    y.set(z, t);
    w.sample(t); // w is also sampled to stay compliant with y

    if(m_propagation_enabled)
    {
      CtcDeriv ctc_deriv;
      ctc_deriv.contract(y, w);
    }

    return contraction;
  }

  bool CtcEval::contract(Interval& t, Interval& z, Tube& y, Tube& w) const
  {
    StructureException::check(y, w);
    DomainException::check(y, t);
    //EmptyException::check(w); // todo: remove this?
    
    if(t.is_degenerated())
      return contract(t.lb(), z, y, w);

    t &= y.invert(z, w ,t);

    if(t.is_degenerated())
      return contract(t.lb(), z, y, w);
    
    z &= y.interpol(t, w);

    y.set(y.interpol(t.lb(), w), t.lb()); w.sample(t.lb());
    y.set(y.interpol(t.ub(), w), t.ub()); w.sample(t.ub());
    // Note: w is also sampled to stay compliant with y

    CtcDeriv ctc_deriv;
    Interval front_gate;
    list<IntervalVector> l_gates;
    TubeSlice *slice_y;
    const TubeSlice *slice_w;

    // 1. Forward propagation

      slice_y = y.getSlice(t.lb());
      slice_w = w.getSlice(t.lb());
      front_gate = slice_y->inputGate()[0] & z;
      l_gates.push_front(IntervalVector(1, front_gate));

      while(slice_y != NULL && slice_y->domain().lb() < t.ub())
      {
        // Forward propagation of the evaluation
        front_gate += slice_y->domain().diam() * slice_w->codomain()[0]; // projection
        front_gate |= z; // evaluation
        front_gate &= slice_y->outputGate()[0]; // contraction

        // Storing temporarily fwd propagation 
        l_gates.push_front(IntervalVector(1, front_gate));

        // Iteration
        slice_y = slice_y->nextSlice();
        slice_w = slice_w->nextSlice();
      }

    // 2. Backward propagation

      slice_y = y.getSlice(previous_float(t.ub()));
      slice_w = w.getSlice(previous_float(t.ub()));
      front_gate = slice_y->outputGate()[0] & z;
      slice_y->setOutputGate(l_gates.front() | IntervalVector(1, front_gate));

      while(slice_y != NULL && slice_y->domain().lb() >= t.lb())
      {
        // Backward propagation of the evaluation
        front_gate -= slice_y->domain().diam() * slice_w->codomain()[0]; // projection
        front_gate |= z; // evaluation
        front_gate &= slice_y->inputGate()[0]; // contraction

        // Updating tube
        l_gates.pop_front();
        slice_y->setInputGate(l_gates.front() | IntervalVector(1, front_gate));
        ctc_deriv.contractGates(*slice_y, *slice_w);

        // Iteration
        slice_y = slice_y->prevSlice();
        slice_w = slice_w->prevSlice();
      }

    // 3. Envelopes contraction

      if(m_propagation_enabled)
        ctc_deriv.contract(y, w);

    // 4. Evaluation contraction

      t &= y.invert(z, w ,t);
      z &= y.interpol(t, w);
    

  /*

    bool inconsistency = false;
    m_bisection_required = false;

    int valid_tsubdomains = 0;
    int index_lb, index_ub;
    int min_index_ctc = 0, max_index_ctc = y.nbSlices() - 1; // set bounds to this contractor
    double old_t_diam = t.diam(), old_z_diam = z.diam();

    // Trying to contract [t]

      t = y.invert(y[t] & z, w, t);

      // The observation [t]x[z] may cross the tube several times
      vector<Interval> v_intv_t;
      y.invert(y[t] & z, v_intv_t, w, t);

    // Trying to contract [z]

      if(t.is_empty())
        inconsistency = true;

      else
      {
        z &= y.interpol(t, w);

        // Computing index

          computeIndex(t, z, y, index_lb, index_ub);

          //if(!propagate) // todo: optimize this? For now, propagation performed by CtcDeriv
          {
            min_index_ctc = max(0, index_lb - 1);
            max_index_ctc = min(index_ub + 1, y.nbSlices() - 1);
          }

        // Initializations

          map<int,Interval> map_new_z;
          #pragma omp for
          for(int i = min_index_ctc ; i <= max_index_ctc ; i++)
            map_new_z[i] = Interval::EMPTY_SET;

          Interval old_z = z;
          z.set_empty();
          t.set_empty();

        // Iteration for each [t] subdomain
        for(int k = 0 ; k < v_intv_t.size() ; k++)
        {
          double dt = y.getSlice(k)->domain().diam();
          bool local_inconsistency = false;

          Interval local_t = v_intv_t[k];
          Interval local_z = old_z;

          if(v_intv_t.size() > 1) // no need for recomputation of [z] and index if only one [t] subdomain
          {
            if(local_t.is_empty())
            {
              local_z.set_empty();
              continue;
            }

            else
            {
              // Trying to contract [local_z]
              local_z &= y.interpol(local_t, w);

              // Computing new index
              computeIndex(local_t, local_z, y, index_lb, index_ub);
            }
          }

          // Trying to contract y

            Interval z_front;

            // Backward

              z_front = local_z;
              for(int i = index_ub ; i >= min_index_ctc ; i--)
              {
                Interval integ_domain;
                Interval slice_dom = y.getSlice(i)->domain();
                Interval slice_deriv = w[i];

                // Over the ith slice
                integ_domain = Interval(0., min(dt, local_t.ub() - slice_dom.lb()));
                map_new_z[i] |= y[i] & (z_front - slice_deriv * integ_domain);

                // On the front, backward way
                integ_domain = Interval(min(dt, max(0., local_t.lb() - slice_dom.lb())), min(dt, local_t.ub() - slice_dom.lb()));
                z_front = y[slice_dom.lb()] & (z_front - slice_deriv * integ_domain);
              }

            // Forward

              z_front = local_z;
              for(int i = index_lb ; i <= max_index_ctc ; i++)
              {
                Interval integ_domain;
                Interval slice_dom = y.getSlice(i)->domain();
                Interval slice_deriv = w[i];

                // Over the ith slice
                integ_domain = Interval(0., min(slice_dom.ub() - local_t.lb(), dt));
                map_new_z[i] |= y[i] & (z_front + slice_deriv * integ_domain);

                // On the front, forward way
                integ_domain = Interval(min(dt, max(0., slice_dom.ub() - local_t.ub())), min(dt, slice_dom.ub() - local_t.lb()));
                z_front = y[slice_dom.ub()] & (z_front + slice_deriv * integ_domain);

              }

            valid_tsubdomains ++;
            z |= local_z;
            t |= local_t;
        } // end of for

        // Synthesis
        {
          Interval prev_z;
          m_y_contracted = false;

          // The synthesis is made over two for-loops
          // so that we can stop the iteration if there is no more propagation

          prev_z = Interval::ALL_REALS;
          for(int i = min_index_ctc ; i <= max_index_ctc ; i++) // forward
          {
            if(!prev_z.intersects(map_new_z[i]))
            {
              inconsistency = true;
              break;
            }

            m_y_contracted |= map_new_z[i].diam() < y[i].diam();
            prev_z = map_new_z[i];
            y.set(map_new_z[i], i);
          }
        }
    }

    if(inconsistency)
    {
      #pragma omp for
      for(int i = min_index_ctc ; i <= max_index_ctc ; i++)
        y.set(Interval::EMPTY_SET, i);

      m_y_contracted = true;
      t.set_empty();
      m_t_contracted = true;
      z.set_empty();
      m_z_contracted = true;
    }

    else
    {
      m_bisection_required = valid_tsubdomains > 1;
      m_t_contracted = t.diam() < old_t_diam;
      m_z_contracted = z.diam() < old_z_diam;
    }

    if(m_propagation_enabled)
      m_y_contracted |= y.ctcDeriv(w); // todo: optimize propagations from evaluation

    return m_z_contracted | m_y_contracted | m_t_contracted;*/
  }

  void CtcEval::computeIndex(const Interval& t, const Interval& z, const Tube& y, int& index_lb, int& index_ub) const
  {
    if(t.is_unbounded() || t.is_empty())
      throw Exception("CtcEval::computeIndex(...)", "unbounded or empty [t]");

    // Slices of index_lb and index_ub strictly enclose the measurement [t]

    index_lb = y.input2index(t.lb());
    // Special case when the lower bound of [t] equals a bound of a slice or is near empty values
    if(y.getSlice(index_lb)->domain().lb() == t.lb() && y[max(0, index_lb - 1)].intersects(z))
      index_lb = max(0, index_lb - 1);

    index_ub = y.input2index(t.ub());
    // Special case when the upper bound of [t] is near empty values
    if(!y[max(0, index_ub)].intersects(z))
      index_ub = max(0, index_ub - 1);
  }

  /*bool CtcEval::tContracted()
  {
    return m_t_contracted;
  }

  bool CtcEval::zContracted()
  {
    return m_z_contracted;
  }
  
  bool CtcEval::yContracted()
  {
    return m_y_contracted;
  }
  
  bool CtcEval::wContracted()
  {
    return false; // w cannot be contracted
    // todo: contraction in case of degenerate tube (w to [0])
  }

  bool CtcEval::tBisectionRequired()
  {
    return m_bisection_required;
  }*/
}