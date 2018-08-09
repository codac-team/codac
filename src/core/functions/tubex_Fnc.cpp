/* ============================================================================
 *  tubex-lib - Fnc class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Fnc.h"
#include "tubex_Slice.h"
#include "tubex_TubeVector.h"
#include "tubex_Tube.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Fnc::Fnc()
  {

  }

  Fnc::Fnc(int n, int m) : m_nb_vars(n), m_img_dim(m)
  {

  }

  Fnc::~Fnc()
  {

  }
  
  const Fnc& Fnc::operator=(const Fnc& f)
  {
    m_nb_vars = f.m_nb_vars;
    m_img_dim = f.m_img_dim;
  }

  int Fnc::nb_vars() const
  {
    return m_nb_vars;
  }

  int Fnc::image_dim() const
  {
    return m_img_dim;
  }

  const Tube Fnc::eval(const TubeVector& x) const
  {
    // todo: optimize this?
    return eval_vector(x)[0];
  }

  const TubeVector Fnc::eval_vector(const TubeVector& x) const
  {
    // todo: check dim x regarding f. f.imgdim can be of 0 and then x 1 in order to keep slicing pattern
    TubeVector y(x, IntervalVector(image_dim()));

    // todo: const Slice *x_slice = x.get_first_slice();
    // todo: Slice *y_slice = y.get_first_slice();
    // todo: 
    // todo: while(x_slice != NULL)
    // todo: {
    // todo:   IntervalVector test = eval_vector(x_slice->domain().lb(), x);
    // todo:   y_slice->set_input_gate(eval_vector(x_slice->domain().lb(), x));
    // todo:   y_slice->set_envelope(eval_vector(x_slice->domain(), x));
    // todo:   
    // todo:   x_slice = x_slice->next_slice();
    // todo:   y_slice = y_slice->next_slice();
    // todo: }
    // todo: 
    // todo: x_slice = x.get_last_slice();
    // todo: y_slice = y.get_last_slice();
    // todo: y_slice->set_output_gate(eval_vector(x_slice->domain().ub(), x));
    // todo: 
    // todo: return y;
  }
}