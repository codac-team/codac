/** 
 *  codac2_directed_ctc.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <cassert>
#include "codac2_directed_ctc.h"
#include "codac2_MatrixBase.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"

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
  {
    assert(y.size() == x1.size());
  }

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
  {
    assert(y.size() == x1.size());
  }


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
    assert(x1.size() == x2.size());
    return x1 + x2;
  }

  VectorOpValue AddOp::fwd(const VectorOpValue& x1, const VectorOpValue& x2)
  {
    assert(x1.da.nb_rows() == x2.da.nb_rows() && x1.da.nb_cols() == x2.da.nb_cols());
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
    assert(x1.size() == x2.size());
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
      AddOp::bwd(*(y._e.data()+i), *(x1._e.data()+i), *(x2._e.data()+i));
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
      SubOp::bwd(*(y._e.data()+i), *(x1._e.data()+i));
  }


// SubOp (binary operations)

  Interval SubOp::fwd(const Interval& x1, const Interval& x2)
  {
    return x1 - x2;
  }

  ScalarOpValue SubOp::fwd(const ScalarOpValue& x1, const ScalarOpValue& x2)
  {
    assert(x1.da.nb_rows() == x2.da.nb_rows() && x1.da.nb_cols() == x2.da.nb_cols());
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
    assert(x1.size() == x2.size());
    return x1 - x2;
  }

  VectorOpValue SubOp::fwd(const VectorOpValue& x1, const VectorOpValue& x2)
  {
    assert(x1.da.nb_rows() == x2.da.nb_rows() && x1.da.nb_cols() == x2.da.nb_cols());
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
    assert(x1.size() == x2.size());
    return x1 - x2;
  }

  MatrixOpValue SubOp::fwd(const MatrixOpValue& x1, const MatrixOpValue& x2)
  {
    assert(x1.a.nb_cols() == x2.a.nb_cols() && x1.a.nb_rows() == x2.a.nb_rows());
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
      SubOp::bwd(*(y._e.data()+i), *(x1._e.data()+i), *(x2._e.data()+i));
  }


// MulOp

  Interval MulOp::fwd(const Interval& x1, const Interval& x2)
  {
    return x1 * x2;
  }

  ScalarOpValue MulOp::fwd(const ScalarOpValue& x1, const ScalarOpValue& x2)
  {
    assert(x1.da.nb_rows() == 1);
    assert(x1.da.nb_rows() == x2.da.nb_rows() && x1.da.nb_cols() == x2.da.nb_cols());

    IntervalMatrix d(1,x1.da.nb_cols());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(0,i) = x1.da(0,i)*x2.a + x1.a*x2.da(0,i);

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
    assert(x1.da.nb_rows() == 1);
    assert(x1.da.nb_cols() == x2.da.nb_cols());
    assert(x2.a.size() == x2.da.nb_rows());

    IntervalMatrix d(x2.da.nb_rows(),x2.da.nb_cols());
    for(size_t i = 0 ; i < d.nb_rows() ; i++)
      for(size_t j = 0 ; j < d.nb_cols() ; j++)
        d(i,j) = x1.da(0,j)*x2.a[i]+x1.a*x2.da(i,j);
    
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain
    };
  }

  void MulOp::bwd(const IntervalVector& y, Interval& x1, IntervalVector& x2)
  {
    assert(y.size() == x2.size());
    for(size_t i = 0 ; i < x2.size() ; i++)
      bwd_mul(y[i], x1, x2[i]);
  }

  IntervalVector MulOp::fwd(const IntervalVector& x1, const Interval& x2)
  {
    return MulOp::fwd(x2,x1);
  }

  VectorOpValue MulOp::fwd(const VectorOpValue& x1, const ScalarOpValue& x2)
  {
    return MulOp::fwd(x2,x1);
  }

  void MulOp::bwd(const IntervalVector& y, IntervalVector& x1, Interval& x2)
  {
    MulOp::bwd(y, x2, x1);
  }

  IntervalVector MulOp::fwd(const IntervalMatrix& x1, const IntervalVector& x2)
  {
    assert(x1.nb_cols() == x2.size());
    return x1 * x2;
  }

  VectorOpValue MulOp::fwd(const MatrixOpValue& x1, const VectorOpValue& x2)
  {
    return {
      fwd(x1.a, /* <<----- x1.m */ x2.m),
      fwd(x1.a, x2.a),
      IntervalMatrix::zeros(x1.a.nb_rows(),x1.a.nb_cols()), // todo
      x1.def_domain && x2.def_domain
    };
  }

  #include "codac2_linear_ctc.h"
  #include "codac2_GaussJordan.h"

  void MulOp::bwd(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2)
  {
    assert(x1.nb_rows() == y.size());
    assert(x1.nb_cols() == x2.size());

    /*if(x1.is_squared()) // not working for any x1
    {
      CtcGaussElim ctc_ge;
      CtcLinearPrecond ctc_gep(ctc_ge);
      IntervalVector y_(y);
      ctc_gep.contract(x1,x2,y_);
    }

    else*/
    {
      IntervalMatrix Q = gauss_jordan(x1.mid());
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
      d(0,i) = (x1.da(0,i)*x2.a-x1.a*x2.da(0,i))/sqr(x2.a);

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain && x2.a != 0. /* def domain of the derivative of div */
    };
  }

  void DivOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    bwd_div(y, x1, x2);
  }

  IntervalVector DivOp::fwd(const IntervalVector& x1, const Interval& x2)
  {
    return x1 / x2;
  }

  VectorOpValue DivOp::fwd(const VectorOpValue& x1, const ScalarOpValue& x2)
  {
    assert(x1.da.size() == x2.da.size());

    IntervalMatrix d(1,x1.da.size());
    assert_release(false && "not implemented yet");

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain && x2.a != 0. /* def domain of the derivative of div */
    };
  }

  void DivOp::bwd(const IntervalVector& y, IntervalVector& x1, Interval& x2)
  {
    assert(x1.size() == y.size());
    for(size_t i = 0 ; i < x1.size() ; i++)
      bwd_div(y[i], x1[i], x2);
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
      d(0,i) = x2.a*x1.da(0,i)*pow(x1.a,x2.a-1.);

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
    assert(x1.da.nb_rows() == 1);

    IntervalMatrix d(1,x1.da.nb_cols());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(0,i) = 2.*x1.a*x1.da(0,i);

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
      d(0,i) = x1.da(0,i)/(2.*sqrt(x1.a));

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.a.is_subset({0,oo}) /* def domain of sqrt */
      && x1.a != 0. /* def domain of the derivative of sqrt */
      && x1.def_domain
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
      d(0,i) = x1.da(0,i)*exp(x1.a);

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


// LogOp

  Interval LogOp::fwd(const Interval& x1)
  {
    return log(x1);
  }

  ScalarOpValue LogOp::fwd(const ScalarOpValue& x1)
  {
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(0,i) = x1.da(0,i)/x1.a;

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.a.is_subset({0,oo}) /* def domain of log */
      && x1.a != 0. /* def domain of the derivative of log */
      && x1.def_domain
    };
  }

  void LogOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_log(y, x1);
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
      d(0,i) = -sin(x1.a)*x1.da(0,i);

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
      d(0,i) = cos(x1.a)*x1.da(0,i);

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


