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
#include "tubex_SamplingException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // todo: avoid redundant gate contractions
  
  CtcHC4::CtcHC4()
  {

  }
  
  bool CtcHC4::contract(ibex::CtcHC4& hc4, TubeVector& x) const
  {
    bool ctc = false;
    TubeSlice *slice = x.get_first_slice();

    while(slice != NULL)
    {
      ctc |= contract(hc4, *slice);
      slice = slice->next_slice();
    }

    return ctc;
  }

  bool CtcHC4::contract(ibex::CtcHC4& hc4, TubeSlice& x) const
  {
    if(x.is_empty())
      return false;
    
    TubeSlice x_old = x;
    IntervalVector box = x.box();
    hc4.contract(box);
    x.set_envelope(box.subvector(1, box.size() - 1));

    box[0] = x.domain().lb();
    box.put(1, x_old.input_gate());
    hc4.contract(box);
    x.set_input_gate(box.subvector(1, box.size() - 1));

    box[0] = x.domain().ub();
    box.put(1, x_old.output_gate());
    hc4.contract(box);
    x.set_output_gate(box.subvector(1, box.size() - 1));

    return x_old != x;
  }
}