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
      fwd(x1.m),
      fwd(x1.a),
      x1.da,
      x1.def_domain
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
      fwd(x1.m),
      fwd(x1.a),
      x1.da,
      x1.def_domain
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
      fwd(x1.m),
      fwd(x1.a),
      IntervalMatrix(0,0), // not supported yet for matrices
      x1.def_domain
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
    assert(x1.da.size() == x2.da.size());
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      x1.da + x2.da,
      x1.def_domain && x2.def_domain
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
    assert(x1.da.rows() == x2.da.rows() && x1.da.cols() == x2.da.cols());
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      x1.da + x2.da,
      x1.def_domain && x2.def_domain
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
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      IntervalMatrix(0,0), // not supported yet for matrices
      x1.def_domain && x2.def_domain
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
      fwd(x1.m),
      fwd(x1.a),
      -x1.da,
      x1.def_domain
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
      fwd(x1.m),
      fwd(x1.a),
      -x1.da,
      x1.def_domain
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
      fwd(x1.m),
      fwd(x1.a),
      IntervalMatrix(0,0), // not supported yet for matrices
      x1.def_domain
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
    assert(x1.da.rows() == x2.da.rows() && x1.da.cols() == x2.da.cols());
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      x1.da - x2.da,
      x1.def_domain && x2.def_domain
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
    assert(x1.da.rows() == x2.da.rows() && x1.da.cols() == x2.da.cols());
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      x1.da - x2.da,
      x1.def_domain && x2.def_domain
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
    assert(x1.a.cols() == x2.a.cols() && x1.a.rows() == x2.a.rows());
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      IntervalMatrix(0,0), // not supported yet for matrices
      x1.def_domain && x2.def_domain
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
    assert(x1.da.rows() == 1);
    assert(x1.da.rows() == x2.da.rows() && x1.da.cols() == x2.da.cols());

    IntervalMatrix d(1,x1.da.cols());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(i) = x1.da(i)*x2.a + x1.a*x2.da(i);

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain
    };
  }

  void MulOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    bwd_mul(y, x1, x2);
  }

  IntervalVector MulOp::fwd(const Interval& x1, const IntervalVector& x2)
  {
    return x1 * x2;
  }

  VectorOpValue MulOp::fwd(const ScalarOpValue& x1, const VectorOpValue& x2)
  {
    assert(x1.da.rows() == 1);
    assert(x1.da.cols() == x2.da.cols());
    assert(x2.a.size() == (size_t)x2.da.rows());

    IntervalMatrix d(x2.da.rows(),x2.da.cols());
    for(size_t i = 0 ; i < (size_t)d.rows() ; i++)
      for(size_t j = 0 ; j < (size_t)d.cols() ; j++)
        d(i,j) = x1.da(j)*x2.a(i)+x1.a*x2.da(i,j);

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain
    };
  }

  #include "codac2_ibex.h"

  void MulOp::bwd(const IntervalVector& y, Interval& x1, IntervalVector& x2)
  {
    auto y_ = to_ibex(y), x2_ = to_ibex(x2);
    auto x1_ = to_ibex(x1);
    ibex::bwd_mul(y_, x1_, x2_);
    x1 &= to_codac(x1_); x2 &= to_codac(x2_);
  }

  IntervalVector MulOp::fwd(const IntervalMatrix& x1, const IntervalVector& x2)
  {
    return x1 * x2;
  }

  VectorOpValue MulOp::fwd(const MatrixOpValue& x1, const VectorOpValue& x2)
  {
    return {
      fwd(x1.a, /* <<----- x1.m */ x2.m),
      fwd(x1.a, x2.a),
      x2.a * IntervalMatrix::zeros(x1.a.rows(),x1.a.cols()) + x1.a * x2.da, // todo
      x1.def_domain && x2.def_domain
    };
  }

  //#include "codac2_ibex.h"
  //#include <ibex_IntervalVector.h> // for ibex::bwd_mul

  #include "codac2_GaussJordan.h"

  void MulOp::bwd(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2)
  {
    assert(x1.rows() == (int)y.size());
    assert(x1.cols() == (int)x2.size());

    if(false && x1.rows() == x1.cols())
    {
      //CtcGaussElim ctc_ge;
      //CtcLinearPrecond ctc_gep(ctc_ge);
      //IntervalVector y_(y);
      //ctc_gep.contract(x1,x2,y_);
    }

    else
    {
      IntervalMatrix Q = gauss_jordan(x1.mid()).template cast<Interval>();
      IntervalVector b_tilde = Q*y;
      IntervalMatrix A_tilde = Q*x1; // should be a tree matrix

      for(int a = 0 ; a < 1 ; a++)
      {
        for(size_t i = 0 ; i < x2.size() ; i++)
        {
          for(size_t k = 0 ; k < b_tilde.size() ; k++)
          {
            Interval u = b_tilde[k];

            for(size_t j = 0 ; j < x2.size() ; j++)
              if(i != j)
                u -= x2[j]*A_tilde(k,j);

            if(A_tilde(k,i).contains(0.))
              continue;

            x2[i] &= u / A_tilde(k,i);
          }
        }
      }

      //ibex::IntervalVector ibex_y(to_ibex(y)), ibex_x2(to_ibex(x2));
      //ibex::IntervalMatrix ibex_x1(to_ibex(x1));
      //ibex::bwd_mul(ibex_y, ibex_x1, ibex_x2, 0.05);
      //x1 &= to_codac(ibex_x1);
      //x2 &= to_codac(ibex_x2);
    }
  }


