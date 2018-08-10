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


    for(int k = 0 ; k < x.nb_slices() ; k++)
    {
      IntervalVector envelope(x.size() + 1);
      IntervalVector ingate(x.size() + 1);
      IntervalVector outgate(x.size() + 1);

      Slice *slice;

      for(int i = 0 ; i < x.size() ; i++) // accessing values
      {
        slice = x[i].get_slice(k);
        envelope[i+1] = slice->codomain();
        ingate[i+1] = slice->input_gate();
        outgate[i+1] = slice->output_gate();
      }
      
      envelope[0] = slice->domain();
      ingate[0] = slice->domain().lb();
      outgate[0] = slice->domain().ub();

      hc4.contract(envelope);
      hc4.contract(ingate);
      hc4.contract(outgate);

      for(int i = 0 ; i < x.size() ; i++) // updating values
      {
        slice = x[i].get_slice(k);
        slice->set_envelope(envelope[i+1]);
        slice->set_input_gate(ingate[i+1]);
        slice->set_output_gate(outgate[i+1]);
      }
    }

    // todo: return value
  }
}