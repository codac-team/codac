/** 
 *  \file codac2_Matrix_addons_IntervalVector.h
 *  
 *  This class reuses some of the functions developed for ibex::IntervalVector. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalVector (IBEX lib, author: Gilles Chabert)
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIX_PLUGIN.
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
Matrix(const std::initializer_list<double>& l)
  : Matrix<codac2::Interval,R,C>(
    [&]() -> int { if(R == 1) return 1; else return l.size(); }(),
    [&]() -> int { if(C == 1) return 1; else return l.size(); }()
  )
{
  assert_release(!std::empty(l));
  Index i = 0;
  for(const auto& li : l)
    (*this)[i++] = codac2::Interval(li);
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
Matrix(const std::initializer_list<std::initializer_list<double>>& l)
  : Matrix<codac2::Interval,R,C>(
    [&]() -> int { if(R == 1) return 1; else return l.size(); }(),
    [&]() -> int { if(C == 1) return 1; else return l.size(); }()
  )
{
  assert_release(!std::empty(l));
  Index i = 0;
  for(const auto& li : l)
    (*this)[i++] = codac2::Interval(li);
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
Matrix(const std::initializer_list<codac2::Interval>& l)
  : Matrix<codac2::Interval,R,C>(
    [&]() -> int { if(R == 1) return 1; else return l.size(); }(),
    [&]() -> int { if(C == 1) return 1; else return l.size(); }()
  )
{
  assert_release(!std::empty(l));
  Index i = 0;
  for(const auto& li : l)
    (*this)[i++] = li;
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
Matrix(int n, const double bounds[][2])
  : Matrix<codac2::Interval,R,C>(
    [&]() -> int { if(R == 1) return 1; else return n; }(),
    [&]() -> int { if(C == 1) return 1; else return n; }(),
    bounds
  )
{
  assert_release(n > 0);
}

template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<U> && IsVectorOrRow<R,C>
inline static auto empty(Index n)
{
  assert_release(n >= 0);
  return Matrix<codac2::Interval,R,C>::constant((int)n,codac2::Interval::empty());
}