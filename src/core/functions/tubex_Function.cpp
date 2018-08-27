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
#include "tubex_Tube.h"
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

  Function::Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* y)
  {
    const char* xdyn[7];
    xdyn[0] = x1;
    xdyn[1] = x2;
    xdyn[2] = x3;
    xdyn[3] = x4;
    xdyn[4] = x5;
    xdyn[5] = x6;
    xdyn[6] = x7;
    construct_from_array(7, xdyn, y);
  }

  Function::Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* y)
  {
    const char* xdyn[8];
    xdyn[0] = x1;
    xdyn[1] = x2;
    xdyn[2] = x3;
    xdyn[3] = x4;
    xdyn[4] = x5;
    xdyn[5] = x6;
    xdyn[6] = x7;
    xdyn[7] = x8;
    construct_from_array(8, xdyn, y);
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

  const Function Function::operator[](int i) const
  {
    // todo: check the following
    tubex::Function fi(*this);
    ibex::Function ibex_fi((*fi.m_ibex_f)[i]);
    delete fi.m_ibex_f;
    fi.m_ibex_f = new ibex::Function(ibex_fi);
    fi.m_img_dim = 1;
    return fi;
  }

  const ibex::Function& Function::ibex_function() const
  {
    return *m_ibex_f;
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

  const Interval Function::eval(const Interval& t) const
  {
    // todo: optimize this?
    return eval_vector(t)[0];
  }

  const Interval Function::eval(const Interval& t, const TubeVector& x) const
  {
    // todo: optimize this?
    return eval_vector(t, x)[0];
  }

  const Interval Function::eval(const Slice& x) const
  {
    // todo: optimize this?
    return eval_vector(x)[0];
  }

  const Tube Function::eval(const TubeVector& x) const
  {
    // todo: optimize this?
    return eval_vector(x)[0];
  }

  const IntervalVector Function::eval_vector(const Interval& t) const
  {
    IntervalVector box(1, t);
    return m_ibex_f->eval_vector(box);
  }

  const IntervalVector Function::eval_vector(const Interval& t, const TubeVector& x) const
  {
    if(nb_vars() == 0)
      return eval_vector(t);

    // todo: check dim x regarding f
    if(x(t).is_empty())
      return IntervalVector(image_dim(), Interval::EMPTY_SET);

    IntervalVector box(nb_vars() + 1); // +1 for system variable (t)
    box[0] = t;
    if(nb_vars() != 0)
      for(int i = 0 ; i < x.size() ; i++)
        box[i+1] = x[i](t);

    return m_ibex_f->eval_vector(box);
  }

  const IntervalVector Function::eval_vector(const Slice& x) const
  {
    return m_ibex_f->eval_vector(x.box());
  }

  const TubeVector Function::eval_vector(const TubeVector& x) const
  {
    // Faster evaluation than the generic Fnc::eval method
    // For now, Function class does not allow inter-temporal evaluations
    // such as delays or integral computations. Hence, the generic method
    // Fnc::eval(Interval t, TubeVector x) can be replaced by a dedicated evaluation

    // todo: update this comment ^

    // todo: check dim x regarding f. f.imgdim can be of 0 and then x 1 in order to keep slicing pattern
    TubeVector y(x); // keeping x's slicing
    y.resize(image_dim());

    if(x.is_empty())
    {
      y.set_empty();
      return y;
    }

    IntervalVector box(x.size() + 1), result(y.size());
    
    const Slice **v_x_slices = new const Slice*[x.size()];
    for(int i = 0 ; i < x.size() ; i++)
      v_x_slices[i] = x[i].get_first_slice();

    Slice **v_y_slices = new Slice*[y.size()];
    for(int i = 0 ; i < y.size() ; i++)
      v_y_slices[i] = y[i].get_first_slice();

    while(v_x_slices[0] != NULL)
    {
      box[0] = v_x_slices[0]->domain();
      for(int i = 0 ; i < x.size() ; i++)
        box[i+1] = v_x_slices[i]->codomain();
      result = m_ibex_f->eval_vector(box);
      for(int i = 0 ; i < y.size() ; i++)
        v_y_slices[i]->set_envelope(result[i]);

      box[0] = box[0].lb();
      for(int i = 0 ; i < x.size() ; i++)
        box[i+1] = v_x_slices[i]->input_gate();
      result = m_ibex_f->eval_vector(box);
      for(int i = 0 ; i < y.size() ; i++)
        v_y_slices[i]->set_input_gate(result[i]);

      for(int i = 0 ; i < x.size() ; i++)
        v_x_slices[i] = v_x_slices[i]->next_slice();
      for(int i = 0 ; i < y.size() ; i++)
        v_y_slices[i] = v_y_slices[i]->next_slice();
    }
    
    for(int i = 0 ; i < x.size() ; i++)
      v_x_slices[i] = x[i].get_last_slice();
    for(int i = 0 ; i < y.size() ; i++)
      v_y_slices[i] = y[i].get_last_slice();

    box[0] = v_x_slices[0]->domain().ub();
    for(int i = 0 ; i < x.size() ; i++)
      box[i+1] = v_x_slices[i]->output_gate();
    result = m_ibex_f->eval_vector(box);
    for(int i = 0 ; i < y.size() ; i++)
      v_y_slices[i]->set_output_gate(result[i]);

    delete[] v_x_slices;
    delete[] v_y_slices;
    return y;
  }
}