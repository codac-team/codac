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
  
  bool CtcHC4::contract(Tube& x1, ibex::CtcHC4& hc4)
  {
    bool ctc = false;
    TubeSlice *slice_x1 = x1.getFirstSlice();

    while(slice_x1 != NULL)
    {
      ctc |= contract(*slice_x1, hc4);
      slice_x1 = slice_x1->nextSlice();
    }

    return ctc;
  }
  
  bool CtcHC4::contract(Tube& x1, Tube& x2, ibex::CtcHC4& hc4)
  {
    StructureException::check(x1, x2);
    
    bool ctc = false;
    TubeSlice *slice_x1 = x1.getFirstSlice();
    TubeSlice *slice_x2 = x2.getFirstSlice();

    while(slice_x1 != NULL)
    {
      ctc |= contract(*slice_x1, *slice_x2, hc4);
      slice_x1 = slice_x1->nextSlice();
      slice_x2 = slice_x2->nextSlice();
    }

    return ctc;
  }
  
  bool CtcHC4::contract(Tube& x1, Tube& x2, Tube& x3, ibex::CtcHC4& hc4)
  {
    StructureException::check(x1, x2);
    StructureException::check(x1, x3);

    bool ctc = false;
    TubeSlice *slice_x1 = x1.getFirstSlice();
    TubeSlice *slice_x2 = x2.getFirstSlice();
    TubeSlice *slice_x3 = x3.getFirstSlice();

    while(slice_x1 != NULL)
    {
      ctc |= contract(*slice_x1, *slice_x2, *slice_x3, hc4);
      slice_x1 = slice_x1->nextSlice();
      slice_x2 = slice_x2->nextSlice();
      slice_x3 = slice_x3->nextSlice();
    }

    return ctc;
  }
  
  bool CtcHC4::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, ibex::CtcHC4& hc4)
  {
    StructureException::check(x1, x2);
    StructureException::check(x1, x3);
    StructureException::check(x1, x4);

    bool ctc = false;
    TubeSlice *slice_x1 = x1.getFirstSlice();
    TubeSlice *slice_x2 = x2.getFirstSlice();
    TubeSlice *slice_x3 = x3.getFirstSlice();
    TubeSlice *slice_x4 = x4.getFirstSlice();

    while(slice_x1 != NULL)
    {
      ctc |= contract(*slice_x1, *slice_x2, *slice_x3, *slice_x4, hc4);
      slice_x1 = slice_x1->nextSlice();
      slice_x2 = slice_x2->nextSlice();
      slice_x3 = slice_x3->nextSlice();
      slice_x4 = slice_x4->nextSlice();
    }

    return ctc;
  }

  bool CtcHC4::contract(TubeSlice& x1, ibex::CtcHC4& hc4)
  {
    TubeSlice x1_old = x1;
    IntervalVector box(2);
    
    box[0] = x1.domain();
    box[1] = x1.codomain();
    hc4.contract(box);
    x1.setEnvelope(box[1]);
    
    box[0] = x1.domain().lb();
    box[1] = x1.inputGate();
    hc4.contract(box);
    x1.setInputGate(box[1]);
    
    box[0] = x1.domain().ub();
    box[1] = x1.outputGate();
    hc4.contract(box);
    x1.setOutputGate(box[1]);

    return x1_old != x1;
  }

  bool CtcHC4::contract(TubeSlice& x1, TubeSlice& x2, ibex::CtcHC4& hc4)
  {
    TubeSlice x1_old = x1, x2_old = x2;
    IntervalVector box(3);
    
    box[0] = x1.domain();
    box[1] = x1.codomain();
    box[2] = x2.codomain();
    hc4.contract(box);
    x1.setEnvelope(box[1]);
    x2.setEnvelope(box[2]);
    
    box[0] = x1.domain().lb();
    box[1] = x1.inputGate();
    box[2] = x2.inputGate();
    hc4.contract(box);
    x1.setInputGate(box[1]);
    x2.setInputGate(box[2]);
    
    box[0] = x1.domain().ub();
    box[1] = x1.outputGate();
    box[2] = x2.outputGate();
    hc4.contract(box);
    x1.setOutputGate(box[1]);
    x2.setOutputGate(box[2]);

    return x1_old != x1 || x2_old != x2;
  }

  bool CtcHC4::contract(TubeSlice& x1, TubeSlice& x2, TubeSlice& x3, ibex::CtcHC4& hc4)
  {
    TubeSlice x1_old = x1, x2_old = x2, x3_old = x3;
    IntervalVector box(4);
    
    box[0] = x1.domain();
    box[1] = x1.codomain();
    box[2] = x2.codomain();
    box[3] = x3.codomain();
    hc4.contract(box);
    x1.setEnvelope(box[1]);
    x2.setEnvelope(box[2]);
    x3.setEnvelope(box[3]);
    
    box[0] = x1.domain().lb();
    box[1] = x1.inputGate();
    box[2] = x2.inputGate();
    box[3] = x3.inputGate();
    hc4.contract(box);
    x1.setInputGate(box[1]);
    x2.setInputGate(box[2]);
    x3.setInputGate(box[3]);
    
    box[0] = x1.domain().ub();
    box[1] = x1.outputGate();
    box[2] = x2.outputGate();
    box[3] = x3.outputGate();
    hc4.contract(box);
    x1.setOutputGate(box[1]);
    x2.setOutputGate(box[2]);
    x3.setOutputGate(box[3]);

    return x1_old != x1 || x2_old != x2 || x3_old != x3;
  }

  bool CtcHC4::contract(TubeSlice& x1, TubeSlice& x2, TubeSlice& x3, TubeSlice& x4, ibex::CtcHC4& hc4)
  {
    TubeSlice x1_old = x1, x2_old = x2, x3_old = x3, x4_old = x4;
    IntervalVector box(5);
    
    box[0] = x1.domain();
    box[1] = x1.codomain();
    box[2] = x2.codomain();
    box[3] = x3.codomain();
    box[4] = x4.codomain();
    hc4.contract(box);
    x1.setEnvelope(box[1]);
    x2.setEnvelope(box[2]);
    x3.setEnvelope(box[3]);
    x4.setEnvelope(box[4]);
    
    box[0] = x1.domain().lb();
    box[1] = x1.inputGate();
    box[2] = x2.inputGate();
    box[3] = x3.inputGate();
    box[4] = x4.inputGate();
    hc4.contract(box);
    x1.setInputGate(box[1]);
    x2.setInputGate(box[2]);
    x3.setInputGate(box[3]);
    x4.setInputGate(box[4]);
    
    box[0] = x1.domain().ub();
    box[1] = x1.outputGate();
    box[2] = x2.outputGate();
    box[3] = x3.outputGate();
    box[4] = x4.outputGate();
    hc4.contract(box);
    x1.setOutputGate(box[1]);
    x2.setOutputGate(box[2]);
    x3.setOutputGate(box[3]);
    x4.setOutputGate(box[4]);

    return x1_old != x1 || x2_old != x2 || x3_old != x3 || x4_old != x4;
  }
}