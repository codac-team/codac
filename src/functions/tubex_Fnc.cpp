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

  int Fnc::nbVars() const
  {
    return m_nb_vars;
  }

  int Fnc::imageDim() const
  {
    return m_img_dim;
  }
  
  const IntervalVector Fnc::eval(double t, const ibex::IntervalVector& x) const
  {
    return eval(Interval(t), x);
  }

  const TubeVector Fnc::eval(const TubeVector& x) const
  {
    // todo: check dim x regarding f. f.imgdim can be of 0 and then x 1 in order to keep slicing pattern
    TubeVector y(x, IntervalVector(imageDim()));

    const TubeSlice *x_slice = x.getFirstSlice();
    TubeSlice *y_slice = y.getFirstSlice();

    while(x_slice != NULL)
    {
      y_slice->setInputGate(eval(x_slice->domain().lb(), x_slice->inputGate()));
      y_slice->setEnvelope(eval(x_slice->domain(), x_slice->codomain()));

      x_slice = x_slice->nextSlice();
      y_slice = y_slice->nextSlice();
    }

    x_slice = x.getLastSlice();
    y_slice = y.getLastSlice();
    y_slice->setOutputGate(eval(x_slice->domain().ub(), x_slice->outputGate()));

    return y;
  }
}