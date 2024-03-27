/** 
 *  Directed contractors
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_DirectedCtc.h"

using namespace std;
using namespace codac2;

// CtcAdd (unary operations)

  Interval CtcAdd::fwd_eval(const Interval& x1)
  {
    return x1;
  }

  void CtcAdd::bwd_eval(const Interval& y, Interval& x1)
  { }

  IntervalVector CtcAdd::fwd_eval(const IntervalVector& x1)
  {
    return x1;
  }

  void CtcAdd::bwd_eval(const IntervalVector& y, IntervalVector& x1)
  { }

  IntervalMatrix CtcAdd::fwd_eval(const IntervalMatrix& x1)
  {
    return x1;
  }

  void CtcAdd::bwd_eval(const IntervalMatrix& y, IntervalMatrix& x1)
  { }

// CtcAdd (binary operations)

  Interval CtcAdd::fwd_eval(const Interval& x1, const Interval& x2)
  {
    return x1 + x2;
  }

  void CtcAdd::bwd_eval(const Interval& y, Interval& x1, Interval& x2)
  {
    bwd_add(y, x1, x2);
  }

  IntervalVector CtcAdd::fwd_eval(const IntervalVector& x1, const IntervalVector& x2)
  {
    return x1 + x2;
  }

  void CtcAdd::bwd_eval(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2)
  {
    assert(y.size() == x1.size() && y.size() == x2.size());
    for(size_t i = 0 ; i < y.size() ; i++)
      CtcAdd::bwd_eval(y[i], x1[i], x2[i]);
  }

  IntervalMatrix CtcAdd::fwd_eval(const IntervalMatrix& x1, const IntervalMatrix& x2)
  {
    return x1 + x2;
  }

  void CtcAdd::bwd_eval(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2)
  {
    assert(y.size() == x1.size() && y.size() == x2.size());
    for(size_t i = 0 ; i < y.size() ; i++)
      CtcAdd::bwd_eval(*(y.data()+i), *(x1.data()+i), *(x2.data()+i));
  }


// CtcSub (unary operations)

  Interval CtcSub::fwd_eval(const Interval& x1)
  {
    return -x1;
  }

  void CtcSub::bwd_eval(const Interval& y, Interval& x1)
  {
    Interval x2_(0.);
    bwd_sub(y, x2_, x1);
  }

  IntervalVector CtcSub::fwd_eval(const IntervalVector& x1)
  {
    return -x1;
  }

  void CtcSub::bwd_eval(const IntervalVector& y, IntervalVector& x1)
  {
    assert(y.size() == x1.size());
    for(size_t i = 0 ; i < y.size() ; i++)
      bwd_eval(y[i], x1[i]);
  }

  IntervalMatrix CtcSub::fwd_eval(const IntervalMatrix& x1)
  {
    return -x1;
  }

  void CtcSub::bwd_eval(const IntervalMatrix& y, IntervalMatrix& x1)
  {
    assert(y.size() == x1.size());
    for(size_t i = 0 ; i < y.size() ; i++)
      CtcSub::bwd_eval(*(y.data()+i), *(x1.data()+i));
  }

// CtcSub (binary operations)

  Interval CtcSub::fwd_eval(const Interval& x1, const Interval& x2)
  {
    return x1 - x2;
  }

  void CtcSub::bwd_eval(const Interval& y, Interval& x1, Interval& x2)
  {
    bwd_sub(y, x1, x2);
  }

  IntervalVector CtcSub::fwd_eval(const IntervalVector& x1, const IntervalVector& x2)
  {
    return x1 - x2;
  }

  void CtcSub::bwd_eval(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2)
  {
    assert(y.size() == x1.size() && y.size() == x2.size());
    for(size_t i = 0 ; i < y.size() ; i++)
      bwd_eval(y[i], x1[i], x2[i]);
  }

  IntervalMatrix CtcSub::fwd_eval(const IntervalMatrix& x1, const IntervalMatrix& x2)
  {
    return x1 - x2;
  }

  void CtcSub::bwd_eval(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2)
  {
    assert(y.size() == x1.size() && y.size() == x2.size());
    for(size_t i = 0 ; i < y.size() ; i++)
      CtcSub::bwd_eval(*(y.data()+i), *(x1.data()+i), *(x2.data()+i));
  }


// CtcMul

  Interval CtcMul::fwd_eval(const Interval& x1, const Interval& x2)
  {
    return x1 * x2;
  }

  void CtcMul::bwd_eval(const Interval& y, Interval& x1, Interval& x2)
  {
    bwd_mul(y, x1, x2);
  }

  IntervalVector CtcMul::fwd_eval(const IntervalMatrix& x1, const IntervalVector& x2)
  {
    return x1 * x2;
  }

  #include "codac2_ibex.h"
  #include <ibex_IntervalVector.h> // for ibex::bwd_mul

  void CtcMul::bwd_eval(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2)
  {
    // only if squared matrix: CtcGaussElim ctc_ge;
    // only if squared matrix: CtcLinearPrecond ctc_gep(ctc_ge);
    // only if squared matrix: IntervalVector y_(y);
    // only if squared matrix: ctc_gep.contract(x1,x2,y_);

    ibex::IntervalVector ibex_y(to_ibex(y)), ibex_x2(to_ibex(x2));
    ibex::IntervalMatrix ibex_x1(to_ibex(x1));
    ibex::bwd_mul(ibex_y, ibex_x1, ibex_x2, 0.05);
    x1 &= to_codac(ibex_x1);
    x2 &= to_codac(ibex_x2);
  }


// CtcSqrt

  Interval CtcSqrt::fwd_eval(const Interval& x1)
  {
    return sqrt(x1);
  }

  void CtcSqrt::bwd_eval(const Interval& y, Interval& x1)
  {
    bwd_sqrt(y, x1);
  }


// CtcSqr

  Interval CtcSqr::fwd_eval(const Interval& x1)
  {
    return sqr(x1);
  }

  void CtcSqr::bwd_eval(const Interval& y, Interval& x1)
  {
    bwd_sqr(y, x1);
  }


// CtcCos

  Interval CtcCos::fwd_eval(const Interval& x1)
  {
    return cos(x1);
  }

  void CtcCos::bwd_eval(const Interval& y, Interval& x1)
  {
    bwd_cos(y, x1);
  }


// CtcSin

  Interval CtcSin::fwd_eval(const Interval& x1)
  {
    return sin(x1);
  }

  void CtcSin::bwd_eval(const Interval& y, Interval& x1)
  {
    bwd_sin(y, x1);
  }


// CtcComponent

  Interval CtcComponent::fwd_eval(const IntervalVector& x1, size_t i)
  {
    assert(i >= 0 && i < x1.size());
    return x1[i];
  }

  void CtcComponent::bwd_eval(const Interval& y, IntervalVector& x1, size_t i)
  {
    assert(i >= 0 && i < x1.size());
    x1[i] &= y;
  }


// CtcMatrix

  void CtcMatrix::fwd_eval_i(IntervalMatrix& m, const IntervalVector& x, size_t i)
  {
    assert((int)i >= 0 && (int)i < m.cols());
    m.resize(x.size(),m.cols());
    m.col(i) = x;
  }