// TanOp

  Interval TanOp::fwd(const Interval& x1)
  {
    return tan(x1);
  }

  ScalarOpValue TanOp::fwd(const ScalarOpValue& x1)
  {
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(0,i) = x1.da(0,i)/sqr(cos(x1.a));

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain && cos(x1.a) != 0. /* def domain of the derivative of tan */
    };
  }

  void TanOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_tan(y, x1);
  }


// AcosOp

  Interval AcosOp::fwd(const Interval& x1)
  {
    return acos(x1);
  }

  ScalarOpValue AcosOp::fwd(const ScalarOpValue& x1)
  {
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(0,i) = -x1.da(0,i)/sqrt(1.-sqr(x1.a));

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.a.is_subset({-1,1}) /* def domain of acos */
      && x1.a != 1. /* def domain of the derivative of acos */
      && x1.def_domain
    };
  }

  void AcosOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_acos(y, x1);
  }


// AsinOp

  Interval AsinOp::fwd(const Interval& x1)
  {
    return asin(x1);
  }

  ScalarOpValue AsinOp::fwd(const ScalarOpValue& x1)
  {
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(0,i) = x1.da(0,i)/sqrt(1.-sqr(x1.a));

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.a.is_subset({-1,1}) /* def domain of asin */
      && x1.a != 1. /* def domain of the derivative of asin */
      && x1.def_domain
    };
  }

  void AsinOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_asin(y, x1);
  }


