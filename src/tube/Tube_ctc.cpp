/* ============================================================================
 *  tube-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "Tube.h"
#include <iostream>
#include <iomanip> // for setprecision()
#ifdef _OPENMP
  #include <omp.h> // for multithreading
#endif

using namespace std;
using namespace ibex;

bool Tube::ctcFwd(const Tube& derivative_tube, const Interval& initial_value)
{
  if(size() != derivative_tube.size() || dt() != derivative_tube.dt())
    cout << "Warning ctcFwd(const Tube& derivative_tube): tube of different size or timestep: "
         << derivative_tube.size() << "/" << size() << endl;

  for(int i = 0 ; i < size() ; i++)
    if(derivative_tube[i].is_empty())
      cout << "Warning ctcFwd(const Tube& derivative_tube): derivative_tube[" << i << "] is empty" << endl;

  bool contraction = false;
  Interval next_y = (*this)[0];
  Interval y_front = next_y & initial_value;

  for(int i = 0 ; i < size() ; i++) // from the past to the future
  {
    double dt_ = derivative_tube.domain(i).diam();
    Interval y_old = next_y;
    Interval y_new = y_old & (y_front + derivative_tube[i] * Interval(0., dt_));
    contraction |= y_new.diam() < y_old.diam();
    set(y_new, i);

    // Discontinuous
    if(y_new.is_empty())
    {
      set(Interval::EMPTY_SET);
      contraction = true;
      break;
    }

    // Preparing next slice computation
    if(i < size() - 1)
    {
      y_front = y_old & (y_front + derivative_tube[i] * dt_);
      next_y = (*this)[i + 1];
      y_front &= next_y;
    }
  }

  return contraction;
}
bool Tube::ctcBwd(const Tube& derivative_tube)
{
  if(size() != derivative_tube.size() || dt() != derivative_tube.dt())
    cout << "Warning ctcBwd(const Tube& derivative_tube): tube of different size or timestep: "
         << derivative_tube.size() << "/" << size() << endl;

  for(int i = 0 ; i < size() ; i++)
    if(derivative_tube[i].is_empty())
      cout << "Warning ctcBwd(const Tube& derivative_tube): derivative_tube[" << i << "] is empty" << endl;

  bool contraction = false;
  Interval next_y = (*this)[size() - 1];
  Interval y_front = next_y & next_y - derivative_tube[size() - 1] * derivative_tube.domain(size() - 1).diam();
  next_y = (*this)[max(0, size() - 2)];

  for(int i = max(0, size() - 2) ; i >= 0 ; i--) // from the future to the past
  {
    double dt_ = derivative_tube.domain(i).diam();
    Interval y_old = (*this)[i];
    Interval y_new = y_old & (y_front - derivative_tube[i] * Interval(0., dt_));
    contraction |= y_new.diam() < y_old.diam();
    set(y_new, i);

    // Discontinuous
    if(y_new.is_empty())
    {
      set(Interval::EMPTY_SET);
      contraction = true;
      break;
    }

    if(i > 0)
    {
      y_front = y_old & (y_front - derivative_tube[i] * dt_);
      next_y = (*this)[i - 1];
      y_front &= next_y;
    }
  }

  return contraction;
}

bool Tube::ctcFwdBwd(const Tube& derivative_tube, const Interval& initial_value)
{
  bool contraction = false;
  contraction |= ctcFwd(derivative_tube, initial_value);
  contraction |= ctcBwd(derivative_tube);
  return contraction;
}

void Tube::ctcIn_computeIndex(const Interval& t, const Interval& y, int& index_lb, int& index_ub)
{
  if(t.is_unbounded() || t.is_empty())
    cout << "Error ctcIn_computeIndex(...): unbounded or empty [t]" << endl;

  // Slices of index_lb and index_ub strictly enclose the measurement [t]

  index_lb = input2index(t.lb());
  // Special case when the lower bound of [t] equals a bound of a slice or is near empty values
  if(domain(index_lb).lb() == t.lb() && (*this)[max(0, index_lb - 1)].intersects(y))
    index_lb = max(0, index_lb - 1);

  index_ub = input2index(t.ub());
  // Special case when the upper bound of [t] is near empty values
  if(!(*this)[max(0, index_ub)].intersects(y))
    index_ub = max(0, index_ub - 1);
}

bool Tube::ctcIn_base(const Tube& derivative_tube, Interval& y, Interval& t,
                      bool& tube_contracted, bool& y_contracted, bool& t_contracted, bool& bisection_required, bool fwd_bwd)
{
  bool inconsistency = false;
  bisection_required = false;

  int valid_tsubdomains = 0;
  int index_lb, index_ub;
  int min_index_ctc = 0, max_index_ctc = size() - 1; // set bounds to this contractor
  double old_t_diam = t.diam(), old_y_diam = y.diam();

  // Trying to contract [t]

    t = invert((*this)[t] & y, t);

    // The observation [t]x[y] may cross the tube several times
    vector<Interval> v_intv_t;
    invert((*this)[t] & y, v_intv_t, t);

  // Trying to contract [y]

    if(t.is_empty())
      inconsistency = true;

    else
    {
      y &= interpol(t, derivative_tube);

      // Computing index

        ctcIn_computeIndex(t, y, index_lb, index_ub);

        if(!fwd_bwd)
        {
          min_index_ctc = max(0, index_lb - 1);
          max_index_ctc = min(index_ub + 1, size() - 1);
        }

      // Initializations

        map<int,Interval> map_new_y;
        #pragma omp for
        for(int i = min_index_ctc ; i <= max_index_ctc ; i++)
          map_new_y[i] = Interval::EMPTY_SET;

        Interval old_y = y;
        y = Interval::EMPTY_SET;
        t = Interval::EMPTY_SET;

      // Iteration for each [t] subdomain
      for(int k = 0 ; k < v_intv_t.size() ; k++)
      {
        bool local_inconsistency = false;

        Interval local_t = v_intv_t[k];
        Interval local_y = old_y;

        if(v_intv_t.size() > 1) // no need for recomputation of [y] and index if only one [t] subdomain
        {
          if(local_t.is_empty())
          {
            local_y.set_empty();
            continue;
          }

          else
          {
            // Trying to contract [local_y]
            local_y &= interpol(local_t, derivative_tube);

            // Computing new index
            ctcIn_computeIndex(local_t, local_y, index_lb, index_ub);
          }
        }

        // Trying to contract this

          Interval y_front;

          // Backward

            y_front = local_y;
            for(int i = index_ub ; i >= min_index_ctc ; i--)
            {
              Interval integ_domain;
              Interval slice_dom = domain(i);
              double dt_ = slice_dom.diam();
              Interval slice_deriv = derivative_tube[i];

              // Over the ith slice
              integ_domain = Interval(0., min(dt_, local_t.ub() - slice_dom.lb()));
              map_new_y[i] |= (*this)[i] & (y_front - slice_deriv * integ_domain);

              // On the front, backward way
              integ_domain = Interval(min(dt_, max(0., local_t.lb() - slice_dom.lb())), min(dt_, local_t.ub() - slice_dom.lb()));
              y_front = (*this)[slice_dom.lb()] & (y_front - slice_deriv * integ_domain);
            }

          // Forward

            y_front = local_y;
            for(int i = index_lb ; i <= max_index_ctc ; i++)
            {
              Interval integ_domain;
              Interval slice_dom = domain(i);
              double dt_ = slice_dom.diam();
              Interval slice_deriv = derivative_tube[i];

              // Over the ith slice
              integ_domain = Interval(0., min(slice_dom.ub() - local_t.lb(), dt_));
              map_new_y[i] |= (*this)[i] & (y_front + slice_deriv * integ_domain);

              // On the front, forward way
              integ_domain = Interval(min(dt_, max(0., slice_dom.ub() - local_t.ub())), min(dt_, slice_dom.ub() - local_t.lb()));
              y_front = (*this)[slice_dom.ub()] & (y_front + slice_deriv * integ_domain);

            }

          valid_tsubdomains ++;
          y |= local_y;
          t |= local_t;
      } // end of for

      // Synthesis
      {
        Interval prev_y;
        tube_contracted = false;

        // The synthesis is made over two for-loops
        // so that we can stop the iteration if there is no more propagation

        prev_y = Interval::ALL_REALS;
        for(int i = min_index_ctc ; i <= max_index_ctc ; i++) // forward
        {
          if(!prev_y.intersects(map_new_y[i]))
          {
            inconsistency = true;
            break;
          }

          tube_contracted |= map_new_y[i].diam() < (*this)[i].diam();
          prev_y = map_new_y[i];
          set(map_new_y[i], i);
        }
      }
  }

  if(inconsistency)
  {
    //cout << "Warning ctcIn(): inconsistency" << endl;

    #pragma omp for
    for(int i = min_index_ctc ; i <= max_index_ctc ; i++)
      set(Interval::EMPTY_SET, i);

    tube_contracted = true;
    t.set_empty();
    t_contracted = true;
    y.set_empty();
    y_contracted = true;
  }

  else
  {
    bisection_required = valid_tsubdomains > 1;
    t_contracted = t.diam() < old_t_diam;
    y_contracted = y.diam() < old_y_diam;
  }
  
  return tube_contracted | y_contracted | t_contracted;
}

bool Tube::ctcIn(const Tube& derivative_tube, Interval& y, Interval& t, bool fwd_bwd)
{
  bool tube_contracted, y_contracted, t_contracted, bisection_required;
  return ctcIn_base(derivative_tube, y, t, tube_contracted, y_contracted, t_contracted, bisection_required, fwd_bwd);
}

bool Tube::ctcIn(const Tube& derivative_tube, const Interval& y, Interval& t, bool fwd_bwd)
{
  Interval y_temp = y;
  bool tube_contracted, y_contracted, t_contracted, bisection_required;
  ctcIn_base(derivative_tube, y_temp, t, tube_contracted, y_contracted, t_contracted, bisection_required, fwd_bwd);
  return tube_contracted | t_contracted;
}

bool Tube::ctcIn(const Tube& derivative_tube, Interval& y, const Interval& t, bool fwd_bwd)
{
  Interval t_temp = t;
  bool tube_contracted, y_contracted, t_contracted, bisection_required;
  ctcIn_base(derivative_tube, y, t_temp, tube_contracted, y_contracted, t_contracted, bisection_required, fwd_bwd);
  return tube_contracted | y_contracted;
}

bool Tube::ctcIn(const Tube& derivative_tube, const Interval& y, const Interval& t, bool fwd_bwd)
{
  Interval y_temp = y;
  Interval t_temp = t;
  bool tube_contracted, y_contracted, t_contracted, bisection_required;
  ctcIn_base(derivative_tube, y_temp, t_temp, tube_contracted, y_contracted, t_contracted, bisection_required, fwd_bwd);
  return tube_contracted;
}

bool Tube::ctcOut(const Interval& y, const Interval& t)
{
  Interval intersected_t = t & domain();
  bool contraction = false;
  pair<Interval,Interval> enc_bounds = eval(intersected_t);

  if(y.intersects(enc_bounds.first))
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
      #pragma omp for
      for(int i = input2index(intersected_t.lb()) ; i < input2index(intersected_t.ub()) ; i++)
      {
        Interval old_y = (*this)[i];
        Interval new_y(max(y.ub(), old_y.lb()), old_y.ub());
        contraction |= new_y.diam() < old_y.diam();
        set(new_y, i);
      }
    }

  else if(y.intersects(enc_bounds.second))
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
      #pragma omp for
      for(int i = input2index(intersected_t.lb()) ; i < input2index(intersected_t.ub()) ; i++)
      {
        Interval old_y = (*this)[i];
        Interval new_y(old_y.lb(), min(old_y.ub(), y.lb()));
        contraction |= new_y.diam() < old_y.diam();
        set(new_y, i);
      }
    }

  else
    return false; // surely no contraction

  return contraction;
}

bool Tube::ctcIntertemporal(Interval& t1, Interval& t2) const
{
  bool contraction = false;
  double t1_diam, t2_diam;

  do
  {
    t1_diam = t1.diam();
    t2_diam = t2.diam();
    t1 &= invert((*this)[t2], t1);
    t2 &= invert((*this)[t1], t2);
    contraction |= t1.diam() < t1_diam || t2.diam() < t2_diam;
  } while(t1.diam() < t1_diam || t2.diam() < t2_diam);

  return contraction;
}

bool Tube::ctcIntertemporal(Interval& y, Interval& t1, Interval& t2) const
{
  bool contraction = false;
  double y_diam = y.diam(), t1_diam, t2_diam;

  y &= (*this)[t1] | (*this)[t2];

  if(y.is_empty())
  {
    t1.set_empty();
    t2.set_empty();
    return true;
  }

  else
  {
    contraction |= y.diam() < y_diam;

    do
    {
      t1_diam = t1.diam();
      t2_diam = t2.diam();
      t1 &= invert(y, t1);
      t2 &= invert(y, t2);
      contraction |= t1.diam() < t1_diam || t2.diam() < t2_diam;
    } while(t1.diam() < t1_diam || t2.diam() < t2_diam);

    return contraction;
  }
}