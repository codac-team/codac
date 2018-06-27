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
#include "tubex_StructureException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // todo: avoid redundant gate contractions
  
  CtcHC4::CtcHC4()
  {

  }
  
  bool CtcHC4::contract(ibex::CtcHC4& hc4, TubeVector& x)
  {
    bool ctc = false;
    TubeSlice *slice = x.getFirstSlice();

    while(slice != NULL)
    {
      ctc |= contract(hc4, *slice);
      slice = slice->nextSlice();
    }

    return ctc;
  }

  bool CtcHC4::contract(ibex::CtcHC4& hc4, TubeSlice& x)
  {
    TubeSlice x_old = x;
    IntervalVector box = x.box();
    hc4.contract(box);
    x.setEnvelope(box.subvector(1, box.size() - 1));

    box[0] = x.domain().lb();
    box.put(1, x_old.inputGate());
    hc4.contract(box);
    x.setInputGate(box.subvector(1, box.size() - 1));

    box[0] = x.domain().ub();
    box.put(1, x_old.outputGate());
    hc4.contract(box);
    x.setOutputGate(box.subvector(1, box.size() - 1));

    return x_old != x;
  }
}