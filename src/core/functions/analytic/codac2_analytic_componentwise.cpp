/** 
 *  \file codac2_analytic_componentwise.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <vector>
#include "codac2_assert.h"
#include "codac2_analytic_componentwise.h"

namespace codac2
{
  AnalyticFunction<ScalarType>
  map_scalar_entries(const AnalyticFunction<ScalarType>& f, const ScalarExprTransform& transform)
  {
    return AnalyticFunction<ScalarType>(f.args(), transform(ScalarExpr(f.expr())));
  }

  AnalyticFunction<VectorType>
  map_scalar_entries(const AnalyticFunction<VectorType>& f, const ScalarExprTransform& transform)
  {
    const auto shape = f.output_shape();

    assert(shape.second == 1
      && "map_scalar_entries(VectorType): only column-vector outputs are supported");

    VectorExpr y(f.expr());
    std::vector<ScalarExpr> entries;
    entries.reserve(static_cast<std::size_t>(shape.first));

    for(Index i = 0 ; i < shape.first ; ++i)
      entries.push_back(transform(y[i]));

    return AnalyticFunction<VectorType>(f.args(), vec(entries));
  }

  AnalyticFunction<MatrixType>
  map_scalar_entries(const AnalyticFunction<MatrixType>& f, const ScalarExprTransform& transform)
  {
    const auto shape = f.output_shape();

    MatrixExpr y(f.expr());
    std::vector<VectorExpr> cols;
    cols.reserve(static_cast<std::size_t>(shape.second));

    for(Index j = 0 ; j < shape.second ; ++j)
    {
      std::vector<ScalarExpr> col_entries;
      col_entries.reserve(static_cast<std::size_t>(shape.first));

      for(Index i = 0 ; i < shape.first ; ++i)
        col_entries.push_back(transform(y(i,j)));

      cols.push_back(vec(col_entries));
    }

    return AnalyticFunction<MatrixType>(f.args(), mat(cols));
  }
}