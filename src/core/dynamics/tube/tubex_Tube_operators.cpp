/* ============================================================================
 *  tubex-lib - Tube class (operators)
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Tube.h"
#include "tubex_Trajectory.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  #define macro_assign_scal(f) \
    \
    const Tube& Tube::f(const Interval& x) \
    { \
      for(Slice *s = first_slice() ; s != NULL ; s = s->next_slice()) \
      { \
        Interval envelope = s->codomain(); \
        Interval ingate = s->input_gate(); \
        s->set_envelope(envelope.f(x)); \
        s->set_input_gate(ingate.f(x)); \
      } \
      Slice *s = last_slice(); \
      Interval outgate = s->output_gate(); \
      s->set_output_gate(outgate.f(x)); \
      return *this; \
    } \
    \
    const Tube& Tube::f(const Trajectory& x) \
    { \
      assert(domain() == x.domain()); \
      for(Slice *s = first_slice() ; s != NULL ; s = s->next_slice()) \
      { \
        Interval envelope = s->codomain(); \
        Interval ingate = s->input_gate(); \
        s->set_envelope(envelope.f(x(s->domain()))); \
        s->set_input_gate(ingate.f(x(Interval(s->domain().lb())))); \
      } \
      Slice *s = last_slice(); \
      Interval outgate = s->output_gate(); \
      s->set_output_gate(outgate.f(x(Interval(s->domain().ub())))); \
      return *this; \
    } \
    \
    const Tube& Tube::f(const Tube& x) \
    { \
      assert(domain() == x.domain()); \
      if(Tube::same_slicing(*this, x)) /* faster */ \
      { \
        Slice *s = first_slice(); \
        const Slice *s_x = x.first_slice(); \
        while(s != NULL) \
        { \
          Interval envelope = s->codomain(); \
          Interval ingate = s->input_gate(); \
          s->set_envelope(envelope.f(s_x->codomain())); \
          s->set_input_gate(ingate.f(s_x->input_gate())); \
          s = s->next_slice(); \
          s_x = s_x->next_slice(); \
        } \
        s = last_slice(); \
        Interval outgate = s->output_gate(); \
        s->set_output_gate(outgate.f(x.last_slice()->output_gate())); \
      } \
      else \
      { \
        Slice *s = first_slice(); \
        Interval s_domain; \
        while(s != NULL) \
        { \
          s_domain = s->domain(); \
          Interval envelope = s->codomain(); \
          Interval ingate = s->input_gate(); \
          s->set_envelope(envelope.f(x(s_domain))); \
          s->set_input_gate(ingate.f(x(s_domain.lb()))); \
          s = s->next_slice(); \
        } \
        s = last_slice(); \
        Interval outgate = s->output_gate(); \
        s->set_output_gate(outgate.f(x(s_domain.ub()))); \
      } \
      return *this; \
    } \
    \

  macro_assign_scal(operator+=);
  macro_assign_scal(operator-=);
  macro_assign_scal(operator*=);
  macro_assign_scal(operator/=);
  macro_assign_scal(operator&=);
  macro_assign_scal(operator|=);
}