/* ============================================================================
 *  tubex-lib - CtcHC4 class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_CtcHC4.h"
#include "tubex_SlicingException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // todo: avoid redundant gate contractions
  
  CtcHC4::CtcHC4(bool preserve_slicing)
    : Ctc(preserve_slicing)
  {

  }
  
  bool CtcHC4::contract(ibex::CtcHC4& hc4, TubeVector& x) const
  {
    if(x.is_empty())
      return false;

    Slice **v_x_slices = new Slice*[x.size()];
    for(int i = 0 ; i < x.size() ; i++)
      v_x_slices[i] = x[i].get_first_slice();

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

      hc4.contract(envelope);
      hc4.contract(ingate);

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
      v_x_slices[i] = x[i].get_last_slice();

    outgate[0] = v_x_slices[0]->domain().ub();
    for(int i = 0 ; i < x.size() ; i++)
      outgate[i+1] = v_x_slices[i]->output_gate();

    hc4.contract(outgate);

    for(int i = 0 ; i < x.size() ; i++)
      v_x_slices[i]->set_output_gate(outgate[i+1]);

    // todo: return value
  }
}