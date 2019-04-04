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

    contract(v_x_slices, x.size());
    delete v_x_slices;
  }

  void CtcFwdBwd::contract(Tube& x1)
  {
    int n = 1;
    assert(n+1 == m_ctc_fwdbwd->nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcFwdBwd::contract(Tube& x1, Tube& x2)
  {
    int n = 2;
    assert(n+1 == m_ctc_fwdbwd->nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcFwdBwd::contract(Tube& x1, Tube& x2, Tube& x3)
  {
    int n = 3;
    assert(n+1 == m_ctc_fwdbwd->nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcFwdBwd::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4)
  {
    int n = 4;
    assert(n+1 == m_ctc_fwdbwd->nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();
    v_x_slices[3] = x4.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcFwdBwd::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5)
  {
    int n = 5;
    assert(n+1 == m_ctc_fwdbwd->nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();
    v_x_slices[3] = x4.first_slice();
    v_x_slices[4] = x5.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcFwdBwd::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6)
  {
    int n = 6;
    assert(n+1 == m_ctc_fwdbwd->nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();
    v_x_slices[3] = x4.first_slice();
    v_x_slices[4] = x5.first_slice();
    v_x_slices[5] = x6.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcFwdBwd::contract(Slice **v_x_slices, int n)
  {
    IntervalVector envelope(n + 1);
    IntervalVector ingate(n + 1);

    while(v_x_slices[0] != NULL)
    {
      envelope[0] = v_x_slices[0]->domain();
      ingate[0] = v_x_slices[0]->domain().lb();

      for(int i = 0 ; i < n ; i++)
      {
        envelope[i+1] = v_x_slices[i]->codomain();
        ingate[i+1] = v_x_slices[i]->input_gate();
      }

      m_ctc_fwdbwd->contract(envelope);
      m_ctc_fwdbwd->contract(ingate);

      for(int i = 0 ; i < n ; i++)
      {
        v_x_slices[i]->set_envelope(envelope[i+1]);
        v_x_slices[i]->set_input_gate(ingate[i+1]);
      }

      if(v_x_slices[0]->next_slice() == NULL) // output gate
      {
        IntervalVector outgate(n + 1);

        outgate[0] = v_x_slices[0]->domain().ub();
        for(int i = 0 ; i < n ; i++)
          outgate[i+1] = v_x_slices[i]->output_gate();

        m_ctc_fwdbwd->contract(outgate);

        for(int i = 0 ; i < n ; i++)
          v_x_slices[i]->set_output_gate(outgate[i+1]);

        break; // end of contractions
      }
      
      else
        for(int i = 0 ; i < n ; i++)
          v_x_slices[i] = v_x_slices[i]->next_slice();
    }
  }
}