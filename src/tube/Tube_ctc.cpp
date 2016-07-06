/* ============================================================================
 *  tube-lib - Tube class
 * ============================================================================
 *  Copyright : Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the MIT license. See the file LICENSE.
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
    Interval y_new = y_old & (y_front + derivative_tube[i] * Interval(0., derivative_tube.getT(i).diam()));
    setY(y_new, i);
    contraction |= y_old.diam() > y_new.diam();

    y_front = y_old & (y_front + derivative_tube[i] * derivative_tube.getT(i).diam());
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
  y_front &= (*this)[size() - 1] - derivative_tube[size() - 1] * derivative_tube.getT(size() - 1).diam();

  for(int i = size() - 2 ; i >= 0 ; i--) // from the future to the past
  {
    Interval y_old = (*this)[i];
    Interval y_new = y_old & (y_front - derivative_tube[i] * Interval(0., derivative_tube.getT(i).diam()));
    setY(y_new, i);
    contraction |= y_old.diam() > y_new.diam();

    y_front = y_old & (y_front - derivative_tube[i] * derivative_tube.getT(i).diam());
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
  bool contraction;
  bool t_contraction = false;
  bool y_contraction = false;
  bool tube_contraction = false;

  if(!(*this)[t].intersects(y))
  {
    cout << "Warning ctcIn(const Tube& derivative_tube, Interval& y, Interval& t): this[t] do not intersect y" << endl;
    cout << "        this[t]=" << (*this)[t] << ", t=" << t << ", y=" << y << endl;
    return false;
  }

  // Trying to contract [t]
  {
    double t_diam = t.diam();
    t &= getT();

    // In forward
    int i = input2index(t.lb());
    while(!(*this)[i].intersects(y) && i < input2index(t.ub()))
    {
      i++;
      t = Interval(getT(i).lb(), t.ub()); // updating lower-bound (contraction)
    }

    // In backward
    i = input2index(t.ub());
    while(!(*this)[i].intersects(y) && i > input2index(t.lb()))
    {
      i--;
      t = Interval(t.lb(), getT(i).ub()); // updating upper-bound (contraction)
    }

    t_contraction = t.diam() < t_diam;
  }

  // Trying to contract [y]
  {
    double y_diam = y.diam();
    y &= (*this)[t];
    y_contraction = y.diam() < y_diam;
  }

  // Trying to contract this
  {
    Tube tube_temp1(*this);

    // Raw contraction without derivative information
    for(int i = input2index(t.lb()) ; i <= input2index(t.ub()) ; i++)
    {
      Interval old_y = (*this)[i];
      Interval new_y = old_y & y;
      tube_temp1.setY(new_y, i);
    }

    // Taking derivative information into account
    Tube tube_temp2(tube_temp1);
    for(int i = input2index(t.lb()) ; i <= input2index(t.ub()) ; i++)
    {
      Tube tube_temp3(tube_temp1);
      Interval new_y = tube_temp3[i];

      // In forward
      for(int j = i ; j <= input2index(t.ub()) ; j++)
      {
        Interval deriv = derivative_tube[j];
        Interval deriv_t = derivative_tube.getT(j);
        new_y += Interval(deriv.lb() * deriv_t.diam(), deriv.ub() * deriv_t.diam());
        tube_temp3.setY(new_y, j);
      }
      new_y = tube_temp1[i];

      // In backward
      for(int j = i ; j >= input2index(t.lb()) ; j--)
      {
        Interval deriv = derivative_tube[j];
        Interval deriv_t = derivative_tube.getT(j);
        new_y -= Interval(deriv.lb() * deriv_t.diam(), deriv.ub() * deriv_t.diam());
        tube_temp3.setY(new_y | tube_temp3[j], j); // an union is made to keep forward's update
      }

      tube_temp2 |= tube_temp3;
    }

    double vol = volume();
    (*this) &= tube_temp2;
    tube_contraction = volume() < vol;
  }

  contraction = t_contraction | tube_contraction | y_contraction;

  return contraction;
}

bool Tube::ctcOut(const Interval& y, const Interval& t)
{
  double volume_before_ctc = volume();
  pair<Interval,Interval> enc_bounds = getEnclosedBounds(t);

  if(y.intersects(enc_bounds.first))
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
      #pragma omp for
      for(int i = input2index(t.lb()) ; i < input2index(t.ub()) ; i++)
      {
        Interval y_i = (*this)[i];
        setY(Interval(max(y.ub(), y_i.lb()), y_i.ub()), i);
      }
    }

  else if(y.intersects(enc_bounds.second))
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
      #pragma omp for
      for(int i = input2index(t.lb()) ; i < input2index(t.ub()) ; i++)
      {
        Interval y_i = (*this)[i];
        setY(Interval(y_i.lb(), min(y_i.ub(), y.lb())), i);
      }
    }

  else
    return false; // surely no contraction

  return volume() < volume_before_ctc;
}