// AtanOp

  Interval AtanOp::fwd(const Interval& x1)
  {
    return atan(x1);
  }

  ScalarOpValue AtanOp::fwd(const ScalarOpValue& x1)
  {
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(0,i) = x1.da(0,i)/(1.+sqr(x1.a));

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain
    };
  }

  void AtanOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_atan(y, x1);
  }


// Atan2Op

  Interval Atan2Op::fwd(const Interval& x1, const Interval& x2)
  {
    return atan2(x1,x2);
  }

  ScalarOpValue Atan2Op::fwd(const ScalarOpValue& x1, const ScalarOpValue& x2)
  {
    assert(x1.da.nb_rows() == 1);
    assert(x1.da.nb_rows() == x2.da.nb_rows() && x1.da.nb_cols() == x2.da.nb_cols());

    IntervalMatrix d(1,x1.da.nb_cols());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(0,i) = (-x1.a*x2.da(0,i)/(sqr(x2.a)+sqr(x1.a)))+(x2.a*x1.da(0,i)/(sqr(x2.a)+sqr(x1.a)));

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain
      && !(x1.a == 0. && x2.a == 0.) /* def domain of the derivative of atan2 */
    };
  }

  void Atan2Op::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    bwd_atan2(y, x1, x2);
  }


// CoshOp

  Interval CoshOp::fwd(const Interval& x1)
  {
    return cosh(x1);
  }

  ScalarOpValue CoshOp::fwd(const ScalarOpValue& x1)
  {
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(0,i) = sinh(x1.a)*x1.da(0,i);

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain
    };
  }

  void CoshOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_cosh(y, x1);
  }


