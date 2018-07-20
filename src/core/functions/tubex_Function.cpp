/* ============================================================================
 *  tubex-lib - Function class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#include "tubex_Function.h"
#include "tubex_TubeVector.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Function::Function(int n, const char** x, const char* y)
  {
    construct_from_array(n, x, y);
  }

  Function::Function(const char* y)
  {
    construct_from_array(0, NULL, y);
  }

  Function::Function(const char* x1, const char* y)
  {
    const char* xdyn[1];
    xdyn[0] = x1;
    construct_from_array(1, xdyn, y);
  }

  Function::Function(const char* x1, const char* x2, const char* y)
  {
    const char* xdyn[2];
    xdyn[0] = x1;
    xdyn[1] = x2;
    construct_from_array(2, xdyn, y);
  }

  Function::Function(const char* x1, const char* x2, const char* x3, const char* y)
  {
    const char* xdyn[3];
    xdyn[0] = x1;
    xdyn[1] = x2;
    xdyn[2] = x3;
    construct_from_array(3, xdyn, y);
  }

  Function::Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* y)
  {
    const char* xdyn[4];
    xdyn[0] = x1;
    xdyn[1] = x2;
    xdyn[2] = x3;
    xdyn[3] = x4;
    construct_from_array(4, xdyn, y);
  }

  Function::Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* y)
  {
    const char* xdyn[5];
    xdyn[0] = x1;
    xdyn[1] = x2;
    xdyn[2] = x3;
    xdyn[3] = x4;
    xdyn[4] = x5;
    construct_from_array(5, xdyn, y);
  }

  Function::Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* y)
  {
    const char* xdyn[6];
    xdyn[0] = x1;
    xdyn[1] = x2;
    xdyn[2] = x3;
    xdyn[3] = x4;
    xdyn[4] = x5;
    xdyn[5] = x6;
    construct_from_array(6, xdyn, y);
  }

  Function::Function(const tubex::Function& f)
    : tubex::Fnc(f.nb_vars(), f.image_dim())
  {
    *this = f;
  }

  Function::~Function()
  {
    delete m_ibex_f;
  }

  const Function& Function::operator=(const Function& f)
  {
    if(m_ibex_f != NULL)
      delete m_ibex_f;
    m_ibex_f = new ibex::Function(*f.m_ibex_f);
    Fnc::operator=(f);
  }

  const IntervalVector Function::eval(const Interval& t) const
  {
    IntervalVector box(1, t);
    return m_ibex_f->eval_vector(box);
  }

  const IntervalVector Function::eval(const Interval& t, const TubeVector& x) const
  {
    if(nb_vars() == 0)
      return eval(t);

    // todo: check dim x regarding f
    if(x[t].is_empty())
      return IntervalVector(image_dim(), Interval::EMPTY_SET);

    IntervalVector box(nb_vars() + 1); // +1 for system variable (t)
    box[0] = t;
    if(nb_vars() != 0)
      box.put(1, x[t]);
    return m_ibex_f->eval_vector(box);
  }

  void Function::construct_from_array(int n, const char** x, const char* y)
  {
    const char* xdyn[n+1];
    xdyn[0] = "t";
    for(int i = 0 ; i < n ; i++) xdyn[i+1] = x[i];

    m_ibex_f = new ibex::Function(n+1, xdyn, y);
    m_nb_vars = n;
    m_img_dim = m_ibex_f->image_dim();
  }
}