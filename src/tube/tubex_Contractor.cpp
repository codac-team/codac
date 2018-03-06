/* ============================================================================
 *  tubex-lib - Tube class
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
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
#include "exceptions/tubex_Exception.h"
#include "exceptions/tubex_DomainException.h"
#include "exceptions/tubex_EmptyException.h"
#include "ibex_CtcFwdBwd.h"
#include <iostream>
#include <iomanip> // for setprecision()
#ifdef _OPENMP
  #include <omp.h> // for multithreading
#endif

using namespace std;
using namespace ibex;

namespace tubex
{
  bool Tube::ctcFwd(const Tube& derivative_tube, const ibex::Interval& initial_value)
  {
    CtcDeriv ctc;
    return ctc.contractFwd(*this, derivative_tube, initial_value);
  }

  bool Tube::ctcBwd(const Tube& derivative_tube)
  {
    CtcDeriv ctc;
    return ctc.contractBwd(*this, derivative_tube);
  }
  
  bool Tube::ctcFwdBwd(const Tube& derivative_tube, const ibex::Interval& initial_value)
  {
    CtcDeriv ctc;
    return ctc.contract(*this, derivative_tube, initial_value);
  }

  bool Tube::ctcEval(const Tube& derivative_tube, Interval& t, Interval& y, bool fwd_bwd)
  {
    CtcEval ctc;
    return ctc.contract(t, y, *this, derivative_tube, fwd_bwd);
  }

  bool Tube::ctcEval(const Tube& derivative_tube, Interval& t, const Interval& y, bool fwd_bwd)
  {
    CtcEval ctc;
    Interval y_temp = y;
    return ctc.contract(t, y_temp, *this, derivative_tube, fwd_bwd);
  }

  bool Tube::ctcEval(const Tube& derivative_tube, const Interval& t, Interval& y, bool fwd_bwd)
  {
    CtcEval ctc;
    Interval t_temp = t;
    return ctc.contract(t_temp, y, *this, derivative_tube, fwd_bwd);
  }

  bool Tube::ctcEval(const Tube& derivative_tube, const Interval& t, const Interval& y, bool fwd_bwd)
  {
    CtcEval ctc;
    Interval y_temp = y;
    Interval t_temp = t;
    return ctc.contract(t_temp, y_temp, *this, derivative_tube, fwd_bwd);
  }

  bool Tube::ctcOut(const Interval& t, const Interval& y)
  {
    Interval intersected_t = t & domain();
    bool contraction = false;
    pair<Interval,Interval> enc_bounds = eval(intersected_t);

    if(y.intersects(enc_bounds.first))
      #pragma omp parallel num_threads(omp_get_num_procs())
      {
        #pragma omp for
        for(int i = input2index(intersected_t.lb()) ; i < input2index(intersected_t.ub()) ; i++)
        {
          Interval old_y = (*this)[i];
          Interval new_y(max(y.ub(), old_y.lb()), old_y.ub());
          contraction |= new_y.diam() < old_y.diam();
          set(new_y, i);
        }
      }

    else if(y.intersects(enc_bounds.second))
      #pragma omp parallel num_threads(omp_get_num_procs())
      {
        #pragma omp for
        for(int i = input2index(intersected_t.lb()) ; i < input2index(intersected_t.ub()) ; i++)
        {
          Interval old_y = (*this)[i];
          Interval new_y(old_y.lb(), min(old_y.ub(), y.lb()));
          contraction |= new_y.diam() < old_y.diam();
          set(new_y, i);
        }
      }

    else
      return false; // surely no contraction

    return contraction;
  }

  bool Tube::ctcIntertemporal(Interval& t1, Interval& t2) const
  {
    bool contraction = false;
    double t1_diam, t2_diam;

    do
    {
      t1_diam = t1.diam();
      t2_diam = t2.diam();
      t1 &= invert((*this)[t2], t1);
      t2 &= invert((*this)[t1], t2);
      contraction |= t1.diam() < t1_diam || t2.diam() < t2_diam;
    } while(t1.diam() < t1_diam || t2.diam() < t2_diam);

    return contraction;
  }

  bool Tube::ctcIntertemporal(Interval& y, Interval& t1, Interval& t2) const
  {
    bool contraction = false;
    double y_diam = y.diam(), t1_diam, t2_diam;

    y &= (*this)[t1] | (*this)[t2];

    if(y.is_empty())
    {
      t1.set_empty();
      t2.set_empty();
      return true;
    }

    else
    {
      contraction |= y.diam() < y_diam;

      do
      {
        t1_diam = t1.diam();
        t2_diam = t2.diam();
        t1 &= invert(y, t1);
        t2 &= invert(y, t2);
        contraction |= t1.diam() < t1_diam || t2.diam() < t2_diam;
      } while(t1.diam() < t1_diam || t2.diam() < t2_diam);

      return contraction;
    }
  }

  bool Tube::ctcPeriodic(const Interval& period)
  {
    bool contraction, at_least_one_contraction = false;

    do
    {
      contraction = false;
      double vol = volume();

      for(int i = 0 ; i < size() ; i++)
      {
        int k;
        Interval shift_dom, dom = domain(i);

        k = 0; // Fwd
        do
        {
          k++;
          shift_dom = domain() & (dom + k*period);

          if(!shift_dom.is_empty())
            set((*this)[shift_dom] & (*this)[dom], dom);

        } while(!shift_dom.is_empty());

        k = 0; // Bwd
        do
        {
          k--;
          shift_dom = domain() & (dom + k*period);

          if(!shift_dom.is_empty())
            set((*this)[shift_dom] & (*this)[dom], dom);

        } while(!shift_dom.is_empty());
      }

      contraction = vol > volume();
      at_least_one_contraction |= contraction;
    } while(contraction);

    return at_least_one_contraction;
  }


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

  bool ctcAbs(Tube& y, Tube& x) { func_ctc_unary(y, x, abs, bwd_abs); }
  bool ctcSqr(Tube& y, Tube& x) { func_ctc_unary(y, x, sqr, bwd_sqr); }
  bool ctcSqrt(Tube& y, Tube& x) { func_ctc_unary(y, x, sqrt, bwd_sqrt); }
  bool ctcPow(Tube& y, Tube& x, int p) { func_ctc_unary_param(y, x, p, pow, bwd_pow); }
  bool ctcPow(Tube& y, Tube& x, double p) { func_ctc_unary_param(y, x, p, pow, bwd_pow); }
  bool ctcPow(Tube& y, Tube& x, Interval& p) { func_ctc_unary_param(y, x, p, pow, bwd_pow); }
  bool ctcRoot(Tube& y, Tube& x, int p) { func_ctc_unary_param(y, x, p, root, bwd_root); }
  bool ctcExp(Tube& y, Tube& x) { func_ctc_unary(y, x, exp, bwd_exp); }
  bool ctcLog(Tube& y, Tube& x) { func_ctc_unary(y, x, log, bwd_log); }
  bool ctcCos(Tube& y, Tube& x) { func_ctc_unary(y, x, cos, bwd_cos); }
  bool ctcSin(Tube& y, Tube& x) { func_ctc_unary(y, x, sin, bwd_sin); }
  bool ctcTan(Tube& y, Tube& x) { func_ctc_unary(y, x, tan, bwd_tan); }
  bool ctcAcos(Tube& y, Tube& x) { func_ctc_unary(y, x, acos, bwd_acos); }
  bool ctcAsin(Tube& y, Tube& x) { func_ctc_unary(y, x, asin, bwd_asin); }
  bool ctcAtan(Tube& y, Tube& x) { func_ctc_unary(y, x, atan, bwd_atan); }
  bool ctcCosh(Tube& y, Tube& x) { func_ctc_unary(y, x, cosh, bwd_cosh); }
  bool ctcSinh(Tube& y, Tube& x) { func_ctc_unary(y, x, sinh, bwd_sinh); }
  bool ctcTanh(Tube& y, Tube& x) { func_ctc_unary(y, x, tanh, bwd_tanh); }
  bool ctcAcosh(Tube& y, Tube& x) { func_ctc_unary(y, x, acosh, bwd_acosh); }
  bool ctcAsinh(Tube& y, Tube& x) { func_ctc_unary(y, x, asinh, bwd_asinh); }
  bool ctcAtanh(Tube& y, Tube& x) { func_ctc_unary(y, x, atanh, bwd_atanh); }
  bool ctcAtan2(Tube& theta, Tube& y, Tube& x) { func_ctc_binary(theta, y, x, atan2, bwd_atan2); }

  bool Tube::contract(Tube& x1, Tube& x2, const Function& f)
  {
    checkStructures(x1, x2);

    if(f.nb_arg() != 2)
      cout << "Tube::contract: wrong argument number" << endl;

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

  bool Tube::contract(Tube& x1, Tube& x2, Tube& x3, const Function& f)
  {
    checkStructures(x1, x2);
    checkStructures(x1, x3);

    if(f.nb_arg() != 3)
      cout << "Tube::contract: wrong argument number" << endl;

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

  bool Tube::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, const Function& f)
  {
    checkStructures(x1, x2);
    checkStructures(x1, x3);
    checkStructures(x1, x4);

    if(f.nb_arg() != 4)
      cout << "Tube::contract: wrong argument number" << endl;

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

  bool Tube::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, const Function& f)
  {
    checkStructures(x1, x2);
    checkStructures(x1, x3);
    checkStructures(x1, x4);
    checkStructures(x1, x5);

    if(f.nb_arg() != 5)
      cout << "Tube::contract: wrong argument number" << endl;

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

  bool Tube::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, const Function& f)
  {
    checkStructures(x1, x2);
    checkStructures(x1, x3);
    checkStructures(x1, x4);
    checkStructures(x1, x5);
    checkStructures(x1, x6);

    if(f.nb_arg() != 6)
      cout << "Tube::contract: wrong argument number" << endl;

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

  bool Tube::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, Tube& x7, const Function& f)
  {
    checkStructures(x1, x2);
    checkStructures(x1, x3);
    checkStructures(x1, x4);
    checkStructures(x1, x5);
    checkStructures(x1, x6);
    checkStructures(x1, x7);

    if(f.nb_arg() != 7)
      cout << "Tube::contract: wrong argument number" << endl;

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

  bool Tube::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, Tube& x7, Tube& x8, const Function& f)
  {
    checkStructures(x1, x2);
    checkStructures(x1, x3);
    checkStructures(x1, x4);
    checkStructures(x1, x5);
    checkStructures(x1, x6);
    checkStructures(x1, x7);
    checkStructures(x1, x8);

    if(f.nb_arg() != 8)
      cout << "Tube::contract: wrong argument number" << endl;

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