// SinhOp

  Interval SinhOp::fwd(const Interval& x1)
  {
    return sinh(x1);
  }

  ScalarOpValue SinhOp::fwd(const ScalarOpValue& x1)
  {
    IntervalMatrix d(1,x1.da.size());
    for(size_t i = 0 ; i < d.size() ; i++)
      d(0,i) = cosh(x1.a)*x1.da(0,i);

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain
    };
  }

  void SinhOp::bwd(const Interval& y, Interval& x1)
  {
    bwd_sinh(y, x1);
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
      d(0,i) = x1.da(0,i)/sqr(cosh(x1.a));

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
      d(0,i) = (x1.a/abs(x1.a))*x1.da(0,i);

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain && x1.a != 0. /* def domain of the derivative of abs */
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
    assert(i >= 0 && i < x1.a.nb_rows());
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


// SubvectorOp

  IntervalVector SubvectorOp::fwd(const IntervalVector& x1, size_t i, size_t j)
  {
    assert(i >= 0 && i < x1.size() && j >= i && j < x1.size());
    return x1.subvector(i,j);
  }

  VectorOpValue SubvectorOp::fwd(const VectorOpValue& x1, size_t i, size_t j)
  {
    assert(i >= 0 && i < x1.a.nb_rows() && j >= i && j < x1.a.nb_rows());
    return {
      fwd(x1.m,i,j),
      fwd(x1.a,i,j),
      x1.da.block(i,0,j-i+1,x1.da.nb_cols()),
      x1.def_domain
    };
  }

  void SubvectorOp::bwd(const IntervalVector& y, IntervalVector& x1, size_t i, size_t j)
  {
    assert(i >= 0 && i < x1.size() && j >= i && j < x1.size());
    assert(j-i < y.size());
    for(size_t k = 0 ; k < j-i+1 ; k++)
      x1[i+k] &= y[k];
  }


// MatrixOp

  void MatrixOp::fwd_i(IntervalMatrix& m, const IntervalVector& x, size_t i)
  {
    assert(i >= 0 && i < m.nb_cols());
    m.resize(x.size(),m.nb_cols());
    m.col(i) = x;
  }


// DetOp

  // For matrices

  Interval DetOp::fwd(const IntervalMatrix& x)
  {
    assert_release(x.is_squared() && "can only compute determinants for a square matrix");
    assert_release((x.nb_rows() == 1 || x.nb_rows() == 2) && "determinant not yet computable for n×n matrices, n>2");

    if(x.nb_rows() == 1) // 1×1 matrix
      return x(0,0);

    else if(x.nb_rows() == 2) // 2×2 matrix
      return x(0,0)*x(1,1)-x(0,1)*x(1,0);

    else
      return Interval::empty(); // unhandled case
  }

  ScalarOpValue DetOp::fwd(const MatrixOpValue& x)
  {
    return {
      fwd(x.m),
      fwd(x.a),
      IntervalMatrix(0,0), // not supported yet for auto diff
      x.def_domain
    };
  }

  void DetOp::bwd(const Interval& y, IntervalMatrix& x)
  {
    assert_release(x.is_squared() && "can only compute determinants for a square matrix");
    assert_release((x.nb_rows() == 1 || x.nb_rows() == 2) && "determinant not yet computable for n×n matrices, n>2");

    if(x.nb_rows() == 1) // 1×1 matrix
      x(0,0) &= y;

    else if(x.nb_rows() == 2) // 2×2 matrix
    {
      Interval z1 = x(0,0)*x(1,1), z2 = x(1,0)*x(0,1);
      SubOp::bwd(y, z1, z2);
      MulOp::bwd(z1, x(0,0), x(1,1));
      MulOp::bwd(z2, x(1,0), x(0,1));
    }

    else
    {
      // unhandled case
    }
  }

  // For two vectors (merged into a 2×2 matrix)

  Interval DetOp::fwd(const IntervalVector& x1, const IntervalVector& x2)
  {
    assert_release(x1.size() == 2 && x2.size() == 2 && "determinant only computable for pairs of 2d vectors");
    return DetOp::fwd(IntervalMatrix(x1,x2));
  }

  ScalarOpValue DetOp::fwd(const VectorOpValue& x1, const VectorOpValue& x2)
  {
    return {
      fwd(IntervalMatrix(x1.m,x2.m)),
      fwd(IntervalMatrix(x1.a,x2.a)),
      IntervalMatrix(0,0), // not supported yet for auto diff
      x1.def_domain && x2.def_domain
    };
  }

  void DetOp::bwd(const Interval& y, IntervalVector& x1, IntervalVector& x2)
  {
    assert_release(x1.size() == 2 && x2.size() == 2 && "determinant only computable for pairs of 2d vectors");
    IntervalMatrix m(2,2);
    m.col(0) = x1; m.col(1) = x2;
    DetOp::bwd(y,m);
    x1 &= m.col(0);
    x2 &= m.col(1);
  }

  // For three vectors (merged into a 3×3 matrix)

  Interval DetOp::fwd(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3)
  {
    assert_release(x1.size() == 3 && x2.size() == 3 && x3.size() == 3 && "determinant only computable for triplet of 3d vectors");
    return DetOp::fwd(IntervalMatrix(x1,x2,x3));
  }

  ScalarOpValue DetOp::fwd(const VectorOpValue& x1, const VectorOpValue& x2, const VectorOpValue& x3)
  {
    return {
      fwd(IntervalMatrix(x1.m,x2.m,x3.m)),
      fwd(IntervalMatrix(x1.a,x2.a,x3.a)),
      IntervalMatrix(0,0), // not supported yet for auto diff
      x1.def_domain && x2.def_domain && x3.def_domain
    };
  }

  void DetOp::bwd(const Interval& y, IntervalVector& x1, IntervalVector& x2, IntervalVector& x3)
  {
    assert_release(x1.size() == 3 && x2.size() == 3 && x3.size() == 3 && "determinant only computable for triplet of 3d vectors");
    IntervalMatrix m(x1,x2,x3);
    DetOp::bwd(y,m);
    x1 &= m.col(0);
    x2 &= m.col(1);
    x3 &= m.col(2);
  }