/** 
 *  CtcStatic class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_CtcStatic.h"
#include "codac_DomainsTypeException.h"

using namespace std;
using namespace ibex;

namespace codac
{
  CtcStatic::CtcStatic(Ctc& static_ctc, bool dynamic_ctc)
    : DynCtc(false), m_static_ctc(static_ctc), m_dynamic_ctc(dynamic_ctc ? 1 : 0)
  {

  }

  // Static members for contractor signature (mainly used for CN Exceptions)
  const string CtcStatic::m_ctc_name = "CtcStatic";
  vector<string> CtcStatic::m_str_expected_doms(
  {
    "Slice[, Slice..]"
  });

  void CtcStatic::contract(vector<Domain*>& v_domains)
  {
    assert(!v_domains.empty());
    assert(!m_dynamic_ctc && "not implemented for inter-temporal constraints");

    for(size_t i = 0 ; i < v_domains.size() ; i++)
    {
      if(v_domains[i]->type() != Domain::Type::T_SLICE)
        throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);
      
      if(i != 0)
        assert(v_domains[i]->slice().tdomain() == v_domains[i-1]->slice().tdomain());
    }

    // If these slices should not be impacted by the contractor
    if(!v_domains[0]->slice().tdomain().intersects(m_restricted_tdomain))
      return;

    int n = v_domains.size();

    IntervalVector envelope(n);
    IntervalVector ingate(n);
    IntervalVector outgate(n);

    for(int i = 0 ; i < n ; i++)
    {
      envelope[i] = v_domains[i]->slice().codomain();
      ingate[i] = v_domains[i]->slice().input_gate();
      outgate[i] = v_domains[i]->slice().output_gate();
    }

    m_static_ctc.contract(envelope);
    m_static_ctc.contract(ingate);
    m_static_ctc.contract(outgate);

    for(int i = 0 ; i < n ; i++)
    {
      v_domains[i]->slice().set_envelope(envelope[i]);
      v_domains[i]->slice().set_input_gate(ingate[i]);
      v_domains[i]->slice().set_output_gate(outgate[i]);
    }
  }

  void CtcStatic::contract(TubeVector& x)
  {
    assert(x.size()+m_dynamic_ctc == m_static_ctc.nb_var);

    Slice **v_x_slices = new Slice*[x.size()];
    for(int i = 0 ; i < x.size() ; i++)
      v_x_slices[i] = x[i].first_slice();

    contract(v_x_slices, x.size());
    delete v_x_slices;
  }

  void CtcStatic::contract(Tube& x1)
  {
    int n = 1;
    assert(n+m_dynamic_ctc == m_static_ctc.nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcStatic::contract(Tube& x1, Tube& x2)
  {
    int n = 2;
    assert(n+m_dynamic_ctc == m_static_ctc.nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcStatic::contract(Tube& x1, Tube& x2, Tube& x3)
  {
    int n = 3;
    assert(n+m_dynamic_ctc == m_static_ctc.nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcStatic::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4)
  {
    int n = 4;
    assert(n+m_dynamic_ctc == m_static_ctc.nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();
    v_x_slices[3] = x4.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcStatic::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5)
  {
    int n = 5;
    assert(n+m_dynamic_ctc == m_static_ctc.nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();
    v_x_slices[3] = x4.first_slice();
    v_x_slices[4] = x5.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcStatic::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6)
  {
    int n = 6;
    assert(n+m_dynamic_ctc == m_static_ctc.nb_var);

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

  void CtcStatic::contract(Slice **v_x_slices, int n)
  {
    IntervalVector envelope(n + m_dynamic_ctc);
    IntervalVector ingate(n + m_dynamic_ctc);

    while(v_x_slices[0] != NULL)
    {
      // If these slices should not be impacted by the contractor
      if(!v_x_slices[0]->tdomain().intersects(m_restricted_tdomain))
      {
        for(int i = 0 ; i < n ; i++)
          v_x_slices[i] = v_x_slices[i]->next_slice();

        // todo: Thin contraction with respect to tube's slicing:
        // the contraction should not be optimal on purpose if the
        // restricted tdomain does not cover the slice's tdomain
        
        continue; // moving to next slice
      }

      if(m_dynamic_ctc)
      {
        envelope[0] = v_x_slices[0]->tdomain();
        ingate[0] = v_x_slices[0]->tdomain().lb();
      }

      for(int i = 0 ; i < n ; i++)
      {
        envelope[i+m_dynamic_ctc] = v_x_slices[i]->codomain();
        ingate[i+m_dynamic_ctc] = v_x_slices[i]->input_gate();
      }

      m_static_ctc.contract(envelope);
      m_static_ctc.contract(ingate);

      for(int i = 0 ; i < n ; i++)
      {
        v_x_slices[i]->set_envelope(envelope[i+m_dynamic_ctc]);
        v_x_slices[i]->set_input_gate(ingate[i+m_dynamic_ctc]);
      }

      if(v_x_slices[0]->next_slice() == NULL) // output gate
      {
        IntervalVector outgate(n + m_dynamic_ctc);

        if(m_dynamic_ctc)
          outgate[0] = v_x_slices[0]->tdomain().ub();

        for(int i = 0 ; i < n ; i++)
          outgate[i+m_dynamic_ctc] = v_x_slices[i]->output_gate();

        m_static_ctc.contract(outgate);

        for(int i = 0 ; i < n ; i++)
          v_x_slices[i]->set_output_gate(outgate[i+m_dynamic_ctc]);

        break; // end of contractions
      }
      
      else
        for(int i = 0 ; i < n ; i++)
          v_x_slices[i] = v_x_slices[i]->next_slice();
    }
  }
}