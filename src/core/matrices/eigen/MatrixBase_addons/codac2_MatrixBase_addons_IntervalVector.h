/** 
 *  \file codac2_MatrixBase_addons_IntervalVector.h
 *  
 *  This class reuses some of the functions developed for ibex::IntervalVector. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalVector (IBEX lib, author: Gilles Chabert)
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIXBASE_PLUGIN.
 *  See: https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html
 *  and the file codac2_matrices.h
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
inline auto complementary() const
{
  return Matrix<codac2::Interval,R,C>((int)this->size()).diff(*this);
}

template<typename OtherDerived,typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
inline std::list<Matrix<codac2::Interval,R,C>> diff(const MatrixBase<OtherDerived>& y, bool compactness = true) const
{
  // This code originates from the ibex-lib
  // See: ibex_TemplateVector.h
  // Author: Gilles Chabert
  // It has been revised with modern C++ and templated types

  const Index n = this->size();
  assert_release(y.size() == n);

  if(y == *this)
    return { };

  Matrix<codac2::Interval,R,C> x = *this;
  Matrix<codac2::Interval,R,C> z = x & y;

  if(z.is_empty())
  {
    if(x.is_empty()) return { };
    else return { x };
  }

  else
  {
    // Check if in one dimension y is flat and x not,
    // in which case the diff returns also x directly
    if(compactness)
      for(Index i = 0 ; i < n ; i++)
        if(z[i].is_degenerated() && !x[i].is_degenerated())
        {
          if(x.is_empty()) return { };
          else return { x };
        }
  }

  std::list<Matrix<codac2::Interval,R,C>> l;

  for(Index var = 0 ; var < n ; var++)
  {
    codac2::Interval c1, c2;
    
    for(const auto& ci : x[var].diff(y[var], compactness))
    {
      assert(!ci.is_empty());

      Matrix<codac2::Interval,R,C> v(n);
      for(Index i = 0 ; i < var ; i++)
        v[i] = x[i];
      v[var] = ci;
      for(Index i = var+1 ; i < n ; i++)
        v[i] = x[i];
      if(!v.is_empty())
        l.push_back(v);
    }

    x[var] = z[var];
  }

  return l;
}