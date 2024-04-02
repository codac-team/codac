/** 
 *  \file
 *  Vector class
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include "codac2_Matrix.h"

namespace codac2
{
  template<int N=Dynamic>
  class Vector_ : public Matrix_<N,1>
  {
    public:

      Vector_()
      { }

      explicit Vector_(size_t n)
        : Matrix_<N,1>(n,1)
      {
        assert(N == Dynamic || N == (int)n);
      }

      explicit Vector_(size_t n, double x)
        : Matrix_<N,1>(n,1,x)
      {
        assert(N == Dynamic || N == (int)n);
      }

      explicit Vector_(size_t n, const double values[])
        : Matrix_<N,1>(n,1,values)
      { }

      explicit Vector_(const double values[])
        : Matrix_<N,1>(N,1,values)
      {
        static_assert(N != Dynamic);
      }

      explicit Vector_(const std::vector<double>& x)
        : Vector_(x.size(), &x[0])
      {
        assert(!x.empty());
      }

      template<int M>
      Vector_(const Matrix_<M,1>& x)
        : Matrix_<M,1>(x)
      {
        static_assert(M == Dynamic || M == N);
      }

      template<size_t M>
      explicit Vector_(const std::array<double,M>& array)
        : Matrix_<N,1>(M,1)
      {
        static_assert(N == Dynamic || N == (int)M);
        for(size_t i = 0 ; i < M ; i++)
          *(this->data()+i) = array[i];
      }

      Vector_(std::initializer_list<double> l)
        : Matrix_<N,1>(l.size(),1)
      {
        assert(N == (int)l.size() || N == -1);
        size_t i = 0;
        for(double li : l)
          (*this)(i++,0) = li;
      }

      template<typename OtherDerived>
      Vector_(const Eigen::MatrixBase<OtherDerived>& other)
        : Matrix_<N,1>(other)
      { }

      template<typename OtherDerived>
      Vector_& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->Eigen::Matrix<double,N,1>::operator=(other);
        return *this;
      }

      void resize(size_t n)
      {
        static_assert(N == Dynamic);
        this->Matrix_<N,1>::resize(n,1);
      }

      template<size_t N1,size_t N2>
      auto subvector() const
      {
        static_assert(N1 >= 0 && N1 < N && N2 >= 0 && N2 < N && N1 <= N2);
        return this->template block<N2-N1+1,1>(N1,0);
      }

      auto subvector(size_t start_id, size_t end_id) const
      {
        assert(end_id >= 0 && start_id >= 0);
        assert(end_id < this->size() && start_id <= end_id);
        return this->block(start_id, 0, end_id-start_id+1, 1);
      }

      template<size_t I,int M>
      void put(const Vector_<M>& x)
      {
        static_assert(I >= 0 && I < N && M+I <= N && M != Dynamic);
        this->template block<M,1>(I,0) << x;
      }

      template<int M>
      void put(size_t start_id, const Vector_<M>& x)
      {
        assert(start_id >= 0 && start_id < this->size());
        assert(start_id+x.size() <= this->size());
        this->block(start_id,0,x.size(),1) << x;
      }

      Matrix_<N,N> as_diag() const
      {
        Matrix diag(this->size(),this->size(),0.);
        for(size_t i = 0 ; i < (size_t)this->size() ; i++)
          diag(i,i) = (*this)[i];
        return diag;

        //return Eigen::Matrix<double,N,N>(this->asDiagonal());
      }

      size_t min_coeff_index() const
      {
        size_t r,c;
        Eigen::Matrix<double,N,1>::minCoeff(&r,&c);
        assert(c == 0);
        return r;
      }

      size_t max_coeff_index() const
      {
        size_t r,c;
        Eigen::Matrix<double,N,1>::maxCoeff(&r,&c);
        assert(c == 0);
        return r;
      }

      static Vector_<N> zeros(size_t n = N)
      {
        assert(n > 0);
        assert(N == Dynamic || N == (int)n);
        return Matrix_<N,1>::zeros(n,1);
      }

      static Vector_<N> ones(size_t n = N)
      {
        assert(n > 0);
        assert(N == Dynamic || N == (int)n);
        return Matrix_<N,1>::ones(n,1);
      }
  };

  template<int N>
  std::ostream& operator<<(std::ostream& os, const Vector_<N>& x)
  {
    os << "(";
    for(size_t i = 0 ; i < x.size() ; i++)
      os << x[i] << (i<x.size()-1 ? " ; " : "");
    os << ")";
    return os;
  }

  using Vector = Vector_<Dynamic>;

}