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
  if(size() != derivative_tube.size())
    cout << "Warning ctcFwd(const Tube& derivative_tube): tube of different size: "
         << derivative_tube.size() << "/" << size() << endl;

  for(int i = 0 ; i < size() ; i++)
    if(derivative_tube[i].is_empty())
      cout << "Warning ctcFwd(const Tube& derivative_tube): derivative_tube[" << i << "] is empty" << endl;

  bool contraction = false;

  Interval y_front = (*this)[0] & initial_value;

  for(int i = 0 ; i < size() ; i++) // from the past to the future
  {
    Interval y_old = (*this)[i];
    Interval y_new = y_old & (y_front + derivative_tube[i] * Interval(0., derivative_tube.domain(i).diam()));
    set(y_new, i);
    contraction |= y_old.diam() > y_new.diam();

    y_front = y_old & (y_front + derivative_tube[i] * derivative_tube.domain(i).diam());
    if(i < size()-1)
      y_front &= (*this)[i+1];
  }

  return contraction;
}

bool Tube::ctcBwd(const Tube& derivative_tube)
{
  if(size() != derivative_tube.size())
    cout << "Warning ctcBwd(const Tube& derivative_tube): tube of different size: "
         << derivative_tube.size() << "/" << size() << endl;

  for(int i = 0 ; i < size() ; i++)
    if(derivative_tube[i].is_empty())
      cout << "Warning ctcBwd(const Tube& derivative_tube): derivative_tube[" << i << "] is empty" << endl;

  bool contraction = false;

  Interval y_front = (*this)[size() - 1];
  y_front &= (*this)[size() - 1] - derivative_tube[size() - 1] * derivative_tube.domain(size() - 1).diam();

  for(int i = size() - 2 ; i >= 0 ; i--) // from the future to the past
  {
    Interval y_old = (*this)[i];
    Interval y_new = y_old & (y_front - derivative_tube[i] * Interval(0., derivative_tube.domain(i).diam()));
    set(y_new, i);
    contraction |= y_old.diam() > y_new.diam();

    y_front = y_old & (y_front - derivative_tube[i] * derivative_tube.domain(i).diam());
    if(i > 0)
      y_front &= (*this)[i-1];
  }

  return contraction;
}

bool Tube::ctcFwdBwd(const Tube& derivative_tube)
{
  bool contraction = false;
  contraction |= ctcFwd(derivative_tube);
  contraction |= ctcBwd(derivative_tube);
  return contraction;
}

bool Tube::ctcIn(const Tube& derivative_tube, Interval& y, Interval& t)
{
  bool tube_contraction = false;

  double old_t_diam = t.diam();
  double old_y_diam = y.diam();

  // Trying to contract [t]

    t = invert((*this)[t] & y, t);

  // Trying to contract [y]

    if(t.is_empty())
      y = Interval::EMPTY_SET;

    else
      y &= (*this)[t];

  // Trying to contract this

    if(t.is_empty() || y.is_empty())
    {
      tube_contraction = !isEmpty();
      set(Interval::EMPTY_SET);
    }

    else
    {
      // Forward
      map<int,Interval> map_fwd;
      Interval integral_fwd(0.);

      for(int i = input2index(t.lb()) ; i <= input2index(t.ub()) ; i++)
      {
        Interval domain_i = domain(i);
        double t_diam_part = max((domain_i & t).diam(), domain_i.diam() - (domain_i & t).diam());
        integral_fwd += t_diam_part * derivative_tube[i];
        map_fwd[i] = integral_fwd;
      }

      // Backward
      map<int,Interval> map_bwd;
      Interval integral_bwd(0.);
      
      for(int i = input2index(t.ub()) ; i >= input2index(t.lb()) ; i--)
      {
        Interval domain_i = domain(i);
        double t_diam_part = max((domain_i & t).diam(), domain_i.diam() - (domain_i & t).diam());
        integral_bwd -= t_diam_part * derivative_tube[i];
        map_bwd[i] = integral_bwd;
      }
      
      // Contraction
      for(int i = input2index(t.lb()) ; i <= input2index(t.ub()) ; i++)
      {
        Interval this_i = (*this)[i];
        Interval y_contraction = this_i & (y + (map_bwd[i] | map_fwd[i]));
        tube_contraction |= y_contraction.diam() < this_i.diam();
        set(y_contraction, i);
      }
    }

  return old_t_diam > t.diam() || old_y_diam > y.diam() || tube_contraction;
}

bool Tube::ctcIn(const Tube& derivative_tube, const Interval& y, Interval& t)
{
  double tube_vol = volume();
  double t_diam = t.diam();

  Interval y_temp = y;

  if(!ctcIn(derivative_tube, y_temp, t))
    return false;

  return volume() < tube_vol || t.diam() < t_diam;
}

bool Tube::ctcIn(const Tube& derivative_tube, Interval& y, const Interval& t)
{
  double tube_vol = volume();
  double y_diam = y.diam();

  Interval t_temp = t;
  
  if(!ctcIn(derivative_tube, y, t_temp))
    return false;

  return volume() < tube_vol || y.diam() < y_diam;
}

bool Tube::ctcIn(const Tube& derivative_tube, const Interval& y, const Interval& t)
{
  double tube_vol = volume();

  Interval y_temp = y;
  Interval t_temp = t;
  
  if(!ctcIn(derivative_tube, y_temp, t_temp))
    return false;

  return volume() < tube_vol;
}

bool Tube::ctcOut(const Interval& y, const Interval& t)
{
  double volume_before_ctc = volume();
  pair<Interval,Interval> enc_bounds = eval(t);

  if(y.intersects(enc_bounds.first))
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
      #pragma omp for
      for(int i = input2index(t.lb()) ; i < input2index(t.ub()) ; i++)
      {
        Interval y_i = (*this)[i];
        set(Interval(max(y.ub(), y_i.lb()), y_i.ub()), i);
      }
    }

  else if(y.intersects(enc_bounds.second))
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
      #pragma omp for
      for(int i = input2index(t.lb()) ; i < input2index(t.ub()) ; i++)
      {
        Interval y_i = (*this)[i];
        set(Interval(y_i.lb(), min(y_i.ub(), y.lb())), i);
      }
    }

  else
    return false; // surely no contraction

  return volume() < volume_before_ctc;
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