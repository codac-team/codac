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
  CtcFunction::CtcFunction(const char* x1, const char* f)
    : Ctc3BCid(*new CtcFwdBwd(*new Function(x1, CtcFunction::parse_f(f).c_str())))
  {
    // todo: clean delete
  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* f)
    : Ctc3BCid(*new CtcFwdBwd(*new Function(x1, x2, CtcFunction::parse_f(f).c_str())))
  {

  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* x3, const char* f)
    : Ctc3BCid(*new CtcFwdBwd(*new Function(x1, x2, x3, CtcFunction::parse_f(f).c_str())))
  {

  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* f)
    : Ctc3BCid(*new CtcFwdBwd(*new Function(x1, x2, x3, x4, CtcFunction::parse_f(f).c_str())))
  {

  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* f)
    : Ctc3BCid(*new CtcFwdBwd(*new Function(x1, x2, x3, x4, x5, CtcFunction::parse_f(f).c_str())))
  {

  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* f)
    : Ctc3BCid(*new CtcFwdBwd(*new Function(x1, x2, x3, x4, x5, x6, CtcFunction::parse_f(f).c_str())))
  {

  }

  CtcFunction::CtcFunction(const Function& f, const Domain& y)
    : Ctc3BCid(*new CtcFwdBwd(*new Function(f), y))
  {

  }
  
  CtcFunction::CtcFunction(const Function& f, const Interval& y)
    : Ctc3BCid(*new CtcFwdBwd(*new Function(f), y))
  {

  }

  CtcFunction::CtcFunction(const Function& f, const IntervalVector& y)
    : Ctc3BCid(*new CtcFwdBwd(*new Function(f), y))
  {

  }

  const string CtcFunction::parse_f(const char* f)
  {
    string str_f(f);
    string str_eq("=");

    if(str_f.find(str_eq) != string::npos)
    {
      str_f.replace(str_f.find(str_eq), str_eq.length(), "-(");
      str_f += ")";
    }

    assert(str_f.find(str_eq) == string::npos && "only one '=' allowed");
    assert(str_f.find(">") == string::npos
        && str_f.find("<") == string::npos
        && "inequalities not supported by this contractor");

    return str_f;
  }

  void CtcFunction::contract(IntervalVector& x)
  {
    assert(x.size() == nb_var);
    Ctc3BCid::contract(x);
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

      Ctc3BCid::contract(envelope);
      Ctc3BCid::contract(ingate);

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

        Ctc3BCid::contract(outgate);

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