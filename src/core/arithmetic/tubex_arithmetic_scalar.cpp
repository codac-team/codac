/** 
 *  Arithmetic operations
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \bugfixes   Bertrand Neveu
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_arithmetic.h"
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
    Slice *slice_y = y.first_slice();
    const Slice *slice_x = x.first_slice();
    while(slice_y != NULL)
    {
      slice_y->set_envelope(-slice_x->codomain());
      slice_y->set_input_gate(-slice_x->input_gate());
      slice_y = slice_y->next_slice(); slice_x = slice_x->next_slice();
    }
    y.last_slice()->set_output_gate(-x.last_slice()->output_gate());
    return y;
  }
    
  #define macro_scal_unary(f) \
    \
    const Tube f(const Tube& x) \
    { \
      Tube y(x); \
      Slice *slice_y = y.first_slice(); \
      const Slice *slice_x = x.first_slice(); \
      while(slice_y != NULL) \
      { \
        slice_y->set_envelope(ibex::f(slice_x->codomain())); \
        slice_y->set_input_gate(ibex::f(slice_x->input_gate())); \
        slice_y = slice_y->next_slice(); slice_x = slice_x->next_slice(); \
      } \
      y.last_slice()->set_output_gate(ibex::f(x.last_slice()->output_gate())); \
      return y; \
    } \
    \

  macro_scal_unary(cos);
  macro_scal_unary(sin);
  macro_scal_unary(abs);
  macro_scal_unary(sqr);
  macro_scal_unary(sqrt);
  macro_scal_unary(exp);
  macro_scal_unary(log);
  macro_scal_unary(tan);
  macro_scal_unary(acos);
  macro_scal_unary(asin);
  macro_scal_unary(atan);
  macro_scal_unary(cosh);
  macro_scal_unary(sinh);
  macro_scal_unary(tanh);
  macro_scal_unary(acosh);
  macro_scal_unary(asinh);
  macro_scal_unary(atanh);
    
  #define macro_scal_unary_param(f, p) \
    \
    const Tube f(const Tube& x, p param) \
    { \
      Tube y(x); \
      Slice *slice_y = y.first_slice(); \
      const Slice *slice_x = x.first_slice(); \
      while(slice_y != NULL) \
      { \
        slice_y->set_envelope(ibex::f(slice_x->codomain(), param)); \
        slice_y->set_input_gate(ibex::f(slice_x->input_gate(), param)); \
        slice_y = slice_y->next_slice(); slice_x = slice_x->next_slice(); \
      } \
      y.last_slice()->set_output_gate(ibex::f(x.last_slice()->output_gate(), param)); \
      return y; \
    } \
    \

  macro_scal_unary_param(pow, int);
  macro_scal_unary_param(pow, double);
  macro_scal_unary_param(pow, const Interval&);
  macro_scal_unary_param(root, int);

  #define macro_scal_binary(f) \
    \
    const Tube f(const Tube& x1, const Tube& x2) \
    { \
      assert(x1.domain() == x2.domain()); \
      Tube y(x1); \
      const Slice *slice_x1, *slice_x2; \
      \
      Tube *x1_resampled = NULL; /* In case of different slicing between x1 and x2, */ \
      Tube *x2_resampled = NULL; /* copies of x1 and x2 will be made and equally resampled. */ \
      \
      if(Tube::same_slicing(x1, x2)) /* faster, no sampling computation needed */ \
      { \
        slice_x1 = x1.first_slice(); \
        slice_x2 = x2.first_slice(); \
      } \
      \
      else \
      { \
        x1_resampled = new Tube(x1); \
        x2_resampled = new Tube(x2); \
        x1_resampled->sample(x2); /* common sampling */ \
        x2_resampled->sample(x1); \
        y.sample(x2); \
        slice_x1 = x1_resampled->first_slice(); \
        slice_x2 = x2_resampled->first_slice(); \
      } \
      \
      Slice *slice_y = y.first_slice(); \
      while(slice_y != NULL) \
      { \
        slice_y->set_envelope(ibex::f(slice_x1->codomain(), slice_x2->codomain())); \
        slice_y->set_input_gate(ibex::f(slice_x1->input_gate(), slice_x2->input_gate())); \
        slice_y = slice_y->next_slice(); slice_x1 = slice_x1->next_slice(); slice_x2 = slice_x2->next_slice(); \
      } \
      y.last_slice()->set_output_gate(ibex::f(x1.last_slice()->output_gate(), x2.last_slice()->output_gate())); \
      \
      if(x1_resampled != NULL) delete x1_resampled; \
      if(x2_resampled != NULL) delete x2_resampled; \
      return y; \
    } \
    \
    const Tube f(const Tube& x1, const Interval& x2) \
    { \
      Tube y(x1); \
      Slice *slice_y = y.first_slice(); \
      const Slice *slice_x1 = x1.first_slice(); \
      while(slice_y != NULL) \
      { \
        slice_y->set_envelope(ibex::f(slice_x1->codomain(), x2)); \
        slice_y->set_input_gate(ibex::f(slice_x1->input_gate(), x2)); \
        slice_y = slice_y->next_slice(); slice_x1 = slice_x1->next_slice(); \
      } \
      y.last_slice()->set_output_gate(ibex::f(x1.last_slice()->output_gate(), x2)); \
      return y; \
    } \
    \
    const Tube f(const Interval& x1, const Tube& x2) \
    { \
      Tube y(x2); \
      Slice *slice_y = y.first_slice(); \
      const Slice *slice_x2 = x2.first_slice(); \
      while(slice_y != NULL) \
      { \
        slice_y->set_envelope(ibex::f(x1, slice_x2->codomain())); \
        slice_y->set_input_gate(ibex::f(x1, slice_x2->input_gate())); \
        slice_y = slice_y->next_slice(); slice_x2 = slice_x2->next_slice(); \
      } \
      y.last_slice()->set_output_gate(ibex::f(x1, x2.last_slice()->output_gate())); \
      return y; \
    } \
    \

  macro_scal_binary(operator+);
  macro_scal_binary(operator-);
  macro_scal_binary(operator*);
  macro_scal_binary(operator/);
  macro_scal_binary(operator|);
  macro_scal_binary(operator&);
  macro_scal_binary(atan2);
}