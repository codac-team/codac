/** 
 *  Directed contractors
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <cassert>
#include "codac2_DirectedCtc.h"

using namespace std;
using namespace codac2;


// AddOp (unary operations)

  Interval AddOp::fwd(const Interval& x1)
  {
    return x1;
  }

  ScalarOpValue AddOp::fwd(const ScalarOpValue& x1)
  {
    return {
      x1.m,
      fwd(x1.a),
      x1.da
    };
  }

  void AddOp::bwd(const Interval& y, Interval& x1)
  { }

  IntervalVector AddOp::fwd(const IntervalVector& x1)
  {
    return x1;
  }

  VectorOpValue AddOp::fwd(const VectorOpValue& x1)
  {
    return {
      x1.m,
      fwd(x1.a),
      x1.da
    };
  }

  void AddOp::bwd(const IntervalVector& y, IntervalVector& x1)
  { }

  IntervalMatrix AddOp::fwd(const IntervalMatrix& x1)
  {
    return x1;
  }

  MatrixOpValue AddOp::fwd(const MatrixOpValue& x1)
  {
    return {
      fwd(x1.a)
    };
  }

  void AddOp::bwd(const IntervalMatrix& y, IntervalMatrix& x1)
  { }


// AddOp (binary operations)

  Interval AddOp::fwd(const Interval& x1, const Interval& x2)
  {
    return x1 + x2;
  }

  ScalarOpValue AddOp::fwd(const ScalarOpValue& x1, const ScalarOpValue& x2)
  {
    return {
      x1.m + x1.m,
      fwd(x1.a, x2.a),
      x1.da + x2.da
    };
  }

  void AddOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    bwd_add(y, x1, x2);
  }

  IntervalVector AddOp::fwd(const IntervalVector& x1, const IntervalVector& x2)
  {
    return x1 + x2;
  }

  VectorOpValue AddOp::fwd(const VectorOpValue& x1, const VectorOpValue& x2)
  {
    return {
      x1.m + x1.m,
      fwd(x1.a, x2.a),
      x1.da + x2.da
    };
  }

  void AddOp::bwd(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2)
  {
    assert(y.size() == x1.size() && y.size() == x2.size());
    for(size_t i = 0 ; i < y.size() ; i++)
      AddOp::bwd(y[i], x1[i], x2[i]);
  }

  IntervalMatrix AddOp::fwd(const IntervalMatrix& x1, const IntervalMatrix& x2)
  {
    return x1 + x2;
  }

  MatrixOpValue AddOp::fwd(const MatrixOpValue& x1, const MatrixOpValue& x2)
  {
    return {
      fwd(x1.a, x2.a)
    };
  }

  void AddOp::bwd(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2)
  {
    assert(y.size() == x1.size() && y.size() == x2.size());
    for(size_t i = 0 ; i < y.size() ; i++)
      AddOp::bwd(*(y.data()+i), *(x1.data()+i), *(x2.data()+i));
  }


// SubOp (unary operations)

  Interval SubOp::fwd(const Interval& x1)
  {
    return -x1;
  }

  ScalarOpValue SubOp::fwd(const ScalarOpValue& x1)
  {
    return {
      -x1.m,
      fwd(x1.a),
      -x1.da
    };
  }

  void SubOp::bwd(const Interval& y, Interval& x1)
  {
    Interval x2_(0.);
    bwd_sub(y, x2_, x1);
  }

  IntervalVector SubOp::fwd(const IntervalVector& x1)
  {
    return -x1;
  }

  VectorOpValue SubOp::fwd(const VectorOpValue& x1)
  {
    return {
      -x1.m,
      fwd(x1.a),
      -x1.da
    };
  }

  void SubOp::bwd(const IntervalVector& y, IntervalVector& x1)
  {
    assert(y.size() == x1.size());
    for(size_t i = 0 ; i < y.size() ; i++)
      bwd(y[i], x1[i]);
  }

  IntervalMatrix SubOp::fwd(const IntervalMatrix& x1)
  {
    return -x1;
  }

  MatrixOpValue SubOp::fwd(const MatrixOpValue& x1)
  {
    return {
      fwd(x1.a)
    };
  }

  void SubOp::bwd(const IntervalMatrix& y, IntervalMatrix& x1)
  {
    assert(y.size() == x1.size());
    for(size_t i = 0 ; i < y.size() ; i++)
      SubOp::bwd(*(y.data()+i), *(x1.data()+i));
  }


// SubOp (binary operations)

  Interval SubOp::fwd(const Interval& x1, const Interval& x2)
  {
    return x1 - x2;
  }

  ScalarOpValue SubOp::fwd(const ScalarOpValue& x1, const ScalarOpValue& x2)
  {
    return {
      x1.m - x2.m,
      fwd(x1.a, x2.a),
      x1.da - x2.da
    };
  }

  void SubOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    bwd_sub(y, x1, x2);
  }

  IntervalVector SubOp::fwd(const IntervalVector& x1, const IntervalVector& x2)
  {
    return x1 - x2;
  }

  VectorOpValue SubOp::fwd(const VectorOpValue& x1, const VectorOpValue& x2)
  {
    return {
      x1.m - x2.m,
      fwd(x1.a, x2.a),
      x1.da - x2.da
    };
  }

  void SubOp::bwd(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2)
  {
    assert(y.size() == x1.size() && y.size() == x2.size());
    for(size_t i = 0 ; i < y.size() ; i++)
      bwd(y[i], x1[i], x2[i]);
  }

  IntervalMatrix SubOp::fwd(const IntervalMatrix& x1, const IntervalMatrix& x2)
  {
    return x1 - x2;
  }

  MatrixOpValue SubOp::fwd(const MatrixOpValue& x1, const MatrixOpValue& x2)
  {
    return {
      fwd(x1.a, x2.a)
    };
  }

  void SubOp::bwd(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2)
  {
    assert(y.size() == x1.size() && y.size() == x2.size());
    for(size_t i = 0 ; i < y.size() ; i++)
      SubOp::bwd(*(y.data()+i), *(x1.data()+i), *(x2.data()+i));
  }


// MulOp

  Interval MulOp::fwd(const Interval& x1, const Interval& x2)
  {
    return x1 * x2;
  }

  ScalarOpValue MulOp::fwd(const ScalarOpValue& x1, const ScalarOpValue& x2)
  {
    return {
      x1.m * x2.m,
      fwd(x1.a, x2.a),
      x2.a * x1.da + x1.a * x2.da
    };
  }

  void MulOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    bwd_mul(y, x1, x2);
  }

  IntervalVector MulOp::fwd(const IntervalMatrix& x1, const IntervalVector& x2)
  {
    return x1 * x2;
  }

  VectorOpValue MulOp::fwd(const MatrixOpValue& x1, const VectorOpValue& x2)
  {
    return {
      x1.a.mid() * x2.m,
      fwd(x1.a, x2.a),
      x2.a * IntervalMatrix::zeros(x1.a.rows(),x1.a.cols()) + x1.a * x2.da // todo
    };
  }

  #include "codac2_ibex.h"
  #include <ibex_IntervalVector.h> // for ibex::bwd_mul

  void MulOp::bwd(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2)
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


// DivOp

  Interval DivOp::fwd(const Interval& x1, const Interval& x2)
  {
    return x1 / x2;
  }

  ScalarOpValue DivOp::fwd(const ScalarOpValue& x1, const ScalarOpValue& x2)
  {
    return {
      x1.m / x2.m,
      fwd(x1.a, x2.a),
      IntervalVector(x1.da.size()) // todo
    };
  }

  void DivOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    bwd_div(y, x1, x2);
  }


// SqrtOp

  Interval SqrtOp::fwd(const Interval& x1)
  {
    return sqrt(x1);
  }

  ScalarOpValue SqrtOp::fwd(const ScalarOpValue& x1)
  {
    return {
      std::sqrt(x1.m),
      fwd(x1.a),
      IntervalVector(x1.da.size()) // todo
    };
  }

  void SqrtOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_sqrt(y, x1);
  }


// SqrOp

  Interval SqrOp::fwd(const Interval& x1)
  {
    return sqr(x1);
  }

  ScalarOpValue SqrOp::fwd(const ScalarOpValue& x1)
  {
    return {
      std::pow(x1.m,2),
      fwd(x1.a),
      2.*x1.a*x1.da
    };
  }

  void SqrOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_sqr(y, x1);
  }


// CosOp

  Interval CosOp::fwd(const Interval& x1)
  {
    return cos(x1);
  }

  ScalarOpValue CosOp::fwd(const ScalarOpValue& x1)
  {
    return {
      std::cos(x1.m),
      fwd(x1.a),
      -sin(x1.a)*x1.da
    };
  }

  void CosOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_cos(y, x1);
  }


// SinOp

  Interval SinOp::fwd(const Interval& x1)
  {
    return sin(x1);
  }

  ScalarOpValue SinOp::fwd(const ScalarOpValue& x1)
  {
    return {
      std::sin(x1.m),
      fwd(x1.a),
      cos(x1.a)*x1.da
    };
  }

  void SinOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_sin(y, x1);
  }


// TanhOp

  Interval TanhOp::fwd(const Interval& x1)
  {
    return tanh(x1);
  }

  ScalarOpValue TanhOp::fwd(const ScalarOpValue& x1)
  {
    return {
      std::tanh(x1.m),
      fwd(x1.a),
      IntervalVector(x1.da.size()) // todo
    };
  }

  void TanhOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_tanh(y, x1);
  }


// AbsOp

  Interval AbsOp::fwd(const Interval& x1)
  {
    return abs(x1);
  }

  ScalarOpValue AbsOp::fwd(const ScalarOpValue& x1)
  {
    return {
      std::fabs(x1.m),
      fwd(x1.a),
      IntervalVector(x1.da.size()) // todo
    };
  }

  void AbsOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_abs(y, x1);
  }


// ComponentOp

  Interval ComponentOp::fwd(const IntervalVector& x1, size_t i)
  {
    assert(i >= 0 && i < x1.size());
    return x1[i];
  }

  ScalarOpValue ComponentOp::fwd(const VectorOpValue& x1, size_t i)
  {
    assert(i >= 0 && i < x1.a.size());
    return {
      x1.m[i],
      fwd(x1.a,i),
      x1.da.row(i) // todo: consider mixted scalar/vector values
    };
  }

  void ComponentOp::bwd(const Interval& y, IntervalVector& x1, size_t i)
  {
    assert(i >= 0 && i < x1.size());
    x1[i] &= y;
  }


// MatrixOp

  void MatrixOp::fwd_i(IntervalMatrix& m, const IntervalVector& x, size_t i)
  {
    assert((int)i >= 0 && (int)i < m.cols());
    m.resize(x.size(),m.cols());
    m.col(i) = x;
  }