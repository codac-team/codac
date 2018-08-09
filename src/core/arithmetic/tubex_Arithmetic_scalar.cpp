/* ============================================================================
 *  tubex-lib - Arithmetic
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "ibex_Interval.h"
#include "tubex_Arithmetic.h"
#include "tubex_DimensionException.h"
#include "tubex_DomainException.h"
#include "tubex_SlicingException.h"
#include "tubex_Slice.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  const Tube operator+(const Tube& x)
  {
    return x;
  }

  const Tube operator-(const Tube& x)
  {
    Tube y(x);
    Slice *slice_y = y.get_first_slice();
    const Slice *slice_x = x.get_first_slice();
    while(slice_y != NULL)
    {
      slice_y->set_envelope(-slice_x->codomain());
      slice_y->set_input_gate(-slice_x->input_gate());
      slice_y = slice_y->next_slice(); slice_x = slice_x->next_slice();
    }
    y.get_last_slice()->set_output_gate(-x.get_last_slice()->output_gate());
    return y;
  }
    
  #define scal_unary(f) \
    \
    const Tube f(const Tube& x) \
    { \
      Tube y(x); \
      Slice *slice_y = y.get_first_slice(); \
      const Slice *slice_x = x.get_first_slice(); \
      while(slice_y != NULL) \
      { \
        slice_y->set_envelope(ibex::f(slice_x->codomain())); \
        slice_y->set_input_gate(ibex::f(slice_x->input_gate())); \
        slice_y = slice_y->next_slice(); slice_x = slice_x->next_slice(); \
      } \
      y.get_last_slice()->set_output_gate(ibex::f(x.get_last_slice()->output_gate())); \
      return y; \
    } \
    \

  scal_unary(cos);
  scal_unary(sin);
  scal_unary(abs);
  scal_unary(sqr);
  scal_unary(sqrt);
  scal_unary(exp);
  scal_unary(log);
  scal_unary(tan);
  scal_unary(acos);
  scal_unary(asin);
  scal_unary(atan);
  scal_unary(cosh);
  scal_unary(sinh);
  scal_unary(tanh);
  scal_unary(acosh);
  scal_unary(asinh);
  scal_unary(atanh);
    
  #define scal_unary_param(f, p) \
    \
    const Tube f(const Tube& x, p param) \
    { \
      Tube y(x); \
      Slice *slice_y = y.get_first_slice(); \
      const Slice *slice_x = x.get_first_slice(); \
      while(slice_y != NULL) \
      { \
        slice_y->set_envelope(ibex::f(slice_x->codomain(), param)); \
        slice_y->set_input_gate(ibex::f(slice_x->input_gate(), param)); \
        slice_y = slice_y->next_slice(); slice_x = slice_x->next_slice(); \
      } \
      y.get_last_slice()->set_output_gate(ibex::f(x.get_last_slice()->output_gate(), param)); \
      return y; \
    } \
    \

  scal_unary_param(pow, int);
  scal_unary_param(pow, double);
  scal_unary_param(pow, const Interval&);
  scal_unary_param(root, int);

  #define scal_binary(f) \
    \
    const Tube f(const Tube& x1, const Tube& x2) \
    { \
      Tube y(x1); \
      Slice *slice_y = y.get_first_slice(); \
      const Slice *slice_x1 = x1.get_first_slice(); \
      const Slice *slice_x2 = x2.get_first_slice(); \
      while(slice_y != NULL) \
      { \
        slice_y->set_envelope(ibex::f(slice_x1->codomain(), slice_x2->codomain())); \
        slice_y->set_input_gate(ibex::f(slice_x1->input_gate(), slice_x2->input_gate())); \
        slice_y = slice_y->next_slice(); slice_x1 = slice_x1->next_slice(); slice_x2 = slice_x2->next_slice(); \
      } \
      y.get_last_slice()->set_output_gate(ibex::f(x1.get_last_slice()->output_gate(), x2.get_last_slice()->output_gate())); \
      return y; \
    } \
    \
    const Tube f(const Tube& x1, const Interval& x2) \
    { \
      Tube y(x1); \
      Slice *slice_y = y.get_first_slice(); \
      const Slice *slice_x1 = x1.get_first_slice(); \
      while(slice_y != NULL) \
      { \
        slice_y->set_envelope(ibex::f(slice_x1->codomain(), x2)); \
        slice_y->set_input_gate(ibex::f(slice_x1->input_gate(), x2)); \
        slice_y = slice_y->next_slice(); slice_x1 = slice_x1->next_slice(); \
      } \
      y.get_last_slice()->set_output_gate(ibex::f(x1.get_last_slice()->output_gate(), x2)); \
      return y; \
    } \
    \
    const Tube f(const Interval& x1, const Tube& x2) \
    { \
      Tube y(x2); \
      Slice *slice_y = y.get_first_slice(); \
      const Slice *slice_x2 = x2.get_first_slice(); \
      while(slice_y != NULL) \
      { \
        slice_y->set_envelope(ibex::f(x1, slice_x2->codomain())); \
        slice_y->set_input_gate(ibex::f(x1, slice_x2->input_gate())); \
        slice_y = slice_y->next_slice(); slice_x2 = slice_x2->next_slice(); \
      } \
      y.get_last_slice()->set_output_gate(ibex::f(x1, x2.get_last_slice()->output_gate())); \
      return y; \
    } \
    \

    scal_binary(operator+);
    scal_binary(operator-);
    scal_binary(operator*);
    scal_binary(operator/);
    scal_binary(operator|);
    scal_binary(operator&);
    scal_binary(atan2);
}