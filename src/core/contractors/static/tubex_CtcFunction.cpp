/** 
 *  CtcFunction class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_CtcFunction.h"
#include "ibex_CtcFwdBwd.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcFunction::CtcFunction(const Function& f)
    : CtcFwdBwd(*new Function(f))
  {
    // todo: clean delete
  }

  CtcFunction::CtcFunction(const Function& f, const Domain& y)
    : CtcFwdBwd(*new Function(f), y)
  {
    // todo: clean delete
  }
  
  CtcFunction::CtcFunction(const Function& f, const Interval& y)
    : CtcFwdBwd(*new Function(f), y)
  {
    // todo: clean delete
  }

  CtcFunction::CtcFunction(const Function& f, const IntervalVector& y)
    : CtcFwdBwd(*new Function(f), y)
  {
    // todo: clean delete
  }

  void CtcFunction::contract(IntervalVector& x)
  {
    assert(x.size() == nb_var);
    CtcFwdBwd::contract(x);
  }

  void CtcFunction::contract(TubeVector& x)
  {
    assert(x.size() == nb_var);

    Slice **v_x_slices = new Slice*[x.size()];
    for(int i = 0 ; i < x.size() ; i++)
      v_x_slices[i] = x[i].first_slice();

    contract(v_x_slices);
    delete v_x_slices;
  }

  void CtcFunction::contract(Tube& x1)
  {
    assert(nb_var == 1);

    Slice **v_x_slices = new Slice*[nb_var];
    v_x_slices[0] = x1.first_slice();

    contract(v_x_slices);
    delete v_x_slices;
  }

  void CtcFunction::contract(Tube& x1, Tube& x2)
  {
    assert(nb_var == 2);

    Slice **v_x_slices = new Slice*[nb_var];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();

    contract(v_x_slices);
    delete v_x_slices;
  }

  void CtcFunction::contract(Tube& x1, Tube& x2, Tube& x3)
  {
    assert(nb_var == 3);

    Slice **v_x_slices = new Slice*[nb_var];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();

    contract(v_x_slices);
    delete v_x_slices;
  }

  void CtcFunction::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4)
  {
    assert(nb_var == 4);

    Slice **v_x_slices = new Slice*[nb_var];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();
    v_x_slices[3] = x4.first_slice();

    contract(v_x_slices);
    delete v_x_slices;
  }

  void CtcFunction::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5)
  {
    assert(nb_var == 5);

    Slice **v_x_slices = new Slice*[nb_var];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();
    v_x_slices[3] = x4.first_slice();
    v_x_slices[4] = x5.first_slice();

    contract(v_x_slices);
    delete v_x_slices;
  }

  void CtcFunction::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6)
  {
    assert(nb_var == 6);

    Slice **v_x_slices = new Slice*[nb_var];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();
    v_x_slices[3] = x4.first_slice();
    v_x_slices[4] = x5.first_slice();
    v_x_slices[5] = x6.first_slice();

    contract(v_x_slices);
    delete v_x_slices;
  }

  void CtcFunction::contract(Slice **v_x_slices)
  {
    IntervalVector envelope(nb_var);
    IntervalVector ingate(nb_var);

    while(v_x_slices[0] != NULL)
    {
      for(int i = 0 ; i < nb_var ; i++)
      {
        envelope[i] = v_x_slices[i]->codomain();
        ingate[i] = v_x_slices[i]->input_gate();
      }

      CtcFwdBwd::contract(envelope);
      CtcFwdBwd::contract(ingate);

      for(int i = 0 ; i < nb_var ; i++)
      {
        v_x_slices[i]->set_envelope(envelope[i]);
        v_x_slices[i]->set_input_gate(ingate[i]);
      }

      if(v_x_slices[0]->next_slice() == NULL) // output gate
      {
        IntervalVector outgate(nb_var);

        for(int i = 0 ; i < nb_var ; i++)
          outgate[i] = v_x_slices[i]->output_gate();

        CtcFwdBwd::contract(outgate);

        for(int i = 0 ; i < nb_var ; i++)
          v_x_slices[i]->set_output_gate(outgate[i]);

        break; // end of contractions
      }
      
      else
        for(int i = 0 ; i < nb_var ; i++)
          v_x_slices[i] = v_x_slices[i]->next_slice();
    }
  }
}