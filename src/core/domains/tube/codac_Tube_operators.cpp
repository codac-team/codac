/** 
 *  Tube class (operators)
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Tube.h"
#include "codac_Trajectory.h"

using namespace std;
using namespace ibex;

namespace codac
{
  #define macro_assign_scal(f) \
    \
    const Tube& Tube::f(const Interval& x) \
    { \
      Slice *s = nullptr; \
      do \
      { \
        if(s == nullptr) /* first iteration */ \
          s = first_slice(); \
        else \
          s = s->next_slice(); \
        \
        s->set_envelope(Interval(s->codomain()).f(x), false); \
        s->set_input_gate(Interval(s->input_gate()).f(x), false); \
        \
      } while(s->next_slice()); \
      \
      s->set_output_gate(Interval(s->output_gate()).f(x), false); \
      return *this; \
    } \
    \
    const Tube& Tube::f(const Trajectory& x) \
    { \
      assert(tdomain() == x.tdomain()); \
      \
      Slice *s = nullptr; \
      do \
      { \
        if(s == nullptr) /* first iteration */ \
          s = first_slice(); \
        else \
          s = s->next_slice(); \
        \
        s->set_envelope(Interval(s->codomain()).f(x(s->tdomain())), false); \
        s->set_input_gate(Interval(s->input_gate()).f(x(Interval(s->tdomain().lb()))), false); \
         \
      } while(s->next_slice()); \
      \
      s->set_output_gate(Interval(s->output_gate()).f(x(Interval(s->tdomain().ub()))), false); \
      return *this; \
    } \
    \
    const Tube& Tube::f(const Tube& x) \
    { \
      assert(tdomain() == x.tdomain()); \
      \
      if(Tube::same_slicing(*this, x)) /* faster */ \
      { \
        Slice *s = nullptr; \
        const Slice *s_x = nullptr; \
        do \
        { \
          if(s == nullptr) /* first iteration */ \
          { \
            s = first_slice(); \
            s_x = x.first_slice(); \
          } \
          \
          else \
          {  \
            s = s->next_slice(); \
            s_x = s_x->next_slice(); \
          } \
          \
          s->set_envelope(Interval(s->codomain()).f(s_x->codomain()), false); \
          s->set_input_gate(Interval(s->input_gate()).f(s_x->input_gate()), false); \
        } while(s->next_slice()); \
        \
        s->set_output_gate(Interval(s->output_gate()).f((s_x->output_gate())), false); \
      } \
      \
      else \
      { \
        Slice *s = nullptr; \
        do \
        { \
          if(s == nullptr) /* first iteration */ \
            s = first_slice(); \
          else \
            s = s->next_slice(); \
          \
          s->set_envelope(Interval(s->codomain()).f(x(s->tdomain())), false); \
          s->set_input_gate(Interval(s->input_gate()).f(x(s->tdomain().lb())), false); \
          \
        } while(s->next_slice()); \
        \
        s->set_output_gate(Interval(s->output_gate()).f(x(s->tdomain().ub())), false); \
      } \
      \
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