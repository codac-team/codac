/** 
 *  Arithmetic operations on tubes
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \bugfixes   Bertrand Neveu
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_tube_arithmetic.h"
#include "codac_Slice.h"

using namespace std;
using namespace ibex;

namespace codac
{
  const Tube operator+(const Tube& x)
  {
    return x;
  }

  const Tube operator-(const Tube& x)
  {
    Tube y(x);
    Slice *s_y = nullptr;
    const Slice *s_x = nullptr;

    do
    {
      if(s_y == nullptr) // first iteration
      {
        s_y = y.first_slice();
        s_x = x.first_slice();
      }

      else
      {
        s_y = s_y->next_slice();
        s_x = s_x->next_slice();
      }

      s_y->set_envelope(-s_x->codomain(), false);
      s_y->set_input_gate(-s_x->input_gate(), false);

    } while(s_y->next_slice());

    s_y->set_output_gate(-s_x->output_gate(), false);
    return y;
  }
    
  #define macro_scal_unary(f) \
    \
    const Tube f(const Tube& x) \
    { \
      Tube y(x); \
      Slice *s_y = nullptr; \
      const Slice *s_x = nullptr; \
      \
      do \
      { \
        if(s_y == nullptr) /* first iteration */ \
        { \
          s_y = y.first_slice(); \
          s_x = x.first_slice(); \
        } \
        \
        else \
        { \
          s_y = s_y->next_slice(); \
          s_x = s_x->next_slice(); \
        } \
        \
        s_y->set_envelope(ibex::f(s_x->codomain()), false); \
        s_y->set_input_gate(ibex::f(s_x->input_gate()), false); \
        \
      } while(s_y->next_slice()); \
      \
      s_y->set_output_gate(ibex::f(s_x->output_gate()), false); \
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
      Slice *s_y = nullptr; \
      const Slice *s_x = nullptr; \
      \
      do \
      { \
        if(s_y == nullptr) /* first iteration */ \
        { \
          s_y = y.first_slice(); \
          s_x = x.first_slice(); \
        } \
        \
        else \
        { \
          s_y = s_y->next_slice(); \
          s_x = s_x->next_slice(); \
        } \
        \
        s_y->set_envelope(ibex::f(s_x->codomain(), param), false); \
        s_y->set_input_gate(ibex::f(s_x->input_gate(), param), false); \
        \
      } while(s_y->next_slice()); \
      \
      s_y->set_output_gate(ibex::f(s_x->output_gate(), param), false); \
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
      assert(x1.tdomain() == x2.tdomain()); \
      \
      Tube y(x1); \
      const Slice *s_x1, *s_x2; \
      \
      Tube *x1_resampled = nullptr; /* In case of different slicing between x1 and x2, */ \
      Tube *x2_resampled = nullptr; /* copies of x1 and x2 will be made and equally resampled. */ \
      \
      if(Tube::same_slicing(x1, x2)) /* faster, no sampling computation needed */ \
      { \
        s_x1 = x1.first_slice(); \
        s_x2 = x2.first_slice(); \
      } \
      \
      else \
      { \
        x1_resampled = new Tube(x1); \
        x2_resampled = new Tube(x2); \
        x1_resampled->sample(x2); /* common sampling */ \
        x2_resampled->sample(x1); \
        y.sample(*x2_resampled); \
        s_x1 = x1_resampled->first_slice(); \
        s_x2 = x2_resampled->first_slice(); \
      } \
      \
      Slice *s_y = nullptr; \
      do \
      { \
        if(s_y == nullptr) /* first iteration */ \
          s_y = y.first_slice(); \
        \
        else \
        { \
          s_y = s_y->next_slice(); \
          s_x1 = s_x1->next_slice(); \
          s_x2 = s_x2->next_slice(); \
        } \
        \
        s_y->set_envelope(ibex::f(s_x1->codomain(), s_x2->codomain()), false); \
        s_y->set_input_gate(ibex::f(s_x1->input_gate(), s_x2->input_gate()), false); \
        \
      } while(s_y->next_slice()); \
      \
      s_y->set_output_gate(ibex::f(s_x1->output_gate(), s_x2->output_gate()), false); \
      \
      if(x1_resampled) delete x1_resampled; \
      if(x2_resampled) delete x2_resampled; \
      return y; \
    } \
    \
    const Tube f(const Tube& x1, const Interval& x2) \
    { \
      Tube y(x1); \
      Slice *s_y = nullptr; \
      const Slice *s_x1 = nullptr; \
      \
      do \
      { \
        if(s_y == nullptr) /* first iteration */ \
        { \
          s_y = y.first_slice(); \
          s_x1 = x1.first_slice(); \
        } \
        \
        else \
        { \
          s_y = s_y->next_slice(); \
          s_x1 = s_x1->next_slice(); \
        } \
        \
        s_y->set_envelope(ibex::f(s_x1->codomain(), x2), false); \
        s_y->set_input_gate(ibex::f(s_x1->input_gate(), x2), false); \
        \
      } while(s_y->next_slice()); \
      \
      s_y->set_output_gate(ibex::f(s_x1->output_gate(), x2), false); \
      \
      return y; \
    } \
    \
    const Tube f(const Interval& x1, const Tube& x2) \
    { \
      Tube y(x2); \
      Slice *s_y = nullptr; \
      const Slice *s_x2 = nullptr; \
      \
      do \
      { \
        if(s_y == nullptr) /* first iteration */ \
        { \
          s_y = y.first_slice(); \
          s_x2 = x2.first_slice(); \
        } \
        \
        else \
        { \
          s_y = s_y->next_slice(); \
          s_x2 = s_x2->next_slice(); \
        } \
        \
        s_y->set_envelope(ibex::f(x1, s_x2->codomain()), false); \
        s_y->set_input_gate(ibex::f(x1, s_x2->input_gate()), false); \
        \
      } while(s_y->next_slice()); \
      \
      s_y->set_output_gate(ibex::f(x1, s_x2->output_gate()), false); \
      \
      return y; \
    } \

  macro_scal_binary(operator+);
  macro_scal_binary(operator-);
  macro_scal_binary(operator*);
  macro_scal_binary(operator/);
  macro_scal_binary(operator|);
  macro_scal_binary(operator&);
  macro_scal_binary(atan2);
  macro_scal_binary(min);
  macro_scal_binary(max);

  #define macro_scal_binary_traj(f, feq) \
    \
    const Tube f(const Tube& x1, const Trajectory& x2) \
    { \
      assert(x1.tdomain() == x2.tdomain()); \
      Tube y(x1); \
      y.feq(x2); \
      return y; \
    } \
    \
    const Tube f(const Trajectory& x1, const Tube& x2) \
    { \
      assert(x1.tdomain() == x2.tdomain()); \
      Tube y(x2); \
      y.feq(x1); \
      return y; \
    } \
    \

  macro_scal_binary_traj(operator|, operator|=);
  macro_scal_binary_traj(operator&, operator&=);

  const Tube operator+(const Tube& x1, const Trajectory& x2)
  {
    assert(x1.tdomain() == x2.tdomain());
    Tube y(x1);
    y.operator+=(x2);
    return y;
  }

  const Tube operator+(const Trajectory& x1, const Tube& x2)
  {
    assert(x1.tdomain() == x2.tdomain());
    Tube y(x2);
    y.operator+=(x1);
    return y;
  }

  const Tube operator-(const Tube& x1, const Trajectory& x2)
  {
    assert(x1.tdomain() == x2.tdomain());
    Tube y(x1);
    y.operator-=(x2);
    return y;
  }

  const Tube operator-(const Trajectory& x1, const Tube& x2)
  {
    assert(x1.tdomain() == x2.tdomain());
    Tube y = -x2;
    y.operator+=(x1);
    return y;
  }

  const Tube operator*(const Tube& x1, const Trajectory& x2)
  {
    assert(x1.tdomain() == x2.tdomain());
    Tube y(x1);
    y.operator*=(x2);
    return y;
  }

  const Tube operator*(const Trajectory& x1, const Tube& x2)
  {
    assert(x1.tdomain() == x2.tdomain());
    Tube y(x2);
    y.operator*=(x1);
    return y;
  }

  const Tube operator/(const Tube& x1, const Trajectory& x2)
  {
    assert(x1.tdomain() == x2.tdomain());
    Tube y(x1);
    y.operator/=(x2);
    return y;
  }

  const Tube operator/(const Trajectory& x1, const Tube& x2)
  {
    assert(x1.tdomain() == x2.tdomain());
    Tube y(x2); // same sampling
    y.set(1.); // updated codomain
    y.operator/=(x2);
    y.operator*=(x1);
    return y;
  }
}