// DivOp

  Interval DivOp::fwd(const Interval& x1, const Interval& x2)
  {
    return x1 / x2;
  }

  ScalarOpValue DivOp::fwd(const ScalarOpValue& x1, const ScalarOpValue& x2)
  {
    assert(x1.da.size() == x2.da.size());

    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(i) = (x1.da(i)*x2.a-x1.a*x2.da(i))/sqr(x2.a);

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain
    };
  }

  void DivOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    bwd_div(y, x1, x2);
  }


// PowOp

  Interval PowOp::fwd(const Interval& x1, const Interval& x2)
  {
    return pow(x1,x2);
  }

  ScalarOpValue PowOp::fwd(const ScalarOpValue& x1, const ScalarOpValue& x2)
  {
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(i) = x2.a*x1.da(i)*pow(x1.a,x2.a-1.);

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain
    };
  }

  void PowOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    bwd_pow(y, x1, x2);
  }


// SqrOp

  Interval SqrOp::fwd(const Interval& x1)
  {
    return sqr(x1);
  }

  ScalarOpValue SqrOp::fwd(const ScalarOpValue& x1)
  {
    assert(x1.da.rows() == 1);

    IntervalMatrix d(1,x1.da.cols());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(i) = 2.*x1.a*x1.da(i);

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain
    };
  }

  void SqrOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_sqr(y, x1);
  }


// SqrtOp

  Interval SqrtOp::fwd(const Interval& x1)
  {
    return sqrt(x1);
  }

  ScalarOpValue SqrtOp::fwd(const ScalarOpValue& x1)
  {
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(i) = x1.da(i)/(2.*sqrt(x1.a));

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.a.is_subset({0,oo}) && x1.def_domain
    };
  }

  void SqrtOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_sqrt(y, x1);
  }


// ExpOp

  Interval ExpOp::fwd(const Interval& x1)
  {
    return exp(x1);
  }

  ScalarOpValue ExpOp::fwd(const ScalarOpValue& x1)
  {
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(i) = x1.da(i)*exp(x1.a);

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain
    };
  }

  void ExpOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_exp(y, x1);
  }


// CosOp

  Interval CosOp::fwd(const Interval& x1)
  {
    return cos(x1);
  }

  ScalarOpValue CosOp::fwd(const ScalarOpValue& x1)
  {
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(i) = -sin(x1.a)*x1.da(i);

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain
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
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(i) = cos(x1.a)*x1.da(i);

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain
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
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(i) = x1.da(i)*sqr(1./cosh(x1.a));

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain
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
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(i) = (x1.a/abs(x1.a))*x1.da(i);

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain
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
    assert(i >= 0 && i < (size_t)x1.a.rows());
    return {
      fwd(x1.m,i),
      fwd(x1.a,i),
      x1.da.row(i),
      x1.def_domain
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