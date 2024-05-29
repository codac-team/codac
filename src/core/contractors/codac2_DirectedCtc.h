/** 
 *  \file codac2_DirectedCtc.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include "codac2_analytic_values.h"

namespace codac2
{
  struct AddOp
  {
    // Unary operations
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);

    static IntervalVector fwd(const IntervalVector& x1);
    static VectorOpValue fwd(const VectorOpValue& x1);
    static void bwd(const IntervalVector& y, IntervalVector& x1);

    static IntervalMatrix fwd(const IntervalMatrix& x1);
    static MatrixOpValue fwd(const MatrixOpValue& x1);
    static void bwd(const IntervalMatrix& y, IntervalMatrix& x1);

    // Binary operations
    static Interval fwd(const Interval& x1, const Interval& x2);
    static ScalarOpValue fwd(const ScalarOpValue& x1, const ScalarOpValue& x2);
    static void bwd(const Interval& y, Interval& x1, Interval& x2);

    static IntervalVector fwd(const IntervalVector& x1, const IntervalVector& x2);
    static VectorOpValue fwd(const VectorOpValue& x1, const VectorOpValue& x2);
    static void bwd(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2);

    static IntervalMatrix fwd(const IntervalMatrix& x1, const IntervalMatrix& x2);
    static MatrixOpValue fwd(const MatrixOpValue& x1, const MatrixOpValue& x2);
    static void bwd(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2);
  };

  struct SubOp
  {
    // Unary operations
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);

    static IntervalVector fwd(const IntervalVector& x1);
    static VectorOpValue fwd(const VectorOpValue& x1);
    static void bwd(const IntervalVector& y, IntervalVector& x1);

    static IntervalMatrix fwd(const IntervalMatrix& x1);
    static MatrixOpValue fwd(const MatrixOpValue& x1);
    static void bwd(const IntervalMatrix& y, IntervalMatrix& x1);

    // Binary operations
    static Interval fwd(const Interval& x1, const Interval& x2);
    static ScalarOpValue fwd(const ScalarOpValue& x1, const ScalarOpValue& x2);
    static void bwd(const Interval& y, Interval& x1, Interval& x2);

    static IntervalVector fwd(const IntervalVector& x1, const IntervalVector& x2);
    static VectorOpValue fwd(const VectorOpValue& x1, const VectorOpValue& x2);
    static void bwd(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2);

    static IntervalMatrix fwd(const IntervalMatrix& x1, const IntervalMatrix& x2);
    static MatrixOpValue fwd(const MatrixOpValue& x1, const MatrixOpValue& x2);
    static void bwd(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2);
  };

  struct MulOp
  {
    static Interval fwd(const Interval& x1, const Interval& x2);
    static ScalarOpValue fwd(const ScalarOpValue& x1, const ScalarOpValue& x2);
    static void bwd(const Interval& y, Interval& x1, Interval& x2);

    static IntervalVector fwd(const Interval& x1, const IntervalVector& x2);
    static VectorOpValue fwd(const ScalarOpValue& x1, const VectorOpValue& x2);
    static void bwd(const IntervalVector& y, Interval& x1, IntervalVector& x2);

    static IntervalVector fwd(const IntervalMatrix& x1, const IntervalVector& x2);
    static VectorOpValue fwd(const MatrixOpValue& x1, const VectorOpValue& x2);
    static void bwd(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2);
  };

  struct DivOp
  {
    static Interval fwd(const Interval& x1, const Interval& x2);
    static ScalarOpValue fwd(const ScalarOpValue& x1, const ScalarOpValue& x2);
    static void bwd(const Interval& y, Interval& x1, Interval& x2);
  };

  struct PowOp
  {
    static Interval fwd(const Interval& x1, const Interval& x2);
    static ScalarOpValue fwd(const ScalarOpValue& x1, const ScalarOpValue& x2);
    static void bwd(const Interval& y, Interval& x1, Interval& x2);
  };

  struct SqrOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct SqrtOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct ExpOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct LogOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct CosOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct SinOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct TanOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct AcosOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct AsinOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct AtanOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct Atan2Op
  {
    static Interval fwd(const Interval& x1, const Interval& x2);
    static ScalarOpValue fwd(const ScalarOpValue& x1, const ScalarOpValue& x2);
    static void bwd(const Interval& y, Interval& x1, Interval& x2);
  };

  struct CoshOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct SinhOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct TanhOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct AbsOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarOpValue fwd(const ScalarOpValue& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  struct ComponentOp
  {
    static Interval fwd(const IntervalVector& x1, size_t i);
    static ScalarOpValue fwd(const VectorOpValue& x1, size_t i);
    static void bwd(const Interval& y, IntervalVector& x1, size_t i);
  };

  struct VectorOp
  {
    template<typename... X, typename = typename std::enable_if<(true && ... && (
        std::is_base_of_v<Interval,X>
      )), void>::type>
    static IntervalVector fwd(const X&... x)
    {
      return IntervalVector({x...});
    }

    template<typename... X, typename = typename std::enable_if<(true && ... && (
        std::is_same_v<ScalarOpValue,X>
      )), void>::type>
    static VectorOpValue fwd(const X&... x)
    {
      IntervalMatrix d(sizeof...(X),std::get<0>(std::tie(x...)).da.cols());
      size_t i = 0;
      ((d.row(i++) = x.da.transpose()), ...);

      bool def_domain = true;
      ((def_domain &= x.def_domain), ...);
      
      return {
        fwd(x.m...),
        fwd(x.a...),
        d,
        def_domain
      };
    }

    template<typename... X>
    static void bwd(const IntervalVector& y, X&... x)
    {
      size_t i = 0;
      ((x &= y[i++]), ...);
    }
  };

  struct MatrixOp
  {
    static void fwd_i(IntervalMatrix& m, const IntervalVector& x, size_t i);

    template<typename... X, typename = typename std::enable_if<(true && ... && (
        std::is_base_of_v<Domain,X>
      )), void>::type>
    static IntervalMatrix fwd(const X&... x)
    {
      throw std::runtime_error("MatrixOp not fully implemented yet");
      IntervalMatrix m(1, sizeof...(X));
      size_t i = 0;
      (MatrixOp::fwd_i(m, x, i++), ...);
      return m;
    }

    template<typename... X, typename = typename std::enable_if<(true && ... && (
        std::is_same_v<VectorOpValue,X>
      )), void>::type>
    static MatrixOpValue fwd(const X&... x)
    {
      throw std::runtime_error("MatrixOp not fully implemented yet");
      return {
        IntervalMatrix({x.m...}),
        IntervalMatrix({x.a...}),
        IntervalMatrix(0,0), // not supported yet for matrices
        true // todo with variadic
      };
    }

    template<typename... X>
    static void bwd(const IntervalMatrix& y, X&... x)
    {
      throw std::runtime_error("MatrixOp not fully implemented yet");
      size_t i = 0;
      ((x &= y.col(i++)), ...);
    }
  };
}