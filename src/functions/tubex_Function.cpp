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

using namespace std;
using namespace ibex;

namespace tubex
{
  Function::Function(const char* y)
  {
    m_ibex_f = new ibex::Function("t", y);
    m_nb_vars = m_ibex_f->nb_var() - 1;
    m_img_dim = m_ibex_f->image_dim();
  }

  Function::Function(const char* x1, const char* y)
  {
    m_ibex_f = new ibex::Function("t", x1, y);
    m_nb_vars = m_ibex_f->nb_var() - 1;
    m_img_dim = m_ibex_f->image_dim();
    // todo: check x1!="t"
  }

  Function::~Function()
  {
    delete m_ibex_f;
  }

  TubeVector Function::eval_vector(const TubeVector& x) const
  {
    // todo: check dim x regarding f. f.imgdim can be of 0 and then x 1 in order to keep slicing pattern
    IntervalVector box(nbVars() + 1); // +1 for system variable (t)
    TubeVector y(x, IntervalVector(imageDim()));

    TubeSlice *x_slice = x.getFirstSlice();
    TubeSlice *y_slice = y.getFirstSlice();

    while(x_slice != NULL)
    {
      // Input gate
      box[0] = x_slice->domain().lb();
      if(nbVars() != 0)
        box.put(1, x_slice->inputGate());
      y_slice->setInputGate(m_ibex_f->eval_vector(box));
      
      // Envelope
      box[0] = x_slice->domain();
      if(nbVars() != 0)
        box.put(1, x_slice->codomain());
      y_slice->setEnvelope(m_ibex_f->eval_vector(box));

      x_slice = x_slice->nextSlice();
      y_slice = y_slice->nextSlice();
    }

    x_slice = x.getLastSlice();
    y_slice = y.getLastSlice();

    // Output gate
    box[0] = x_slice->domain().ub();
    if(nbVars() != 0)
        box.put(1, x_slice->outputGate());
    y_slice->setOutputGate(m_ibex_f->eval_vector(box));

    return y;
  }
}