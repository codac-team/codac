/** 
 *  CtcFwdBwd class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_CtcFwdBwd.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcFwdBwd::CtcFwdBwd(const tubex::Function& f)
    : Ctc(), m_f(new tubex::Function(f)),
      m_ctc_fwdbwd(new ibex::CtcFwdBwd(m_f->ibex_function()))
  {

  }
  
  CtcFwdBwd::~CtcFwdBwd()
  {
    delete m_ctc_fwdbwd;
    delete m_f;
  }

  void CtcFwdBwd::contract(TubeVector& x)
  {
    assert(x.size()+1 == m_ctc_fwdbwd->nb_var);

    Slice **v_x_slices = new Slice*[x.size()];
    for(int i = 0 ; i < x.size() ; i++)
      v_x_slices[i] = x[i].first_slice();

    IntervalVector envelope(x.size() + 1);
    IntervalVector ingate(x.size() + 1);

    while(v_x_slices[0] != NULL)
    {
      envelope[0] = v_x_slices[0]->domain();
      ingate[0] = v_x_slices[0]->domain().lb();

      for(int i = 0 ; i < x.size() ; i++)
      {
        envelope[i+1] = v_x_slices[i]->codomain();
        ingate[i+1] = v_x_slices[i]->input_gate();
      }

      m_ctc_fwdbwd->contract(envelope);
      m_ctc_fwdbwd->contract(ingate);

      for(int i = 0 ; i < x.size() ; i++)
      {
        v_x_slices[i]->set_envelope(envelope[i+1]);
        v_x_slices[i]->set_input_gate(ingate[i+1]);
      }
      
      for(int i = 0 ; i < x.size() ; i++)
        v_x_slices[i] = v_x_slices[i]->next_slice();
    }

    IntervalVector outgate(x.size() + 1);

    for(int i = 0 ; i < x.size() ; i++)
      v_x_slices[i] = x[i].last_slice();

    outgate[0] = v_x_slices[0]->domain().ub();
    for(int i = 0 ; i < x.size() ; i++)
      outgate[i+1] = v_x_slices[i]->output_gate();

    m_ctc_fwdbwd->contract(outgate);

    for(int i = 0 ; i < x.size() ; i++)
      v_x_slices[i]->set_output_gate(outgate[i+1]);
  }
}