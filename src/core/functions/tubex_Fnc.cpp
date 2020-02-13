/** 
 *  Fnc class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

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

  Fnc::Fnc(int nb_vars, int img_dim, bool is_intertemporal)
    : m_nb_vars(nb_vars), m_img_dim(img_dim), m_is_intertemporal(is_intertemporal)
  {
    assert(img_dim > 0);
  }

  Fnc::~Fnc()
  {

  }
  
  const Fnc& Fnc::operator=(const Fnc& f)
  {
    m_nb_vars = f.m_nb_vars;
    m_img_dim = f.m_img_dim;
    m_is_intertemporal = f.m_is_intertemporal;
  }

  int Fnc::nb_vars() const
  {
    return m_nb_vars;
  }

  int Fnc::image_dim() const
  {
    return m_img_dim;
  }

  bool Fnc::is_intertemporal() const
  {
    return m_is_intertemporal;
  }

  const Tube Fnc::eval(const TubeVector& x) const
  {
    // todo: optimize this?
    return eval_vector(x)[0];
  }

  const TubeVector Fnc::eval_vector(const TubeVector& x) const
  {
    if(nb_vars() != 0)
      assert(x.size() == nb_vars());

    TubeVector y(x); // keeping x's slicing
    y.resize(image_dim());

    if(x.is_empty())
    {
      y.set_empty();
      return y;
    }

    IntervalVector res_codomain(y.size()), res_gate(y.size());
    Slice **v_sy = new Slice*[y.size()];
    for(int i = 0 ; i < y.size() ; i++)
      v_sy[i] = NULL;

    do
    {
      if(v_sy[0] == NULL) // first iteration
        for(int i = 0 ; i < y.size() ; i++)
          v_sy[i] = y[i].first_slice();

      else
        for(int i = 0 ; i < y.size() ; i++)
          v_sy[i] = v_sy[i]->next_slice();

      res_codomain = eval_vector(v_sy[0]->domain(), x);
      res_gate = eval_vector(v_sy[0]->domain().lb(), x);

      for(int i = 0 ; i < y.size() ; i++)
      {
        v_sy[i]->set_envelope(res_codomain[i], false);
        v_sy[i]->set_input_gate(res_gate[i], false);
      }

    } while(v_sy[0]->next_slice() != NULL);
    
    res_gate = eval_vector(v_sy[0]->domain().ub(), x);
    for(int i = 0 ; i < y.size() ; i++)
      v_sy[i]->set_output_gate(res_gate[i], false);

    delete[] v_sy;
    return y;
  }
}