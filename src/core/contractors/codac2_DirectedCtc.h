/** 
 *  \file
 *  Directed contractors
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <string>
#include <codac2_Interval.h>
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>

namespace codac2
{
  struct OpValueBase
  {
    virtual ~OpValueBase() = default;
  };

  struct ScalarOpValue : public OpValueBase
  {
    using Domain = Interval;

    Interval m;
    Interval a;
    IntervalMatrix da;
    bool def_domain = true;

    ScalarOpValue(
      const Interval& m_, const Interval& a_, const IntervalMatrix& da_, bool def_domain_ = true)
      : m(m_), a(a_), da(da_), def_domain(def_domain_)
    { }

    ScalarOpValue(const Interval& a_, size_t total_args_size)
      : ScalarOpValue(a_.mid(), a_, IntervalMatrix::zeros(1,total_args_size))
    { }

    ScalarOpValue(const Interval& a_, size_t total_args_size, size_t id_first, size_t id_last)
      : ScalarOpValue(a_, total_args_size)
    {
      assert(id_first <= id_last);
      assert(id_first >= 0 && id_last < total_args_size);

      for(size_t i = id_first ; i <= id_last ; i++)
        da(0,i) = 1.;
    }

    ScalarOpValue& operator&=(const ScalarOpValue& x)
    {
      a &= x.a;
      da &= x.da;
      def_domain &= x.def_domain;
      return *this;
    }
  };

  struct VectorOpValue : public OpValueBase
  {
    using Domain = IntervalVector;
    
    IntervalVector m;
    IntervalVector a;
    IntervalMatrix da;
    bool def_domain = true;

    VectorOpValue(
      const IntervalVector& m_, const IntervalVector& a_, const IntervalMatrix& da_, bool def_domain_ = true)
      : m(m_), a(a_), da(da_), def_domain(def_domain_)
    { }

    VectorOpValue(const IntervalVector& a_, size_t total_args_size)
      : VectorOpValue(a_.mid(), a_, IntervalMatrix::zeros(a_.size(),total_args_size))
    { }

    VectorOpValue(const IntervalVector& a_, size_t total_args_size, size_t id_first, size_t id_last)
      : VectorOpValue(a_, total_args_size)
    {
      assert(id_first <= id_last);
      assert(id_first >= 0 && id_last < total_args_size);

      for(size_t i = id_first ; i <= id_last ; i++)
        da(i,i) = 1.;
    }

    VectorOpValue& operator&=(const VectorOpValue& x)
    {
      a &= x.a;
      da &= x.da;
      def_domain &= x.def_domain;
      return *this;
    }
  };

  struct MatrixOpValue : public OpValueBase
  {
    using Domain = IntervalMatrix;
    
    IntervalMatrix a;

    MatrixOpValue(const IntervalMatrix& a_)
      : a(a_)
    { }

    MatrixOpValue(const IntervalMatrix& a_, size_t total_args_size)
      : MatrixOpValue(a_)
    { }

    MatrixOpValue(const IntervalMatrix& a_, size_t total_args_size, size_t id_first, size_t id_last)
      : MatrixOpValue(a_)
    { }

    MatrixOpValue& operator&=(const MatrixOpValue& x)
    {
      a &= x.a;
      return *this;
    }

    virtual void compute_centered_form(const IntervalVector& flatten_x)
    { }
  };

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
      assert(false && "tocheck");
      IntervalMatrix m(1, sizeof...(X));
      size_t i = 0;
      (MatrixOp::fwd_i(m, x, i++), ...);
      return m;
    }

    template<typename... X>
    static void bwd(const IntervalMatrix& y, X&... x)
    {
      assert(false && "tocheck");
      size_t i = 0;
      ((x &= y.col(i++)), ...);
    }

    template<typename... X, typename = typename std::enable_if<(true && ... && (
        std::is_same_v<VectorOpValue,X>
      )), void>::type>
    static MatrixOpValue fwd(const X&... x)
    {
      assert(false && "tocheck");
      return {
        IntervalMatrix({x.a...})
      };
    }
  };
}