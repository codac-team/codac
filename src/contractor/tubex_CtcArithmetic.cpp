/* ============================================================================
 *  tubex-lib - Tube arithmetic contractors
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2017
 * ---------------------------------------------------------------------------- */

#include "tubex_CtcArithmetic.h"
#include "tubex_DomainException.h"
#include "ibex_CtcFwdBwd.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  #define func_ctc_unary(y, x, f, bwd_f) \
      bool contraction = false; \
      bool local_contraction; \
      checkStructures(x, y); \
      for(int i = 0 ; i < x.size() ; i++) \
      { \
        do \
        { \
          local_contraction = false; \
          local_contraction |= y.intersect(ibex::f(x[i]), i); \
          Interval x_i = x[i]; \
          ibex::bwd_f(y[i], x_i); \
          local_contraction |= x.intersect(x_i, i); \
          contraction |= local_contraction; \
        } while(local_contraction); \
      } \
      return contraction;

  #define func_ctc_unary_param(y, x, param, f, bwd_f) \
      bool contraction = false; \
      bool local_contraction; \
      checkStructures(x, y); \
      for(int i = 0 ; i < x.size() ; i++) \
      { \
        do \
        { \
          local_contraction = false; \
          local_contraction |= y.intersect(ibex::f(x[i], param), i); \
          Interval x_i = x[i]; \
          ibex::bwd_f(y[i], param, x_i); \
          local_contraction |= x.intersect(x_i, i); \
          contraction |= local_contraction; \
        } while(local_contraction); \
      } \
      return contraction;

  #define func_ctc_binary(c, a, b, f, bwd_f) \
      bool contraction = false; \
      bool local_contraction; \
      checkStructures(c, a); \
      checkStructures(c, b); \
      for(int i = 0 ; i < a.size() ; i++) \
      { \
        do \
        { \
          local_contraction = false; \
          local_contraction |= c.intersect(ibex::f(a[i], b[i]), i); \
          Interval a_i = a[i]; \
          Interval b_i = b[i]; \
          ibex::bwd_f(c[i], a_i, b_i); \
          ibex::bwd_f(c[i], a_i, b_i); \
          local_contraction |= a.intersect(a_i, i); \
          local_contraction |= b.intersect(b_i, i); \
          contraction |= local_contraction; \
        } while(local_contraction); \
      } \
      return contraction;

  bool CtcArithmetic::ctcAbs(Tube& y, Tube& x) { func_ctc_unary(y, x, abs, bwd_abs); }
  bool CtcArithmetic::ctcSqr(Tube& y, Tube& x) { func_ctc_unary(y, x, sqr, bwd_sqr); }
  bool CtcArithmetic::ctcSqrt(Tube& y, Tube& x) { func_ctc_unary(y, x, sqrt, bwd_sqrt); }
  bool CtcArithmetic::ctcPow(Tube& y, Tube& x, int p) { func_ctc_unary_param(y, x, p, pow, bwd_pow); }
  bool CtcArithmetic::ctcPow(Tube& y, Tube& x, double p) { func_ctc_unary_param(y, x, p, pow, bwd_pow); }
  bool CtcArithmetic::ctcPow(Tube& y, Tube& x, Interval& p) { func_ctc_unary_param(y, x, p, pow, bwd_pow); }
  bool CtcArithmetic::ctcRoot(Tube& y, Tube& x, int p) { func_ctc_unary_param(y, x, p, root, bwd_root); }
  bool CtcArithmetic::ctcExp(Tube& y, Tube& x) { func_ctc_unary(y, x, exp, bwd_exp); }
  bool CtcArithmetic::ctcLog(Tube& y, Tube& x) { func_ctc_unary(y, x, log, bwd_log); }
  bool CtcArithmetic::ctcCos(Tube& y, Tube& x) { func_ctc_unary(y, x, cos, bwd_cos); }
  bool CtcArithmetic::ctcSin(Tube& y, Tube& x) { func_ctc_unary(y, x, sin, bwd_sin); }
  bool CtcArithmetic::ctcTan(Tube& y, Tube& x) { func_ctc_unary(y, x, tan, bwd_tan); }
  bool CtcArithmetic::ctcAcos(Tube& y, Tube& x) { func_ctc_unary(y, x, acos, bwd_acos); }
  bool CtcArithmetic::ctcAsin(Tube& y, Tube& x) { func_ctc_unary(y, x, asin, bwd_asin); }
  bool CtcArithmetic::ctcAtan(Tube& y, Tube& x) { func_ctc_unary(y, x, atan, bwd_atan); }
  bool CtcArithmetic::ctcCosh(Tube& y, Tube& x) { func_ctc_unary(y, x, cosh, bwd_cosh); }
  bool CtcArithmetic::ctcSinh(Tube& y, Tube& x) { func_ctc_unary(y, x, sinh, bwd_sinh); }
  bool CtcArithmetic::ctcTanh(Tube& y, Tube& x) { func_ctc_unary(y, x, tanh, bwd_tanh); }
  bool CtcArithmetic::ctcAcosh(Tube& y, Tube& x) { func_ctc_unary(y, x, acosh, bwd_acosh); }
  bool CtcArithmetic::ctcAsinh(Tube& y, Tube& x) { func_ctc_unary(y, x, asinh, bwd_asinh); }
  bool CtcArithmetic::ctcAtanh(Tube& y, Tube& x) { func_ctc_unary(y, x, atanh, bwd_atanh); }
  bool CtcArithmetic::ctcAtan2(Tube& theta, Tube& y, Tube& x) { func_ctc_binary(theta, y, x, atan2, bwd_atan2); }

  bool CtcArithmetic::contract(Tube& x1, Tube& x2, const Function& f)
  {
    checkStructures(x1, x2);

    if(f.nb_arg() != 2)
      cout << "contract: wrong argument number" << endl;

    double volume = x1.volume() + x2.volume();

    Function fbis(f);
    NumConstraint c(fbis);
    CtcFwdBwd ctc(c);

    for(int i = 0 ; i < x1.size() ; i++)
    {
      IntervalVector box(2);
      box[0] = x1[i]; box[1] = x2[i];
      ctc.contract(box);
      x1.set(box[0], i); x2.set(box[1], i);
    }

    return volume < x1.volume() + x2.volume();
  }

  bool CtcArithmetic::contract(Tube& x1, Tube& x2, Tube& x3, const Function& f)
  {
    checkStructures(x1, x2);
    checkStructures(x1, x3);

    if(f.nb_arg() != 3)
      cout << "contract: wrong argument number" << endl;

    double volume = x1.volume() + x2.volume() + x3.volume();

    Function fbis(f);
    NumConstraint c(fbis);
    CtcFwdBwd ctc(c);

    for(int i = 0 ; i < x1.size() ; i++)
    {
      IntervalVector box(3);
      box[0] = x1[i]; box[1] = x2[i]; box[2] = x3[i];
      ctc.contract(box);
      x1.set(box[0], i); x2.set(box[1], i); x3.set(box[2], i);
    }

    return volume < x1.volume() + x2.volume() + x3.volume();
  }

  bool CtcArithmetic::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, const Function& f)
  {
    checkStructures(x1, x2);
    checkStructures(x1, x3);
    checkStructures(x1, x4);

    if(f.nb_arg() != 4)
      cout << "contract: wrong argument number" << endl;

    double volume = x1.volume() + x2.volume() + x3.volume() + x4.volume();

    Function fbis(f);
    NumConstraint c(fbis);
    CtcFwdBwd ctc(c);

    for(int i = 0 ; i < x1.size() ; i++)
    {
      IntervalVector box(4);
      box[0] = x1[i]; box[1] = x2[i]; box[2] = x3[i]; box[3] = x4[i];
      ctc.contract(box);
      x1.set(box[0], i); x2.set(box[1], i); x3.set(box[2], i); x4.set(box[3], i);
    }

    return volume < x1.volume() + x2.volume() + x3.volume() + x4.volume();
  }

  bool CtcArithmetic::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, const Function& f)
  {
    checkStructures(x1, x2);
    checkStructures(x1, x3);
    checkStructures(x1, x4);
    checkStructures(x1, x5);

    if(f.nb_arg() != 5)
      cout << "contract: wrong argument number" << endl;

    double volume = x1.volume() + x2.volume() + x3.volume() + x4.volume() + x5.volume();

    Function fbis(f);
    NumConstraint c(fbis);
    CtcFwdBwd ctc(c);

    for(int i = 0 ; i < x1.size() ; i++)
    {
      IntervalVector box(5);
      box[0] = x1[i]; box[1] = x2[i]; box[2] = x3[i]; box[3] = x4[i]; box[4] = x5[i];
      ctc.contract(box);
      x1.set(box[0], i); x2.set(box[1], i); x3.set(box[2], i); x4.set(box[3], i); x5.set(box[4], i);
    }

    return volume < x1.volume() + x2.volume() + x3.volume() + x4.volume() + x5.volume();
  }

  bool CtcArithmetic::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, const Function& f)
  {
    checkStructures(x1, x2);
    checkStructures(x1, x3);
    checkStructures(x1, x4);
    checkStructures(x1, x5);
    checkStructures(x1, x6);

    if(f.nb_arg() != 6)
      cout << "contract: wrong argument number" << endl;

    double volume = x1.volume() + x2.volume() + x3.volume() + x4.volume() + x5.volume() + x6.volume();

    Function fbis(f);
    NumConstraint c(fbis);
    CtcFwdBwd ctc(c);

    for(int i = 0 ; i < x1.size() ; i++)
    {
      IntervalVector box(6);
      box[0] = x1[i]; box[1] = x2[i]; box[2] = x3[i]; box[3] = x4[i]; box[4] = x5[i]; box[5] = x6[i];
      ctc.contract(box);
      x1.set(box[0], i); x2.set(box[1], i); x3.set(box[2], i); x4.set(box[3], i); x5.set(box[4], i); x6.set(box[5], i);
    }

    return volume < x1.volume() + x2.volume() + x3.volume() + x4.volume() + x5.volume() + x6.volume();
  }

  bool CtcArithmetic::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, Tube& x7, const Function& f)
  {
    checkStructures(x1, x2);
    checkStructures(x1, x3);
    checkStructures(x1, x4);
    checkStructures(x1, x5);
    checkStructures(x1, x6);
    checkStructures(x1, x7);

    if(f.nb_arg() != 7)
      cout << "contract: wrong argument number" << endl;

    double volume = x1.volume() + x2.volume() + x3.volume() + x4.volume() + x5.volume() + x6.volume() + x7.volume();

    Function fbis(f);
    NumConstraint c(fbis);
    CtcFwdBwd ctc(c);

    for(int i = 0 ; i < x1.size() ; i++)
    {
      IntervalVector box(7);
      box[0] = x1[i]; box[1] = x2[i]; box[2] = x3[i]; box[3] = x4[i]; box[4] = x5[i]; box[5] = x6[i]; box[6] = x7[i];
      ctc.contract(box);
      x1.set(box[0], i); x2.set(box[1], i); x3.set(box[2], i); x4.set(box[3], i); x5.set(box[4], i); x6.set(box[5], i); x7.set(box[6], i);
    }

    return volume < x1.volume() + x2.volume() + x3.volume() + x4.volume() + x5.volume() + x6.volume() + x7.volume();
  }

  bool CtcArithmetic::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, Tube& x7, Tube& x8, const Function& f)
  {
    checkStructures(x1, x2);
    checkStructures(x1, x3);
    checkStructures(x1, x4);
    checkStructures(x1, x5);
    checkStructures(x1, x6);
    checkStructures(x1, x7);
    checkStructures(x1, x8);

    if(f.nb_arg() != 8)
      cout << "contract: wrong argument number" << endl;

    double volume = x1.volume() + x2.volume() + x3.volume() + x4.volume() + x5.volume() + x6.volume() + x7.volume() + x8.volume();

    Function fbis(f);
    NumConstraint c(fbis);
    CtcFwdBwd ctc(c);

    for(int i = 0 ; i < x1.size() ; i++)
    {
      IntervalVector box(8);
      box[0] = x1[i]; box[1] = x2[i]; box[2] = x3[i]; box[3] = x4[i]; box[4] = x5[i]; box[5] = x6[i]; box[6] = x7[i]; box[7] = x8[i];
      ctc.contract(box);
      x1.set(box[0], i); x2.set(box[1], i); x3.set(box[2], i); x4.set(box[3], i); x5.set(box[4], i); x6.set(box[5], i); x7.set(box[6], i); x8.set(box[7], i);
    }

    return volume < x1.volume() + x2.volume() + x3.volume() + x4.volume() + x5.volume() + x6.volume() + x7.volume() + x8.volume();
  }
}