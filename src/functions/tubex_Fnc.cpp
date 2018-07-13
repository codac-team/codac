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
#include "tubex_TubeSlice.h"

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

  const TubeVector Fnc::eval(const TubeVector& x) const
  {
    // todo: check dim x regarding f. f.imgdim can be of 0 and then x 1 in order to keep slicing pattern
    TubeVector y(x, IntervalVector(image_dim()));

    const TubeSlice *x_slice = x.get_first_slice();
    TubeSlice *y_slice = y.get_first_slice();

    while(x_slice != NULL)
    {
      y_slice->set_input_gate(eval(x_slice->domain().lb(), x));
      y_slice->set_envelope(eval(x_slice->domain(), x));

      x_slice = x_slice->next_slice();
      y_slice = y_slice->next_slice();
    }

    x_slice = x.get_last_slice();
    y_slice = y.get_last_slice();
    y_slice->set_output_gate(eval(x_slice->domain().ub(), x));

    return y;
  }
}